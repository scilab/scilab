// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2006-2008 - INRIA - Serge.Steer@inria.fr
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->

// <-- Non-regression test for bug 6665 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=6665
//
// <-- Short Description -->
//legend (caption) nor properly saved or loaded
plot2d();
legend(['a';'b';'c']);
f=gcf();
save(TMPDIR+'/foo.scg','f');
clf();
load(TMPDIR+'/foo.scg');
if f.children.children(1).type<>'Legend' then pause,end
l=f.children.children(1);
if or(l.text<>['a';'b';'c']) then pause,end

