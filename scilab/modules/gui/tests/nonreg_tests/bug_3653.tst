// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Vincent COUVERT
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

//
// <-- Non-regression test for bug 3653 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=3653
//
// <-- Short Description -->
// When we create a figure with menus and axes as children, and want to delete the axes, the "delete" function deletes the uimenus too.

f=figure(1);
m=uimenu(f,"label","test");
a=gca();
delete(a);

menu = findobj("label","test");
if isempty(menu) then pause; end
if menu.type<>"uimenu" then pause; end

delete(f);
