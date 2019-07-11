// _  _ ____ _    ____ ___ _
//  \/  |  | |    |  |  |  |
// _/\_ |__| |___ |__|  |  |___
//
// Calcium mechanism
//
// Thalamocortical model for a propofol-induced α-rhythm associated with loss of consciousness
// ShiNung Ching, Aylin Cimenser, Patrick L. Purdon, Emery N. Brown, Nancy J. Kopell
// Proceedings of the National Academy of Sciences Dec 2010, 107 (52) 22665-22670; DOI: 10.1073/pnas.1017069108
//
// https://www.pnas.org/content/107/52/22665

#ifndef CALCIUMMECH
#define CALCIUMMECH
#include "mechanism.hpp"
#include <limits>


//inherit controller class spec
class CalciumMech: public mechanism {

protected:
public:

    // specify parameters + initial conditions for
    // mechanism that controls a conductance
    CalciumMech()
    {

        controlling_class = "unset";

    }


    void integrate(void);
    void integrateMS(int, double, double);

    void checkSolvers(int);

    void connect(compartment*);
    void connect(conductance*);
    void connect(synapse*);

    int getFullStateSize(void);
    int getFullState(double * cont_state, int idx);
    double getState(int);

    double Cadot(double);

};


double CalciumMech::getState(int idx) {
    return std::numeric_limits<double>::quiet_NaN();
}


int CalciumMech::getFullStateSize() {
    return 0;
}


int CalciumMech::getFullState(double *cont_state, int idx) {
    // do nothing
    return idx;
}



// connection methods
void CalciumMech::connect(compartment* comp_)
{
    comp = comp_;
    comp->addMechanism(this);
}

void CalciumMech::connect(conductance* cond_)
{
    mexErrMsgTxt("[CalciumMech] This mechanism cannot connect to a conductance object");
}

void CalciumMech::connect(synapse* syn_)
{
    mexErrMsgTxt("[CalciumMech] This mechanism cannot connect to a synapse object");
}



void CalciumMech::integrate(void)
{

    double Ca = comp->Ca_prev;

    double Ca_temp = Cadot(Ca);

    if (Ca_temp < 0)
    {
        comp->Ca = 0;
    }
    else
    {
        comp->Ca = Ca_temp;
    }
}


double CalciumMech::Cadot(double Ca_)
{
    return (-10 * comp->i_Ca_prev)/192971 + (0.00024 - Ca)/5;
}

// Runge-Kutta 4 integrator
void CalciumMech::integrateMS(int k, double V, double Ca_)
{
    if (k == 4){return;}
    comp->k_Ca[k] = dt*(Cadot(Ca_));
}

void CalciumMech::checkSolvers(int k)
{
    if (k == 0){
        return;
    } else if (k == 4){
        return;
    } else {
        mexErrMsgTxt("[CalciumMech] unsupported solver order\n");
    }
}



#endif
