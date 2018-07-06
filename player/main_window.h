#include <QtWidgets/QWidget>

class QSlider;

class MainWindow : public QWidget
{
public:
    explicit MainWindow();

private:
    QSlider *slider;
};