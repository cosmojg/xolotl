## create a multi-compartment model of a soma and neurite
## voltage clamp the soma, record the rectifying current
## add the rectifying current back into the soma
## to attempt to reproduce voltage-clamped neuron activity

from neuron import h, gui

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
gbars       = [104, 11.76, 4.7, 0.1, 390, 250, 2e3] * 1e-4  # S/cm^2
Erevs       = [-80, 30, 30, -20, -80, -80, 50]              # mV

# add all conductances to the Section objects
for cond in conds:
    soma.insert(cond)
    dend.insert(cond)

# update the parameters of the conductances
for sec in [soma, dend]:
    for seg in sec:
        for i, cond in enumerate(conds):
            this = getattr(seg, cond)
            sec.this.gbar = gbars[i]
            sec.this.Erev = Erevs[i]
