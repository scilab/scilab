// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008-2008 - INRIA - Jean-Baptiste Silvy
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->

// <-- Non-regression test for bug 2968 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=2968
//
// <-- Short Description -->
// Subplot generates error when an uicontrol is inserted in the figure.

subplot(1,2,1);
h = gcf();
h1 = uicontrol(h,'style','listbox','position',[10 10 150 160]);
set(h1,'string',"item 1|item 2|item 3");
set(h1,'value',[1 3]);

// used to generates an error
ierr = execstr('subplot(1,2,2)','errcatch');
if ierr <> 0 then pause, end

X=1:100;
plot(X,X);
