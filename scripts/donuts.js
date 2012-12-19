const C = 0.9991;
const N = 3000;
const MAX_TRIALS = 1000;
var A = 0.4;

var Sx, Sy;
var i = 0; // wird im C++-Code nach jedem erfolgreich platzierten Element inkrementiert
var H, W; // Breite und Höhe der Grafik kommen aus dem C++-Code

var Aspect = W / H;

function proceed()
{
    A = A * C;
    Sy = A / 2;
    Sx = Sy / Aspect;
}

function getShape(x0, y0)
{
    var outerPath = new PainterPath;
    outerPath.addEllipse(x0-Sx, y0-Sy, Sx, Sy);
    var innerPath = new PainterPath;
    var d = 0.8;
    innerPath.addEllipse(x0-Sx*d, y0-Sy*d, Sx*d, Sy*d);
    return outerPath.subtracted(innerPath);
}

function getColor(x0, y0)
{
    return new Color(200 * i / N + 55, 50 + 205 * Math.random(), 100 + 155 + Math.random());
}

MainWindow.setBackgroundBrush(new Brush(20, 20, 20));
