// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - 2010 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
//
// <-- Non-regression test for bug 4968 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=4968
//
// <-- Short Description -->
// part() or strsubst() problem with UTF-8 when exec ???
// =============================================================================
URL = "http://www.scilab.org/abc.htm";
// =============================================================================
tmp = strsubst(URL, "//", "§");
if tmp <> "http:§www.scilab.org/abc.htm" then pause,end
// =============================================================================
i = strcspn(tmp, "/");
if length("http:§www.scilab.org") <> 20 then pause, end
if i <> 20 then pause,end
// =============================================================================
server = strsubst(part(tmp, 1:i), "§", "//");
if part(tmp, 1:i) <> 'http:§www.scilab.org' then pause, end
if server <> 'http://www.scilab.org' then pause, end
// =============================================================================
