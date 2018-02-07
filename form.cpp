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
    spinBoxSizeT->setMinimum(1);
    spinBoxSizeT->setMaximum(20);
    labelNT_1 = new QLabel(tr("Integration points"));
    labelNT_2 = new QLabel(tr("NT = "));
    labelNT_2->setAlignment(Qt::AlignRight);
    spinBoxNT = new QSpinBox();
    spinBoxNT->setMinimum(1);
    spinBoxNT->setMaximum(100);

    sliderSizeT = new QSlider(Qt::Horizontal);
    sliderSizeT->setRange(kTMin, kTMax);
    sliderSizeT->setSingleStep(1);
    sliderSizeT->setTickInterval(1);
    sliderSizeT->setTickPosition(QSlider::TicksBelow);
    sliderSizeT->setValue(1);

    sliderNT = new QSlider(Qt::Horizontal);
    sliderNT->setRange(kNtMin, kNtMax);
    sliderNT->setTickInterval(10);
    sliderNT->setTickPosition(QSlider::TicksBelow);
    sliderNT->setValue(kNtMin);

    labelDT_1 = new QLabel(tr("Integration step"));
    labelDT_2 = new QLabel(tr("∆t = "));
    labelDT_2->setAlignment(Qt::AlignRight);
    labelDT = new QLabel();

    QGridLayout *layoutParam = new QGridLayout();
    layoutParam->addWidget(labelEquation, 0, 0, 1, 1);
    layoutParam->addWidget(comboBoxEquation, 0, 1, 1, 3);
    layoutParam->addWidget(labelSizeT_1, 1, 0, 1, 1, Qt::AlignBaseline);
    layoutParam->addWidget(labelSizeT_2, 1, 1, 1, 1, Qt::AlignBaseline);
    layoutParam->addWidget(spinBoxSizeT, 1, 2, 1, 1);
    layoutParam->addWidget(sliderSizeT, 1, 3, 1, 1);
    layoutParam->addWidget(labelNT_1, 2, 0, 1, 1, Qt::AlignBaseline);
    layoutParam->addWidget(labelNT_2, 2, 1, 1, 1, Qt::AlignBaseline);
    layoutParam->addWidget(spinBoxNT, 2, 2, 1, 1);
    layoutParam->addWidget(sliderNT, 2, 3, 1, 1);
    layoutParam->addWidget(labelDT_1, 3, 0, 1, 1);
    layoutParam->addWidget(labelDT_2, 3, 1, 1, 1);
    layoutParam->addWidget(labelDT, 3, 2, 1, 1);

    setLayout(layoutParam);

    connect(sliderSizeT, SIGNAL(valueChanged(int)), this, SLOT(update_sizet(int)));
    connect(sliderNT, SIGNAL(valueChanged(int)), this, SLOT(update_nt(int)));
    connect(spinBoxSizeT, SIGNAL(valueChanged(int)), this, SLOT(update_sizet(int)));
    connect(spinBoxNT, SIGNAL(valueChanged(int)), this, SLOT(update_nt(int)));

}

Form::~Form()
{
}

void Form::update_sizet(int n)
{
    spinBoxSizeT->blockSignals(true);
    sliderSizeT->blockSignals(true);

    sliderSizeT->setValue(n);
    spinBoxSizeT->setValue(n);

    spinBoxSizeT->blockSignals(false);
    sliderSizeT->blockSignals(false);
}

void Form::update_nt(int n)
{
    spinBoxNT->blockSignals(true);
    sliderNT->blockSignals(true);

    sliderNT->setValue(n);
    spinBoxNT->setValue(n);

    spinBoxNT->blockSignals(false);
    sliderNT->blockSignals(false);
}
