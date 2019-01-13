// _  _ ____ _    ____ ___ _
//  \/  |  | |    |  |  |  |
// _/\_ |__| |___ |__|  |  |___
//
// morris-lecar model implemented as a mechanism
//
//
//
#ifndef MORRISLECAR
#define MORRISLECAR
#include "mechanism.hpp"
#include <limits>

//inherit controller class spec
class MorrisLecar: public mechanism {

protected:

public:

    // default parameter values
    double gbar_Leak    = 0;
    double gbar_Ca      = 0;
    double gbar_Kd      = 0;
    double E_Leak       = -50;
    double E_Ca         = 120;
    double E_Kd         = -80;
    double phi          = 1;
    double v1;
    double v2;
    double v3;
    double v4;

    // default state variable values
    double n            = 0;

    // constructor
    MorrisLecar(double gbar_Leak_, double gbar_Kd_, double gbar_Ca_, double E_Kd_, double E_Ca_, double E_Leak_, double phi_, double v1_, double v2_, double v3_, double v4_, double n_)
    {
        //  parameters
        gbar_Leak = gbar_Leak_;
        gbar_Kd = gbar_Kd_;
        gbar_Ca = gbar_Ca_;
        E_Kd    = E_Kd_;
        E_Ca    = E_Ca_;
        E_Leak  = E_Leak_;
        phi     = phi_;
        v1      = v1_;
        v2      = v2_;
        v3      = v3_;
        v4      = v4_;

        // state variables
        n       = n_;
    }

    void checkSolvers(int);
    void integrate(void);
    void integrateMS(int, double, double);

    double Vdot(double, double);
    double Ndot(double, double);

    void connect(compartment*);
    void connect(conductance*);
    void connect(synapse*);

    int getFullState(double* cont_state, int idx);
    int getFullStateSize(void);

};

// bookkeeping methods
double MorrisLecar::getState(int idx)
{
    return std::numeric_limits<double>::quiet_NaN();
}

int MorrisLecar::getFullStateSize()
{
    return 0;
}

int MorrisLecar::getFullState(double *cont_state, int idx)
{
    // do nothing
    return idx;
}

// connection methods
void MorrisLecar::connect(compartment* comp_)
{
    comp = comp_;
    comp->addMechanism(this);
}

void MorrisLecar::connect(conductance* cond_)
{
    mexErrMsgTxt("[MorrisLecar] This mechanism cannot connect to a conductance object")
}

void MorrisLecar::connect(synapse* syn_)
{
    mexErrMsgTxt("[MorrisLecar] This mechanism cannot connect to a synapse object\n");
}

double MorrisLecar::Vdot(double v, double n)
{
    double m_inf = 0.5 * (1 + tanh((V - v1)/v2));
    return - gbar_Leak * (v - E_Leak) - gbar_Ca * m_inf *  (v - E_Ca) - gbar_Kd * n * (v - E_Kd);
}

double MorrisLecar::Ndot(double v, double n)
{
    double n_inf = 0.5 * (1 + tanh((v - v1)/v2));
    double n_tau = 1.0 / (phi * cosh((V - v3)/(2*v4)));
    return (n_inf - n) / n_tau;
}

// integration methods
void MorrisLecar::integrate(void)
{
    // euler's method
    double v    = comp->V;
    n           = n + Ndot(v, n) * dt;
    v           = v + Vdot(v, n) * dt;
    comp->V     = v;
}

void MorrisLecar::integrateMS(int k, double v, double Ca_)
{
    mexErrMsgTxt("[MorrisLecar] This integration method isn't supported for this mechanism yet\n");
}


#endif
