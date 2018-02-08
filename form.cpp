#include "form.h"

#include <cmath>

#include <QLayout>

#include <QDebug>

static double direct(double t, Form::EquationType type)
{
    switch (type)
    {
    case Form::Decay:
        return std::exp(-t);
    case Form::Grow:
        return std::exp(t);
    case Form::Oscillate:
        return std::cos(t);
    }
    return 0.0;
}

static void setGrid(QValueAxis* ax)
{
    ax->setGridLineVisible(true);
    QPen pen = ax->gridLinePen();
    pen.setWidth(2);
    pen.setColor(Qt::gray);
    ax->setGridLinePen(pen);
}

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

    QValueAxis *axisXEulerSolution = new QValueAxis;
    axisXEulerSolution->setLineVisible(false);
    setGrid(axisXEulerSolution);
    axisXEulerSolution->setLabelsVisible(false);
    axisXEulerSolution->setRange(0.0, 1.0);
    eulerSolutionChart->addAxis(axisXEulerSolution, Qt::AlignBottom);
    seriesEulerIdeal->attachAxis(axisXEulerSolution);
    seriesEulerSolution->attachAxis(axisXEulerSolution);
    QValueAxis *axisYEulerSolution = new QValueAxis;
    axisYEulerSolution->setLineVisible(false);
    setGrid(axisYEulerSolution);
    axisYEulerSolution->setLabelsVisible(false);
    axisYEulerSolution->setRange(0.0, 1.003);
    eulerSolutionChart->addAxis(axisYEulerSolution, Qt::AlignLeft);
    seriesEulerIdeal->attachAxis(axisYEulerSolution);
    seriesEulerSolution->attachAxis(axisYEulerSolution);

    eulerSolution = new QChartView();
    eulerSolution->setRenderHint(QPainter::Antialiasing);
    eulerSolution->setChart(eulerSolutionChart);

    QChart *eulerLocalChart = new QChart();
    eulerLocalChart->addSeries(seriesEulerLocal);
    eulerLocalChart->setTitle(tr("Local error"));
    eulerLocalChart->legend()->hide();

    QValueAxis *axisXEulerLocal= new QValueAxis;
    axisXEulerLocal->setLineVisible(false);
    setGrid(axisXEulerLocal);
    axisXEulerLocal->setLabelsVisible(false);
    axisXEulerLocal->setRange(0.0, 1.0);
    eulerLocalChart->addAxis(axisXEulerLocal, Qt::AlignBottom);
    seriesEulerLocal->attachAxis(axisXEulerLocal);
    QValueAxis *axisYEulerLocal = new QValueAxis;
    axisYEulerLocal->setLineVisible(false);
    setGrid(axisYEulerLocal);
    axisYEulerLocal->setLabelsVisible(false);
    axisYEulerLocal->setRange(0.0, 1.003);
    eulerLocalChart->addAxis(axisYEulerLocal, Qt::AlignLeft);
    seriesEulerLocal->attachAxis(axisYEulerLocal);

    eulerLocal = new QChartView();
    eulerLocal->setRenderHint(QPainter::Antialiasing);
    eulerLocal->setChart(eulerLocalChart);

    QChart *eulerGlobalChart = new QChart();
    eulerGlobalChart->addSeries(seriesEulerGlobal);
    eulerGlobalChart->setTitle(tr("Global error"));
    eulerGlobalChart->legend()->hide();

    QValueAxis *axisXEulerGlobal= new QValueAxis;
    axisXEulerGlobal->setLineVisible(false);
    setGrid(axisXEulerGlobal);
    axisXEulerGlobal->setLabelsVisible(false);
    axisXEulerGlobal->setRange(0.0, 1.0);
    eulerGlobalChart->addAxis(axisXEulerGlobal, Qt::AlignBottom);
    seriesEulerGlobal->attachAxis(axisXEulerGlobal);
    QValueAxis *axisYEulerGlobal = new QValueAxis;
    axisYEulerGlobal->setLineVisible(false);
    setGrid(axisYEulerGlobal);
    axisYEulerGlobal->setLabelsVisible(false);
    axisYEulerGlobal->setRange(0.0, 1.003);
    eulerGlobalChart->addAxis(axisYEulerGlobal, Qt::AlignLeft);
    seriesEulerGlobal->attachAxis(axisYEulerGlobal);

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

    EquationType type = comboBoxEquation->currentData().value<EquationType>();

    QList<QPointF> ideal;
    for (int n = 0; n <= kIdealPoints; ++n)
    {
        double t = param->get_tmax() / kIdealPoints * n;
        ideal << QPointF(t / param->get_tmax(), direct(t, type));
    }

    seriesEulerIdeal->clear();
    seriesEulerIdeal->append(ideal);
    seriesLeapfrogIdeal->clear();
    seriesLeapfrogIdeal->append(ideal);
    seriesTwostepIdeal->clear();
    seriesTwostepIdeal->append(ideal);
    seriesRungekuttaIdeal->clear();
    seriesRungekuttaIdeal->append(ideal);

    seriesEulerSolution->clear();
    seriesLeapfrogSolution->clear();
    seriesTwostepSolution->clear();
    seriesRungekuttaSolution->clear();

    seriesEulerLocal->clear();
    seriesLeapfrogLocal->clear();
    seriesTwostepLocal->clear();
    seriesRungekuttaLocal->clear();

    seriesEulerGlobal->clear();
    seriesLeapfrogGlobal->clear();
    seriesTwostepGlobal->clear();
    seriesRungekuttaGlobal->clear();

    double yaxis_max = 0.0, yaxis_min = 0.0;
    switch (type)
    {
    case Decay:
        yaxis_max = 1.5;
        yaxis_min = -0.5;
        break;
    case Grow:
        yaxis_max = 3;
        yaxis_min = 0.0;
        break;
    case Oscillate:
        yaxis_max = 1.5;
        yaxis_min = -1.5;
        break;
    }
    eulerSolution->chart()->axisY()->setRange(yaxis_min, yaxis_max);/*
    leapfrogSolution->chart()->axisY()->setRange(yaxis_min, yaxis_max);
    twostepSolution->chart()->axisY()->setRange(yaxis_min, yaxis_max);
    rungekuttaSolution->chart()->axisY()->setRange(yaxis_min, yaxis_max);*/

    updateLabels();
    updateSolution();
}

void Form::updateSolution()
{
    method_ = static_cast<MethodType>(tabWidgetMethods->currentIndex());

}
