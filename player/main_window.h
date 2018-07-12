
#include <QtWidgets/QMainWindow>

#include <fstream>

class QMenu;
class QSlider;
class QLabel;

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
	QLabel *myLabel;

	QString movieFile;
	std::ifstream fpIn;	

	VulkanWindow *m_window;
	QSlider *slider;
};