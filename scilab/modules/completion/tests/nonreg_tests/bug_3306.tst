// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- Non-regression test for bug 3306 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=3306
//
// <-- Short Description -->
// Automatic completion swallows the first completed letter when using double quotes (") around a string, while it gives good 

// <-- JVM NOT MANDATORY -->

ilib_verbose(0);

exec('SCI/modules/completion/tests/utilities/build_primitives.sce',-1);
exec('SCI/modules/completion/tests/utilities/loader.sce',-1);

fd = mopen(TMPDIR+'/foofile.ext','wt');
mclose(fd);

cd TMPDIR;

//scipad('foof[TAB]
currentline = "scipad(''foof";

r = getfilepartlevel(currentline);
if r <> 'foof' then pause,end;

r = completion(getfilepartlevel(currentline));
if r <> 'foofile.ext' then pause,end

r = completeline(currentline,'foofile.ext',getpartlevel(currentline),getfilepartlevel(currentline),%t);
if r <> 'scipad(''foofile.ext' then pause,end;

//scipad("foof[TAB]
currentline = "scipad(""foof";

r = getfilepartlevel(currentline);
if r <> 'foof' then pause,end

r = completion(getfilepartlevel(currentline));
if r <> 'foofile.ext' then pause,end

r = completeline(currentline,'foofile.ext',getfilepartlevel(currentline),getpartlevel(currentline),%t);
if r <> 'scipad(""foofile.ext' then pause,end;

                    