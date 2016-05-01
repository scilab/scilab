// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - DIGITEO - Vincent COUVERT
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
//
// <-- Non-regression test for bug 7163 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=7163
//
// <-- Short Description -->
// tree2code wrongly replaced every (:) occurence in a function definition with (eye()).

function foo(r)
    r(:)=1
endfunction

txt = tree2code(macr2tree(foo));

if or(txt<>["function [] = foo(r)";"r(:) = 1";"endfunction";""]) then pause; end
