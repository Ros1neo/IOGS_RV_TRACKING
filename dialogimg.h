#ifndef DIALOGIMG_H
#define DIALOGIMG_H

#include <QDialog>

using namespace std;

namespace Ui {
class DialogImg;
}

class DialogImg : public QDialog
{
    Q_OBJECT

public:
    explicit DialogImg(QWidget *parent = 0);
    ~DialogImg();

    void showImage(QImage& img, const QString &title);

signals:
    void click(const QRgb& color);

protected:
    void mouseMoveEvent(QMouseEvent* e);

private:
    Ui::DialogImg *ui;
};

#endif // DIALOGIMG_H
