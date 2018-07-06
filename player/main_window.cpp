#include "main_window.h"

#include <QtWidgets/QtWidgets>
#include <QtWidgets/QVBoxLayout>

MainWindow::MainWindow()
{
    slider = new QSlider(Qt::Horizontal);
    slider->setFocusPolicy(Qt::StrongFocus);
    slider->setTickPosition(QSlider::TicksBothSides);
    slider->setTickInterval(10);
    slider->setSingleStep(1);

    QBoxLayout *slidersLayout = new QBoxLayout(QBoxLayout::TopToBottom);
    slidersLayout->addWidget(slider);
    setLayout(slidersLayout);
}
