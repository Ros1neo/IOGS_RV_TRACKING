#include "dialogimg.h"
#include "ui_dialogimg.h"
#include <QMouseEvent>
#include <iostream>

DialogImg::DialogImg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogImg)
{
    ui->setupUi(this);
}

DialogImg::~DialogImg()
{
    delete ui;
}

void DialogImg::showImage(QImage &img, const QString& title)
{
    setWindowTitle(title);
    ui->label_img->setPixmap(QPixmap::fromImage(img));
    show();
}

void DialogImg::mouseMoveEvent(QMouseEvent *e)
{
    QPoint mouse(e->pos().x()-ui->label_img->pos().x(), e->pos().y()-ui->label_img->pos().y());
    if(mouse.x() >=0 && mouse.x() < ui->label_img->width() && mouse.y() >= 0 && mouse.y() < ui->label_img->height())
    {
        click(ui->label_img->pixmap()->toImage().pixel(mouse));
    }
}
