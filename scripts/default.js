// Faktor, um den die Fläche jedes folgenden
// platzierten Elements schrumpfen soll
const C = 0.9991;

// so viel Elemente sollen ingesamt platziert werden
const N = 3000;

// so viel Versuche unternehmen, ein Element
// bestimmter Größe im Bild unterzubringen
const MAX_TRIALS = 1000;

// Bruchteil der Fläche, die das erste platzierte
// Element einnehmen soll
var A = 0.4;

// Skalierung der Koordinaten in x- und y-Richtung
var Sx, Sy;

// zum Start = 0, wird im C++-Code nach jedem
// erfolgreich platzierten Element inkrementiert
var i;

// Breite und Höhe der Grafik kommen aus dem C++-Code
var H, W;

//
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
