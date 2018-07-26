#include "main_window.h"

#include "hellovulkantexture.h"

#include <cuda_utils/NvCodecUtils.h>
#include <cuda_kernels/overlay_kernel.h>

#include <QtWidgets/QtWidgets>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>

#include <opencv2/opencv.hpp>

MainWindow::MainWindow(VulkanWindow *w, VulkanWindow *w2)
    : m_windowOriginal(w), m_windowEdit(w2)
{
  QWidget *widget = new QWidget;
  setCentralWidget(widget);

  QWidget *wrapper = QWidget::createWindowContainer(w);
  QWidget *wrapper2 = QWidget::createWindowContainer(w2);

  slider = new QSlider(Qt::Horizontal);
  slider->setFocusPolicy(Qt::StrongFocus);
  slider->setTickPosition(QSlider::TicksBothSides);
  slider->setTickInterval(10);
  slider->setSingleStep(1);

  connect(slider, SIGNAL(valueChanged(int)), this, SLOT(setValue(int)));

  myLabel = new QLabel;
  QGridLayout *gridLayout = new QGridLayout(widget);
  gridLayout->addWidget(wrapper, 0, 0, 0);
  gridLayout->addWidget(wrapper2, 0, 1, 0);
  gridLayout->setColumnStretch(0, 1);
  gridLayout->setColumnStretch(1, 1);
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

  // Not using smart pointers right now, creates weird artifacts in texture,
  // perhaps due to premature release.
  // std::unique_ptr<uint8_t[]> pHostFrame(new uint8_t[frameSize]);
  m_fpIn.seekg(value * frameSize);

  uint8_t *buffer = new uint8_t[frameSize];
  std::streamsize nRead = m_fpIn.read(reinterpret_cast<char*>(buffer), frameSize).gcount();
  uint8_t *buffer2 = new uint8_t[frameSize];
  // Copy the original frame over
  memcpy(buffer2, buffer, frameSize);

  cv::Mat cv_overlay_image_rgba = cv::imread("/home/lex/bazel_nvcc/overlay1.png", CV_LOAD_IMAGE_COLOR);
  cv::cvtColor(cv_overlay_image_rgba, cv_overlay_image_rgba, CV_BGR2RGBA);
  uchar4 *overlayImageRawSrc = (uchar4 *)cv_overlay_image_rgba.ptr<unsigned char>(0);

  int size = 160*314; // Size of overlay img
  uchar4 *overlayImageRaw = new uchar4[160*314];
  for (int i = 0; i < size; i++) {
    overlayImageRaw[i].x = overlayImageRawSrc[i].x;
    overlayImageRaw[i].y = overlayImageRawSrc[i].y;
    overlayImageRaw[i].z = overlayImageRawSrc[i].z;
    overlayImageRaw[i].w = 255;
  }
  struct OverlayImage overlay = {overlayImageRaw, 160, 314, 0, 0};
  cv::Mat imageWithData = cv::Mat(720, 1280, CV_8UC4, buffer).clone();
  _overlay_image(reinterpret_cast<uchar4*>(buffer2), 720, 1280, overlay);

  delete overlayImageRaw;
  /*
  Save frames as images, for debugging purposes
  cv::Mat imageWithData = cv::Mat(720, 1280, CV_8UC4, buffer).clone();*/
  //cvtColor(imageWithData, imageWithData, CV_BGR2RGBA);
  //cv::imwrite("/home/lex/cv.jpg", imageWithData);

  m_windowOriginal->updateFrame(reinterpret_cast<uint8_t*>(buffer));
  m_windowEdit->updateFrame(reinterpret_cast<uint8_t*>(buffer2));
}
