#include "parameters.h"

Parameters::Parameters(double tmax, int nt):
    tmax_(tmax), nt_(nt)
{
    set_tstep();
}

int Parameters::get_nt() const
{
    return nt_;
}

double Parameters::get_tmax() const
{
    return tmax_;
}

double Parameters::get_tstep() const
{
    return tstep_;
}

void Parameters::set_tmax(int tmax)
{
    tmax_ = tmax;
    set_tstep();
}

void Parameters::set_nt(int nt)
{
    nt_ = nt;
    set_tstep();
}

QString Parameters::toQString() const
{
    return QString::number(tmax_) + " " + QString::number(nt_);
}

void Parameters::set_tstep()
{
    tstep_ = tmax_ / nt_;
}
