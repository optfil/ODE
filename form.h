#ifndef FORM_H
#define FORM_H

#include <QWidget>
#include <QLabel>
#include <QComboBox>
#include <QSpinBox>

class Form : public QWidget
{
    Q_OBJECT

public:
    Form(QWidget *parent = 0);
    ~Form();

    enum EquationType {Decay, Grow, Oscillate};
    Q_ENUM(EquationType)

    QLabel *labelEquation;
    QComboBox *comboBoxEquation;
    QLabel *labelSizeT_1, *labelSizeT_2, *labelNT_1, *labelNT_2;
    QSpinBox *spinBoxSizeT, *spinBoxNT;
    QLabel *labelDT_1, *labelDT_2, *labelDT;
};

#endif // FORM_H
