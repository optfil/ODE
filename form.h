#ifndef FORM_H
#define FORM_H

#include <complex>

#include <QWidget>
#include <QComboBox>
#include <QLabel>
#include <QPushButton>
#include <QSlider>
#include <QSpinBox>
#include <QTabWidget>
#include <QTimer>

#include <QtCharts/QtCharts>
QT_CHARTS_USE_NAMESPACE

#include "parameters.h"


constexpr int kTMin = 1;
constexpr int kTMax = 20;
constexpr int kNtMin = 2;
constexpr int kNtMax = 100;
constexpr int kIdealPoints = 200;

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
    void updateGUI();
    void initiateState();
    void selectMethod(int n);
    void Solve();
    void Tick();

private:
    QLabel *labelEquation;
    QComboBox *comboBoxEquation;
    QLabel *labelSizeT_1, *labelSizeT_2, *labelNT_1, *labelNT_2;
    QSpinBox *spinBoxSizeT, *spinBoxNT;
    QSlider *sliderSizeT, *sliderNT;
    QLabel *labelDT_1, *labelDT_2, *labelDT;
    QPushButton *pushButtonSolve;
    QTabWidget *tabWidgetMethods;
    QChartView *eulerSolution, *leapfrogSolution, *twostepSolution, *rungekuttaSolution;
    QChartView *localError, *globalError;
    QLineSeries *seriesEulerIdeal, *seriesLeapfrogIdeal, *seriesTwostepIdeal, *seriesRungekuttaIdeal;
    QLineSeries *seriesEulerSolution, *seriesLeapfrogSolution, *seriesTwostepSolution, *seriesRungekuttaSolution;
    QLineSeries *seriesEulerLocal, *seriesLeapfrogLocal, *seriesTwostepLocal, *seriesRungekuttaLocal;
    QLineSeries *seriesEulerGlobal, *seriesLeapfrogGlobal, *seriesTwostepGlobal, *seriesRungekuttaGlobal;

    QTimer *timer;

    EquationType eq_type_;
    Parameters *param;
    double t_cur_;
    std::complex<double> euler_y_, leapfrog_y_, leapfrog_new_y_, twostep_y_, rungekutta_y_;
};

#endif // FORM_H
