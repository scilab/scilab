// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - SCilab Enterprises
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

// <-- Non-regression test for bug 12950 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=12950
//
// <-- Short Description -->
// getURL ignores the proxy settings

atomsSaveConfig(%T);

atomsSetConfig("useProxy", "True");
atomsSetConfig("proxyHost", "123aa");

instr = "getURL(''http://www.scilab.org'', ''scilab_homepage.html'');";

if getos() == 'Windows' then
  errReason = "Could not resolve proxy: 123aa; Host not found";
else
  errReason = "Couldn''t resolve proxy ''123aa''";
end
errMsg =  msprintf(gettext("Transfer did not complete successfully: %s\n"), errReason);
assert_checkerror(instr, errMsg);

filepath = fullfile(pwd(), 'scilab_homepage.html');
if isfile(filepath) then
  deletefile(filepath);
end;

atomsRestoreConfig();
