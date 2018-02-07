#ifndef PARAMETERS_H
#define PARAMETERS_H

#include <QString>


class Parameters
{
public:
    Parameters(double tmax, int nt);

    int get_nt() const;
    double get_tmax() const;
    double get_tstep() const;

    void set_tmax(int tmax);
    void set_nt(int nt);

    QString toQString() const;

private:
    double tmax_, tstep_;
    int nt_;

    void set_tstep();
};

#endif // PARAMETERS_H
