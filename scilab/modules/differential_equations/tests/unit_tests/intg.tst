// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2007-2008 - INRIA
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
//
// <-- ENGLISH IMPOSED -->

ilib_verbose(0);
// Function written in the Scilab language
function y = f(x), y = x*sin(30*x)/sqrt(1-((x/(2*%pi))^2)), endfunction
exact = -2.5432596188;
I = intg(0, 2*%pi, f);
if abs(exact-I) > 1e-9 then pause, end

// Function with an argument written in the Scilab language
function y = f1(x, w), y = x*sin(w*x)/sqrt(1-((x/(2*%pi))^2)), endfunction
I = intg(0, 2*%pi, list(f1, 30));
if abs(exact-I) > 1e-9 then pause, end

// Function written in Fortran (a Fortran compiler is required)
// define a Fortran function
cd TMPDIR;
F=["      double precision function ffun(x)"
"      double precision x, pi"
"      pi = 3.14159265358979312d+0"
"      ffun = x*sin(30.0d+0*x)/sqrt(1.0d+0-(x/(2.0d+0*pi))**2)"
"      return"
"      end"];
mputl(F, fullfile(TMPDIR, "ffun.f"));

// compile the function
l = ilib_for_link("ffun", "ffun.f", [], "f");

// add the function to the working environment
link(l, "ffun", "f");

// integrate the function
I = intg(0, 2*%pi, "ffun");
abs(exact-I);
if abs(exact-I) > 1e-9 then pause,end

// Function written in C (a C compiler is required)
// define a C function
C=["#include <math.h>"
"double cfun(double *x)"
"{"
"  double y, pi = 3.14159265358979312;"
"  y = *x/(2.0e0*pi);"
"  return *x*sin(30.0e0**x)/sqrt(1.0e0-y*y);"
"}"];
mputl(C, fullfile(TMPDIR, "cfun.c"));

// compile the function
l = ilib_for_link("cfun", "cfun.c", [], "c");

// add the function to the working environment
link(l, "cfun", "c");

// integrate the function
I = intg(0, 2*%pi, "cfun");
if abs(exact-I) > 1e-9 then pause,end

// Test third output argument
[i, err, ierr] = intg(0, 1, f);
if abs(ierr) <> 0 then pause,end

prot = funcprot();
funcprot(0);
function y = f(x), y = cos(x); endfunction
funcprot(prot);
[i, err, ierr] = intg(0, %pi, f);
if abs(ierr) <> 2 then pause,end

// IEEE compatibility
// Error 264: "Wrong value for argument #i: Must not contain NaN or Inf."
if execstr("I = intg(%inf, 0, f)", "errcatch")    <> 264 then pause,end
if execstr("I = intg(-%inf, 0, f)", "errcatch")   <> 264 then pause,end
if execstr("I = intg(%nan, 0, f)", "errcatch")    <> 264 then pause,end
if execstr("I = intg(0, %inf, f)", "errcatch")    <> 264 then pause,end
if execstr("I = intg(0, -%inf, f)", "errcatch")   <> 264 then pause,end
if execstr("I = intg(0, %nan, f)", "errcatch")    <> 264 then pause,end
if execstr("I = intg(%nan, %nan, f)", "errcatch") <> 264 then pause,end
