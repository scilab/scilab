// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008-2008 - INRIA - Jean-Baptiste Silvy
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->

// <-- Non-regression test for bug 1903 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=1903
//
// <-- Short Description -->
// Under windows only, the surf function "eats" some facets

function z=ff(x,y),z=(2*x^2*y+y^2)/(x^2+2*y^2);endfunction
x=-7:0.4:7;
z=feval(x,x,ff);z(10 :15,17:22)=%nan;
f=gcf();f.color_map=hotcolormap(128);// choix de table de couleur

clf();surf(x,x,z)  

// the hole in the middle of the surface is expected
// but i Scilb 4.x, some facets are missing along the surface edges


