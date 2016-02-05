// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
// <-- CLI SHELL MODE -->
// =============================================================================
// Unitary tests for mgetl function
//==============================================================================
if mgetl('SCI/etc/scilab.start', 0) <> [] then pause,end
//==============================================================================
fd = mopen('SCI/etc/scilab.start', 'rt');
if mgetl(fd, 0) <> [] then pause,end
mclose(fd);
//==============================================================================
ref = ["1 Scilab";"2 Puffin";"3 ";"4 Puffin";"5 Scilab"];
mputl(ref, TMPDIR + "/text.txt");
//==============================================================================
txt = mgetl(TMPDIR + "/text.txt");
if or(ref <> txt) then pause,end
//==============================================================================
txt = mgetl(TMPDIR + "/text.txt", -10);
if or(ref <> txt) then pause,end
//==============================================================================
txt = mgetl(TMPDIR + "/text.txt", -1);
if or(ref <> txt) then pause,end
//==============================================================================
txt = mgetl(TMPDIR + "/text.txt", 1);
if txt <> ref(1) then pause,end
//==============================================================================
txt = mgetl(TMPDIR + "/text.txt", 2);
if or(txt <> ref(1:2)) then pause,end
//==============================================================================
txt = mgetl(TMPDIR + "/text.txt", 100);
if or(txt <> ref) then pause,end
//==============================================================================
fd = mopen(TMPDIR + "/text2.txt","wt");
r = mgetl(fd);
mclose(fd);
if r <> [] then pause,end
//==============================================================================
fd = mopen(TMPDIR + "/text.txt","rt");
r = mgetl(fd, 1);
if r <> ref(1) then pause,end
//==============================================================================
r = mgetl(fd, 2);
if r <> ref(2:3) then pause,end
//==============================================================================
r = mgetl(fd, 3);
if r <> ref(4:5) then pause,end
//==============================================================================
mclose(fd);
//==============================================================================
txt = mgetl("SCI/modules/fileio/tests/unit_tests/text.txt");
if and(size(txt) <> [6 1]) then pause,end
//==============================================================================
// creates a file with some differents end of line
ref = ['A text with different end of line';
'text with LF:';
'text with CRLF:';
'text no EOL.'];

CR = '\r';
LF = '\n';

fd = mopen(TMPDIR + '/mgetl_text_tst.txt', 'wt');
mfprintf(fd, ref(1) + CR + LF);
mfprintf(fd, ref(2) + LF);
mfprintf(fd, ref(3) + CR + LF);
mfprintf(fd, ref(4));
mclose(fd);
r = mgetl(TMPDIR + '/mgetl_text_tst.txt');
if ~and(r == ref) then pause,end
//==============================================================================
txt = mgetl("SCI/modules/fileio/tests/unit_tests/text-UTF8BOM.txt");
if and(size(txt) <> [13 1]) then pause,end
r = "Scilab is a numerical computational package developed since 1990 by researchers from the INRIA and the Ecole nationale des ponts et chaussees (ENPC). It is, since the creation of the Scilab consortium in May 2003, developed and maintained by the INRIA.";
if r <> txt(1) then pause, end

ierr = execstr('mgetl(0,1)','errcatch');
if ierr <> 999 then pause,end
ierr = execstr('mgetl(6,1)','errcatch');
if ierr <> 999 then pause,end

file_1 = TMPDIR+'/test_binary_1.bin';
fid = mopen(file_1,'wb');
mput(1996,'l');
mputstr('Scilab');
mclose(fid);
fid = mopen(file_1,'rb');
assert_checkequal(mget(1,'l'),1996);
assert_checkequal(mgetstr(6),"Scilab");
mclose(fid);
