---
name: control
description: |
  Control systems / CACSD in Scilab: syslin transfer functions and state
  space, time responses (csim, dsimul), frequency plots (bode, nyquist,
  evans), margins, pole placement, discretization, PID. Scilab diverges
  heavily from MATLAB's Control Toolbox (no tf/step/margin/place). Trigger
  words: "transfer function", "PID", "bode", "state space", "closed loop",
  "poles", "stability".
triggers: fonction de transfert, transfer function, syslin, asservissement, régulateur, regulator, pid, bode, nyquist, evans, lieu des racines, root locus, boucle fermée, closed loop, boucle ouverte, open loop, marge de gain, marge de phase, gain margin, phase margin, margin, pôles, poles, espace d'état, state space, state-space, stabilité, stability, correcteur, controller, c2d, discrétiser, discrétise, discrétisation, discretize, discretization, tustin, bilinéaire, bilinear, zoh, step response, réponse indicielle, lqr, lqe, kalman, tf, feedback, observateur, observer, commandable, commandabilité, controllable, controllability, observable, observabilité, observability, placement de pôles, pole placement, avance de phase, retard de phase, phase lead, lead, lag, ziegler, anti-windup, antiwindup, gain critique
---

# Scilab control systems (CACSD)

Everything is a `syslin` — NOT tf/ss objects:

```scilab
s  = %s;                                 // or s = poly(0, 's')
G  = syslin("c", 1, s^2 + 2*s + 1);      // continuous, num/den
G2 = syslin("c", A, B, C [, D]);         // continuous state space
Hz = syslin(0.01, num, den);             // discrete, sampling period 0.01 s
```

Access: `G.num`, `G.den`, `G2.A/.B/.C/.D`; convert with `tf2ss(G)` and
`ss2tf(G2)`. Simplify after algebra with `simp` (or `clean` for tiny terms).

| MATLAB (DO NOT USE) | Scilab |
|---------------------|--------|
| `tf(num, den)` | `syslin("c", num, den)` (polynomials in `%s`) |
| `ss(A,B,C,D)` | `syslin("c", A, B, C, D)` |
| `step(G)` | `y = csim("step", t, G); plot(t, y)` |
| `impulse(G)` | `csim("imp", t, G)` |
| `lsim(G,u,t)` | `csim(u, t, G)` (u vector or function) |
| `feedback(G, H)` | `G /. H` (the feedback operator) |
| `margin(G)` | `g_margin(G)`, `p_margin(G)`, or `show_margins(G)` |
| `rlocus(G)` | `evans(G)` |
| `pzmap(G)` | `plzr(G)` |
| `pole(G)` | `roots(G.den)` or `spec(G2.A)` |
| `c2d(G, dt)` | `dscr(G, dt)` (returns STATE SPACE) |
| `c2d(G, dt, 'tustin')` | `cls2dls(tf2ss(G), dt [, fp])` |
| `place(A,B,p)` | `K = ppol(A, B, p)` |
| `lqr(A,B,Q,R)` | `K = -lqr(syslin("c",A,B,eye(A)), Q, R)` (sign!) |
| `allmargin` / critical gain | `kpure(G)` (gain where poles cross jω) |

## Block algebra

```scilab
Gc = C * G;              // series
Gp = G1 + G2;            // parallel
T  = Gc /. 1;            // unit negative feedback: Gc/(1+Gc)
T2 = G /. H;             // G/(1+G*H)
T  = simp(T);
```

`/.` binds loosely: parenthesize, `(C*G) /. 1`. **Always put a space
before the number**: `G/.1` parses as `G / .1` (division by 0.1), so the
loop is silently never closed.

## Time responses

```scilab
t = 0:0.01:5;
y = csim("step", t, T);          // continuous systems only
plot(t, y); xgrid();

// arbitrary input: a vector sampled on t, or a function u(t)
u = sin(2*%pi*t);
y = csim(u, t, T);

// initial condition (state space): free response from x0
y = csim(zeros(t), t, Gss, x0);

// step metrics: overshoot and 5 % settling time
yf = horner(T, 0);                             // final value (static gain)
D  = 100*(max(y) - yf)/yf;
k  = find(abs(y - yf) > 0.05*abs(yf));
ts = t(k($) + 1);                              // NOT find(..., "last")

// discrete systems
u = ones(1, 100);
y = dsimul(tf2ss(Hz), u);        // dsimul takes STATE-SPACE input
// or: y = flts(u, Hz);          // works directly on the transfer function
```

## Frequency domain

```scilab
bode(G);                          // magnitude + phase (Hz by default)
bode(G, 0.01, 100);               // frequency range in Hz
bode(G, 0.01, 100, "rad");        // axis in rad/s — fmin/fmax STILL in Hz
gainplot(G); nyquist(G); black(G);   // Nichols chart = black()
[gm, fr] = g_margin(G);           // gain margin (dB) + frequency (Hz)
[pm, fp] = p_margin(G);           // phase margin (deg) + frequency (Hz)
show_margins(G);                  // bode with margins highlighted
repfreq(G, frq)                   // complex response at given frequencies
```

Frequencies are in Hz, not rad/s — convert with `w = 2*%pi*f` when a
model is written in rad/s.

Infinite margins come back EMPTY: `g_margin` gives `gm=%inf, fr=[]`,
`p_margin` gives `[]` or `%inf` with `fp=[]`. Passing `[]` to `mprintf`
errors, so test before printing:

```scilab
[pm, fp] = p_margin(G);
if isempty(fp) then mprintf("phase margin: infinite\n");
else mprintf("PM = %.1f deg at %.3f rad/s\n", pm, 2*%pi*fp); end
```

Root locus / critical gain:

```scilab
evans(G, Kmax);                   // Kmax bounds the plot; legend labels are unreliable
[Kc, R] = kpure(G);               // gains putting closed-loop poles on jω, R = those poles
[Kb, sb] = krac2(G);              // breakaway points on the real axis
```

## PID

Build it directly as a transfer function:

```scilab
Kp = 2; Ki = 1; Kd = 0.1; N = 100;          // N: derivative filter
C  = syslin("c", Kp + Ki/s + Kd*s*N/(s+N)); // filtered PID
T  = (C*G) /. 1;
y  = csim("step", t, T);
```

An unfiltered `Kd*s` makes the PID improper: `syslin` refuses it; filter
the derivative. If an ideal PID is really wanted, keep it a plain rational
and wrap the (proper) loop instead: `L = syslin("c", Cr*G_rational)`.

## State space design

```scilab
K = ppol(A, B, [-2, -3+%i, -3-%i]);   // pole placement (K such that A-B*K)
L = ppol(A', C', poles)';             // observer gain by duality
spec(A - B*K)                          // check closed-loop poles
```

`cont_mat`/`obsv_mat` give controllability/observability matrices (check
with `rank`).

### LQR — NOT MATLAB's signature, and OPPOSITE sign to ppol

```scilab
P  = syslin("c", A, B, eye(A));       // only A and B are used here
Kl = lqr(P, Q, R);                     // Scilab convention: u = +Kl*x, A+B*Kl stable
K  = -Kl;                              // back to u = -K*x, same as ppol / MATLAB
spec(A - B*K)                          // check
```

`lqr(A,B,Q,R)` does not exist. The one-argument form `lqr(P12)` reads the
weights from `P12.C`/`P12.D` (Q=C'C, R=D'D). `lqe` and `obscont` follow the
same syslin-based style.

## Discretization

```scilab
Gd = dscr(G, 0.01);          // zero-order hold — ALWAYS returns state space,
Hz = ss2tf(Gd);              //   even from a transfer function (Gd.num fails)
Hz.dt                         // sampling period

// Tustin / bilinear: cls2dls exists, and wants STATE-SPACE input
Ht = ss2tf(cls2dls(tf2ss(G), 0.01));        // plain Tustin
Hp = ss2tf(cls2dls(tf2ss(G), 0.01, fp));    // with prewarping at fp (Hz)
Ht.dt = 0.01;                                // cls2dls leaves dt = "d": restore it
```

ZOH is exact at the sampling instants for a step input; Tustin has a
direct feedthrough (y(0) ≠ 0) but preserves frequency behavior — the usual
choice for discretizing a controller.

## Pitfalls

- `csim` rejects discrete systems and `dsimul` wants state space: pick the
  right pair (continuous→csim, discrete→dsimul/flts).
- Polynomial display order in Scilab is ascending powers; do not paste
  MATLAB's descending coefficient vectors without flipping.
- After feedback algebra always `simp(...)` — otherwise common factors
  accumulate and orders explode.
- `G/.1` without a space is division by 0.1, not feedback (see above).
- `ppol` gives `A-B*K`, `lqr` gives `A+B*K`: never mix the two gains
  without flipping the sign.
