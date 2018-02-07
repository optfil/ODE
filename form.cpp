#include "form.h"

#include <QLayout>

Form::Form(QWidget *parent)
    : QWidget(parent)
{
    comboBoxEquation = new QComboBox();
    comboBoxEquation->addItem(tr("y' = -y"), QVariant(Decay));
    comboBoxEquation->addItem(tr("y' = y"), QVariant(Grow));
    comboBoxEquation->addItem(tr("y' = iy"), QVariant(Oscillate));

    labelSizeT_1 = new QLabel(tr("Integration time"));
    labelSizeT_2 = new QLabel(tr("T = "));
    spinBoxSizeT = new QSpinBox();
    labelNT_1 = new QLabel(tr("Integration points"));
    labelNT_2 = new QLabel(tr("NT = "));
    spinBoxNT = new QSpinBox();
    labelDT_1 = new QLabel(tr("Integration step"));
    labelDT_2 = new QLabel(tr("∆t = "));
    labelDT = new QLabel();

    QGridLayout *layoutT = new QGridLayout();
    layoutT->addWidget(labelSizeT_1, 0, 0, 1, 1);
    layoutT->addWidget(labelSizeT_2, 0, 1, 1, 1);
    layoutT->addWidget(spinBoxSizeT, 0, 2, 1, 1);
    layoutT->addWidget(labelNT_1, 1, 0, 1, 1);
    layoutT->addWidget(labelNT_2, 1, 1, 1, 1);
    layoutT->addWidget(spinBoxNT, 1, 2, 1, 1);
    layoutT->addWidget(labelDT_1, 2, 0, 1, 1);
    layoutT->addWidget(labelDT_2, 2, 1, 1, 1);
    layoutT->addWidget(labelDT, 2, 2, 1, 1);

    QVBoxLayout *layoutParam = new QVBoxLayout();
    layoutParam->addWidget(comboBoxEquation);
    layoutParam->addLayout(layoutT);

    setLayout(layoutParam);
}

Form::~Form()
{
}
