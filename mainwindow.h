#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/objdetect/objdetect.hpp"
#include <dialogimg.h>

using namespace std;
using namespace cv;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void updateComputation();
    void getDialogClick(const QRgb& color);

    void on_btn_quit_clicked();

    void on_pushButton_clicked();

private:
    void init();
    void initCapture();
    void updateCapture();
    void showCapture(DialogImg *dialog, const Mat& frame, const QString& title);
    void updateFaceTracking();
    void updateColorTracking();
    Rect findBiggestBlob(cv::Mat & mask);

private:
    Ui::MainWindow *ui;
    VideoCapture _cap;
    bool _computationCompleted;
    Mat _frame;
    Mat _face;
    Mat _color;
    QRgb _color_selected;
    CascadeClassifier _classifier;
};

#endif // MAINWINDOW_H
