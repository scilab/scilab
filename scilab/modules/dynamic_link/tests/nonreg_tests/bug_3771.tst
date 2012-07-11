// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - DIGITEO - Mickael BAUDIN
// Copyright (C) 2008-2010 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- ENGLISH IMPOSED -->
// <-- CLI SHELL MODE -->

// <-- Non-regression test for bug 3771 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=3771
//
// <-- Short Description -->
// Manage absolute path names in ilib_gen_loader
// =============================================================================
cd(TMPDIR);
name = "skeleton";
// call a link
tables = [];
libs = [SCI + "/bin/linear_algebra_f", SCI + "/bin/linear_algebra"];
ilib_gen_loader(name, tables, libs);
res1 = mgetl('loader.sce');
grepaboutlink = grep(res1, 'link(');
if size(grepaboutlink, '*') <> 5 then pause,end
refONE = "link(''" + libs(1) + "'' + getdynlibext());";
refTWO = "link(''" + libs(2) + "'' + getdynlibext());";
if res1(grepaboutlink(3)) <> refONE then pause,end
if res1(grepaboutlink(4)) <> refTWO then pause,end
// =============================================================================
name = "skeleton2";
tables = ['f1','sci_f1';'f2','sci_f2'];
// call addinter
libs = [SCI + "/bin/linear_algebra_f", SCI + "/bin/linear_algebra"];
ilib_gen_loader(name, tables, libs);
res2 = mgetl('loader.sce');
grepaboutlink = grep(res2, 'link');
if size(grepaboutlink, '*') <> 5 then pause,end
refONE = "link(''" + libs(1) + "'' + getdynlibext());";
refTWO = "link(''" + libs(2) + "'' + getdynlibext());";
if res2(grepaboutlink(4)) <> refONE then pause,end
if res2(grepaboutlink(5)) <> refTWO then pause,end
// =============================================================================
