// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
//
// Copyright (C) 2021-2023 - UTC - Stéphane MOTTELET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->

//HEAT EQUATION, BAND solver vs FULL
function [dv] = f_chaleur(t,v,h,lambda,c,rho,f)
  lambda_over_h2 = lambda/h/h;
  c_rho = c*rho;
  dv = (f+lambda_over_h2*( [0;v(1:$-1)] -2*v +[v(2:$);0] ))/c_rho;
endfunction
function j = jac_f_chaleur(t,v)
    j = JCONSTBAND;
endfunction

L=1; N=2000;
dx = L/N; x = linspace(dx,L-dx,N-1)';
lambda = 1;
c=200;
rho=7893;
d=0.02;
tf=300;
section=%pi*d^2/4;
rhoLin=rho*section;
f = x>1/4 & x<1/3;
v0=zeros(N-1,1);
tspan=[0 logspace(0,3,19)]// vecteur des temps

JCONSTBAND = -lambda/c/rhoLin/dx/dx*[0 -ones(1,N-2); 2*ones(1,N-1); -ones(1,N-2) 0];
JCONSTSPARSE = -lambda/c/rhoLin/dx/dx*(diag(sparse(2*ones(1,N-1)))-diag(sparse(ones(1,N-2)),-1)-diag(sparse(ones(1,N-2)),1));

// Problem definition, also used by tests/benchmarks/sundials_band.tst
HEAT = list(f_chaleur,dx,lambda,c,rhoLin,f);

// CVODE

timer();
[t,v] = cvode(HEAT,tspan,v0,jacBand=[1,1]);
t2cvode=timer()

[t,v] = cvode(HEAT,tspan,v0,jacBand=[1,1],jacobian=jac_f_chaleur);
t3cvode=timer()

[tb,vb] = cvode(HEAT,tspan,v0,jacBand=[1,1],jacobian=JCONSTBAND);
t4cvode=timer()
[ts,vs] = cvode(HEAT,tspan,v0,jacobian=JCONSTSPARSE);

assert_checkalmostequal(vb,vs)

// ARKODE

timer();
[t,v] = arkode(HEAT,tspan,v0,jacBand=[1,1],atol=1e-9);
t2arkode=timer()

[t,v] = arkode(HEAT,tspan,v0,jacBand=[1,1],jacobian=jac_f_chaleur,atol=1e-9);
t3arkode=timer()

[tb,vb] = arkode(HEAT,tspan,v0,jacBand=[1,1],jacobian=JCONSTBAND,atol=1e-9);
t4arkode=timer()
[ts,vs] = arkode(HEAT,tspan,v0,jacobian=JCONSTSPARSE,atol=1e-9);

assert_checkalmostequal(vb,vs,1e-6)

// SUNDIALS API DLL entrypoints (rhs and band Jacobian)

heatcode=[
"#include <nvector/nvector_serial.h>"
"#include <sunmatrix/sunmatrix_band.h>"
"int heatRhs(sunrealtype t, N_Vector V, N_Vector Vd, void *user_data)  "
"{"
"    int n = NV_LENGTH_S(V);"
"    double *v = NV_DATA_S(V);"
"    double *vdot = NV_DATA_S(Vd);"
"    double *par = (double*) user_data;"
"    double dx = par[0];"
"    double lambda_over_dx2 = par[1]/dx/dx;"
"    double c_rho = par[2]*par[3];"
"    double f = 0.0;"
"    double x = dx;"
""
"    vdot[0] = lambda_over_dx2*(-2.0*v[0]+v[1])/c_rho;"
"    for (int i=1; i < n-1; i++)"
"    {"
"        x = (i+1)*dx;"
"        f = x>1.0/4.0 && x<1.0/3.0 ? 1.0 : 0.0;"
"        vdot[i] = (f+lambda_over_dx2*(v[i-1]-2.0*v[i]+v[i+1]))/c_rho;"
"    }"
"    vdot[n-1] = lambda_over_dx2*(-2.0*v[n-1]+v[n-2])/c_rho;"
"    return 0;"
"}"
""
"int heatJac(sunrealtype t, N_Vector V, N_Vector Vd, SUNMatrix J, void *user_data, N_Vector tmp1, N_Vector tmp2, N_Vector tmp3)"
"{"
"    double *par = (double*) user_data;"
"    double dx = par[0];"
"    double lambda = par[1];"
"    double c = par[2];"
"    double rhoLin = par[3];"
"    double alpha = lambda/c/rhoLin/dx/dx;"
""
"   for (int j=0; j < NV_LENGTH_S(V); j++)"
"   {"
"       double *diagj = SM_COLUMN_B(J,j);"
"       diagj[-1] = alpha; diagj[0] = -2*alpha; diagj[1] = alpha;"
"   }"
"   return 0;"
"}"
]

mputl(heatcode,TMPDIR+"/heat.c")
SUN_Clink(["heatRhs";"heatJac"],TMPDIR+"/heat.c",load=%t);

[t,v,info] = cvode(list("heatRhs",[dx,lambda,c,rhoLin]),tspan,v0,jacBand=[1 1]);
[t1,v1,info1] = cvode(HEAT,tspan,v0,jacBand=[1,1]);
assert_checkalmostequal(v,v1)

[t,v,info] = cvode(list("heatRhs",[dx,lambda,c,rhoLin]),tspan,v0,jacBand=[1 1],jacobian=list("heatJac",[dx,lambda,c,rhoLin]));
[t1,v1,info1] = cvode(list("heatRhs",[dx,lambda,c,rhoLin]),tspan,v0,jacBand=[1 1],jacobian=JCONSTBAND);
[t2,v2,info2] = cvode(HEAT,tspan,v0,jacBand=[1,1],jacobian=JCONSTBAND);
assert_checkalmostequal(v,v1)
assert_checkalmostequal(v,v2)







