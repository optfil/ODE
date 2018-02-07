#ifndef FORM_H
#define FORM_H

#include <QWidget>
#include <QLabel>
#include <QComboBox>
#include <QSlider>
#include <QSpinBox>

#include "Parameters.h"


constexpr int kTMin = 1;
constexpr int kTMax = 20;
constexpr int kNtMin = 2;
constexpr int kNtMax = 100;

class Form : public QWidget
{
    Q_OBJECT

public:
    Form(QWidget *parent = 0);
    ~Form();

    enum EquationType {Decay, Grow, Oscillate};
    Q_ENUM(EquationType)

    enum MethodType {Euler, LeapFrog, TwoStep, RungeKutta};
    Q_ENUM(MethodType)

private slots:
    void update_nt(int n);
    void update_sizet(int n);
    void updateLabels();
    void initiateState();

private:
    QLabel *labelEquation;
    QComboBox *comboBoxEquation;
    QLabel *labelSizeT_1, *labelSizeT_2, *labelNT_1, *labelNT_2;
    QSpinBox *spinBoxSizeT, *spinBoxNT;
    QSlider *sliderSizeT, *sliderNT;
    QLabel *labelDT_1, *labelDT_2, *labelDT;

    Parameters *param;
    double t_cur_;
    MethodType method_;
};

#endif // FORM_H
