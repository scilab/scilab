// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - DIGITEO - Mickael BAUDIN
// Copyright (C) 2008 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- ENGLISH IMPOSED -->
// <-- JVM NOT MANDATORY -->

// <-- Non-regression test for bug 3771 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=3771
//
// <-- Short Description -->
// Manage absolute path names in ilib_gen_loader

cd TMPDIR
name = "skeleton";
tables = [];
libs = [SCI+"/bin/linear_algebra_f",SCI+"/bin/linear_algebra"];
ilib_gen_loader(name,tables,libs);
res = mgetl('loader.sce');
grepaboutlink = grep(res,'link');
if size(grepaboutlink,'*') <> 2 then pause,end
refONE = "link(''" + libs(1) + getdynlibext() + ""');";
refTWO = "link(''" + libs(2) + getdynlibext() + ""');";

if res(grepaboutlink(1)) <> refONE then pause,end
if res(grepaboutlink(2)) <> refTWO then pause,end
