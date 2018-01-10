#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTimer>

DialogImg* frame_rgb;
DialogImg* frame_face;
DialogImg* frame_color;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    _computationCompleted(true)
{
    ui->setupUi(this);

    // dialog images
    frame_rgb = new DialogImg();
    frame_face = new DialogImg();
    frame_color = new DialogImg();

    // dialog signals
    connect(frame_rgb, SIGNAL(click(const QRgb&)), this, SLOT(getDialogClick(const QRgb&)));
    connect(frame_face, SIGNAL(click(const QRgb&)), this, SLOT(getDialogClick(const QRgb&)));
    connect(frame_color, SIGNAL(click(const QRgb&)), this, SLOT(getDialogClick(const QRgb&)));

    // timer
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(updateComputation()));
    timer->start(30);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateComputation()
{
    if(_computationCompleted)
    {
        _computationCompleted = false;
        init();
        updateCapture();
        showCapture(frame_rgb, _frame, "camera output");

        updateColorTracking();
        showCapture(frame_color, _color, "color tracking");

        updateFaceTracking();
        showCapture(frame_face, _face, "face tracking");

        _computationCompleted = true;
    }
}

void MainWindow::getDialogClick(const QRgb &color)
{
    int r = qRed(color);
    int g = qGreen(color);
    int b = qBlue(color);
    ui->label_selected_color->setText(QString::number(r)+"\t"+QString::number(g)+"\t"+QString::number(b));
}

bool captureInit = false;
bool faceInit = false;
void MainWindow::init()
{
    // init capture
    initCapture();
}

void MainWindow::initCapture()
{
    if(!captureInit)
    {
        // from a camera
        //_cap = VideoCapture(0);
        // from a video file
        _cap = VideoCapture("../IOGS_RV_TRACKING/data/video.mp4");
        if(!_cap.isOpened())
        {
            cout << "Error init caption" << endl;
            return;
        }
        captureInit = true;
    }
}

void MainWindow::updateCapture()
{
    Mat frame;
    bool success = _cap.read(frame);
    if(success)
        frame.copyTo(_frame);
    else
    {
        captureInit = false;
        initCapture();
    }
}

void MainWindow::showCapture(DialogImg* dialog, const Mat &frame, const QString &title)
{
    Mat frameRGB;
    frame.copyTo(frameRGB);
    cvtColor(frame, frameRGB, CV_BGR2RGB);
    const uchar *qImageBuffer = (const uchar*)frameRGB.data;
    QImage img(qImageBuffer, frameRGB.cols, frameRGB.rows, frameRGB.step, QImage::Format_RGB888);
    dialog->showImage(img, title);
}

void MainWindow::updateFaceTracking()
{
    // TODO : gérez le color tracking
    _frame.copyTo(_face);
}

void MainWindow::updateColorTracking()
{
    // TODO : gérez le color tracking
    _frame.copyTo(_color);
}

void MainWindow::on_btn_quit_clicked()
{
    exit(EXIT_SUCCESS);
}
