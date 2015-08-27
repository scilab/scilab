//<-- CLI SHELL MODE -->
// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - INRIA - Serge Steer
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- Non-regression test for bug 4266 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=4266
//
// <-- Short Description -->
// assigning a string to an element of a matrix of numbers gives an error. 
a=zeros(1,5);
if execstr('a(1)=''a'';','errcatch')==0 then pause,end

