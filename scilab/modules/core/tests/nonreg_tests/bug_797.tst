//<-- CLI SHELL MODE -->
// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2005-2008 - INRIA - Pierre MARECHAL <pierre.marechal@inria.fr>
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- Non-regression test for bug 797 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=797
//
// <-- Short Description -->
//    Toutes les versions
//
//    Objet : Comportemment �trange avec format
//    REF  01062004-1545
//
//    voici un comportement �trange avec format (test� sous la
//    2.7.2 sur Win 2000), celui-ci est il volontaire ?
//
//    ----------------------------------------------------------
//    -->J=0.001
//    J =
//
//    .001
//
//
//    -->format("v",6)
//    -->J
//    J =
//
//    .001 <- OK c'est bien ce qu'on souhaite 
// ...

J=0.001;
format("v",7);
diary(TMPDIR+"/bug797.dia");
disp(J);
diary(0);

DIAFILECONTENTS=mgetl(TMPDIR+"/bug797.dia");

REFFILECONTENTS=[" ";
	"    0.001  ";]

if or(DIAFILECONTENTS<>REFFILECONTENTS) then pause,end
