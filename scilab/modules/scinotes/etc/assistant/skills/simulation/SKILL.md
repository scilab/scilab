---
name: simulation
description: |
  Simulating dynamical systems in Scilab: the ode solver (signatures, stiff
  problems, events/root finding, extra parameters), dae for implicit systems,
  discrete iterations. Scilab's ode differs completely from MATLAB's
  ode45/odeset. Trigger words: "ode", "differential equation", "simulate",
  "integrator", "stiff", "runge-kutta", "initial condition", "trajectory".
triggers: simule, simulez, simulons, ode, ode45, ode23, ode15s, ode15i, odeset, cvode, arkode, ida, kinsol, sundials, algébro-différentiel, algébro-différentielle, robertson, lotka-volterra, lotka, proie, prédateur, proie-prédateur, bifurcation, suite logistique, récurrence, balle, chute libre, pas de temps, time step, événement, event, events, rebond, bounce, oscillateur, oscillator, lorenz, van der pol, chaos, chaotique, système dynamique, dynamical system, équation différentielle, équations différentielles, differential equation, differential equations, edo, dae, simulation, simuler, simulate, intégrateur, integrator, intégration numérique, runge-kutta, stiff, raide, condition initiale, initial conditions, trajectoire, trajectory, dynamique, dynamics, pendule, pendulum
---

# Scilab ODE simulation

Two families — neither is ode45/ode23/ode15s/odeset (they do not exist):
- **SUNDIALS** (`cvode`, `arkode`, `ida`, `kinsol`, `sprkode`) — modern,
  MATLAB-like call and named options: prefer it for new code (see below).
- **`ode`** — the classic ODEPACK front-end, still everywhere in old scripts.

## SUNDIALS — cvode / arkode / ida

```scilab
[t, y] = cvode(f, [t0 tf], y0)                  // adaptive output points
[t, y] = cvode(f, tgrid, y0)                    // values at tgrid
[t, y, info] = cvode(list(f, p1, p2), [t0 tf], y0, method="BDF", ..
                     rtol=1e-8, atol=1e-10, maxSteps=100000)
[t, y] = arkode(f, [t0 tf], y0)                 // explicit/IMEX Runge-Kutta
[t, y] = ida(res, [t0 tf], y0, yp0)             // implicit DAE, r = res(t, y, yp)
x = kinsol(F, x0)                               // non-linear system F(x) = 0
```
- `f(t, y)` returns a column; `y` is (states x times), `t` a row.
- Options are NAMED: `method` ("ADAMS"/"BDF" for cvode), `rtol`, `atol`
  (scalar or per-component vector), `hMax` (max step — NOT "maxStep"),
  `maxSteps` (default 500 internal steps between outputs: raise it with
  tight tolerances or long spans, else "mxstep steps taken" error).
- Events: `events=evFn` with `function [e, term, dir] = evFn(t, y)` —
  `e` the zero-crossing functions, `term = 1` stops the integration,
  `dir = -1/1/0` crossing direction. Results in `info.te`, `info.ye`,
  `info.ie`.
```scilab
function [e, term, dir] = ground(t, y)
    e = y(1); term = 1; dir = -1;               // stop when altitude falls to 0
endfunction
[t, y, info] = cvode(fall, [0 10], [10; 0], events=ground);   // info.te = impact time
```
- Continue a solution: `sol = cvode(...); sol2 = cvode(sol, tfinal)`.
- Energy-conserving (Hamiltonian) problems over long times: `sprkode`
  (symplectic), fixed step REQUIRED. Both functions take the FULL state and
  return a full-size vector, each filling only its own half:
```scilab
function d = f1(t, pq), d = [-pq(2); 0]; endfunction   // dp/dt part (p = pq(1))
function d = f2(t, pq), d = [0; pq(1)];  endfunction   // dq/dt part (q = pq(2))
[t, pq] = sprkode(f1, f2, [0 20], [1; 0], fixedStep=0.1);
```
  Long spans need `maxSteps=` too. Oscillator over 1000 periods: energy
  drift ~5e-8 with sprkode vs ~0.15 with cvode defaults.

## ode (classic)


```scilab
y = ode(y0, t0, t, f)
[y, w, iw] = ode([type,] y0, t0, t [,rtol [,atol]], f [,jac] [,w, iw])
[y, rd] = ode("root", y0, t0, t [,rtol [,atol]], f [,jac], ng, g)
y = ode("discrete", y0, t0, t, f)
```

- `y0`: initial state, COLUMN vector. `t0`: initial time. `t`: row vector of
  output times (t0 not repeated in it).
- `f(t, y)` returns dy/dt as a column the same size as `y0`.
- Result `y` has one COLUMN per requested time: plot with `plot(t, y(1,:))`.

| MATLAB (DO NOT USE) | Scilab |
|---------------------|--------|
| `[t,y]=ode45(f,tspan,y0)` | `[t, y] = cvode(f, tspan, y0)` (y transposed vs MATLAB), or `y = ode(y0, t0, t, f)` |
| `ode15s` | `cvode(..., method="BDF")` or `ode("stiff", y0, t0, t, f)` |
| `odeset('RelTol',...)` | named options `rtol=`, `atol=`, `maxSteps=` (cvode), or positional `rtol, atol` (ode) |
| event function + `'Events'` | `cvode(..., events=evFn)`, or `ode("root", ...)` with `ng`, `g` |
| `ode15i` / DAE | `ida(res, tspan, y0, yp0)` or `dae` |
| `fsolve` (for F(x)=0) | `kinsol(F, x0)` or `fsolve` |
| `deval` | ask for the needed `t` grid directly |

## Basic example (pendulum)

```scilab
function dy = pendulum(t, y)
    dy = [y(2); -9.81/0.5*sin(y(1)) - 0.1*y(2)];
endfunction

t = linspace(0, 10, 500);
y = ode([%pi/4; 0], 0, t, pendulum);
plot(t, y(1,:));
```

## Extra parameters: list(f, p1, p2, ...)

Do NOT use global variables; pass parameters with a list:

```scilab
function dy = f(t, y, a, b)
    dy = a*y + b;
endfunction
y = ode(1, 0, t, list(f, -2, 0.5));   // calls f(t, y, -2, 0.5)
```

## Stiff problems

`ode("stiff", y0, t0, t, f)` (BDF). Providing the Jacobian helps:
`ode("stiff", y0, t0, t, f, jac)` with `J = jac(t, y)`. Other types:
`"adams"`, `"rk"`, `"rkf"`, `"fix"`, `"discrete"`. `"rkf"` FAILS on a dense
output grid ("rkf45 exit with state 7: too much output"): keep the default
or use `"fix"`/`cvode` when you need many output points.

## Events — ode("root")

Integration stops when any component of `g(t, y)` crosses zero:

```scilab
function r = hitground(t, y)
    r = y(1);                      // altitude = 0
endfunction
[y, rd] = ode("root", [10; 0], 0, 0:0.01:10, fall, 1, hitground);
// rd(1) = stop time; rd(2:$) = indices of the triggered surfaces.
// y contains the columns computed up to the event: y(:,$) is the state there.
```

`ng` is the number of components of `g`. To continue after the event
(e.g. a bouncing ball), restart `ode("root", ...)` from `rd(1)` with the
modified state in a loop.

## Discrete systems

`ode("discrete", y0, k0, kvect, f)` iterates `y(k+1) = f(k, y(k))` —
no interpolation, `kvect` are integer steps.

## Implicit systems — dae

For `F(t, y, ydot) = 0` (index-1 DAE):

```scilab
function [r, ires] = res(t, y, ydot)
    r = [ydot(1) - y(2); y(1)^2 + y(2)^2 - 1];
    ires = 0;
endfunction
y = dae([y0, ydot0], t0, t, res);
```

Initial `ydot0` must be consistent (`F(t0,y0,ydot0)=0`). Each output
column stacks `[y; ydot]` (rows 1:n = y, n+1:2n = ydot). `dassl`, `dasrt`,
`daskr` and `impl` were removed: use `dae` or `ida`.

## Tolerances and options

- Defaults: `rtol = 1e-5` (1e-7 to 1e-9 for orbital/chaotic dynamics),
  `atol = 1e-7`. Pass them positionally after `t`.
- NEVER call `odeoptions()` or `daeoptions()`: they open a modal dialog
  that blocks the session (and the assistant's tool call). Set the vector
  directly, e.g. `%ODEOPTIONS = [1, 0, 0, %inf, 0, 2, 10000, 12, 5, 0, -1, -1]`
  (7th entry = max steps; see scilab_help odeoptions for the layout), and
  `clear %ODEOPTIONS` to restore defaults. Simpler: use cvode's named options.

## Pitfalls

- `f` MUST return a column; a row silently produces wrong reshapes.
- `y` is (states x times): `y(:, $)` is the final state, `y(1, :)` the
  first state's trajectory — do not transpose blindly.
- An unstable/exploding simulation is usually the MODEL or the time span,
  not the solver: check signs and units before shrinking tolerances.
- Long simulations: request fewer output points (`t` grid) rather than
  shrinking the internal step; the solver steps adaptively anyway.
