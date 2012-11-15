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
    var path = new PainterPath;
    path.addEllipse(x0-Sx, y0-Sy, Sx, Sy);
    return path;
}

function getColor(x0, y0)
{
    var m = 200 * i / N;
    return new Color(m + 55, 0, m + 55);
}

MainWindow.setBackgroundBrush(new Brush(20, 20, 20));
