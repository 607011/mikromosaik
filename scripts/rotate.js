var A = 0.1;
var C = 0.9993;
var N = 2560;
var MAX_TRIALS = 6000;
var S = 0;

function proceed()
{
    A = A * C;
    S = A / 2;
}

function getShape(x0, y0)
{
    var path = new PainterPath;
    path.addRect(x0, y0, S, S);
    var tx = new Transform;
    tx.rotate(4 * Math.random() - 2);
    return tx.map(path);
}


MainWindow.startTiling()
