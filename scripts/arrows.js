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
    path.moveTo(x0, y0);
    path.lineTo(x0+0.15*Sx, y0+0.40*Sy);
    path.lineTo(x0+0.05*Sx, y0+0.30*Sy);
    path.lineTo(x0+0.05*Sx, y0+1.20*Sy);
    path.lineTo(x0+0.15*Sx, y0+1.60*Sy);
    path.lineTo(x0, y0+1.30*Sy);
    path.lineTo(x0-0.15*Sx, y0+1.60*Sy);
    path.lineTo(x0-0.05*Sx, y0+1.20*Sy);
    path.lineTo(x0-0.05*Sx, y0+0.30*Sy);
    path.lineTo(x0-0.15*Sx, y0+0.40*Sy);
    return path;
}

function getColor(x0, y0)
{
    return new Color(100 + 155 * i / N, 155 + 100 * Math.random(),  255 * Math.random());
}

MainWindow.setBackgroundBrush(new Brush(20, 20, 20));

