// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

//===============================
// Tests fileparts
//===============================

// Error messages

errmsg1 = msprintf(_("%s: Wrong number of input argument(s): %d to %d expected.\n"), "fileparts", 1, 2);
assert_checkerror("fileparts(SCI+''/etc/scilab.start'', ""extension"", ""extension"")", errmsg1);

errmsg2 = msprintf(_("%s: Wrong number of output arguments: %d expected.\n"),  "fileparts", 1);
assert_checkerror("[a,b] = fileparts(SCI+''/etc/scilab.start'',""extension"")", errmsg2);

errmsg3 = msprintf(_("%s: Wrong type for input argument #%d: string expected.\n"), "fileparts", 1);
assert_checkerror("[path,fname,extension] = fileparts([]);", errmsg3);

errmsg4 = msprintf(_("%s: Wrong value for input argument #%d.\n"), "fileparts", 2);
assert_checkerror("extension = fileparts(SCI+''/etc/scilab.start'',''rpr'');", errmsg4);

errmsg5 = msprintf(_("%s: Wrong type for input argument #%d: string expected.\n"), "fileparts", 2);
assert_checkerror("extension = fileparts(SCI+''/etc/scilab.start'',10);", errmsg5);

errmsg6 = msprintf(_("%s: Wrong type for input argument #%d: string expected.\n"), "fileparts", 1);
assert_checkerror("extension = fileparts(10,10);", errmsg6);

errmsg7 = msprintf(_("%s: Wrong size for input argument #%d: string expected.\n"), "fileparts", 2);
assert_checkerror("fileparts(SCI+''/etc/scilab.start'', [""extension"", ""extension""])", errmsg7);

// For a string
[path,fname,extension] = fileparts(SCI+'/etc/scilab.start');
assert_checkequal(path, pathconvert(SCI+'/etc/',%t,%t));
assert_checkequal(fname, 'scilab');
assert_checkequal(extension, '.start');
//===============================
r = fileparts(SCI+'/etc/scilab.start','path');
assert_checkequal(r, pathconvert(SCI+'/etc/',%t,%f));
//===============================
r = fileparts(SCI+'/etc/scilab.start','fname');
assert_checkequal(r, 'scilab');
//===============================
r = fileparts(SCI+'/etc/scilab.start','extension');
assert_checkequal(r, '.start');
//===============================

// For an empty string
[path,fname,extension] = fileparts("");
assert_checkequal(path, "");
assert_checkequal(fname, "");
assert_checkequal(extension, "");
//===============================
r = fileparts("", 'path');
assert_checkequal(r, "");
//===============================
r = fileparts("", 'fname');
assert_checkequal(fname, "");
//===============================
r = fileparts("", 'extension');
assert_checkequal(extension, "");

// For a matrix of strings (1x2)
[path,fname,extension] = fileparts([SCI+'/etc/scilab.start', SCI+'/etc/scilab.start']);
assert_checkequal(path, [pathconvert(SCI+'/etc/',%t,%t), pathconvert(SCI+'/etc/',%t,%t)]);
assert_checkequal(fname, ['scilab', 'scilab']);
assert_checkequal(extension, ['.start', '.start']);
//===============================
r = fileparts([SCI+'/etc/scilab.start', SCI+'/etc/scilab.start'],'path');
assert_checkequal(r, [pathconvert(SCI+'/etc/',%t,%f), pathconvert(SCI+'/etc/',%t,%f)]);
//===============================
r = fileparts([SCI+'/etc/scilab.start', SCI+'/etc/scilab.start'],'fname');
assert_checkequal(r, ['scilab', 'scilab']);
//===============================
r = fileparts([SCI+'/etc/scilab.start', SCI+'/etc/scilab.start'],'extension');
assert_checkequal(r, ['.start', '.start']);
// For a matrix of strings (2x1)
[path,fname,extension] = fileparts([SCI+'/etc/scilab.start'; SCI+'/etc/scilab.start']);
assert_checkequal(path, [pathconvert(SCI+'/etc/',%t,%t); pathconvert(SCI+'/etc/',%t,%t)]);
assert_checkequal(fname, ['scilab'; 'scilab']);
assert_checkequal(extension, ['.start'; '.start']);
//===============================
r = fileparts([SCI+'/etc/scilab.start'; SCI+'/etc/scilab.start'],'path');
assert_checkequal(r, [pathconvert(SCI+'/etc/',%t,%f); pathconvert(SCI+'/etc/',%t,%f)]);
//===============================
r = fileparts([SCI+'/etc/scilab.start'; SCI+'/etc/scilab.start'],'fname');
assert_checkequal(r, ['scilab'; 'scilab']);
//===============================
r = fileparts([SCI+'/etc/scilab.start'; SCI+'/etc/scilab.start'],'extension');
assert_checkequal(r, ['.start'; '.start']);
// For a matrix of strings with no extension
[path,fname,extension] = fileparts([SCI+'/etc/scilab.start', SCI+'/etc/scilab']);
assert_checkequal(path, [pathconvert(SCI+'/etc/',%t,%t), pathconvert(SCI+'/etc/',%t,%t)]);
assert_checkequal(fname, ['scilab', 'scilab']);
assert_checkequal(extension, ['.start', ""]);
//===============================
r = fileparts([SCI+'/etc/scilab.start', SCI+'/etc/scilab'],'path');
assert_checkequal(r, [pathconvert(SCI+'/etc/',%t,%f), pathconvert(SCI+'/etc/',%t,%f)]);
//===============================
r = fileparts([SCI+'/etc/scilab.start', SCI+'/etc/scilab'],'fname');
assert_checkequal(r, ['scilab', 'scilab']);
//===============================
r = fileparts([SCI+'/etc/scilab.start', SCI+'/etc/scilab'],'extension');
assert_checkequal(r, ['.start', ""]);
// For a matrix of strings with no path
[path,fname,extension] = fileparts([SCI+'/etc/scilab.start', 'scilab.start']);
assert_checkequal(path, [pathconvert(SCI+'/etc/',%t,%t), ""]);
assert_checkequal(fname, ['scilab', 'scilab']);
assert_checkequal(extension, ['.start', ".start"]);
//===============================
r = fileparts([SCI+'/etc/scilab.start', 'scilab.start'], 'path');
assert_checkequal(r, [pathconvert(SCI+'/etc/',%t,%f), ""]);
//===============================
r = fileparts([SCI+'/etc/scilab.start', 'scilab.start'], 'fname');
assert_checkequal(r, ['scilab', 'scilab']);
//===============================
r = fileparts([SCI+'/etc/scilab.start', 'scilab.start'], 'extension');
assert_checkequal(r, ['.start', '.start']);
