## create a multi-compartment model of a soma and neurite
## voltage clamp the soma, record the rectifying current
## add the rectifying current back into the soma
## to attempt to reproduce voltage-clamped neuron activity

from neuron import h, gui
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

conds       = ['acurrent', 'cas', 'cat', 'hcurrent', 'kca', 'kd', 'nav']
gbars       = [104, 11.76, 4.7, 0, 390, 250, 0] * 1e-4  # S/cm^2
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
        for i, cond in enumerate(conds):
            this = getattr(seg, cond)
            sec.this.gbar = gbars[i]
            sec.this.Erev = Erevs[i]

## Generate the voltage waveform

# time parameters
h.dt        = 0.1
h.t         = h.dt
h.tstop     = 5000

t           = h.Vector(1e-3 * np.linspace(h.dt, h.tstop, h.tstop/h.dt))
V_clamp     = h.Vector(np.linspace(0, 30, length(t)) + 30 * np.sin(1e-3 * 1:length(t)) - 50)

V_clamp.play(soma(0.5).V, t, True)
