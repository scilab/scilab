// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- Non-regression test for bug 4067 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=4067
//
// <-- Short Description -->
// Completion STD mode.

// <-- CLI SHELL MODE -->

ilib_verbose(0);
ierr = exec(SCI+"/modules/completion/tests/utilities/build_primitives.sce","errcatch",-1);
if ierr<>0 then pause, end
ierr = exec(TMPDIR + "/completion/loader.sce","errcatch",-1);
if ierr<>0 then pause, end

chdir(TMPDIR);
mkdir('bug_4067');
chdir('bug_4067');
fd = mopen('bugA.sce','wt');
mclose(fd);
fd = mopen('bugB.sce','wt');
mclose(fd);

//exec bug<tab>
currentline = 'exec bug';
r = getfilepartlevel(currentline);
if r <> 'bug' then pause,end
r = completion(getfilepartlevel(currentline),'files');
if and(gsort(r) <> gsort(['bugA.sce';'bugB.sce'])) then pause,end

newline = completeline(currentline,'bugA.sce',getfilepartlevel(currentline),getpartlevel(currentline),%t);
if newline<>'exec bugA.sce' then pause,end

// -->cd /h<TAB>
currentline = 'cd /h';
r = getfilepartlevel(currentline);
if r <> '/h' then pause,end
newline = completeline(currentline,'/home',getfilepartlevel(currentline),getpartlevel(currentline),%t);
if newline<>'cd /home' then pause,end
