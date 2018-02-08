#include "form.h"

#include <QLayout>

#include <QDebug>

Form::Form(QWidget *parent)
    : QWidget(parent), param(nullptr), t_cur_(0.0)
{
    seriesEulerIdeal = new QLineSeries();
    seriesEulerIdeal->setColor(Qt::blue);
    seriesEulerIdeal->setPen(QPen(seriesEulerIdeal->pen().brush(), 3));
    seriesLeapfrogIdeal = new QLineSeries();
    seriesLeapfrogIdeal->setColor(Qt::blue);
    seriesLeapfrogIdeal->setPen(QPen(seriesLeapfrogIdeal->pen().brush(), 3));
    seriesTwostepIdeal = new QLineSeries();
    seriesTwostepIdeal->setColor(Qt::blue);
    seriesTwostepIdeal->setPen(QPen(seriesTwostepIdeal->pen().brush(), 3));
    seriesRungekuttaIdeal = new QLineSeries();
    seriesRungekuttaIdeal->setColor(Qt::blue);
    seriesRungekuttaIdeal->setPen(QPen(seriesRungekuttaIdeal->pen().brush(), 3));
    seriesEulerSolution = new QLineSeries();
    seriesEulerSolution->setColor(Qt::red);
    seriesEulerSolution->setPen(QPen(seriesEulerSolution->pen().brush(), 3));
    seriesLeapfrogSolution = new QLineSeries();
    seriesLeapfrogSolution->setColor(Qt::red);
    seriesLeapfrogSolution->setPen(QPen(seriesLeapfrogSolution->pen().brush(), 3));
    seriesTwostepSolution = new QLineSeries();
    seriesTwostepSolution->setColor(Qt::red);
    seriesTwostepSolution->setPen(QPen(seriesTwostepSolution->pen().brush(), 3));
    seriesRungekuttaSolution = new QLineSeries();
    seriesRungekuttaSolution->setColor(Qt::red);
    seriesRungekuttaSolution->setPen(QPen(seriesRungekuttaSolution->pen().brush(), 3));
    seriesEulerLocal = new QLineSeries();
    seriesEulerLocal->setColor(Qt::red);
    seriesEulerLocal->setPen(QPen(seriesEulerLocal->pen().brush(), 3));
    seriesLeapfrogLocal= new QLineSeries();
    seriesLeapfrogLocal->setColor(Qt::red);
    seriesLeapfrogLocal->setPen(QPen(seriesLeapfrogLocal->pen().brush(), 3));
    seriesTwostepLocal = new QLineSeries();
    seriesTwostepLocal->setColor(Qt::red);
    seriesTwostepLocal->setPen(QPen(seriesTwostepLocal->pen().brush(), 3));
    seriesRungekuttaLocal = new QLineSeries();
    seriesRungekuttaLocal->setColor(Qt::red);
    seriesRungekuttaLocal->setPen(QPen(seriesRungekuttaLocal->pen().brush(), 3));
    seriesEulerGlobal = new QLineSeries();
    seriesEulerGlobal->setColor(Qt::red);
    seriesEulerGlobal->setPen(QPen(seriesEulerGlobal->pen().brush(), 3));
    seriesLeapfrogGlobal= new QLineSeries();
    seriesLeapfrogGlobal->setColor(Qt::red);
    seriesLeapfrogGlobal->setPen(QPen(seriesLeapfrogGlobal->pen().brush(), 3));
    seriesTwostepGlobal = new QLineSeries();
    seriesTwostepGlobal->setColor(Qt::red);
    seriesTwostepGlobal->setPen(QPen(seriesTwostepGlobal->pen().brush(), 3));
    seriesRungekuttaGlobal = new QLineSeries();
    seriesRungekuttaGlobal->setColor(Qt::red);
    seriesRungekuttaGlobal->setPen(QPen(seriesRungekuttaGlobal->pen().brush(), 3));

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
    spinBoxNT->setMinimum(2);
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
    labelDT_1->setAlignment(Qt::AlignTop);
    labelDT_2 = new QLabel(tr("∆t = "));
    labelDT_2->setAlignment(Qt::AlignRight | Qt::AlignTop);
    labelDT = new QLabel();
    labelDT->setAlignment(Qt::AlignTop);

    QWidget *eulerWidget = new QWidget();

    QChart *eulerSolutionChart = new QChart();
    eulerSolutionChart->addSeries(seriesEulerIdeal);
    eulerSolutionChart->addSeries(seriesEulerSolution);
    eulerSolutionChart->setTitle(tr("Solution"));
    eulerSolutionChart->legend()->hide();
    // add axis, grid
    eulerSolution = new QChartView();
    eulerSolution->setRenderHint(QPainter::Antialiasing);
    eulerSolution->setChart(eulerSolutionChart);

    QChart *eulerLocalChart = new QChart();
    eulerLocalChart->addSeries(seriesEulerLocal);
    eulerLocalChart->setTitle(tr("Local error"));
    eulerLocalChart->legend()->hide();
    // add axis, grid
    eulerLocal = new QChartView();
    eulerLocal->setRenderHint(QPainter::Antialiasing);
    eulerLocal->setChart(eulerLocalChart);

    QChart *eulerGlobalChart = new QChart();
    eulerGlobalChart->addSeries(seriesEulerGlobal);
    eulerGlobalChart->setTitle(tr("Global error"));
    eulerGlobalChart->legend()->hide();
    // add axis, grid
    eulerGlobal = new QChartView();
    eulerGlobal->setRenderHint(QPainter::Antialiasing);
    eulerGlobal->setChart(eulerGlobalChart);

    QVBoxLayout *layoutEuler = new QVBoxLayout();
    layoutEuler->addWidget(eulerSolution);
    layoutEuler->addWidget(eulerLocal);
    layoutEuler->addWidget(eulerGlobal);
    eulerWidget->setLayout(layoutEuler);

    QWidget *leapfrogWidget = new QWidget();
    QWidget *twostepWidget = new QWidget();
    QWidget *rungekuttaWidget = new QWidget();

    tabWidgetMethods = new QTabWidget();
    tabWidgetMethods->addTab(eulerWidget, tr("Euler"));
    tabWidgetMethods->addTab(leapfrogWidget, tr("Leap frog"));
    tabWidgetMethods->addTab(twostepWidget, tr("Two-step"));
    tabWidgetMethods->addTab(rungekuttaWidget, tr("Runge-Kutta"));

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

    QHBoxLayout *layoutMain = new QHBoxLayout();
    layoutMain->addLayout(layoutParam);
    layoutMain->addWidget(tabWidgetMethods);

    setLayout(layoutMain);

    connect(comboBoxEquation, SIGNAL(currentIndexChanged(int)), this, SLOT(initiateState()));
    connect(sliderSizeT, SIGNAL(valueChanged(int)), this, SLOT(update_sizet(int)));
    connect(sliderNT, SIGNAL(valueChanged(int)), this, SLOT(update_nt(int)));
    connect(spinBoxSizeT, SIGNAL(valueChanged(int)), this, SLOT(update_sizet(int)));
    connect(spinBoxNT, SIGNAL(valueChanged(int)), this, SLOT(update_nt(int)));
    connect(tabWidgetMethods, SIGNAL(currentChanged(int)), this, SLOT(updateSolution()));

    initiateState();

    qDebug() << labelNT_1->alignment() << labelNT_2->alignment() << spinBoxNT->alignment();
    qDebug() << labelDT_1->alignment() << labelDT_2->alignment() << labelDT->alignment();
}

Form::~Form()
{
    delete param;
}

void Form::update_sizet(int n)
{
    spinBoxSizeT->blockSignals(true);
    sliderSizeT->blockSignals(true);

    sliderSizeT->setValue(n);
    spinBoxSizeT->setValue(n);

    spinBoxSizeT->blockSignals(false);
    sliderSizeT->blockSignals(false);

    initiateState();
}

void Form::update_nt(int n)
{
    spinBoxNT->blockSignals(true);
    sliderNT->blockSignals(true);

    sliderNT->setValue(n);
    spinBoxNT->setValue(n);

    spinBoxNT->blockSignals(false);
    sliderNT->blockSignals(false);

    initiateState();
}

void Form::updateLabels()
{
    labelDT->setText(QString::number(param->get_tstep(), 'f', 3));
}

void Form::initiateState()
{
    delete param;
    param = new Parameters(spinBoxSizeT->value(), spinBoxNT->value());

    updateLabels();
}

void Form::updateSolution()
{
    method_ = static_cast<MethodType>(tabWidgetMethods->currentIndex());
}
