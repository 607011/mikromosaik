// Startwerte für A und C
var A = 0.3;
var C = 0.99985;

// Anzahl Formen
var N = 3000;

// max. Anzahl Versuche, eine Form unterzubringen
var MAX_TRIALS = 1000;

// "Breite" der Form
var Sx, Sy, i, Aspect = W/H;


// proceed() wird bei jeder Iteration vor dem Platzieren einer weiteren Form aufgerufen
function proceed()
{
    A = A * C;
    Sy = A / 2;
    Sx = Sy / Aspect;
}

// getShape() liefert den QPainterPath für die zu platzierende Form
function getShape(x0, y0)
{
    var path = new PainterPath;
    path.addRect(x0, y0, Sx, Sy);
    var tx1 = new Transform;
    tx1.translate(-x0, -y0);
    tx1.rotate((45 * Math.random() - 22.5), 0);
    var tx2 = new Transform;
    tx2.translate(x0, y0);
    return tx2.map(tx1.map(path));
}

// getColor() liefert die Farbe, in der die Form gezeichnet werden soll
function getColor(x0, y0)
{
    return new Color(255 * Math.random(), 255 * Math.random(), 255 * Math.random(), 50);
}

MainWindow.setTileMode(true);
MainWindow.setBackgroundBrush(new Brush(25, 25, 25));
