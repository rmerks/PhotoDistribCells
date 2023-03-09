#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <random>
#include <iostream>
#include <QFileDialog>
#include <QLCDNumber>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{

    ui->setupUi(this);
    QObject::connect(ui->actionLoad, SIGNAL(triggered()), this, SLOT(ReadImageFile()));
    QObject::connect(ui->actionSave, SIGNAL(triggered()), this, SLOT(SaveCells()));
    QObject::connect(ui->horizontalSlider, &QSlider::valueChanged, this, &MainWindow::DropCells);
    QObject::connect(ui->horizontalSlider, SIGNAL(valueChanged(int)), ui->lcdNumber, SLOT(display(int)) );
    im=new QImage(500,500,QImage::Format_RGB32);
     im->fill(QColor("black"));

     ui->imagedisplay->setPixmap(QPixmap::fromImage(*im));
     ui->imagedisplay->show();

}

MainWindow::~MainWindow()
{
    delete ui;
}



void MainWindow::ReadImageFile() {

   QMessageBox *m=new QMessageBox;

   QString fileName = QFileDialog::getOpenFileName(this,
       tr("Open Image"), "/Users/roel/Documents/GitHub/CellDistrib/PhotoCellDistrib/Data/", tr("Image Files (*.png *.jpg *.bmp *.gif)"));

   if (fileName.isNull() || !im->load(fileName)) {
        m->setText("The Image could not be loaded");
        m->show();
    }  else {

      DropCells(10000);
   }
   delete m;
}

void MainWindow::DropCells(int ncells) {

QImage im2=QImage(im->scaledToWidth(500));

QImage im3=QImage(im2.convertToFormat(QImage::Format_Grayscale8));
QImage im2b=QImage(im3.convertToFormat(QImage::Format_RGB16));
QVector<int> v(im3.width()*im3.height());
int i=0;
for (int x=0;x<im3.width();x++) {
  for (int y=0;y<im3.height();y++) {
      QRgb p=im3.pixel(x,y);
      v[i++]=p;
  }
}
std::cerr << v.size() << std::endl;

//const int nstars = 100;   // maximum number of stars to distribute
//int *dist = v.data();

std::default_random_engine generator;
std::discrete_distribution<int> distribution(v.begin(),v.end());


im2b.fill(QColor("black"));

std::cerr << im3.width() << " " << im3.height() << std::endl;
for (int i=0; i<ncells; ++i) {
int number = distribution(generator);
int px,py;
px=number/im3.height();
py=number-(im3.height()*px);
cells.append(QPoint(px,py));
//std::cerr << px << ", " << py << std::endl;
im2b.setPixelColor(QPoint(px,py),QColor("red"));
}


ui->imagedisplay->setPixmap(QPixmap::fromImage(im2b));
ui->imagedisplay->show();
}

void MainWindow::SaveCells(void) {
    QString fileName = QFileDialog::getSaveFileName(this,
        tr("Save Cells"), "/Users/roel/Documents/GitHub/CellDistrib/PhotoCellDistrib/Data/", tr("Cell Files (*.csv)"));
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return;

    QTextStream out(&file);

    out << "x,y,z,cell type\n";
    for (auto c: cells) {
       out << (double)c.x() << "," << (double)c.y() << "," << 0. << "," << ui->lineEdit->text() << "\n";
    }

}
