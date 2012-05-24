// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2005-2008 - INRIA - Pierre MARECHAL <pierre.marechal@inria.fr>
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

// <-- Non-regression test for bug 871 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=871
//
// <-- Short Description -->
//    The Semicolon operator does not work on functions called
//    without parenthesis on args '()'. For instance:
//
//    --> rand()         // OK
//    ans  =
//
//        0.2113249  
//
//    --> rand();         // OK
//    [NO OUTPUT]
//
//    --> rand            // OK
//     ans  =
//
//        0.7560439 
//
//    --> rand;          // BUG: the semicolon doesn't work!!!!
//     ans  =
//
//        0.7560439

SCIFILE=TMPDIR+"/bug871.dia";

diary(SCIFILE);
rand;
diary(0);

SCIFILECONTENTS=mgetl(SCIFILE);

REFCONTENTS=[" ";
			"-->rand;";
			" ";
			"-->diary(0);"];

if or(REFCONTENTS<>SCIFILECONTENTS) then pause,end
