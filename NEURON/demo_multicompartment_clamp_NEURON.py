## create a multi-compartment model of a soma and neurite
## voltage clamp the soma, record the rectifying current
## add the rectifying current back into the soma
## to attempt to reproduce voltage-clamped neuron activity

from neuron import h, gui
import matplotlib.pyplot as pyplot
import numpy as np

## Instantiate neuronal compartments

soma = h.Section(name='soma')
dend = h.Section(name='dend')

# properties for the soma
soma.L      = 0.05 / 10     # mm -> cm
soma.diam   = 0.025 / 5     # mm radius -> cm diameter
soma.cm     = 10 / 10       # nF/mm^2 -> μF/cm^2
soma.Ra     = 1e-3 * 1e5    # MΩ⋅mm -> Ω⋅cm
soma.nseg   = 1

# properties for the neurite compartments
dend.L      = 0.35
dend.diam   = 0.1 / 10
dend.cm     = 10 / 10
dend.Ra     = 1e-3 * 1e5
dend.nseg   = 10

## Add conductances

conds       = ['acurrent', 'cas', 'cat', 'hcurrent', 'kca', 'kd', 'na']
gbars       = np.array([104, 11.76, 4.7, 0, 390, 250, 0]) * 1e-4  # S/cm^2
Erevs       = [-80, 30, 30, -20, -80, -80, 50]              # mV

# add all conductances to the Section objects
# add calcium mechanism
for sec in [soma, dend]:
    sec.insert('cad')
    for cond in conds:
        sec.insert(cond)

# update the parameters of the conductances
for sec in [soma, dend]:
    for seg in sec:
        seg.acurrent.gbar = gbars[0]
        seg.cas.gbar = gbars[1]
        seg.cat.gbar = gbars[2]
        seg.hcurrent.gbar = gbars[3]
        seg.kca.gbar = gbars[4]
        seg.kd.gbar = gbars[5]
        seg.na.gbar = gbars[6]
        seg.acurrent.Erev = Erevs[0]
        # seg.cas.Erev = Erevs[1]
        # seg.cat.Erev = Erevs[2]
        seg.hcurrent.Erev = Erevs[3]
        seg.kca.Erev = Erevs[4]
        seg.kd.Erev = Erevs[5]
        seg.na.Erev = gbars[6]

## Generate the voltage waveform

# time parameters
h.dt        = 0.1
h.t         = h.dt
h.tstop     = 5000

# instantiate a single-electrode voltage clamp
t           = h.Vector(1e-3 * np.linspace(h.dt, h.tstop, h.tstop/h.dt))
V_clamp     = h.Vector(np.linspace(0, 30, len(t)) + 30 * np.sin(1e-3 * np.arange(0, len(t)) - 50))
myVClamp    = h.SEClamp(soma(0.5))
V_clamp.play(myVClamp._ref_amp1, t, True)

## Simulate the model while voltage-clamping

# acquire the clamping current
t_vec_v     = h.Vector()
v_vec_v     = h.Vector()
I_clamp     = h.Vector()
t_vec_v.record(h._ref_t)
v_vec_v.record(soma(0.5)._ref_v)
I_clamp.record(myVClamp._ref_i)

# simulate the model
h.finitialize(-65)
h.dt        = 0.1
h.t         = h.dt
h.tstop     = 5000
h.run()

pyplot.figure()
pyplot.plot(t_vec_v, v_vec_v)
pyplot.show()

## Simulate the model while current clamping

# storage vectors
t_vec_i     = h.Vector()
v_vec_i     = h.Vector()
t_vec_i.record(h._ref_t)
v_vec_i.record(soma(0.5)._ref_v)

# set up the current clamp
myIClamp    = h.IClamp(soma(0.5))
I_clamp.play(myIClamp._ref_i, t_vec_i, True)

# simulate the model
h.finitialize(-65)
h.dt        = 0.1
h.t         = h.dt
h.tstop     = 5000
h.run()

## Visualize the results

h.dt        = 0.1
h.t         = h.dt
h.tstop     = 5000

pyplot.figure
pyplot.plot(t_vec_v, v_vec_v)

# pyplot.figure(figsize=(8,4))
# pyplot.plot(t_vec_v, v_vec_v);
# pyplot.plot(t_vec_i, v_vec_i)
# pyplot.xlabel('time (ms)')
# pyplot.ylabel('mV')
# pyplot.show()
