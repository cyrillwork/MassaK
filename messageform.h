#ifndef MESSAGEFORM_H
#define MESSAGEFORM_H

#include <QWidget>

namespace Ui {
class MessageForm;
}

class MessageForm : public QWidget
{
    Q_OBJECT

public:
    explicit MessageForm(QWidget *parent = nullptr);
    ~MessageForm();

    void setTextAndShow(int deviceStatus);

private slots:

    void on_closeButton_released();
    void on_closeButton_clicked();

private:
    Ui::MessageForm *ui;
};

#endif // MESSAGEFORM_H
