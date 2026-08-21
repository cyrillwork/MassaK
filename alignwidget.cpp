#include "alignwidget.h"
#include "ui_alignwidget.h"

#include <iostream>
#include <QPalette>
#include <QColor>

#include "mainwindow.h"

AlignWidget::AlignWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AlignWidget)
{
    ui->setupUi(this);
    setWindowFlag(Qt::FramelessWindowHint, true);
    //setWindowState(Qt::WindowFullScreen);
    //showFullScreen();

    // QPixmap pixmap("quit.png");
    // QIcon ButtonIcon(pixmap);
    // ui->closeButton->setIcon(ButtonIcon);
    // ui->closeButton->setIconSize(pixmap.rect().size());
    // auto rrr = pixmap.rect().size();
    // rrr.setHeight(rrr.height() + 10);
    // rrr.setWidth(rrr.width() + 10);
    // ui->closeButton->setFixedSize(rrr);


    // QPalette palette = ui->finishButton->palette();
    // QString hexValue = "#2E5596";
    // QColor color_b;
    // color_b.setRgb(QColor(hexValue).rgb());
    // palette.setColor(QPalette::Button, color_b);
    // //palette.setColor(QPalette::Button, QColor(Qt::darkBlue));

    // palette.setColor(QPalette::ButtonText, QColor(Qt::white));
    // ui->finishButton->setPalette(palette);
    // //ui->finishButton->show();

    ui->versionLabel->setText(PROGRAM_VERSION);
}

AlignWidget::~AlignWidget()
{
    delete ui;
}

void AlignWidget::connectMainWindow(QWidget *parent)
{
    connect(this, &AlignWidget::finishAlignWidget, (MainWindow*)parent, &MainWindow::on_finishAlignWidget);
    connect(this, &AlignWidget::closeApplication,  (MainWindow*)parent, &MainWindow::on_closeButton_released);
}

void AlignWidget::setAlignWidgetType(bool type, const DisplayParameters& display)
{
    if(type) {
        ui->titleLabel->setText("Юстировка при нулевой нагрузке");
        ui->infoLabel->setText("- При успокоившихся весах нажать \"Продолжить\"");
        ui->w_clbLabel->setText("- Разгрузить весы");
        ui->finishButton->setText("Продолжить");
    } else {
        ui->titleLabel->setText("Юстировка под нагрузкой");
        ui->infoLabel->setText("- При успокоившихся весах нажать \"Готово\"");
        {
            std::string _temp = "- Установить нагрузку " + display.weight_clb;
            ui->w_clbLabel->setText(QString(_temp.c_str()));
        }
        ui->finishButton->setText("Готово");
    }
}

void AlignWidget::on_closeButton_released()
{
    if(verbose) {
        std::cout << "on_closeButton_released" << std::endl;
    }

    emit closeApplication();
}

void AlignWidget::on_finishButton_released()
{
    if(verbose) {
        std::cout << "on_finishButton_released" << std::endl;
    }
    emit finishAlignWidget();
}

bool AlignWidget::getAlignWidgetType() const
{
    return alignType;
}

void AlignWidget::updateWeightInfo(const ScalesParameters& scalesParameters, const DisplayParameters& display)
{
    { //set Massa
        char _buff[32] = {};
        double weight = scalesParameters.weight * 0.001;

        ::sprintf(_buff, "%.3f", weight);

        if(scalesParameters.weight_stable) {
            ui->kgLabel->setText( "kg" );
            //::sprintf(_buff, "%.3f kg", weight);
        } else {
            ui->kgLabel->setText( "  " );
            //::sprintf(_buff, "%.3f", weight);
        }

//        QString weight_temp(_buff);
//        ui->weightLabel->setText(weight_temp);

        ui->weightLabel->setText( QString(_buff) );
    }


    { //labels        
      //  ui->zeroLabel->setText(QString(display.weight_zero.c_str()));
      //  ui->netLabel->setText(QString(display.weight_net.c_str()));
    }
}


void AlignWidget::on_closeButton_clicked()
{

}

