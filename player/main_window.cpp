#include "main_window.h"

#include "hellovulkantexture.h"

#include <QtWidgets/QtWidgets>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QGridLayout>

MainWindow::MainWindow(VulkanWindow *w, VulkanWindow *w2)
    : m_window(w), m_window2(w2)
{

    QWidget *wrapper = QWidget::createWindowContainer(w);
    QWidget *wrapper2 = QWidget::createWindowContainer(w2);

    slider = new QSlider(Qt::Horizontal);
    slider->setFocusPolicy(Qt::StrongFocus);
    slider->setTickPosition(QSlider::TicksBothSides);
    slider->setTickInterval(10);
    slider->setSingleStep(1);

    QGridLayout *gridLayout = new QGridLayout(this);
    gridLayout->addWidget(wrapper, 0, 0, 0);
    gridLayout->addWidget(wrapper2, 0, 1, 0);
    gridLayout->setColumnStretch(0, 1);
    gridLayout->setColumnStretch(1, 1);
    gridLayout->addWidget(slider, 1, 0, 0);
    setLayout(gridLayout);
}
