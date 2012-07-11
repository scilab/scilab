// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008-2008 - DIGITEO - Simon LIPP <simon.lipp@inria.fr>
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

// <-- Non-regression test for bug 3462 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=3462
//

// <-- Short Description -->
//   The match string returned by regexp is not the right one if the subject
//   string contains backslashes.

if regexp("\n", "/n/") <> 2 then pause,end
if regexp("\>15Hello, world.", "/world/") <> 12 then pause,end

[start,end,match] = regexp("/usr\local/en_US","/([a-z][a-z]_[A-Z][A-Z])$/");
if [start,end] <> [12,16] then pause,end;
if match <> "en_US" then pause,end