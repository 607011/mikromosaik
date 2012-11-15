var A = 0.25;
var C = 0.9993;
var N = 2560;
var MAX_TRIALS = 60;
var Sx, Sy, Aspect = W/H;

function proceed()
{
    A = A * C;
    Sy = A / 2;
    Sx = Sy / Aspect;
}

function getShape(x0, y0)
{
    var path = new PainterPath;
    path.addRect(x0, y0, Sx, Sy);
    var tx = new Transform;
    tx.shear((Math.random() - 0.5), 0);
    return tx.map(path);
}


MainWindow.setBackgroundBrush(new Brush(15, 15, 15));
