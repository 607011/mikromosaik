var A = 0.25;
var C = 0.9993;
var N = 2560;
var MAX_TRIALS = 60;
var Sx, Sy, Aspect = H/W;

var d = MainWindow.debug;

/*
function proceed()
{
    A = A * C;
    Sy = A / 2;
    Sx = Aspect * Sy;
}
*/

function getShape(x0, y0)
{
    var path = new PainterPath;
    path.addRect(x0, y0, Sx, Sy);
    var tx = new Transform;
    tx.shear((Math.random() - 0.5), 0);
    return tx.map(path);
}

var IMG = new Image(":/images/Pilzmaennchen.png");

var B = new Brush(IMG);

var g = new Gradient;

// d("Gradient.ReflectSpread = " + Gradient.ReflectSpread);

var radialgradient = new RadialGradient;
radialgradient.setCenter(0, 0);
radialgradient.setRadius(20);
radialgradient.setFocalPoint(5, 0);
radialgradient.setFocalRadius(10);

// d("radialgradient instanceof RadialGradient => " + (radialgradient instanceof Gradient));
radialgradient.setSpread(Gradient.ReflectSpread);


MainWindow.setBackgroundBrush(new Brush(15, 15, 15));
