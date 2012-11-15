var A = 0.20;
var C = 0.99971;
var N = 2000;
var MAX_TRIALS = 100;
var S = 0;
var i = 0;

function proceed()
{
    A = A * C;
    S = A / 2;
}

function getShape(x0, y0)
{
    var path = new PainterPath;
    path.addRoundedRect(x0, y0, S, S, S/5, S/5);
    var tx = new Transform;
    tx.rotate((6 * Math.random() - 3), 0);
    return tx.map(path);
}

function getColor() 
{
    return new Color(200 * Math.random(), 200, 200 * Math.random());
}



MainWindow.setBackgroundColor(new Brush(15, 15, 15));
