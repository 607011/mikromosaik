// Copyright (c) 2012 Oliver Lau <ola@ct.de>, Heise Zeitschriften Verlag
// All rights reserved.

#ifndef __MAINWINDOW_H_
#define __MAINWINDOW_H_

#include <QMainWindow>
#include <QScrollArea>
#include <QMenu>
#include <QAction>
#include <QScriptEngine>
#ifndef QT_NO_DEBUG
#include <QScriptEngineDebugger>
#endif
#include <QString>
#include <QVector>
#include <QImage>

#include "stroke.h"
#include "scriptrunner.h"
#include "jsedit/jsedit.h"
#include "kineticscroller.h"


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = NULL);
    ~MainWindow();

protected:
    void closeEvent(QCloseEvent*);
    void paintEvent(QPaintEvent*);
    void keyPressEvent(QKeyEvent*);

private:
    Ui::MainWindow *ui;
    ScriptRunner mScriptRunner;
    QString mScriptFilename;
    QImage mImage;
    JSEdit mEditor;
    QScrollArea mScrollArea;
    KineticScroller mKineticScroller;
    QPoint mOrigin;
    qreal mScale;
    QBrush mBackgroundBrush;
    bool mDirty;
    QVector<Stroke> mDrawing;
    static const int MaxRecentFiles = 10;
    QAction* mRecentScriptFileActs[MaxRecentFiles];

public slots:
    void debug(const QString&);
    void zoomBy(qreal factor, const QPoint& pos);
    void scrollBy(qreal dx, qreal dy);
    void setTileMode(bool enabled);
    void setBackgroundBrush(const QBrush&);

private: // methods
    void paintOn(QPainter&);
    void startTiling(void);
    void saveSettings(void);
    void restoreSettings(void);
    void updateEditorWindowTitle(void);
    void appendToRecentFileList(const QString& fileName, const QString& listName);
    void updateRecentFileActions(const QString& listName, QMenu* menu, QAction* actions[]);
    void checkDirtyScript(void);
    void resetDrawingPosition(void);

private slots:
    void scrollBy(const QPoint& d);
    void scriptChanged(void);
    void tilingProgressed(const QVector<Stroke>&, int);
    void loadRecentScript(void);
    void tileThreadFinished(void);
    void runStopScript(void);
    void loadScript(const QString& filename);
    void newScript(void);
    void openScript(void);
    void saveScriptAs(void);
    void saveScript(QString filename = QString());
    void saveTilesAs(void);
    void saveTiles(const QString& filename);
    void about(void);
    void aboutQt(void);
};

#endif // __MAINWINDOW_H_
