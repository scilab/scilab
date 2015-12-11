//<-- CLI SHELL MODE -->
// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2005-2008 - INRIA - Serge Steer
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- Non-regression test for bug 1962 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=1962
//
// <-- Short Description -->
// primitive commands does not work as expected when overloaded by a macro

// <-- ENGLISH IMPOSED -->

funcprot(0);

function ok=clear(varargin),ok=%t,endfunction
clear xxyyzz
if ~ans then pause,end
clear('xxyyzz')
if ~ans then pause,end
clear=null() //reset initial definition
