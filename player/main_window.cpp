#include "main_window.h"

#include "hellovulkantexture.h"

#include <QtWidgets/QtWidgets>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>

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

  myLabel = new QLabel;
  QGridLayout *gridLayout = new QGridLayout(widget);
  gridLayout->addWidget(myLabel, 0, 0, 0);
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
  movieFile = QFileDialog::getOpenFileName(this,
    tr("Open Movie File"), QDir::homePath(), tr("All files (*.*)"));
      
  std::ifstream fpIn((char *)movieFile.data(), std::ifstream::in | std::ifstream::binary);

  int frameSize = 3686400; // 1280 * 720 * 4
  std::unique_ptr<uint8_t[]> pHostFrame(new uint8_t[frameSize]);
  std::streamsize nRead = fpIn.read(reinterpret_cast<char*>(pHostFrame.get()), frameSize).gcount();
  QImage image("/home/lex/test.jpg");

  myLabel->setPixmap(QPixmap::fromImage(image));

  qDebug() << movieFile; 
}