// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
// <-- CLI SHELL MODE -->
//
// <-- NO CHECK ERROR OUTPUT -->
//
// <-- Non-regression test for bug 8462 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=8462
//
// <-- Short Description -->
// bvodeS crashed on Windows and did not return a error.


// calcul de concentration dans un grain sphérique
L0 = 0; // m
L = 2e-3; // rayon du grain (m)
D = 1e-5; // (m2.s-1)
Cs = 1e-1; // mol/l
k = 1e-1; // mol^(-n-1)/s
n = 1; // ordre de la réaction
Phi = sqrt(k*Cs^n*L^2/(D*Cs));

// l'equation de diffusion : D d2C/dr2 + 2/r dC/dr = rA est rendue adimensionnelle:
// d2C'/dr'2+ 2/r'* dC'/dr' =Phi^2 C'^n
// Conditions aux limites C'(1)=1 , dC'/dr'(0)=0
global Phi L n

function rhs=fsub(x,z) // dérivées du système d'équations différentielles
    global Phi L n
    // z(1)=C'
    // z(2)=dC'/dr'

    if x==0 then
        rhs = Phi^2 * z(1)^n;
    else
        rhs = Phi^2 * z(1)^n // - 2/x * z(2);
    end

endfunction

function g=gsub(i,z) // conditions aux limites
    g(1)=z(2)
    g(2)=z(1)-1
    g=g'
endfunction

o = 1; // nombre d'équations différentielles
m = [2]; // ordre des équations différentielles du système (ici, 1 équations d'ordre 2)
zeta = [L0 L]; // points liés aux conditions aux limites dans l'ordre des valeurs
nx = 11;
x = linspace(L0,L,nx); // points de discrétisation

//as stated in the doc :
// 1 <= ltol(1) < ltol(2) < ... < ltol(ntol) <= M where M is sum(m)
ltol = m;
tol = 1e-14;

function [z,lhs]=ystart(x)
    z = zeros(2,1);
    z(1) = 1;
    z(2) = 0;
    lhs(1)= 0;
endfunction

ierr = 0;
ierr = execstr("z = bvodeS(x,m,o,L0,L,fsub,gsub,zeta,ltol=ltol,tol=tol,ystart=ystart)", "errcatch");
if ierr <> 999 then pause, end
