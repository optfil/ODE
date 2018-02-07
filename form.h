#ifndef FORM_H
#define FORM_H

#include <QWidget>
#include <QLabel>
#include <QComboBox>
#include <QSlider>
#include <QSpinBox>


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

private slots:
    void update_nt(int n);
    void update_sizet(int n);

private:
    QLabel *labelEquation;
    QComboBox *comboBoxEquation;
    QLabel *labelSizeT_1, *labelSizeT_2, *labelNT_1, *labelNT_2;
    QSpinBox *spinBoxSizeT, *spinBoxNT;
    QSlider *sliderSizeT, *sliderNT;
    QLabel *labelDT_1, *labelDT_2, *labelDT;
};

#endif // FORM_H
