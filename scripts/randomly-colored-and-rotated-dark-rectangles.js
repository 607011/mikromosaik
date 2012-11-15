// Startwerte für A und C
var A = 0.4;
var C = 0.9998;

// Anzahl Formen
var N = 3000;

// max. Anzahl Versuche, eine Form unterzubringen
var MAX_TRIALS = 500;

// Breite und Höhe der Form
var Sx, Sy;

// Breite und Höhe des Zielbildes werden auf C++-Seite gesetzt
var W, H, Aspect = H / W;

// proceed() wird bei jeder Iteration vor dem Platzieren einer weiteren Form aufgerufen
function proceed()
{
    A = A * C;
    Sy = .5 * A;
    Sx = Aspect * Sy;
}

// getShape() liefert den QPainterPath für die zu platzierende Form
function getShape(x0, y0)
{
    var path = new PainterPath;
    path.addRect(x0, y0, Sx, Sy);
    var tx1 = new Transform;
    tx1.translate(-x0-Sx/2, -y0-Sy/2);
    tx1.scale(Sx, Sy);
    tx1.rotate((20 * Math.random() - 10), 0);
    tx1.scale(1/Sx, 1/Sy);
    var tx2 = new Transform;
    tx2.translate(x0+Sx, y0+Sy);
    return tx2.map(tx1.map(path));
}

// getColor() liefert die Farbe, in der die Form gezeichnet werden soll
function getColor(x0, y0)
{
    return new Color(255 * Math.random(), 255 * Math.random(), 255 * Math.random(), 80);
}

MainWindow.setTileMode(true);
MainWindow.setBackgroundBrush(new Brush(15, 15, 15));
