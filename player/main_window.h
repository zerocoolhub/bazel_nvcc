#include <QtWidgets/QMainWindow>

class VulkanWindow;

class QMenu;
class QSlider;

class MainWindow : public QMainWindow
{
public:
    explicit MainWindow(VulkanWindow *w);

private:
	QMenu *fileMenu;
    QSlider *slider;
    VulkanWindow *m_window;

	void createMenus();
};