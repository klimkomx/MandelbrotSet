#include "mandelbrot.h"

const double EPS = 1e-7;

int pointCheck(complex<double>& point, const int MANDELBROT_ITERATIONS_COUNT) {
    complex<double> z = complex<double>(0.0, 0.0);
    int i = 0;
    for (i = 0; i < MANDELBROT_ITERATIONS_COUNT; ++i) {
        z = z * z + point;
        if (z.pointRadius() > 2.0 + EPS) {
            break;
        }
    }
    return i;
}
