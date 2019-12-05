#include "mainwindow.h"

#include<algorithm>
#include<QPainter>
#include<QDebug>
#include<QColor>
#include<QMouseEvent>
#include<QPoint>
#include<QImage>
#include<QRect>

#include "settings.h"
#include "complex.h"
#include "mandelbrot.h"

QColor mycolors[MANDELBROT_ITERATIONS_COUNT+2];
QPoint beginpoint,
       endpoint,
       tmppoint;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    QWidget* cwidg = new QWidget(this);
    this->setCentralWidget(cwidg);

    this -> setMinimumSize(2*DISPLAYSIZE+1, 2*DISPLAYSIZE+1);
    this -> setMaximumSize(2*DISPLAYSIZE+1, 2*DISPLAYSIZE+1);

    for (int i = 0, j = 0; j <= MANDELBROT_ITERATIONS_COUNT; i += 3, ++j)
        mycolors[j] = (i <= 360) ? QColor::fromHsl(i, 200, 127).name() : mycolors[j-1];

    image = new QImage(2*DISPLAYSIZE+1, 2*DISPLAYSIZE+1, QImage::Format_RGB32);
}

MainWindow::~MainWindow()
{
}

inline int min(int x, int y) {
    return (x > y) ? y : x;
}
inline int max(int x, int y) {
    return (x < y) ? y : x;
}


void MainWindow::mousePressEvent(QMouseEvent *event) {
    ISPRESSED = true;
    endpoint = beginpoint = event->pos();
}

void MainWindow::mouseMoveEvent(QMouseEvent *event) {
    if (!ISPRESSED)
        return;
    tmppoint = event -> pos();
    if (tmppoint.x() < 0 || tmppoint.y() < 0 || tmppoint.x() > 2*DISPLAYSIZE || tmppoint.y() > 2 * DISPLAYSIZE)
        return;
    tmppoint = endpoint;
    endpoint = event -> pos();
    repaint();

    return;
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event) {
    ISPRESSED = false;

    if (!(tmppoint.x() < 0 || tmppoint.y() < 0 || tmppoint.x() > 2*DISPLAYSIZE || tmppoint.y() > 2 * DISPLAYSIZE))
        endpoint = event->pos();

    int delx = endpoint.x() - beginpoint.x(), dely;
    if (delx > 0) {
        dely = abs(endpoint.y() - beginpoint.y());
        if (dely != 0) {
            BEGIN_COORD_X = double(min(endpoint.x(), beginpoint.x()))*STEP_COORD + BEGIN_COORD_X;
            BEGIN_COORD_Y = double(2.0*DISPLAYSIZE - max(endpoint.y(), beginpoint.y()))*STEP_COORD + BEGIN_COORD_Y;
            STEP_COORD = double(max(delx, dely))/(2.0*double(DISPLAYSIZE))*STEP_COORD;
            repaint();
        }
    }
    else if (delx < 0) {
        dely = abs(endpoint.y() - beginpoint.y());
        if (dely != 0) {
            STEP_COORD = (2.0 * double(DISPLAYSIZE)) / double(max(delx, dely)) * STEP_COORD;
            BEGIN_COORD_X -= double(min(endpoint.x(), beginpoint.x()))*STEP_COORD;
            BEGIN_COORD_Y -= double(2.0*DISPLAYSIZE - max(endpoint.y(), beginpoint.y()))*STEP_COORD;
            repaint();
        }
    }
}


void MainWindow::paintEvent(QPaintEvent *event){

    QPainter painter(this);

    painter.translate(DISPLAYSIZE, DISPLAYSIZE);
    painter.scale(1,-1);
    if (ISPRESSED) {
            int px1 = min(beginpoint.x(),tmppoint.x()) - DISPLAYSIZE,
                    px2 = max(beginpoint.x(),tmppoint.x()) - DISPLAYSIZE,
                    py1 = DISPLAYSIZE - max(beginpoint.y(),tmppoint.y()),
                    py2 = DISPLAYSIZE - min(beginpoint.y(), tmppoint.y());
//            qDebug() << px1 << " " << py1 << " " << px2 << " " << py2;

            for (int i = px1; i <= px2; ++i) {
                image->setPixelColor(i+DISPLAYSIZE, DISPLAYSIZE + py1, mycolors[imagecopy[i + DISPLAYSIZE][DISPLAYSIZE - py1]].toRgb());
                image->setPixelColor(i+DISPLAYSIZE, DISPLAYSIZE + py2, mycolors[imagecopy[i + DISPLAYSIZE][DISPLAYSIZE - py2]].toRgb());
            }
            for (int i = py1; i <= py2; ++i) {
                image->setPixelColor(px1+DISPLAYSIZE, DISPLAYSIZE + i, mycolors[imagecopy[px1 + DISPLAYSIZE][-i + DISPLAYSIZE]].toRgb());
                image->setPixelColor(px2+DISPLAYSIZE, DISPLAYSIZE + i, mycolors[imagecopy[px2 + DISPLAYSIZE][-i + DISPLAYSIZE]].toRgb());
            }
            px1 = min(beginpoint.x(),endpoint.x()) - DISPLAYSIZE,
                    px2 = max(beginpoint.x(),endpoint.x()) - DISPLAYSIZE,
                    py1 = DISPLAYSIZE - max(beginpoint.y(),endpoint.y()),
                    py2 = DISPLAYSIZE - min(beginpoint.y(), endpoint.y());


            for (int i = px1; i <= px2; ++i) {
                image->setPixelColor(i+DISPLAYSIZE, DISPLAYSIZE + py1, Qt::black);
                image->setPixelColor(i+DISPLAYSIZE, DISPLAYSIZE + py2, Qt::black);
            }
            for (int i = py1; i <= py2; ++i) {
                image->setPixelColor(px1+DISPLAYSIZE, DISPLAYSIZE + i, Qt::black);
                image->setPixelColor(px2+DISPLAYSIZE, DISPLAYSIZE + i, Qt::black);
            }
            painter.drawImage(QPoint(-DISPLAYSIZE, -DISPLAYSIZE), *image);
            return;
    }

    for(int i = -DISPLAYSIZE; i <= DISPLAYSIZE; ++i){
        for (int j = -DISPLAYSIZE; j <= DISPLAYSIZE; ++j) {
            complex<double> mypoint = complex<double>((i + DISPLAYSIZE)*STEP_COORD + BEGIN_COORD_X, (DISPLAYSIZE - j)*STEP_COORD + BEGIN_COORD_Y);
            if (DARKMODE) {
                if (pointCheck(mypoint, MANDELBROT_ITERATIONS_COUNT) == MANDELBROT_ITERATIONS_COUNT)
                    image->setPixelColor(i + DISPLAYSIZE, DISPLAYSIZE - j, Qt::black),
                            imagecopy[i+DISPLAYSIZE][j+DISPLAYSIZE] = 1;
                else
                    image->setPixelColor(i+DISPLAYSIZE, DISPLAYSIZE - j, Qt::white),
                            imagecopy[i+DISPLAYSIZE][j+DISPLAYSIZE] = 0;
            } else {
                int q = pointCheck(mypoint, MANDELBROT_ITERATIONS_COUNT);
                imagecopy[i+DISPLAYSIZE][j+DISPLAYSIZE] = q;
                image -> setPixelColor(i+DISPLAYSIZE, DISPLAYSIZE - j, mycolors[q].toRgb());

            }
        }
    }
    painter.drawImage(QPoint(-DISPLAYSIZE, -DISPLAYSIZE), *image);
    return;
}
