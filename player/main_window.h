#include <QtWidgets/QWidget>

class VulkanWindow;

class QSlider;

class MainWindow : public QWidget
{
public:
    explicit MainWindow(VulkanWindow *w, VulkanWindow *w2);

private:
    QSlider *slider;
    VulkanWindow *m_window;
    VulkanWindow *m_window2;
};