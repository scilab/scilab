// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Simon GARESTE <simon.gareste@scilab.org>
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- ENGLISH IMPOSED -->

// <-- CLI SHELL MODE -->

load("SCI/modules/atoms/macros/atoms_internals/lib");

// If previous test did not end properly, restore, else backup config file
atomsRestoreConfig(%T);
atomsSaveConfig();

// Do not use the autoload system
atomsSetConfig("autoloadAddAfterInstall","False");
atomsSetConfig("Verbose" ,"False");
config_downloadTool = atomsGetConfig("downloadTool");

// Start config
// =============================================================================
ref=struct("useProxy", "False", "downloadTool",config_downloadTool,"autoloadAddAfterInstall","False","Verbose","False");
assert_checkequal(atomsGetConfig(),ref);

// CamelCase test
// =============================================================================
ref=struct("useProxy", "False", "downloadTool",config_downloadTool,"autoloadAddAfterInstall","False","Verbose","True");
assert_checkequal(string(atomsSetConfig("Verbose","True")),"1");
assert_checkequal(atomsGetConfig(),ref);
assert_checkequal(string(atomsSetConfig("verbose","true")),"0");
assert_checkequal(atomsGetConfig(),ref);
assert_checkequal(string(atomsSetConfig("verbose","True")),"0");
assert_checkequal(atomsGetConfig(),ref);
assert_checkequal(string(atomsSetConfig("Verbose","true")),"0");
assert_checkequal(atomsGetConfig(),ref);
assert_checkequal(string(atomsSetConfig("Verbose","True")),"0");
assert_checkequal(atomsGetConfig(),ref);

// Wrong key/value test
// =============================================================================
assert_checkerror("atomsSetConfig(""verbose"",""scilab"")","scilab: Wrong value for input configuration argument: True or False expected.");
assert_checkequal(atomsGetConfig(),ref);
assert_checkerror("atomsSetConfig(""scilab"",""true"")","scilab: Wrong key for input configuration argument.");
assert_checkequal(atomsGetConfig(),ref);

// Exhaustive key list test
// =============================================================================
ref=struct("useProxy","False",..
"proxyUser","scilab",..
"proxyPort","42",..
"proxyPassword","scilab",..
"proxyHost","myproxy",..
"offline","False",..
"downloadTool",config_downloadTool,..
"autoloadAddAfterInstall","False",..
"autoload","True",..
"Verbose","True");

assert_checkequal(string(atomsSetConfig("useProxy","False")),"0");
assert_checkequal(string(atomsSetConfig("proxyHost","myproxy")),"1");
assert_checkequal(string(atomsSetConfig("proxyPort","42")),"1");
assert_checkequal(string(atomsSetConfig("proxyUser","scilab")),"1");
assert_checkequal(string(atomsSetConfig("proxyPassword","scilab")),"1");
assert_checkequal(string(atomsSetConfig("downloadTool",config_downloadTool)),"0");
assert_checkequal(string(atomsSetConfig("offline","False")),"1");
assert_checkequal(string(atomsSetConfig("autoload","True")),"1");
assert_checkequal(atomsGetConfig(),ref);


// Restore original values
// =============================================================================
atomsRestoreConfig(%T);
