#include <QtWidgets/QMainWindow>

class QMenu;
class QSlider;

class VulkanWindow;

class MainWindow : public QMainWindow {
 public:
	explicit MainWindow(VulkanWindow *w);

 private:
	void createMenus();

	QMenu *fileMenu;
	QSlider *slider;
	VulkanWindow *m_window;
};