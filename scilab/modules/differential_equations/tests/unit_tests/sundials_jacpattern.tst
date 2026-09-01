// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
//
// Copyright (C) 2022 - UTC - Stéphane Mottelet
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// For more information, see the COPYING file which you should have received

// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->

function dydt = vdp(t,y,mu)
    dydt = [y(2,:)
            mu*(1-y(1,:).*y(1,:)).*y(2,:)-y(1,:)];
end

function out=vdpJac(t,y,mu)
    out = [0 1
          -2*mu*y(1)*y(2)-1 mu*(1-y(1)*y(1))];
end

mu = 1;
y0 = [2;1];

tspan = 0:0.1:10;

sp = sparse([0 1;1 1]);
[t,y,info0] = cvode(list(vdp,mu), tspan, y0, method="BDF");
[t,y,info1] = cvode(list(vdp,mu), tspan, y0, jacobian=vdpJac);
[t,y,info2] = cvode(list(vdp,mu), tspan, y0, jacPattern=sp);

function out=fun(t,f)
    out = LAP*f - f.*f.*f + f + 2;
endfunction

function out=jac(t,f)
    d = -3*f.*f+1;
    out = LAP+diag(sparse(d));
endfunction

// define square domain [-1,1] x [-1,1]
n = 50;
x=linspace(-1,1,n);
y=x;
[X,Y]=meshgrid(x,x);

// build finite differences operators
dx=x(2)-x(1);
d1x=sparse(ones(n-1,1));
d0x=sparse(ones(n,1));
lap = (diag(d1x,-1)+diag(d1x,1)-2*diag(d0x))/dx/dx;
// use Kronecker product to build matrix of d/dx^2 and d/dy^2
LAP = lap .*. speye(n,n) + speye(n,n) .*. lap;

f0 = ones(n*n,1);

// The dense reference run lives in tests/benchmarks/sundials_jacpattern.tst
[t,y1cvode,info1cvode]=cvode(fun,tspan,f0,jacBand=[n n]);
[t,y2cvode,info2cvode]=cvode(fun,tspan,f0,jacPattern=LAP);
[t,y3cvode,info3cvode]=cvode(fun,tspan,f0,jacobian=jac);

assert_checkalmostequal(y1cvode,y2cvode,1e-7);
assert_checkalmostequal(y1cvode,y3cvode,1e-7);
assert_checkalmostequal(y2cvode,y3cvode,1e-7);

assert_checkequal(info1cvode.stats.nRhsEvalsFD,303);
assert_checkequal(info2cvode.stats.nRhsEvalsFD,15);
assert_checkequal(info3cvode.stats.nRhsEvalsFD,0);

OPT.method="ARK548L2SA_DIRK_8_4_5";
OPT.rtol=1e-8;
OPT.atol=1e-10;
// The dense reference run lives in tests/benchmarks/sundials_jacpattern.tst
[t,y1arkode,info1arkode]=arkode(fun,tspan,f0,jacBand=[n n],options=OPT);
[t,y2arkode,info2arkode]=arkode(fun,tspan,f0,jacPattern=LAP,options=OPT);
[t,y3arkode,info3arkode]=arkode(fun,tspan,f0,jacobian=jac,options=OPT);

assert_checkalmostequal(y1arkode,y2arkode,1e-6);
assert_checkalmostequal(y1arkode,y3arkode,1e-6);
assert_checkalmostequal(y2arkode,y3arkode,1e-6);

assert_checkequal(info1arkode.stats.nRhsEvalsFD,5656);
assert_checkequal(info2arkode.stats.nRhsEvalsFD,280);
assert_checkequal(info3arkode.stats.nRhsEvalsFD,0);

