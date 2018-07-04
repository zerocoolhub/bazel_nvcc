#ifndef SLIDERSGROUP_H
#define SLIDERSGROUP_H

#include <QtWidgets/QGroupBox>

class QDial;
class QScrollBar;
class QSlider;

class SlidersGroup : public QGroupBox
{
    // Q_OBJECT We're not using QTCreator, so we want this commented out

public:
    SlidersGroup(Qt::Orientation orientation, const QString &title,
                 QWidget *parent = 0);

signals:
    void valueChanged(int value);

public slots:
    void setValue(int value);
    void setMinimum(int value);
    void setMaximum(int value);
    void invertAppearance(bool invert);
    void invertKeyBindings(bool invert);

private:
    QSlider *slider;
    QScrollBar *scrollBar;
    QDial *dial;
};

#endif