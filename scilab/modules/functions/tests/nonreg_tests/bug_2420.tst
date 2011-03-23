// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2007-2008 - INRIA - Serge STEER <serge.steer@inria.fr>
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- JVM NOT MANDATORY -->

// <-- Non-regression test for bug 2420 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=2420
//
// <-- Short Description -->
//    fun2string converts the sequence \@ as carriage return

function a=strudel(),a='+\@+';endfunction
t=fun2string(strudel);
if stripblanks(t)<>"function a=ans(),a = ''+\@+'';endfunction" then pause,end
