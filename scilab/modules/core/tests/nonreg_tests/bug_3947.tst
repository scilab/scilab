// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- Non-regression test for bug 3687 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=3687
//
// <-- Short Description -->
// "format" doesn't check input value 

txt=[sci2map([1 2;3 4],'a');sci2map(%s^2+3*%s+4,'p')];

ref_txt = [ 'a := array(1..2,1..2);';
'a[1,1] := 1;';
'a[1,2] := 2;';
'a[2,1] := 3;';
'a[2,2] := 4;';
'p := 4+s*(3+s) ;'];
if and(txt <> ref_txt) then pause,end

fmt = format();
format(fmt(2),fmt(1));
fmt2 = format();
if  fmt <> fmt2 then pause,end