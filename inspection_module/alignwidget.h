#ifndef ALIGNWIDGET_H
#define ALIGNWIDGET_H

#include <QWidget>

namespace Ui {
class CheckingWidget;
}

class AlignWidget : public QWidget
{
    Q_OBJECT
public:
    explicit AlignWidget(QWidget *parent = nullptr);
    ~AlignWidget();

private slots:
    void on_closeButton_released();

    void on_closeButton_clicked();

private:
    Ui::CheckingWidget *ui;
};

#endif // ALIGNWIDGET_H
