// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - Digiteo - Yann Collette
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->

// <-- Non-regression test for bug 4432 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=4432
//
// <-- Short Description -->
// When the variation of f of a data set is too small, plot hangs
// 

s = sin(0:0.1:100);

scf();

subplot(411);
m = 0;
ierr = execstr('plot(m+s);','errcatch');
if ierr~=0 then pause; end;

subplot(412);
m = 1e6;
ierr = execstr('plot(m+s);','errcatch');
if ierr~=0 then pause; end;

subplot(413);
m = 1e8;
ierr = execstr('plot(m+s);','errcatch');
if ierr~=0 then pause; end;

subplot(414);
m = 1e9;
ierr = execstr('plot(m+s);','errcatch');
if ierr~=0 then pause; end;
