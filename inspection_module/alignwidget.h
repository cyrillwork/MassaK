#ifndef ALIGNWIDGET_H
#define ALIGNWIDGET_H

#include <QWidget>

//#include "mainwindow.h"


namespace Ui {
class AlignWidget;
class MainWindow;
}

class AlignWidget : public QWidget
{
    Q_OBJECT
public:
    explicit AlignWidget(QWidget *parent = nullptr);
    ~AlignWidget();
    void connectMainWindow(QWidget *parent);

signals:
    void finishAlignWidget();

private slots:
    void on_closeButton_released();

    void on_closeButton_clicked();

    void on_finishButton_released();

private:
    Ui::AlignWidget *ui;
};

#endif // ALIGNWIDGET_H
