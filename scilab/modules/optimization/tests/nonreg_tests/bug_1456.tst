// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Vincent Couvert <vincent.couvert@inria.fr>
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

// <-- Non-regression test for bug 1456 -->
// NOT FIXED
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=1456
//
// <-- Short Description -->
//  Detailed description of the problem
// Numerical problem with lmisolver
// Text of the scilab error message
// Z0 does not satisfy equality conditions for dual feasibility.
//  !--error   230
// semidef fails
// at line     238 of function lmisolver                called by :
// [XLISTF,OPT]=lmisolver(list(eye(A)),fct)

// <-- NOT FIXED -->

A=[1 -24 0 -240;1 -12 -60 -120;0 1 0 0;0 0 1 0];

function [LME,LMI,OBJ]=fct(XLIST)
	T=XLIST(1);
	LME=T-T';
	LMI=-A*T-T*A'-eye();
	OBJ=[];
endfunction

[XLISTF,OPT]=lmisolver(list(eye(A)),fct);
Tcomputed = XLISTF(1);
Texpected = [9.898862973943269 , -0.148040709667531 ,  0.084466211100534  ,  0.056050336743578 ;
  -0.148040709667531 ,  0.009915577328041 , -0.000240812494752 , -0.002102817295201 ;
   0.084466211100534 , -0.000240812494752 ,  0.002102817295201 , -0.000240812494554 ;
   0.056050336743578 , -0.002102817295201 , -0.000240812494554 , 0.000795767345537];
Texpected=   1.0e+08 * Texpected;
if or(abs(Tcomputed-Texpected)>%eps) then bugmes();quit;end
