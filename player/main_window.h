
#include <QtWidgets/QMainWindow>

#include <fstream>

class QMenu;
class QSlider;
class QLabel;

class VulkanWindow;

// NOTE: Because we use Q_OBJECT pre-processor flag + signals/slots, will have to generate
// appropriate .cpp file for bazel.
class MainWindow : public QMainWindow {
    Q_OBJECT

 public:
	explicit MainWindow(VulkanWindow *w, VulkanWindow *w2);

signals:
    void valueChanged(int value);

public slots:
    void setValue(int value);

 private:
 	void createActions();	
	void createMenus();

	void openFile();

	QMenu *fileMenu;
	QAction *openAction;
	QLabel *myLabel;

	QString movieFile;
  	std::ifstream m_fpIn;

	VulkanWindow *m_windowOriginal;
	VulkanWindow *m_windowEdit;
	QSlider *slider;
};