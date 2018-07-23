#include "main_window.h"

#include "hellovulkantexture.h"

#include <QtWidgets/QtWidgets>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>

#include <opencv2/opencv.hpp>

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

  connect(slider, SIGNAL(valueChanged(int)), this, SLOT(setValue(int)));

  myLabel = new QLabel;
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
  movieFile = QFileDialog::getOpenFileName(this,
    tr("Open Movie File"), QDir::homePath(), tr("All files (*.*)"));
  // Need to go from QString to char* array for fpIn
  QByteArray movieNameBytes = movieFile.toLatin1();
  const char *movieNameChar = movieNameBytes.data(); 
     
  m_fpIn = std::ifstream(movieNameChar, std::ifstream::in | std::ifstream::binary);
  setValue(1);
}

void MainWindow::setValue(int value)
{
  printf("value: %d\n", value);


  int frameSize = 3686400; // 1280 * 720 * 4
  std::unique_ptr<uint8_t[]> pHostFrame(new uint8_t[frameSize]);
  m_fpIn.seekg(value * frameSize);

  uint8_t *buffer= new uint8_t[frameSize];
  std::streamsize nRead = m_fpIn.read(reinterpret_cast<char*>(buffer), frameSize).gcount();


  cv::Mat imageWithData = cv::Mat(720, 1280, CV_8UC4, buffer).clone();
  cvtColor(imageWithData, imageWithData, CV_BGR2RGBA);
  cv::imwrite("/home/lex/cv.jpg", imageWithData);

  m_window->updateFrame(buffer);
}
