#include "form.h"

#include <QLayout>

Form::Form(QWidget *parent)
    : QWidget(parent)
{
    labelEquation = new QLabel(tr("Equation type"));
    comboBoxEquation = new QComboBox();
    comboBoxEquation->addItem(tr("y' = -y"), QVariant(Decay));
    comboBoxEquation->addItem(tr("y' = y"), QVariant(Grow));
    comboBoxEquation->addItem(tr("y' = iy"), QVariant(Oscillate));

    labelSizeT_1 = new QLabel(tr("Integration time"));
    labelSizeT_2 = new QLabel(tr("T = "));
    labelSizeT_2->setAlignment(Qt::AlignRight);
    spinBoxSizeT = new QSpinBox();
    labelNT_1 = new QLabel(tr("Integration points"));
    labelNT_2 = new QLabel(tr("NT = "));
    labelNT_2->setAlignment(Qt::AlignRight);
    spinBoxNT = new QSpinBox();
    labelDT_1 = new QLabel(tr("Integration step"));
    labelDT_2 = new QLabel(tr("∆t = "));
    labelDT_2->setAlignment(Qt::AlignRight);
    labelDT = new QLabel();

    QGridLayout *layoutParam = new QGridLayout();
    layoutParam->addWidget(labelEquation, 0, 0, 1, 1);
    layoutParam->addWidget(comboBoxEquation, 0, 1, 1, 2);
    layoutParam->addWidget(labelSizeT_1, 1, 0, 1, 1);
    layoutParam->addWidget(labelSizeT_2, 1, 1, 1, 1);
    layoutParam->addWidget(spinBoxSizeT, 1, 2, 1, 1);
    layoutParam->addWidget(labelNT_1, 2, 0, 1, 1);
    layoutParam->addWidget(labelNT_2, 2, 1, 1, 1);
    layoutParam->addWidget(spinBoxNT, 2, 2, 1, 1);
    layoutParam->addWidget(labelDT_1, 3, 0, 1, 1);
    layoutParam->addWidget(labelDT_2, 3, 1, 1, 1);
    layoutParam->addWidget(labelDT, 3, 2, 1, 1);

    setLayout(layoutParam);
}

Form::~Form()
{
}
