// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2012 - DIGITEO - Antoine ELIAS
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- TEST WITH GRAPHIC -->
//
// <-- Non-regression test for bug 6644 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=6644
//
// <-- Short Description -->
//It is possible to attach uimenu to the default figure (good) but unfortunately
//it is not used when a new  figure is created.

f=gdf() ;
uimenu(f,'Label','foo');
assert_checkequal(f.children(1).type, "uimenu");

plot(1:10);
f=gcf();
assert_checkequal(f.children(2).type, "uimenu");
