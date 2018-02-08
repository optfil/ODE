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

static std::complex<double> func(std::complex<double> y, double /* t */, Form::EquationType type)
{
    switch (type)
    {
    case Form::Decay:
        return -y;
    case Form::Grow:
        return y;
    case Form::Oscillate:
        return std::complex<double>(0.0, 1.0) * y;
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
    : QWidget(parent), param(nullptr),
      t_cur_(0.0), euler_y_(1.0, 0.0), leapfrog_y_(1.0, 0.0), leapfrog_new_y_(1.0, 0.0), twostep_y_(1.0, 0.0), rungekutta_y_(1.0, 0.0)
{
    timer = new QTimer();
    timer->setInterval(30);

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

    pushButtonSolve = new QPushButton(tr("Start"));

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

    QChart *leapfrogSolutionChart = new QChart();
    leapfrogSolutionChart->addSeries(seriesLeapfrogIdeal);
    leapfrogSolutionChart->addSeries(seriesLeapfrogSolution);
    leapfrogSolutionChart->setTitle(tr("Solution"));
    leapfrogSolutionChart->legend()->hide();

    QValueAxis *axisXLeapfrogSolution = new QValueAxis;
    axisXLeapfrogSolution->setLineVisible(false);
    setGrid(axisXLeapfrogSolution);
    axisXLeapfrogSolution->setLabelsVisible(false);
    axisXLeapfrogSolution->setRange(0.0, 1.0);
    leapfrogSolutionChart->addAxis(axisXLeapfrogSolution, Qt::AlignBottom);
    seriesLeapfrogIdeal->attachAxis(axisXLeapfrogSolution);
    seriesLeapfrogSolution->attachAxis(axisXLeapfrogSolution);
    QValueAxis *axisYLeapfrogSolution = new QValueAxis;
    axisYLeapfrogSolution->setLineVisible(false);
    setGrid(axisYLeapfrogSolution);
    axisYLeapfrogSolution->setLabelsVisible(false);
    axisYLeapfrogSolution->setRange(0.0, 1.003);
    leapfrogSolutionChart->addAxis(axisYLeapfrogSolution, Qt::AlignLeft);
    seriesLeapfrogIdeal->attachAxis(axisYLeapfrogSolution);
    seriesLeapfrogSolution->attachAxis(axisYLeapfrogSolution);

    leapfrogSolution = new QChartView();
    leapfrogSolution->setRenderHint(QPainter::Antialiasing);
    leapfrogSolution->setChart(leapfrogSolutionChart);

    QChart *leapfrogLocalChart = new QChart();
    leapfrogLocalChart->addSeries(seriesLeapfrogLocal);
    leapfrogLocalChart->setTitle(tr("Local error"));
    leapfrogLocalChart->legend()->hide();

    QValueAxis *axisXLeapfrogLocal= new QValueAxis;
    axisXLeapfrogLocal->setLineVisible(false);
    setGrid(axisXLeapfrogLocal);
    axisXLeapfrogLocal->setLabelsVisible(false);
    axisXLeapfrogLocal->setRange(0.0, 1.0);
    leapfrogLocalChart->addAxis(axisXLeapfrogLocal, Qt::AlignBottom);
    seriesLeapfrogLocal->attachAxis(axisXLeapfrogLocal);
    QValueAxis *axisYLeapfrogLocal = new QValueAxis;
    axisYLeapfrogLocal->setLineVisible(false);
    setGrid(axisYLeapfrogLocal);
    axisYLeapfrogLocal->setLabelsVisible(false);
    axisYLeapfrogLocal->setRange(0.0, 1.003);
    leapfrogLocalChart->addAxis(axisYLeapfrogLocal, Qt::AlignLeft);
    seriesLeapfrogLocal->attachAxis(axisYLeapfrogLocal);

    leapfrogLocal = new QChartView();
    leapfrogLocal->setRenderHint(QPainter::Antialiasing);
    leapfrogLocal->setChart(leapfrogLocalChart);

    QChart *leapfrogGlobalChart = new QChart();
    leapfrogGlobalChart->addSeries(seriesLeapfrogGlobal);
    leapfrogGlobalChart->setTitle(tr("Global error"));
    leapfrogGlobalChart->legend()->hide();

    QValueAxis *axisXLeapfrogGlobal= new QValueAxis;
    axisXLeapfrogGlobal->setLineVisible(false);
    setGrid(axisXLeapfrogGlobal);
    axisXLeapfrogGlobal->setLabelsVisible(false);
    axisXLeapfrogGlobal->setRange(0.0, 1.0);
    leapfrogGlobalChart->addAxis(axisXLeapfrogGlobal, Qt::AlignBottom);
    seriesLeapfrogGlobal->attachAxis(axisXLeapfrogGlobal);
    QValueAxis *axisYLeapfrogGlobal = new QValueAxis;
    axisYLeapfrogGlobal->setLineVisible(false);
    setGrid(axisYLeapfrogGlobal);
    axisYLeapfrogGlobal->setLabelsVisible(false);
    axisYLeapfrogGlobal->setRange(0.0, 1.003);
    leapfrogGlobalChart->addAxis(axisYLeapfrogGlobal, Qt::AlignLeft);
    seriesLeapfrogGlobal->attachAxis(axisYLeapfrogGlobal);

    leapfrogGlobal = new QChartView();
    leapfrogGlobal->setRenderHint(QPainter::Antialiasing);
    leapfrogGlobal->setChart(leapfrogGlobalChart);

    QVBoxLayout *layoutLeapfrog = new QVBoxLayout();
    layoutLeapfrog->addWidget(leapfrogSolution);
    layoutLeapfrog->addWidget(leapfrogLocal);
    layoutLeapfrog->addWidget(leapfrogGlobal);
    leapfrogWidget->setLayout(layoutLeapfrog);

    QWidget *twostepWidget = new QWidget();

    QChart *twostepSolutionChart = new QChart();
    twostepSolutionChart->addSeries(seriesTwostepIdeal);
    twostepSolutionChart->addSeries(seriesTwostepSolution);
    twostepSolutionChart->setTitle(tr("Solution"));
    twostepSolutionChart->legend()->hide();

    QValueAxis *axisXTwostepSolution = new QValueAxis;
    axisXTwostepSolution->setLineVisible(false);
    setGrid(axisXTwostepSolution);
    axisXTwostepSolution->setLabelsVisible(false);
    axisXTwostepSolution->setRange(0.0, 1.0);
    twostepSolutionChart->addAxis(axisXTwostepSolution, Qt::AlignBottom);
    seriesTwostepIdeal->attachAxis(axisXTwostepSolution);
    seriesTwostepSolution->attachAxis(axisXTwostepSolution);
    QValueAxis *axisYTwostepSolution = new QValueAxis;
    axisYTwostepSolution->setLineVisible(false);
    setGrid(axisYTwostepSolution);
    axisYTwostepSolution->setLabelsVisible(false);
    axisYTwostepSolution->setRange(0.0, 1.003);
    twostepSolutionChart->addAxis(axisYTwostepSolution, Qt::AlignLeft);
    seriesTwostepIdeal->attachAxis(axisYTwostepSolution);
    seriesTwostepSolution->attachAxis(axisYTwostepSolution);

    twostepSolution = new QChartView();
    twostepSolution->setRenderHint(QPainter::Antialiasing);
    twostepSolution->setChart(twostepSolutionChart);

    QChart *twostepLocalChart = new QChart();
    twostepLocalChart->addSeries(seriesTwostepLocal);
    twostepLocalChart->setTitle(tr("Local error"));
    twostepLocalChart->legend()->hide();

    QValueAxis *axisXTwostepLocal= new QValueAxis;
    axisXTwostepLocal->setLineVisible(false);
    setGrid(axisXTwostepLocal);
    axisXTwostepLocal->setLabelsVisible(false);
    axisXTwostepLocal->setRange(0.0, 1.0);
    twostepLocalChart->addAxis(axisXTwostepLocal, Qt::AlignBottom);
    seriesTwostepLocal->attachAxis(axisXTwostepLocal);
    QValueAxis *axisYTwostepLocal = new QValueAxis;
    axisYTwostepLocal->setLineVisible(false);
    setGrid(axisYTwostepLocal);
    axisYTwostepLocal->setLabelsVisible(false);
    axisYTwostepLocal->setRange(0.0, 1.003);
    twostepLocalChart->addAxis(axisYTwostepLocal, Qt::AlignLeft);
    seriesTwostepLocal->attachAxis(axisYTwostepLocal);

    twostepLocal = new QChartView();
    twostepLocal->setRenderHint(QPainter::Antialiasing);
    twostepLocal->setChart(twostepLocalChart);

    QChart *twostepGlobalChart = new QChart();
    twostepGlobalChart->addSeries(seriesTwostepGlobal);
    twostepGlobalChart->setTitle(tr("Global error"));
    twostepGlobalChart->legend()->hide();

    QValueAxis *axisXTwostepGlobal= new QValueAxis;
    axisXTwostepGlobal->setLineVisible(false);
    setGrid(axisXTwostepGlobal);
    axisXTwostepGlobal->setLabelsVisible(false);
    axisXTwostepGlobal->setRange(0.0, 1.0);
    twostepGlobalChart->addAxis(axisXTwostepGlobal, Qt::AlignBottom);
    seriesTwostepGlobal->attachAxis(axisXTwostepGlobal);
    QValueAxis *axisYTwostepGlobal = new QValueAxis;
    axisYTwostepGlobal->setLineVisible(false);
    setGrid(axisYTwostepGlobal);
    axisYTwostepGlobal->setLabelsVisible(false);
    axisYTwostepGlobal->setRange(0.0, 1.003);
    twostepGlobalChart->addAxis(axisYTwostepGlobal, Qt::AlignLeft);
    seriesTwostepGlobal->attachAxis(axisYTwostepGlobal);

    twostepGlobal = new QChartView();
    twostepGlobal->setRenderHint(QPainter::Antialiasing);
    twostepGlobal->setChart(twostepGlobalChart);

    QVBoxLayout *layoutTwostep = new QVBoxLayout();
    layoutTwostep->addWidget(twostepSolution);
    layoutTwostep->addWidget(twostepLocal);
    layoutTwostep->addWidget(twostepGlobal);
    twostepWidget->setLayout(layoutTwostep);

    QWidget *rungekuttaWidget = new QWidget();

    QChart *rungekuttaSolutionChart = new QChart();
    rungekuttaSolutionChart->addSeries(seriesRungekuttaIdeal);
    rungekuttaSolutionChart->addSeries(seriesRungekuttaSolution);
    rungekuttaSolutionChart->setTitle(tr("Solution"));
    rungekuttaSolutionChart->legend()->hide();

    QValueAxis *axisXRungekuttaSolution = new QValueAxis;
    axisXRungekuttaSolution->setLineVisible(false);
    setGrid(axisXRungekuttaSolution);
    axisXRungekuttaSolution->setLabelsVisible(false);
    axisXRungekuttaSolution->setRange(0.0, 1.0);
    rungekuttaSolutionChart->addAxis(axisXRungekuttaSolution, Qt::AlignBottom);
    seriesRungekuttaIdeal->attachAxis(axisXRungekuttaSolution);
    seriesRungekuttaSolution->attachAxis(axisXRungekuttaSolution);
    QValueAxis *axisYRungekuttaSolution = new QValueAxis;
    axisYRungekuttaSolution->setLineVisible(false);
    setGrid(axisYRungekuttaSolution);
    axisYRungekuttaSolution->setLabelsVisible(false);
    axisYRungekuttaSolution->setRange(0.0, 1.003);
    rungekuttaSolutionChart->addAxis(axisYRungekuttaSolution, Qt::AlignLeft);
    seriesRungekuttaIdeal->attachAxis(axisYRungekuttaSolution);
    seriesRungekuttaSolution->attachAxis(axisYRungekuttaSolution);

    rungekuttaSolution = new QChartView();
    rungekuttaSolution->setRenderHint(QPainter::Antialiasing);
    rungekuttaSolution->setChart(rungekuttaSolutionChart);

    QChart *rungekuttaLocalChart = new QChart();
    rungekuttaLocalChart->addSeries(seriesRungekuttaLocal);
    rungekuttaLocalChart->setTitle(tr("Local error"));
    rungekuttaLocalChart->legend()->hide();

    QValueAxis *axisXRungekuttaLocal= new QValueAxis;
    axisXRungekuttaLocal->setLineVisible(false);
    setGrid(axisXRungekuttaLocal);
    axisXRungekuttaLocal->setLabelsVisible(false);
    axisXRungekuttaLocal->setRange(0.0, 1.0);
    rungekuttaLocalChart->addAxis(axisXRungekuttaLocal, Qt::AlignBottom);
    seriesRungekuttaLocal->attachAxis(axisXRungekuttaLocal);
    QValueAxis *axisYRungekuttaLocal = new QValueAxis;
    axisYRungekuttaLocal->setLineVisible(false);
    setGrid(axisYRungekuttaLocal);
    axisYRungekuttaLocal->setLabelsVisible(false);
    axisYRungekuttaLocal->setRange(0.0, 1.003);
    rungekuttaLocalChart->addAxis(axisYRungekuttaLocal, Qt::AlignLeft);
    seriesRungekuttaLocal->attachAxis(axisYRungekuttaLocal);

    rungekuttaLocal = new QChartView();
    rungekuttaLocal->setRenderHint(QPainter::Antialiasing);
    rungekuttaLocal->setChart(rungekuttaLocalChart);

    QChart *rungekuttaGlobalChart = new QChart();
    rungekuttaGlobalChart->addSeries(seriesRungekuttaGlobal);
    rungekuttaGlobalChart->setTitle(tr("Global error"));
    rungekuttaGlobalChart->legend()->hide();

    QValueAxis *axisXRungekuttaGlobal= new QValueAxis;
    axisXRungekuttaGlobal->setLineVisible(false);
    setGrid(axisXRungekuttaGlobal);
    axisXRungekuttaGlobal->setLabelsVisible(false);
    axisXRungekuttaGlobal->setRange(0.0, 1.0);
    rungekuttaGlobalChart->addAxis(axisXRungekuttaGlobal, Qt::AlignBottom);
    seriesRungekuttaGlobal->attachAxis(axisXRungekuttaGlobal);
    QValueAxis *axisYRungekuttaGlobal = new QValueAxis;
    axisYRungekuttaGlobal->setLineVisible(false);
    setGrid(axisYRungekuttaGlobal);
    axisYRungekuttaGlobal->setLabelsVisible(false);
    axisYRungekuttaGlobal->setRange(0.0, 1.003);
    rungekuttaGlobalChart->addAxis(axisYRungekuttaGlobal, Qt::AlignLeft);
    seriesRungekuttaGlobal->attachAxis(axisYRungekuttaGlobal);

    rungekuttaGlobal = new QChartView();
    rungekuttaGlobal->setRenderHint(QPainter::Antialiasing);
    rungekuttaGlobal->setChart(rungekuttaGlobalChart);

    QVBoxLayout *layoutRungekutta = new QVBoxLayout();
    layoutRungekutta->addWidget(rungekuttaSolution);
    layoutRungekutta->addWidget(rungekuttaLocal);
    layoutRungekutta->addWidget(rungekuttaGlobal);
    rungekuttaWidget->setLayout(layoutRungekutta);

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
    layoutParam->addWidget(pushButtonSolve, 4, 1, 1, 2, Qt::AlignTop);
    layoutParam->setRowStretch(4, 1);

    QHBoxLayout *layoutMain = new QHBoxLayout();
    layoutMain->addLayout(layoutParam);
    layoutMain->addWidget(tabWidgetMethods);

    setLayout(layoutMain);

    connect(comboBoxEquation, SIGNAL(currentIndexChanged(int)), this, SLOT(updateGUI()));
    connect(sliderSizeT, SIGNAL(valueChanged(int)), this, SLOT(update_sizet(int)));
    connect(sliderNT, SIGNAL(valueChanged(int)), this, SLOT(update_nt(int)));
    connect(spinBoxSizeT, SIGNAL(valueChanged(int)), this, SLOT(update_sizet(int)));
    connect(spinBoxNT, SIGNAL(valueChanged(int)), this, SLOT(update_nt(int)));
    connect(pushButtonSolve, SIGNAL(clicked(bool)), this, SLOT(Solve()));
    connect(timer, SIGNAL(timeout()), this, SLOT(Tick()));

    updateGUI();
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

    updateGUI();
}

void Form::update_nt(int n)
{
    spinBoxNT->blockSignals(true);
    sliderNT->blockSignals(true);

    sliderNT->setValue(n);
    spinBoxNT->setValue(n);

    spinBoxNT->blockSignals(false);
    sliderNT->blockSignals(false);

    updateGUI();
}

void Form::updateLabels()
{
    labelDT->setText(QString::number(param->get_tstep(), 'f', 3));
}

void Form::updateGUI()
{
    delete param;
    param = new Parameters(spinBoxSizeT->value(), spinBoxNT->value());

    eq_type_ = comboBoxEquation->currentData().value<EquationType>();

    QList<QPointF> ideal;
    for (int n = 0; n <= kIdealPoints; ++n)
    {
        double t = param->get_tmax() / kIdealPoints * n;
        ideal << QPointF(t / param->get_tmax(), direct(t, eq_type_));
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
    switch (eq_type_)
    {
    case Decay:
        yaxis_max = 1.5;
        yaxis_min = -0.5;
        break;
    case Grow:
        yaxis_max = std::min(std::exp(param->get_tmax()), 50.0);
        yaxis_min = 0.0;
        break;
    case Oscillate:
        yaxis_max = 2.0;
        yaxis_min = -2.0;
        break;
    }
    eulerSolution->chart()->axisY()->setRange(yaxis_min, yaxis_max);
    leapfrogSolution->chart()->axisY()->setRange(yaxis_min, yaxis_max);
    twostepSolution->chart()->axisY()->setRange(yaxis_min, yaxis_max);
    rungekuttaSolution->chart()->axisY()->setRange(yaxis_min, yaxis_max);

    updateLabels();
}

void Form::initiateState()
{
    t_cur_ = 0.0;
    euler_y_ = std::complex<double>(1.0, 0.0);
    leapfrog_y_ = leapfrog_new_y_ = std::complex<double>(1.0, 0.0);
    for (int n = 0; n < 10; ++n)
        leapfrog_new_y_ += 0.1*param->get_tstep() * func(leapfrog_new_y_, t_cur_ + n * 0.1*param->get_tstep(), eq_type_);
    twostep_y_ = std::complex<double>(1.0, 0.0);
    rungekutta_y_ = std::complex<double>(1.0, 0.0);
}

void Form::Solve()
{
    pushButtonSolve->setEnabled(false);
    comboBoxEquation->setEnabled(false);
    spinBoxSizeT->setEnabled(false);
    spinBoxNT->setEnabled(false);
    sliderSizeT->setEnabled(false);
    sliderNT->setEnabled(false);

    updateGUI();
    initiateState();

    seriesEulerSolution->append(t_cur_ / param->get_tmax(), std::real(euler_y_));
    seriesLeapfrogSolution->append(t_cur_ / param->get_tmax(), std::real(leapfrog_y_));
    seriesTwostepSolution->append(t_cur_ / param->get_tmax(), std::real(twostep_y_));
    seriesRungekuttaSolution->append(t_cur_ / param->get_tmax(), std::real(rungekutta_y_));

    timer->start();
}

void Form::Tick()
{
    if (t_cur_ < param->get_tmax() + 1e-3*param->get_tstep())
    {
        euler_y_ += func(euler_y_, t_cur_, eq_type_) * param->get_tstep();

        std::complex<double> leapfrog_tmp = leapfrog_new_y_;
        leapfrog_new_y_ = leapfrog_y_ + 2.0*param->get_tstep() * func(leapfrog_new_y_, t_cur_, eq_type_);
        leapfrog_y_ = leapfrog_tmp;

        std::complex<double> twostep_tmp = twostep_y_ + 0.5 * param->get_tstep() * func(twostep_y_, t_cur_, eq_type_);
        twostep_y_ += param->get_tstep() * func(twostep_tmp, t_cur_ + 0.5*param->get_tstep(), eq_type_);

        std::complex<double> rk1 = func(rungekutta_y_, t_cur_, eq_type_) * param->get_tstep();
        std::complex<double> rk2 = func(rungekutta_y_ + 0.5*rk1, t_cur_ + 0.5*param->get_tstep(), eq_type_) * param->get_tstep();
        std::complex<double> rk3 = func(rungekutta_y_ + 0.5*rk2, t_cur_ + 0.5*param->get_tstep(), eq_type_) * param->get_tstep();
        std::complex<double> rk4 = func(rungekutta_y_ + rk3, t_cur_ + param->get_tstep(), eq_type_) * param->get_tstep();
        rungekutta_y_ += 1.0/6.0 * (rk1 + 2.0*rk2 + 2.0*rk3 + rk4);

        t_cur_ += param->get_tstep();

        seriesEulerSolution->append(t_cur_ / param->get_tmax(), std::real(euler_y_));
        seriesLeapfrogSolution->append(t_cur_ / param->get_tmax(), std::real(leapfrog_y_));
        seriesTwostepSolution->append(t_cur_ / param->get_tmax(), std::real(twostep_y_));
        seriesRungekuttaSolution->append(t_cur_ / param->get_tmax(), std::real(rungekutta_y_));
    }
    else
    {
        timer->stop();
        pushButtonSolve->setEnabled(true);
        comboBoxEquation->setEnabled(true);
        spinBoxSizeT->setEnabled(true);
        spinBoxNT->setEnabled(true);
        sliderSizeT->setEnabled(true);
        sliderNT->setEnabled(true);
    }
}
