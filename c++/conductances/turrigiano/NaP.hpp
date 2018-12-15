// _  _ ____ _    ____ ___ _
//  \/  |  | |    |  |  |  |
// _/\_ |__| |___ |__|  |  |___
//
// persistent Sodium conductance
// from Turrigiano ... Marder 1995
// http://www.jneurosci.org/content/jneuro/15/5/3640.full.pdf
#ifndef NAP
#define NAP
#include "conductance.hpp"

//inherit conductance class spec
class NaP: public conductance {

public:

    // specify parameters + initial conditions
    NaP(double g_, double E_, double m_, double h_)
    {
        gbar = g_;
        E = E_;
        m = m_;
        h = h_;

        p = 3;
        q = 1;

        // defaults
 if (isnan(gbar)) { gbar = 0; }
        if (isnan (m)) { m = 0; }
        if (isnan (h)) { h = 1; }
        if (isnan (E)) { E = 50; }
    }

    void integrate(double, double);

    double m_inf(double, double);
    double h_inf(double, double);
    double tau_m(double, double);
    double tau_h(double, double);
    string getClass(void);
};

string NaP::getClass(){return "NaP";}

double NaP::m_inf(double V, double Ca) {return 1.0/(1.0+exp((V+26.8)/-8.2));}
double NaP::h_inf(double V, double Ca) {return 1.0/(1.0+exp((V+48.5)/4.8));}
double NaP::tau_m(double V, double Ca) {return 19.8 - 10.7/(1+exp((V+26.5)/-8.6));}
double NaP::tau_h(double V, double Ca)  {return 666 - 379/(1+exp((V+33.6)/-11.7));}

#endif
