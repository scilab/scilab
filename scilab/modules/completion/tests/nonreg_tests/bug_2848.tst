// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- Non-regression test for bug 2848 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=2848
//
// <-- Short Description -->
//    When using the TAB key for completion in the console, after using once a Scilab
//    macro, its name appears twice in the completion list: as a macro and as a variable.


with_atlas();
[a,b,c,d]=completion('with');
if c <> [] then pause,end

with_tk();
[a,b,c,d]=completion('with');
if c <> [] then pause,end

clear with_atlas
clear with_tk
[a,b,c,d]=completion('with');
if c <> [] then pause,end

with_atlas();
with_tk();
deff('[x]=with_foo(y,z)','x=y+z');
[a,b,c,d]=completion('with');
if or(c <> 'with_foo') then pause,end

clear with_atlas
clear with_tk
clear with_foo

[a,b,c,d]=completion('with');
if c <> [] then pause,end
