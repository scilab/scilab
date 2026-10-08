---
name: optimization
description: |
  Numerical optimization in Scilab: choosing the right solver (optim,
  fminsearch, leastsq, lsqrsolve, datafit, fsolve, karmarkar, qpsolve) and the
  idiosyncratic optim cost-function signature. Trigger words: "minimize",
  "maximize", "optimization", "fit", "least squares", "curve fitting", "solve
  equation", "root", "linear programming", "quadratic programming", "fminsearch".
triggers: minimize, minimiser, minimise, maximize, maximiser, maximise, optimization, optimisation, fit, fitting, ajuster, ajustement, moindres carrés, least squares, curve fitting, fsolve, fminsearch, optim, leastsq, datafit, qpsolve, karmarkar, root, racine, racines, solve, résoudre, résous, résolution, équation, équations, système non linéaire, ajuste, ajuster le modèle, front de pareto, linear programming, programmation linéaire, quadratic programming, programmation quadratique, optimum, extremum, minimum, maximum, contrainte, contraintes, constraint, constraints, régression, regression, polyfit, identifier, identification, paramètres optimaux, algorithme génétique, genetic algorithm, recuit simulé, simulated annealing, multi-objectif, pareto, fzero, fmincon, fminunc, linprog, quadprog, lsqcurvefit
---

# Scilab numerical optimization

## Which solver to use

| Problem | Function |
|---------|----------|
| Minimize smooth f(x), gradient available | `optim` (quasi-Newton, best default) |
| Minimize f(x), NO gradient | `optim` + `NDcost` wrapper, or `fminsearch` (Nelder-Mead, derivative-free) |
| Non-linear least squares min Σ f(x)² | `leastsq` (gradient-based) or `lsqrsolve` (Levenberg-Marquardt) |
| Fit model parameters to measured data | `datafit` (weighted, bound-constrained) |
| Solve non-linear system f(x) = 0 | `fsolve` |
| Linear programming | `karmarkar` |
| Quadratic programming | `qpsolve` / `qld` |
| Linear matrix inequalities | `lmisolver` / `lmitool` |
| Polynomial fit | `polyfit(x, y, n)` (descending powers, like MATLAB) |
| Linear least squares A*x ≈ b | `lsq(A, b)` or `A \ b` |
| Global / multimodal, non-smooth | `optim_ga` (genetic), `optim_sa` (annealing) |
| Multi-objective | `optim_moga`, `optim_nsga2` |

MATLAB names that DO NOT exist in Scilab: `fminunc`, `fmincon`, `lsqnonlin`,
`lsqcurvefit`, `linprog`, `quadprog`, `fzero`, `fminbnd`, `ga`, `fminimax`.
Map them to the table above:
- `fzero` → `fsolve` (1 unknown works fine)
- `fminbnd` → `optim(cost, "b", a, b, x0)` (1-D with bounds)
- `fmincon` with bounds only → `optim` with `"b"`; general non-linear
  constraints have no direct built-in equivalent (penalty method, or
  `optim_ga` with a penalized cost)
- `ga` → `optim_ga`
- `linpro`/`quapro` were removed (external quapro toolbox)

## optim — THE signature to get right
The cost function MUST have this exact signature — this is NOT MATLAB's
`fminunc` (which takes `f = cost(x)`):
```scilab
function [f, g, ind] = cost(x, ind)
    // f: objective value at x ; g: gradient at x ; ind: mode flag (pass through)
    xref = [1; 2; 3];
    f = 0.5 * norm(x - xref)^2;
    g = x - xref;
endfunction

x0 = [1; -1; 1];
[fopt, xopt] = optim(cost, x0)                    // quasi-Newton "qn" (default)
[fopt, xopt, gopt] = optim(cost, x0, "gc")        // conjugate gradient (large n)
[fopt, xopt, gopt] = optim(cost, x0, "nd")        // non-differentiable f
[fopt, xopt] = optim(cost, "b", lb, ub, x0)       // bound constraints
[fopt, xopt] = optim(cost, x0, "ar", nap, iter)   // limit calls/iterations
```
- `ind` is a communication flag with the solver: just declare it and pass it
  through. Setting `ind < 0` inside cost means "f not computable here";
  `ind = 0` interrupts the optimization.
- Extra model parameters: pass a list — `optim(list(cost, param1, param2), x0)`
  with `function [f, g, ind] = cost(x, ind, param1, param2)`.
- No gradient available? Wrap with NDcost (finite differences):
```scilab
function f = myf(x, a)
    f = a * norm(x)^2;
endfunction
[fopt, xopt] = optim(list(NDcost, myf, 0.5), x0)
```
- Do NOT name a user function `f` when NDcost is involved: NDcost has a
  local variable `f`, and by Scilab's dynamic scoping a cost function that
  calls your `f(x)` gets that scalar instead ("numderivative: ... Invalid
  index"). Use `fobj`, `cost`... (`varargin` in the signature is NOT needed.)

## fminsearch — derivative-free (MATLAB-compatible)
```scilab
[x, fval] = fminsearch(costf, x0)          // costf: function f = costf(x)
opt = optimset("TolX", 1e-6, "MaxIter", 500, "Display", "iter");
[x, fval, exitflag, output] = fminsearch(costf, x0, opt)
```
Robust but slow; fine for small dimension (n <= ~10), no constraints.

## Least squares & data fitting
```scilab
// leastsq: minimize ||fun(x)||^2 ; fun returns the residual VECTOR
function e = residual(x, tm, ym)
    e = ym - x(1) * exp(-x(2) * tm);
endfunction
[fopt, xopt] = leastsq(list(residual, tm, ym), x0)

// lsqrsolve: Levenberg-Marquardt, m = number of residuals
function e = fct(x, m)
    e = ym - x(1) * exp(-x(2) * tm);
endfunction
[xopt, v] = lsqrsolve(x0, fct, m)

// datafit: fit model G(p, z) to data columns Z, optional bounds
[p, dmin, status] = datafit(G, Z, p0)
[p, dmin, status] = datafit(G, Z, "b", p_inf, p_sup, p0)
```

## Non-linear equations
```scilab
[x, v, info] = fsolve(x0, fct)         // solves fct(x) = 0
[x, v, info] = fsolve(x0, fct, fjac)   // with jacobian, better convergence
```
Judge convergence on the residual `norm(v)`, not on `info` alone:
without a jacobian `fsolve` often returns `info = 4` ("not making good
progress") even at an exact solution (`norm(v)` ~ 1e-16). `info = 1`
means converged; with an analytic jacobian it is reported reliably.

## Global optimization (optim_ga)
```scilab
function f = rast(x)                        // multimodal test function
    f = 20 + sum(x.^2 - 10*cos(2*%pi*x));
endfunction
PGA = init_param();
PGA = add_param(PGA, "dimension", 2);
PGA = add_param(PGA, "minbound", [-5; -5]);
PGA = add_param(PGA, "maxbound", [5; 5]);
[pop_opt, fobj_opt] = optim_ga(rast, 50, 30, 0.1, 0.7, %f, PGA);
[fbest, k] = min(fobj_opt);  xbest = pop_opt(k);   // pop_opt is a list
```
Stochastic: rerun or refine the best point with `optim`.

Multi-objective: `optim_nsga2` / `optim_moga` return the final POPULATION,
which still contains dominated points — extract the front with
`[Fp, Xp] = pareto_filter(F, X)` (also works on a plain sampling of X).
They are slow (~30 s for 100 × 50 on a 1-D problem): for 1 or 2 variables,
a fine sampling + `pareto_filter` is exact and instant.

## Linear / quadratic programming
```scilab
xopt = karmarkar(Aeq, beq, c)                    // min c'x, Aeq*x = beq
xopt = karmarkar(Aeq, beq, c, [], [], [], [], [], A, b, lb, ub)  // + A*x <= b, bounds
[x] = qpsolve(Q, p, C, b, ci, cs, me)            // min 0.5 x'Qx + p'x
```
- karmarkar minimizes, is interior-point: results are ~1e-5 off the vertex
  by default. Tighten with `rtolf` (5th argument) around 1e-8; much smaller
  values (1e-10) can end on a spurious exitflag -2 ("unbounded").
- Once `A` or `lb` is given, x >= 0 is no longer implicit: pass `lb`.
- qpsolve: the first `me` rows of C are equalities; `Q` must be symmetric
  positive definite; bounds go to `ci`/`cs`, not into C.

## Guidelines
- Always test the cost/residual function alone on x0 with execute_scilab_code
  BEFORE calling the solver — most failures are in the user function
- Check convergence outputs (err/info/exitflag/status), don't just return xopt
- Minimizing? All solvers minimize; to maximize f, minimize -f
- For anything unusual (options, algorithms, termination criteria), read
  scilab_help optim / leastsq / fminsearch — the pages are detailed
