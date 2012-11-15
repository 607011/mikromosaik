// Copyright (c) 2012 Oliver Lau <ola@ct.de>, Heise Zeitschriften Verlag
// All rights reserved.

#include <QtCore>
#include <QtCore/QDebug>
#include <QPainter>
#include <QSettings>
#include <QFileInfo>
#include <QFile>
#include <QFileDialog>
#include <QTextStream>
#include <QImageWriter>
#include <QListIterator>
#include <QStringListIterator>
#include <QByteArray>
#include <QBuffer>
#include <QtGui/QMessageBox>
#include <QSvgGenerator>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "main.h"


MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , mScale(1.0)
    , mBackgroundBrush(Qt::white)
    , mDirty(false)
{
    ui->setupUi(this);
    for (int i = 0; i < MaxRecentFiles; ++i) {
        QAction* act = new QAction(this);
        act->setVisible(false);
        mRecentScriptFileActs[i] = act;
        QObject::connect(act, SIGNAL(triggered()), SLOT(loadRecentScript()));
        ui->menuOpenRecentScript->addAction(act);
    }
    setWindowTitle(tr("%1 %2").arg(AppName).arg(AppVersion));
    setFocusPolicy(Qt::StrongFocus);
    setFocus(Qt::OtherFocusReason);

    mKineticScroller.attachTo(this);
    QObject::connect(&mKineticScroller, SIGNAL(scrollBy(const QPoint&)), SLOT(scrollBy(const QPoint&)));
    QObject::connect(&mKineticScroller, SIGNAL(zoomBy(qreal, const QPoint&)), SLOT(zoomBy(qreal, const QPoint&)));

    QScriptValue m = mScriptThread.engine().newQObject(this);
    mScriptThread.engine().globalObject().setProperty("MainWindow", m);

    QObject::connect(&mScriptThread, SIGNAL(tilingProgressed(QVector<Stroke>, int)), SLOT(tilingProgressed(const QVector<Stroke>&, int)), Qt::BlockingQueuedConnection);
    QObject::connect(&mScriptThread, SIGNAL(debug(QString)), SLOT(debug(const QString&)), Qt::BlockingQueuedConnection);
    QObject::connect(&mScriptThread, SIGNAL(finished()), SLOT(tileThreadFinished()));
    QObject::connect(&mScriptThread, SIGNAL(backgroundBrushChanged(const QBrush&)), SLOT(setBackgroundBrush(const QBrush&)));
    QObject::connect(ui->runStopPushButton, SIGNAL(clicked()), SLOT(runStopScript()));
    QObject::connect(ui->actionShowLineNumbers, SIGNAL(toggled(bool)), &mEditor, SLOT(setLineNumbersVisible(bool)));
    QObject::connect(ui->actionBracketMatching, SIGNAL(toggled(bool)), &mEditor, SLOT(setBracketsMatchingEnabled(bool)));
    QObject::connect(ui->actionTextWrap, SIGNAL(toggled(bool)), &mEditor, SLOT(setTextWrapEnabled(bool)));
    QObject::connect(ui->actionCodeFolding, SIGNAL(toggled(bool)), &mEditor, SLOT(setCodeFoldingEnabled(bool)));
    QObject::connect(ui->actionTileMode, SIGNAL(toggled(bool)), &mScriptThread, SLOT(setTileMode(bool)));
    QObject::connect(ui->widthSpinBox, SIGNAL(valueChanged(int)), &mScriptThread, SLOT(setDrawingWidth(int)));
    QObject::connect(ui->heightSpinBox, SIGNAL(valueChanged(int)), &mScriptThread, SLOT(setDrawingHeight(int)));
    QObject::connect(ui->actionNewScript, SIGNAL(triggered()), SLOT(newScript()));
    QObject::connect(ui->actionOpenScript, SIGNAL(triggered()), SLOT(openScript()));
    QObject::connect(ui->actionSaveScript, SIGNAL(triggered()), SLOT(saveScript()));
    QObject::connect(ui->actionSaveScriptAs, SIGNAL(triggered()), SLOT(saveScriptAs()));
    QObject::connect(ui->actionSaveTilesAs, SIGNAL(triggered()), SLOT(saveTilesAs()));
    QObject::connect(ui->actionAbout, SIGNAL(triggered()), SLOT(about()));
    QObject::connect(ui->actionAboutQt, SIGNAL(triggered()), SLOT(aboutQt()));

    // set up script editor
    mEditor.setTabStopWidth(2);
    mEditor.setWordWrapMode(QTextOption::NoWrap);
    QFont monospace;
#if defined(Q_OS_MAC)
    monospace.setPointSize(12);
    monospace.setFamily("Monaco");
#else
    monospace.setPointSize(8);
    monospace.setFamily("Monospace");
#endif
    monospace.setStyleHint(QFont::TypeWriter);
    mEditor.setFont(monospace);
    mEditor.setStyleSheet("background-color: rgba(20, 20, 20, 200)");
    mEditor.setColor(JSEdit::Background,    QColor(20, 20, 20, 200));
    mEditor.setColor(JSEdit::Normal,        QColor("#FFFFFF"));
    mEditor.setColor(JSEdit::Comment,       QColor("#666666"));
    mEditor.setColor(JSEdit::Number,        QColor("#DBF76C"));
    mEditor.setColor(JSEdit::String,        QColor("#5ED363"));
    mEditor.setColor(JSEdit::Operator,      QColor("#FF7729"));
    mEditor.setColor(JSEdit::Identifier,    QColor("#FFFFFF"));
    mEditor.setColor(JSEdit::Keyword,       QColor("#FDE15D"));
    mEditor.setColor(JSEdit::BuiltIn,       QColor("#9CB6D4"));
    mEditor.setColor(JSEdit::Cursor,        QColor("#1E346B"));
    mEditor.setColor(JSEdit::Marker,        QColor("#DBF76C"));
    mEditor.setColor(JSEdit::BracketMatch,  QColor("#1AB0A6"));
    mEditor.setColor(JSEdit::BracketError,  QColor("#A82224"));
    mEditor.setColor(JSEdit::FoldIndicator, QColor("#555555"));

    ui->consolePlainTextEdit->setFont(monospace);

    QObject::connect(&mEditor, SIGNAL(textChanged()), SLOT(scriptChanged()));

    ui->verticalLayout->insertWidget(0, &mEditor);

    restoreSettings();

    updateEditorWindowTitle();
}


void MainWindow::updateEditorWindowTitle(void)
{
    ui->dockWidget->setWindowTitle(tr("%1 - Editor: %2%3")
                                   .arg(AppName)
                                   .arg(mScriptFilename.isEmpty()? tr("<untitled>"): QFileInfo(mScriptFilename).fileName())
                                   .arg(mDirty?"*":""));
}


void MainWindow::scriptChanged(void)
{
    mDirty = true;
    updateEditorWindowTitle();
}


MainWindow::~MainWindow()
{
    saveSettings();
    delete ui;
}


void MainWindow::restoreSettings(void)
{
    QSettings settings(Company, AppName);
    restoreGeometry(settings.value("MainWindow/geometry").toByteArray());
    mScriptFilename = settings.value("Options/Editor/scriptFilename", ":/scripts/default.js").toString();
    loadScript(mScriptFilename);
    ui->dockWidget->setFloating(settings.value("Options/Editor/floating", false).toBool());
    ui->dockWidget->restoreGeometry(settings.value("Options/Editor/geometry").toByteArray());
    ui->actionTextWrap->setChecked(settings.value("Options/Editor/textWrap", true).toBool());
    ui->actionBracketMatching->setChecked(settings.value("Options/Editor/bracketMatching", true).toBool());
    ui->actionCodeFolding->setChecked(settings.value("Options/Editor/codeFolding", true).toBool());
    ui->actionShowLineNumbers->setChecked(settings.value("Options/Editor/lineNumbersVisible", true).toBool());
    ui->actionTileMode->setChecked(settings.value("Options/tileMode", true).toBool());
    ui->widthSpinBox->setValue(settings.value("Options/imageWidth", width()).toInt());
    ui->heightSpinBox->setValue(settings.value("Options/imageHeight", height()).toInt());
    updateRecentFileActions("Options/recentScriptFileList", ui->menuOpenRecentScript, mRecentScriptFileActs);
}


void MainWindow::saveSettings(void)
{
    QSettings settings(Company, AppName);
    settings.setValue("MainWindow/geometry", saveGeometry());
    settings.setValue("Options/Editor/geometry", ui->dockWidget->saveGeometry());
    settings.setValue("Options/Editor/floating", ui->dockWidget->isFloating());
    settings.setValue("Options/Editor/scriptFilename", mScriptFilename);
    settings.setValue("Options/Editor/script", mEditor.toPlainText());
    settings.setValue("Options/Editor/textWrap", ui->actionTextWrap->isChecked());
    settings.setValue("Options/Editor/bracketMatching", ui->actionBracketMatching->isChecked());
    settings.setValue("Options/Editor/codeFolding", ui->actionCodeFolding->isChecked());
    settings.setValue("Options/Editor/lineNumbersVisible", ui->actionShowLineNumbers->isChecked());
    settings.setValue("Options/tileMode", ui->actionTileMode->isChecked());
    settings.setValue("Options/imageWidth", ui->widthSpinBox->value());
    settings.setValue("Options/imageHeight", ui->heightSpinBox->value());
}


void MainWindow::closeEvent(QCloseEvent* e)
{
    saveSettings();
    mScriptThread.stop();
    checkDirtyScript();
    e->accept();
}


void MainWindow::keyPressEvent(QKeyEvent* e)
{
    if (e->key() == Qt::Key_Escape) {
        resetDrawingPosition();
    }
    e->accept();
}


void MainWindow::paintEvent(QPaintEvent*)
{
    QPainter p(this);
    if (mDrawing.isEmpty()) {
        p.fillRect(rect(), QColor(10, 5, 5));
        return;
    }
    p.setRenderHint(QPainter::Antialiasing);
    p.translate(mOrigin);
    p.scale(mScale, mScale);
    p.save();
    paintOn(p);
    p.restore();
    p.setCompositionMode(QPainter::CompositionMode_Screen);
    p.setBrush(Qt::transparent);
    p.setPen(QPen(QColor(206, 252, 24, 128), 1, Qt::DashLine));
    p.drawRect(0, 0, ui->widthSpinBox->value(), ui->heightSpinBox->value());
}


void MainWindow::paintOn(QPainter& p)
{
    p.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    p.fillRect(0, 0, ui->widthSpinBox->value(), ui->heightSpinBox->value(), mBackgroundBrush);
    p.scale(ui->widthSpinBox->value(), ui->heightSpinBox->value());
    p.setPen(Qt::transparent);
    for (QVector<Stroke>::const_iterator it = mDrawing.constBegin(); it != mDrawing.constEnd(); ++it) {
        p.setBrush(it->brush());
        p.drawPath(it->path());
    }
}


void MainWindow::setTileMode(bool enabled)
{
    ui->actionTileMode->setChecked(enabled);
    // mScriptThread.setTileMode(enabled);
}


void MainWindow::setBackgroundBrush(const QBrush& brush)
{
    mBackgroundBrush = brush;
    update();
}


void MainWindow::scrollBy(const QPoint& d)
{
    mOrigin -= d;
    update();
}


void MainWindow::scrollBy(qreal dx, qreal dy)
{
    scrollBy(QPoint(dx, dy));
}


void MainWindow::zoomBy(qreal factor, const QPoint& pos)
{
    mScale *= factor;
    QPointF d = (mOrigin - pos) * (1 - factor);
    scrollBy(d.toPoint());
    update();
}


void MainWindow::resetDrawingPosition(void)
{
    mKineticScroller.stopMotion();
    mOrigin = QPoint((width() - ui->widthSpinBox->value()) / 2, (height() - ui->heightSpinBox->value()) / 2);
    mScale = 1.0;
    update();
}


void MainWindow::tilingProgressed(const QVector<Stroke>& drawing, int N)
{
    mDrawing = drawing;
    ui->progressBar->setValue(ui->progressBar->maximum() * mDrawing.size() / N);
    update();
}


void MainWindow::tileThreadFinished(void)
{
    ui->runStopPushButton->setText(tr("Run"));
    ui->progressBar->setValue(0);
    ui->progressBar->setEnabled(false);
    mScriptThread.engine().collectGarbage();
}


void MainWindow::startTiling(void)
{
    if (!mScriptThread.isRunning()) {
        mScriptThread.resume();
        ui->progressBar->setEnabled(true);
    }
}


void MainWindow::runStopScript(void)
{
    if (ui->runStopPushButton->text() == tr("Run")) {
        if (mEditor.toPlainText().isEmpty()) {
            debug(tr("Empty script. Doing nothing."));
            return;
        }
        QScriptEngine& engine = mScriptThread.engine();
        QScriptValue& globals = engine.globalObject();
        // QScriptEngine vergisst keine globalen Objekte aus vorigen Evaluierungen.
        // Deshalb kann es vorkommen, dass etwa getBrush() noch existiert, obwohl
        // die Funktion im aktuellen Skript gar nicht mehr definiert ist, was zur
        // ungewollten Texturierung der Formen führt.
        globals.setProperty("getBrush", QScriptValue::NullValue);
        globals.setProperty("getColor", QScriptValue::NullValue);
        globals.setProperty("getShape", QScriptValue::NullValue);
        globals.setProperty("proceed", QScriptValue::NullValue);
        globals.setProperty("W", ui->widthSpinBox->value());
        globals.setProperty("H", ui->heightSpinBox->value());
        const QScriptValue& result = engine.evaluate(mEditor.toPlainText());
        if (result.isError()) {
            debug(tr("%1 (line: %2)").arg(result.toString()).arg(result.property("lineNumber").toInt32()));
        }
        else if (engine.hasUncaughtException()) {
            debug(engine.uncaughtException().toString());
            QStringListIterator ex(engine.uncaughtExceptionBacktrace());
            while (ex.hasNext())
                debug(QString("  %1").arg(ex.next()));
        }
        else {
            resetDrawingPosition();
            startTiling();
            ui->runStopPushButton->setText(tr("Stop"));
        }
    }
    else {
        mScriptThread.stop();
    }
}


void MainWindow::debug(const QString& message)
{
    ui->consolePlainTextEdit->appendPlainText(message);
}


void MainWindow::checkDirtyScript(void)
{
    if (mDirty) {
        const int rc = QMessageBox::warning(this, tr("Script modified"), tr("You've modified the script. Do you want to save the changes?"), QMessageBox::Yes, QMessageBox::No);
        if (rc == QMessageBox::Yes) {
            if (mScriptFilename.isEmpty())
                saveScriptAs();
            else
                saveScript(mScriptFilename);
        }
    }
}


void MainWindow::newScript(void)
{
    checkDirtyScript();
    mDrawing.clear();
    update();
    mEditor.setPlainText(QString());
    mDirty = false;
    mScriptFilename = QString();
    updateEditorWindowTitle();
}


void MainWindow::loadRecentScript(void)
{
    const QAction* const action = qobject_cast<QAction*>(sender());
    if (action)
        loadScript(action->data().toString());
}


void MainWindow::openScript(void)
{
    const QString& filename = QFileDialog::getOpenFileName(this, tr("Open script"), QString(), tr("Script files (*.js)"));
    if (filename.isEmpty())
        return;
    loadScript(filename);
}


void MainWindow::loadScript(const QString& filename)
{
    if (!filename.isEmpty()) {
        QFile file(filename);
        const bool success = file.open(QIODevice::ReadOnly | QIODevice::Text);
        if (success) {
            const QString& script = file.readAll();
            mEditor.setPlainText(script);
            file.close();
            mScriptFilename = filename;
            mDirty = false;
            appendToRecentFileList(filename, "Options/recentScriptFileList");
            updateRecentFileActions("Options/recentScriptFileList", ui->menuOpenRecentScript, mRecentScriptFileActs);
        }
    }
    updateEditorWindowTitle();
}


void MainWindow::saveScriptAs(void)
{
    const QString& filename = QFileDialog::getSaveFileName(this, tr("Save script as"), QString(), tr("Script files (*.js)"));
    if (!filename.isEmpty())
        saveScript(filename);
}


void MainWindow::saveScript(QString filename)
{
    if (filename.isEmpty())
        filename = mScriptFilename;
    if (filename.isEmpty()) {
        saveScriptAs();
        return;
    }
    QFile file(filename);
    const bool success = file.open(QIODevice::WriteOnly | QIODevice::Text);
    if (success) {
        QTextStream out(&file);
        out << mEditor.toPlainText();
        file.close();
        statusBar()->showMessage(tr("Script saved as '%1'").arg(filename), 3000);
        mScriptFilename = filename;
        mDirty = false;
        updateEditorWindowTitle();
        appendToRecentFileList(filename, "Options/recentScriptFileList");
        updateRecentFileActions("Options/recentScriptFileList", ui->menuOpenRecentScript, mRecentScriptFileActs);
    }
}


void MainWindow::saveTilesAs(void)
{
    QString suffixes = "*.svg";
    QListIterator<QByteArray> formats(QImageWriter::supportedImageFormats());
    while (formats.hasNext())
        suffixes += " *." + formats.next();
    const QString& filename = QFileDialog::getSaveFileName(this, tr("Save image"), QString(), tr("Image files (%1)").arg(suffixes));
    if (filename.isEmpty())
        return;
    saveTiles(filename);
}


void MainWindow::saveTiles(const QString& filename)
{
    QFileInfo fi(filename);
    const int w = ui->widthSpinBox->value();
    const int h = ui->heightSpinBox->value();
    if (fi.suffix() == "svg") {
        QSvgGenerator generator;
        generator.setFileName(filename);
        generator.setTitle(tr("Generated with %1").arg(QFileInfo(mScriptFilename).baseName()));
        generator.setDescription(tr("Generated with `%1 %2%3`. Copyright (c) 2012 Oliver Lau, Heise Zeitschriften Verlag. See %4 for more info.")
                                 .arg(AppName).arg(AppVersion).arg(AppMinorVersion).arg(AppUrl));
        generator.setSize(QSize(w, h));
        generator.setViewBox(QRectF(0, 0, w, h));
        QPainter painter(&generator);
        paintOn(painter);
    }
    else {
        QImage image(w, h, QImage::Format_ARGB32);
        QPainter painter(&image);
        paintOn(painter);
        image.save(filename);
    }
}


void MainWindow::appendToRecentFileList(const QString& fileName, const QString& listName)
{
    QSettings settings(Company, AppName);
    QStringList files = settings.value(listName).toStringList();
    files.removeAll(fileName);
    files.prepend(fileName);
    while (files.size() > MaxRecentFiles)
        files.removeLast();
    settings.setValue(listName, files);
}


void MainWindow::updateRecentFileActions(const QString& listName, QMenu* menu, QAction* actions[])
{
    QStringList updatedFiles;
    QSettings settings(Company, AppName);
    QStringListIterator file(settings.value(listName).toStringList());
    while (file.hasNext() && updatedFiles.size() < MaxRecentFiles) {
        const QString& filename = file.next();
        QFileInfo fInfo(filename);
        // lesbare Dateien behalten, Duplikate verwerfen
        if (!updatedFiles.contains(filename) && fInfo.isFile() && fInfo.isReadable()) {
            const int i = updatedFiles.size();
            const QString& text = tr("&%1 %2").arg(i).arg(fInfo.fileName());
            actions[i]->setText(text);
            actions[i]->setData(filename);
            actions[i]->setVisible(true);
            updatedFiles.append(filename);
        }
    }
    for (int i = updatedFiles.size(); i < MaxRecentFiles; ++i)
        actions[i]->setVisible(false);
    menu->setEnabled(updatedFiles.size() > 0);
    settings.setValue(listName, updatedFiles);
}


void MainWindow::about(void)
{
    QMessageBox::about(this, tr("About %1 %2%3").arg(AppName).arg(AppVersionNoDebug).arg(AppMinorVersion),
                       tr("<p><b>%1</b> demonstrates how Qt programs can be controlled by QtScript. "
                          "See <a href=\"%2\" title=\"%1 project homepage\">%2</a> for more info.</p>"
                          "<p>Copyright &copy; 2012 %3 &lt;%4&gt;</p>"
                          "<p>This program is free software: you can redistribute it and/or modify "
                          "it under the terms of the GNU General Public License as published by "
                          "the Free Software Foundation, either version 3 of the License, or "
                          "(at your option) any later version.</p>"
                          "<p>This program is distributed in the hope that it will be useful, "
                          "but WITHOUT ANY WARRANTY; without even the implied warranty of "
                          "MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the "
                          "GNU General Public License for more details.</p>"
                          "You should have received a copy of the GNU General Public License "
                          "along with this program. "
                          "If not, see <a href=\"http://www.gnu.org/licenses/gpl-3.0\">http://www.gnu.org/licenses</a>.</p>")
                       .arg(AppName).arg(AppUrl).arg(AppAuthor).arg(AppAuthorMail));
}


void MainWindow::aboutQt(void)
{
    QMessageBox::aboutQt(this);
}
