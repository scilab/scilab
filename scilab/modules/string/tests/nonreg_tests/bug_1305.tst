// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Vincent COUVERT
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

// <-- Non-regression test for bug 1305 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=1305
// tree2code doesn't indent the body of the function code in beautify mode; moreover, it adds a blank line before endfunction

function b=bobo(); a=7; if a>1 then; b=5; end; endfunction

txt = tree2code(macr2tree(bobo),%t);

txtref = ["function [b] = bobo()";"  a = 7;";"  if a>1 then";"    b = 5;";"  end;";"endfunction";""];

if or(txt<>txtref) then pause; end
