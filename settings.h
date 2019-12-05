#ifndef SETTINGS_H
#define SETTINGS_H

#include <QColor>
#include <QImage>
const int MANDELBROT_ITERATIONS_COUNT = 500;
int DISPLAYSIZE = 500;
const double C = 10.0;
const double EPS = 1e-7;

double BEGIN_COORD_X = -2.0;
double BEGIN_COORD_Y = -2.0;
double STEP_COORD = 2.0 / double(DISPLAYSIZE);
bool ISPRESSED = false;
bool ERASERECT = false;
QImage * image;
int imagecopy[2*501][2*501];


const bool DARKMODE = false;
#endif // SETTINGS_H
