const C = 0.99821;
const N = 2500;
const MAX_TRIALS = 500;
// const IMG = new Image(":/images/Lego-Tactical-Guy.jpg");
const IMG = new Image(":/images/Pilzmaennchen.png");

var brush = new Brush(IMG);

var A = 0.35, S;

var aspect = W/H;


function proceed()
{
    A = A * C;
    S = A / 2;
}


function getShape(x0, y0)
{
    var path = new PainterPath;
    path.addRoundedRect(x0-S/2, y0-S/2, S/aspect, S, S/9/aspect, S/9);
    return path;
}


function getBrush(x0, y0)
{
    var transform = new Transform;
    transform.translate(x0-S/2, y0-S/2);
    transform.scale(S/IMG.width()/aspect, S/IMG.height());
    brush.setTransform(transform);
    return brush;
}


MainWindow.setBackgroundBrush(new Brush(15, 15, 15));
