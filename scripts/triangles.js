var A = 0.33;
var C = 0.999281;
var N = 2560;
var MAX_TRIALS = 1000;
var Sx, Sy, W, H;
var i = 0;
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
    if (i % 2 == 0) {
        path.moveTo(x0, y0-Sy);
        path.lineTo(x0+Sx, y0+Sy);
        path.lineTo(x0-Sx, y0+Sy);
        path.lineTo(x0, y0-Sy);
    }
    else {
        path.moveTo(x0, y0+Sy);
        path.lineTo(x0+Sx, y0-Sy);
        path.lineTo(x0-Sx, y0-Sy);
        path.lineTo(x0, y0+Sy);
    }
    return path;
}

function getColor(x0, y0)
{
    var g = 55+200*i/N;
    return new Color(g, g, 0);
}

MainWindow.setBackgroundBrush(new Brush(255, 255, 255));
