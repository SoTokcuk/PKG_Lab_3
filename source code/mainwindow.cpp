#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QImage>
#include <QPainter>
#include <QStaticText>
#include <QFileDialog>

QImage *bmp, *buf;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    bmp = NULL;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MyWidget::paintEvent(QPaintEvent *event)
{
    QPainter p(this);

    if (bmp == NULL)
    {
        p.drawStaticText( 10, 10, QStaticText( "Загрузите картинку" ) );
        return;
    }

    if (buf == NULL)
        p.drawImage( 0,0, *bmp );
    else
        p.drawImage( 0,0, *buf );
}

void MainWindow::on_pushButton_clicked()
{
    QString fname = QFileDialog::getOpenFileName( this );

    bmp = new QImage();
    bmp->load( fname );

    ui->widget->repaint();

    delete buf;
    buf = NULL;
}

void MainWindow::on_pushButton_2_clicked()
{
    if (buf == NULL) return;

    for( int y = 0; y < bmp->height(); y++ )
    {
        for( int x = 0; x < bmp->width(); x++ )
        {
            QRgb c = bmp->pixel( x, y );

            int r, g, b, v;

            r = c & 0xFF;
            g = (c >> 8) & 0xFF;
            b = (c >> 16) & 0xFF;

            if ( (r + g + b) / 3 > 128 )
                v = 255;
            else
                v = 0;

            bmp->setPixel( x, y, v | (v << 8) | (v << 16) );
        }
    }

    ui->widget->repaint();
}

typedef int TMask[3][3];

TMask mask_lst[] =
{
    {
        { 1,1,1 },
        { 1,1,1 },
        { 1,1,1 },
    },
    {
        { 0,0,0 },
        { 1,0,1 },
        { 0,0,0 },
    },
    {
        { 0,0,0 },
        { 1,1,1 },
        { 0,0,0 },
    },
    {
        { 0,1,0 },
        { 0,0,0 },
        { 0,1,0 },
    },
    {
        { 0,1,0 },
        { 0,1,0 },
        { 0,1,0 },
    },
    {
        { 0,1,0 },
        { 1,1,1 },
        { 0,1,0 },
    },
    {
        { 0,0,0 },
        { 0,1,0 },
        { 0,1,0 },
    },
    {
        { 0,0,0 },
        { 0,1,1 },
        { 0,0,0 },
    },
    {
        { 0,0,0 },
        { 0,1,1 },
        { 0,1,0 },
    },
};

void MainWindow::func( int typ, QImage *from, QImage *to )
{
    TMask m, *pm;

    if (ui->radioButton->isChecked())
        pm = &mask_lst[0];
    if (ui->radioButton_2->isChecked())
        pm = &mask_lst[1];
    if (ui->radioButton_3->isChecked())
        pm = &mask_lst[2];
    if (ui->radioButton_4->isChecked())
        pm = &mask_lst[3];
    if (ui->radioButton_5->isChecked())
        pm = &mask_lst[4];
    if (ui->radioButton_6->isChecked())
        pm = &mask_lst[5];
    if (ui->radioButton_7->isChecked())
        pm = &mask_lst[6];
    if (ui->radioButton_8->isChecked())
        pm = &mask_lst[7];
    if (ui->radioButton_9->isChecked())
        pm = &mask_lst[8];

    int x, y, x2, y2, cnt;

    cnt = 0;
    for( y = 0; y < 3; y++ )
        for( x = 0; x < 3; x++ )
        {
            m[x][y] = (*pm)[x][y];
            if (m[x][y] == 1)
                cnt++;
        }

    for( y2 = 0; y2 < from->height()-3; y2++ )
        for( x2 = 0; x2 < from->width()-3; x2++ )
        {
            int s;

            s = 0;
            for( y = 0; y < 3; y++ )
                for( x = 0; x < 3; x++ )
                {
                    if (m[x][y] == 0)
                        continue;

                    QRgb c = from->pixel( x + x2, y + y2 );

                    if ( (c & 1) != 0 )
                        s++;
                }

            if (typ == 0)
            { // min
                if (s < cnt)
                    s = 0;
            }
            else if (typ == 1)
            { // max
                if (s > 0)
                    s = cnt;
            }

            to->setPixel( x2+1, y2+1, (s == 0 ? 0 : 0xFFffFF) );
        }
}

void MainWindow::on_pushButton_3_clicked()
{
    if (buf == NULL)
        buf = new QImage( bmp->width(), bmp->height(), QImage::Format_RGB32 );

    func( 0, bmp, buf );

    ui->widget->repaint();
}

void MainWindow::on_pushButton_4_clicked()
{
    if (buf == NULL)
        buf = new QImage( bmp->width(), bmp->height(), QImage::Format_RGB32 );

    func(1, bmp, buf );

    ui->widget->repaint();
}

void MainWindow::on_pushButton_5_clicked()
{
    if (bmp == NULL)
        return;

    delete bmp;

    bmp = buf;

    buf = NULL;

    ui->widget->repaint();
}

void MainWindow::on_pushButton_6_clicked()
{
    if (buf == NULL)
        buf = new QImage( bmp->width(), bmp->height(), QImage::Format_RGB32 );

    QImage *buf2 = new QImage( bmp->width(), bmp->height(), QImage::Format_RGB32 );

    func( 0, bmp, buf2 );
    func( 1, buf2, buf );

    ui->widget->repaint();
}
