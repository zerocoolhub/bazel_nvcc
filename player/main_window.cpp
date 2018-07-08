#include "main_window.h"

#include "hellovulkantexture.h"

#include <QtWidgets/QtWidgets>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QGridLayout>

MainWindow::MainWindow(VulkanWindow *w)
    : m_window(w)
{

    QWidget *wrapper = QWidget::createWindowContainer(w);

    slider = new QSlider(Qt::Horizontal);
    slider->setFocusPolicy(Qt::StrongFocus);
    slider->setTickPosition(QSlider::TicksBothSides);
    slider->setTickInterval(10);
    slider->setSingleStep(1);

    QGridLayout *gridLayout = new QGridLayout(this);
    gridLayout->addWidget(wrapper, 0, 0, 0);
    gridLayout->setColumnStretch(0, 0);
    gridLayout->setColumnStretch(1, 0);
    gridLayout->addWidget(slider, 1, 0, 0);
    setLayout(gridLayout);
}
