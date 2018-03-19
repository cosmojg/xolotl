// xolotl
// this template file is used by xolotl.transpile() to convert
// the xolotl pseudo-object in MATLAB into a C++
// file that can be compiled from mex
// DON'T TOUCH ANYTHING HERE!

#include <cmath>
#include <vector>
#include <typeinfo>
#include "mex.h"

//xolotl:include_headers_here


using namespace std;


void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
    // declare pointers to outputs
    double *output_state;
    double *output_V;
    double *output_Ca;
    double *output_I_clamp;
    double *output_cond_state; // conductances
    double *output_syn_state;  // synapses
    double *output_cont_state; // controllers


    //xolotl:define_v_clamp_idx

    // make an empty network
    network xolotl_network;

    int nsteps;
    int n_synapses = 0; // keeps track of how many synapses we have

    //xolotl:input_declarations


    double delta_temperature = (temperature - temperature_ref)/10;

    //xolotl:insert_constructors

    //xolotl:add_conductances_here

    //xolotl:add_synapses_here

    //xolotl:add_controllers_here

    //xolotl:add_neurons_to_network

    nsteps = (int) floor(t_end/dt);
    int n_comp = (int) (xolotl_network.comp).size(); // these many compartments

    // compute cond_state_dim
    int cond_state_dims[n_comp];
    int cond_state_dim = 0;
    for (int i = 0; i < n_comp; i ++)
    {
        int n_cond = ((xolotl_network.comp[i])->n_cond);
        cond_state_dims[i] = 2*n_cond; // only m, h -- gbar will be returned with controllers, if any
        cond_state_dim += 2*n_cond;
    }

    plhs[0] = mxCreateDoubleMatrix(1, param_size, mxREAL); //output state
    plhs[1] = mxCreateDoubleMatrix(n_comp, nsteps, mxREAL); // V
    plhs[2] = mxCreateDoubleMatrix(2*n_comp, nsteps, mxREAL); // Ca + E_Ca
    plhs[3] = mxCreateDoubleMatrix(1, nsteps, mxREAL); // I_clamp
    plhs[4] = mxCreateDoubleMatrix(cond_state_dim, nsteps, mxREAL); // cond_state
    plhs[5] = mxCreateDoubleMatrix(2*n_synapses, nsteps, mxREAL); // synapse gbar + state
    plhs[6] = mxCreateDoubleMatrix(2*n_controllers, nsteps, mxREAL); // controllers gbar + mrna

    output_state = mxGetPr(plhs[0]);
    output_V = mxGetPr(plhs[1]);
    output_Ca = mxGetPr(plhs[2]);
    output_I_clamp = mxGetPr(plhs[3]);
    output_cond_state = mxGetPr(plhs[4]);
    output_syn_state = mxGetPr(plhs[5]);
    output_cont_state = mxGetPr(plhs[6]);

    // make arrays which will store the full cond. state
    double * full_cond_state = new double[cond_state_dim];
    int cond_idx = 0;
    double * I_ext_now = new double[n_comp];
    // make sure I_ext_now is zero
    for(int q = 0; q < n_comp; q++)
    {
        I_ext_now[q] = 0.0;
    }

    // do the integration
    for(int i = 0; i < nsteps; i++)
    {
        for(int j = 0; j < n_comp; j++)
        {
            //xolotl:enable_when_I_ext
            //I_ext_now[j] = I_ext[i];
        }
        //xolotl:disable_when_clamped
        xolotl_network.integrate(dt,I_ext_now, delta_temperature);
        //xolotl:enable_when_clamped
        //xolotl_network.integrateClamp(V_clamp[i],dt, delta_temperature);
        //xolotl:enable_when_clamped
        //output_I_clamp[i] = xolotl_network.comp[0]->I_clamp;

        //xolotl:read_synapses_here

        //xolotl:read_controllers_here

        // here we're getting the state of every compartment -- V, Ca, and all conductances
        cond_idx = 0;
        for (int j = 0; j < n_comp; j++)
        {
            output_V[i*n_comp + j] = xolotl_network.comp[j]->V;
            output_Ca[i*2*n_comp + j] = xolotl_network.comp[j]->Ca;
            output_Ca[i*2*n_comp + j + n_comp] = xolotl_network.comp[j]->E_Ca;

            xolotl_network.comp[j]->get_cond_state(full_cond_state);

            // get the states of every conductance
            for (int k = 0; k < cond_state_dims[j]; k++)
            {
                output_cond_state[i*cond_state_dim + cond_idx] = full_cond_state[k];
                cond_idx ++;
            }
        }

    }

    // here, we are reading the full state back, so we 
    // can easily update the xolotl object in the MATLAB wrapper
    //xolotl:read_state_back


}
