#include <QtWidgets/QWidget>

class VulkanWindow;

class QSlider;

class MainWindow : public QWidget
{
public:
    explicit MainWindow(VulkanWindow *w);

private:
    QSlider *slider;
    VulkanWindow *m_window;
};