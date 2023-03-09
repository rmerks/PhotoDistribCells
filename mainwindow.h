#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


public slots:
    void ReadImageFile(void);
    void DropCells(int ncells);
    void SaveCells();

private:
    Ui::MainWindow *ui;
    QImage *im;
    QList<QPoint> cells;


};
#endif // MAINWINDOW_H
