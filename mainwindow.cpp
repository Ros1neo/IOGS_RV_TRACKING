#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTimer>

DialogImg* frame_rgb;
DialogImg* frame_face;
DialogImg* frame_color;
bool MaskisVisible = false;

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

    //load Classifier
    _classifier.load("../IOGS_RV_TRACKING/haarcascade_frontalface_alt.xml");
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
    _color_selected = color;
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
    _frame.copyTo(_face);


    // Detect faces
    std::vector<Rect> faces;
    _classifier.detectMultiScale( _face, faces, 1.1, 2, 0|CV_HAAR_SCALE_IMAGE, Size(30, 30) );

    // Draw circles on the detected faces
    Rect Face;
    for( uint i = 0; i < faces.size(); i++ )
    {
        if (faces.at(i).height*faces.at(i).width>Face.width*Face.height){
            Face = faces.at(i);
        }
    }
    Point center( Face.x + Face.width*0.5, Face.y + Face.height*0.5 );
    ellipse( _face, center, Size( Face.width*0.5, Face.height*0.5), 0, 0, 360, Scalar( 255, 0, 255 ), 4, 8, 0 );
}

void MainWindow::updateColorTracking()
{
    // TODO : gérez le color tracking
    _frame.copyTo(_color);

    //Affichage des values de threshold
    ui->RThresholdValue->setText(QString::number(ui->R_slider->value()));
    ui->GThresholdValue->setText(QString::number(ui->G_slider->value()));
    ui->BThresholdValue->setText(QString::number(ui->B_slider->value()));

    int Bmin = qBlue(_color_selected)-ui->B_slider->value();
    int Gmin = qGreen(_color_selected)-ui->G_slider->value();
    int Rmin = qRed(_color_selected)-ui->R_slider->value();
    int Bmax = qBlue(_color_selected)+ui->B_slider->value();
    int Gmax = qGreen(_color_selected)+ui->G_slider->value();
    int Rmax = qRed(_color_selected)+ui->R_slider->value();

    ui->Bmin->setText(QString::number(Bmin));
    ui->Gmin->setText(QString::number(Gmin));
    ui->Rmin->setText(QString::number(Rmin));
    ui->Bmax->setText(QString::number(Bmax));
    ui->Gmax->setText(QString::number(Gmax));
    ui->Rmax->setText(QString::number(Rmax));

    //Update de mask avec les plages de couleurs
    Mat mask; //matrice d'affichage du colortracking en noir
    Mat invMask; //mask inversé
    inRange(_color,Scalar(Bmin,Gmin,Rmin), Scalar(Bmax,Gmax,Rmax),mask);
    int erodeN  = ui->erodeN->value();
    int erodeIterations = ui->erodeIter->value();
    int dilateN  = ui->erodeN->value();
    int dilateIterations = ui->erodeIter->value();
    erode(mask,mask,getStructuringElement(MORPH_ERODE, Size(erodeN*2+1, erodeN*2+1), Point(erodeN, erodeN)),Point(-1,-1),erodeIterations);
    dilate(mask,mask,getStructuringElement(MORPH_DILATE, Size(dilateN*2+1, dilateN*2+1), Point(dilateN, dilateN)),Point(-1,-1),dilateIterations);
    bitwise_not(mask,invMask);

    //SimpleBlobDetector
    // Paramètres
    SimpleBlobDetector::Params params;
    //params.minThreshold = 49;
    //params.maxThreshold = 50;
    params.filterByArea = true;
    params.minArea = 2000;
    params.maxArea = 307200;
    params.filterByCircularity = true;
    params.minCircularity = 0;
    params.maxCircularity = 1;
    params.filterByConvexity = true;
    params.minConvexity = 0;
    params.maxConvexity = 1;
    params.filterByInertia = true;
    params.minInertiaRatio = 0;
    params.maxInertiaRatio = 1;
    ui->Ed->setText(QString::number(ui->erodeN->value()));
    ui->Niter->setText(QString::number(ui->erodeIter->value()));

    //Detector sur le masque binaire
    SimpleBlobDetector detector(params);
    std::vector<KeyPoint> keypoints;
    detector.detect(invMask,keypoints);

    //Selection du blob le plus gros
    float maxS = 0;
    KeyPoint KPmax;
    for (unsigned int i=0;i<keypoints.size();i++){
        KeyPoint KPi = keypoints.at(i);
        if (KPi.size>maxS)
            KPmax = keypoints.at(i);
    }
    std::vector<KeyPoint> MaxKP;
    MaxKP.push_back(KPmax);


    //Affichage
    if(MaskisVisible)
        _color.setTo(0,mask);
    drawKeypoints( _color, MaxKP, _color, Scalar(0,0,255), DrawMatchesFlags::DRAW_RICH_KEYPOINTS );


}

void MainWindow::on_btn_quit_clicked()
{
    exit(EXIT_SUCCESS);
}

void MainWindow::on_pushButton_clicked()
{
    MaskisVisible = !MaskisVisible;
}
