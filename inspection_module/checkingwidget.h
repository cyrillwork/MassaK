#ifndef CHECKINGWIDGET_H
#define CHECKINGWIDGET_H

#include <QWidget>

namespace Ui {
class CheckingWidget;
}

class CheckingWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CheckingWidget(QWidget *parent = nullptr);
    ~CheckingWidget();

private:
    Ui::CheckingWidget *ui;
};

#endif // CHECKINGWIDGET_H
