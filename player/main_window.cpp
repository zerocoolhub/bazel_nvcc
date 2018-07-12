#include "main_window.h"

#include "hellovulkantexture.h"

#include <QtWidgets/QtWidgets>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QGridLayout>

MainWindow::MainWindow(VulkanWindow *w)
    : m_window(w)
{
  QWidget *widget = new QWidget;
  setCentralWidget(widget);

  QWidget *wrapper = QWidget::createWindowContainer(w);

  slider = new QSlider(Qt::Horizontal);
  slider->setFocusPolicy(Qt::StrongFocus);
  slider->setTickPosition(QSlider::TicksBothSides);
  slider->setTickInterval(10);
  slider->setSingleStep(1);

  QGridLayout *gridLayout = new QGridLayout(widget);
  gridLayout->addWidget(wrapper, 0, 0, 0);
  gridLayout->setColumnStretch(0, 0);
  gridLayout->setColumnStretch(1, 0);
  gridLayout->addWidget(slider, 1, 0, 0);
  widget->setLayout(gridLayout);

  createActions();
  createMenus();

  setWindowTitle("Player");
}

void MainWindow::createActions() {
  openAction = new QAction(tr("&Open"), this);
  openAction->setShortcuts(QKeySequence::Open);
  openAction->setStatusTip(tr("Open movie file"));
  connect(openAction, &QAction::triggered, this, &MainWindow::openFile);
}

void MainWindow::createMenus() {
  fileMenu = menuBar()->addMenu(tr("&File"));
  fileMenu->addAction(openAction);
}

void MainWindow::openFile() {
  qDebug("open file\n");
}