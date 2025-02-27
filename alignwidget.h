#ifndef ALIGNWIDGET_H
#define ALIGNWIDGET_H

#include <QWidget>

#include "scales_parameters.h"


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

    void setAlignWidgetType(bool type, const DisplayParameters& display);
    bool getAlignWidgetType() const;

    void updateWeightInfo(const ScalesParameters& scalesParameters, const DisplayParameters& display);

signals:
    void finishAlignWidget();

private slots:
    void on_closeButton_released();
    void on_closeButton_clicked();
    void on_finishButton_released();

private:
    Ui::AlignWidget *ui;

    bool alignType = true; // true - with zero, false - with max weight
    bool verbose = false;
};

#endif // ALIGNWIDGET_H
