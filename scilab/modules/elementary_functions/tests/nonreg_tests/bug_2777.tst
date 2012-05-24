// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

c = ["Formula_1"  "Formula_2"  "Formula_3"  "Formula_4"];
if ( intersect("Formula_2", c ) <> "Formula_2" ) then pause,end;
if intersect("Formula_1", c ) <> "Formula_1" then pause,end;
if intersect("Formula_4", c ) <> "Formula_4" then pause,end;

A = intersect(["Formula_1" "Formula_2"],c);
if( find(A=="Formula_1") ) == [] then pause,end;
if( find(A=="Formula_2") ) == [] then pause,end;
if( find(A=="Formula_3") ) <> [] then pause,end;
if( find(A=="Formula_4") ) <> [] then pause,end;

B = intersect(c,c);
if( find(B=="Formula_1") ) == [] then pause,end;
if( find(B=="Formula_2") ) == [] then pause,end;
if( find(B=="Formula_3") ) == [] then pause,end;
if( find(B=="Formula_4") ) == [] then pause,end;
