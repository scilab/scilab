// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

// <-- Non-regression test for bug 12956 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=12956
//
// <-- Short Description -->
// splitURL with no protocol in URL provokes an access violation exception

// No protocol
[proto, server, path, query, username, port, frag] = splitURL("www.scilab.org");
assert_checkequal(proto, "");
assert_checkequal(server, "");
assert_checkequal(path, "www.scilab.org");
assert_checkequal(query, "");
assert_checkequal(username, "");
assert_checkequal(port, int32(0));
assert_checkequal(frag, "");
