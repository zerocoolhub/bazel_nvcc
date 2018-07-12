#include <QtWidgets/QMainWindow>

class QMenu;
class QSlider;

class VulkanWindow;

class MainWindow : public QMainWindow {
 public:
	explicit MainWindow(VulkanWindow *w);

 private:
 	void createActions();	
	void createMenus();

	void openFile();

	QMenu *fileMenu;
	QAction *openAction;

	QString movieFile;

	VulkanWindow *m_window;
	QSlider *slider;
};