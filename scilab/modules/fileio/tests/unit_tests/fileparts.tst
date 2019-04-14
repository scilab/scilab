// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA
// Copyright (C) 2019 - Samuel GOUGEON
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->

//===============================
// Tests fileparts
//===============================

// Error messages
// ==============
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

// On the root directory
// ---------------------
// fileparts("c:") => "": WRONG. "c:/" expected : http://bugzilla.scilab.org/16063
pathfiles = ["c:/", "c:/abc" "c:/.abc" "/" "/abc" "/.abc"];
[path, fname, ext] = fileparts(pathfiles);
pathref = ['c:/','c:/','c:/','/','/','/'];
if getos()=="Windows"
    pathref = strsubst(pathref, "/", "\");
end
assert_checkequal(path, pathref);
assert_checkequal(fname, ["" "abc" ".abc" "" "abc" ".abc"]);
assert_checkequal(ext, emptystr(1,6));

// With the "." directory
// ----------------------
// With absolute paths
pathfiles = ["c:/.", "c:/./", "c:/./.", "c:/abc/.", "c:/abc/./", "c:/abc/./.tmp"];
[path, fname, ext] = fileparts(pathfiles);
pathref = ['c:/','c:/./','c:/./','c:/abc/','c:/abc/./', 'c:/abc/./'];
if getos()=="Windows"
    pathref = strsubst(pathref, "/", "\");
end
assert_checkequal(path, pathref);
assert_checkequal(fname, ["."  "" "." "." "" ".tmp"]);
assert_checkequal(ext, emptystr(1,6));

pathfiles = ["/.", "/./", "/./.", "/abc/.", "/abc/./", "/abc/./.tmp"];
[path, fname, ext] = fileparts(pathfiles);
pathref = pathconvert(['/','/./','/./','/abc/','/abc/./', '/abc/./'],%t,%t);
assert_checkequal(path, pathref);
assert_checkequal(fname, ["." "" "." "." "" ".tmp"]);
assert_checkequal(ext, emptystr(1,6));

// With relative paths
pathfiles = ["./abc", "./." "./.abc", "./abc/./def"];
[path, fname, ext] = fileparts(pathfiles);
pathref = ['./' './' './' './abc/./'];
if getos()=="Windows"
    pathref = strsubst(pathref, "/", "\");
end
assert_checkequal(path, pathref);
assert_checkequal(fname, ["abc"  "." ".abc" "def"]);
assert_checkequal(ext, emptystr(1,4));

// With the ".." directory
// -----------------------
// With absolute paths
pathfiles = ["c:/..", "c:/abc/..", "c:/abc/../efg", "/..", "/abc/..", "/abc/../efg"];

[path, fname, ext] = fileparts(pathfiles);
pathref = ['c:/','c:/abc/','c:/abc/../', '/','/abc/','/abc/../'];
if getos()=="Windows"
    pathref = strsubst(pathref, "/", "\");
end
assert_checkequal(path, pathref);
//assert_checkequal(fname, ["."  "." "efg" "." "." "efg"]); // WRONG
//assert_checkequal(ext, ["." "." "" "." "." ""]);          // WRONG
// => http://bugzilla.scilab.org/16063

// With relative paths
pathfiles = ["abc/..", "./..", "./../", "abc/../def", "abc/../.."];

[path, fname, ext] = fileparts(pathfiles);
pathref = pathconvert(['abc/','./','./../','abc/../','abc/../'],%t,%t);
assert_checkequal(path, pathref);
//assert_checkequal(fname, ["."  "."  ""  "def"  "."]); // WRONG
//assert_checkequal(ext,   ["."  "."  ""  ""     "."]); // WRONG
// => http://bugzilla.scilab.org/16063

// For SCI+'/etc/scilab.start'
// --------------------------
[path,fname,extension] = fileparts(SCI+'/etc/scilab.start');
assert_checkequal(path, pathconvert(SCI+'/etc/',%t,%t));
assert_checkequal(fname, 'scilab');
assert_checkequal(extension, '.start');
r = fileparts(SCI+'/etc/scilab.start','path');
assert_checkequal(r, pathconvert(SCI+'/etc/',%t,%f));
r = fileparts(SCI+'/etc/scilab.start','fname');
assert_checkequal(r, 'scilab');
r = fileparts(SCI+'/etc/scilab.start','extension');
assert_checkequal(r, '.start');

// Filename starting with "."
// --------------------------
[path,fname,extension] = fileparts('/tmp/.scilab');
assert_checkequal(path, pathconvert('/tmp/',%t,%t));
assert_checkequal(fname, '.scilab');
assert_checkequal(extension, '');
r = fileparts('/tmp/.scilab','path');
assert_checkequal(r, pathconvert('/tmp/',%t,%f));
r = fileparts('/tmp/.scilab','fname');
assert_checkequal(r, '.scilab');
r = fileparts('/tmp/.scilab','extension');
assert_checkequal(r, '');

// Filename with multiple "."
// --------------------------
pathfile = '/tmp/scilab.inc.txt'
[path,fname,extension] = fileparts(pathfile);
assert_checkequal(path, pathconvert('/tmp/',%t,%t));
assert_checkequal(fname, 'scilab.inc');
assert_checkequal(extension, '.txt');
r = fileparts(pathfile,'path');
assert_checkequal(r, pathconvert('/tmp/',%t,%f));
r = fileparts(pathfile,'fname');
assert_checkequal(r, 'scilab.inc');
r = fileparts(pathfile,'extension');
assert_checkequal(r, '.txt');

// For the empty string
// --------------------
[path,fname,extension] = fileparts("");
assert_checkequal(path, "");
assert_checkequal(fname, "");
assert_checkequal(extension, "");

r = fileparts("", 'path');
assert_checkequal(r, "");
r = fileparts("", 'fname');
assert_checkequal(fname, "");
r = fileparts("", 'extension');
assert_checkequal(extension, "");

// For a row of strings
// --------------------
[path,fname,extension] = fileparts([SCI+'/etc/scilab.start', SCI+'/etc/scilab.start']);
assert_checkequal(path, [pathconvert(SCI+'/etc/',%t,%t), pathconvert(SCI+'/etc/',%t,%t)]);
assert_checkequal(fname, ['scilab', 'scilab']);
assert_checkequal(extension, ['.start', '.start']);

r = fileparts([SCI+'/etc/scilab.start', SCI+'/etc/scilab.start'],'path');
assert_checkequal(r, [pathconvert(SCI+'/etc/',%t,%f), pathconvert(SCI+'/etc/',%t,%f)]);
r = fileparts([SCI+'/etc/scilab.start', SCI+'/etc/scilab.start'],'fname');
assert_checkequal(r, ['scilab', 'scilab']);
r = fileparts([SCI+'/etc/scilab.start', SCI+'/etc/scilab.start'],'extension');
assert_checkequal(r, ['.start', '.start']);

// For a column of strings
// -----------------------
[path,fname,extension] = fileparts([SCI+'/etc/scilab.start'; SCI+'/etc/scilab.start']);
assert_checkequal(path, [pathconvert(SCI+'/etc/',%t,%t); pathconvert(SCI+'/etc/',%t,%t)]);
assert_checkequal(fname, ['scilab'; 'scilab']);
assert_checkequal(extension, ['.start'; '.start']);

r = fileparts([SCI+'/etc/scilab.start'; SCI+'/etc/scilab.start'],'path');
assert_checkequal(r, [pathconvert(SCI+'/etc/',%t,%f); pathconvert(SCI+'/etc/',%t,%f)]);
r = fileparts([SCI+'/etc/scilab.start'; SCI+'/etc/scilab.start'],'fname');
assert_checkequal(r, ['scilab'; 'scilab']);
r = fileparts([SCI+'/etc/scilab.start'; SCI+'/etc/scilab.start'],'extension');
assert_checkequal(r, ['.start'; '.start']);

// For a matrix of strings with no extension
// -----------------------------------------
[path,fname,extension] = fileparts([SCI+'/etc/scilab.start', SCI+'/etc/scilab']);
assert_checkequal(path, [pathconvert(SCI+'/etc/',%t,%t), pathconvert(SCI+'/etc/',%t,%t)]);
assert_checkequal(fname, ['scilab', 'scilab']);
assert_checkequal(extension, ['.start', ""]);

r = fileparts([SCI+'/etc/scilab.start', SCI+'/etc/scilab'],'path');
assert_checkequal(r, [pathconvert(SCI+'/etc/',%t,%f), pathconvert(SCI+'/etc/',%t,%f)]);
r = fileparts([SCI+'/etc/scilab.start', SCI+'/etc/scilab'],'fname');
assert_checkequal(r, ['scilab', 'scilab']);
r = fileparts([SCI+'/etc/scilab.start', SCI+'/etc/scilab'],'extension');
assert_checkequal(r, ['.start', ""]);

// For a matrix of strings with no path
// ====================================
[path,fname,extension] = fileparts([SCI+'/etc/scilab.start', 'scilab.start']);
assert_checkequal(path, [pathconvert(SCI+'/etc/',%t,%t), ""]);
assert_checkequal(fname, ['scilab', 'scilab']);
assert_checkequal(extension, ['.start', ".start"]);

r = fileparts([SCI+'/etc/scilab.start', 'scilab.start'], 'path');
assert_checkequal(r, [pathconvert(SCI+'/etc/',%t,%f), ""]);
r = fileparts([SCI+'/etc/scilab.start', 'scilab.start'], 'fname');
assert_checkequal(r, ['scilab', 'scilab']);
r = fileparts([SCI+'/etc/scilab.start', 'scilab.start'], 'extension');
assert_checkequal(r, ['.start', '.start']);
