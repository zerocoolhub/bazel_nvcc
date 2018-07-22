// Simple C++ program to display "Hello World"
 
// Header file for input output functions
#include<iostream> 
#include <fstream>

#include <opencv2/opencv.hpp>

// Simply read the first frame in a decoded video and save it as an image via OpenCV,
// to show that we can successfully deal with the decoded video as simple RGBA frames. 
int main()
{
  std::ifstream fpIn("/home/lex/bazel_nvcc/out.h264", std::ifstream::in | std::ifstream::binary);

  int frameSize = 3686400; // 1280 * 720 * 4
  std::unique_ptr<uint8_t[]> pHostFrame(new uint8_t[frameSize]);
  std::streamsize nRead = fpIn.read(reinterpret_cast<char*>(pHostFrame.get()), frameSize).gcount();

  cv::Mat imageWithData = cv::Mat(720, 1280, CV_8UC4, pHostFrame.get()).clone();
  cvtColor(imageWithData, imageWithData, CV_BGR2RGBA);
  cv::imwrite("/home/lex/cv.jpg", imageWithData);

  return 0;
}