// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- TEST WITH GRAPHIC -->  
//
// <-- Non-regression test for bug 4948 -->
// 
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=4948
//
// <-- Short Description -->
// implement a simple Runge Kutta method for solving a second order 
// differential equation have a strange behaviour.

///   VARIABLES DECLARATION   ///


clear;

funcprot(0);


v0 = 0;// INITIAL CONDITIONS OF THE DIFFERENTIAL EQUATION
u0 = 0;// INITIAL CONDITIONS OF THE DIFFERENTIAL EQUATION

y0 = [v0;  u0]; 


F = 1;  // system's parameters
m = 1;  // system's parameters
c = 0.5;  // system's parameters
k = 1;  // system's parameters
h = 0.05; //[sec]
t = 0:h:20;
y = zeros(2,length(t));

y(:,1) = y0;  //sets initial condition
v = 0;
u = 0;
// [v u] = 0;


/// DEREVATIVE FUNCTION
function uvdot = f(t,y)
    uvdot = [ -c/m , -k/m ; 1 , 0] * y + [F/m; 0];
endfunction

// RUNGE KUTTA FUNCTION
function yn = rk(t,y)
  k1 = f(t,y);
  k2 = f(t+0.5*h,y+0.5*h*k1);
  k3 = f(t+0.5*h,y+0.5*h*k2);
  k4 = f(t+h,y+h*k3);
  yn = y + h*(k1 + 2*k2 + 2*k3 + k4)/6;
endfunction

// SOLVING THE DIFF. EQUATION 
for i = 1:(length(t) - 1)
  y(:,i+1) = rk(t(i),y(:,i));
end

plot(t,y(1,:));
xgrid();
