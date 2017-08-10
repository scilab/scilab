//<-- CLI SHELL MODE -->
// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2005-2008 - INRIA - Pierre MARECHAL <pierre.marechal@inria.fr>
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- Non-regression test for bug 1200 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=1200
//
// <-- Short Description -->
//    Incoherence dans le format d'un nombre.
//    En prenant par exemple j=0.001, on obtient un affichage 
//    incoherent avec format :
//    format("v",6) => 0.001
//    format("v",7) => 1.E-3
//    format("v",8) => 0.001

J=0.001;
format("v",7);
diary(TMPDIR+"/bug1200.dia");
disp(J);
diary(0);

DIAFILECONTENTS=mgetl(TMPDIR+"/bug1200.dia");

REFFILECONTENTS=[" ";
"-->disp(J);";
" ";
"    0.001  ";
" ";
"-->diary(0);"];
  

if or(DIAFILECONTENTS<>REFFILECONTENTS) then pause,end
