var M = MainWindow;

// Startwerte für A und C
var A = 0.25;
var C = 0.9997;

// Anzahl Formen
var N = 2500;

// max. Anzahl Versuche, eine Form unterzubringen
var MAX_TRIALS = 500;

// "Breite" und "Höhe" der Form
var Sx = undefined;
var Sy = undefined;

var i = 0, W, H; // set in C++ code
var Aspect = W/H;


// proceed() wird bei jeder Iteration vor dem Platzieren einer weiteren Form aufgerufen
function proceed()
{
    A = A * C;
    Sy = A / 2;
    Sx = Sy / Aspect;
}


function makeCross(x0, y0, w, h)
{
    var path = new PainterPath;
    path.moveTo(x0-w/6, y0-h/2);
    path.lineTo(x0+w/6, y0-h/2);
    path.lineTo(x0+w/6, y0-h/6);
    path.lineTo(x0+w/2, y0-h/6);
    path.lineTo(x0+w/2, y0+h/6);
    path.lineTo(x0+w/6, y0+h/6);
    path.lineTo(x0+w/6, y0+h/2);
    path.lineTo(x0-w/6, y0+h/2);
    path.lineTo(x0-w/6, y0+h/6);
    path.lineTo(x0-w/2, y0+h/6);
    path.lineTo(x0-w/2, y0-h/6);
    path.lineTo(x0-w/6, y0-h/6);
    path.lineTo(x0-w/6, y0-h/6);
    return path;
}

// getShape() liefert den QPainterPath für die zu platzierende Form
function getShape(x0, y0)
{
    if (i == 0)
        return makeCross(0.5, 0.5, 0.8/Aspect, 0.8);
    return makeCross(x0, y0, Sx, Sy);
}

// getColor() liefert die Farbe, in der die Form gezeichnet werden soll
function getColor(x0, y0)
{
    var dx = x0-0.5;
    var dy = y0-0.5;
    var d = 0.9 - Math.sqrt(dx*dx+dy*dy);
    var rgb = rgbFromWaveLength(380 + (400 * d));
    if (i == 0)
         return new Color(220, 36, 31);
    return new Color(rgb[0], rgb[1], rgb[2]);
}
