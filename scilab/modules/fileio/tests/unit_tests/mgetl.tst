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

function [filename, txt] = create_file(arg)
    if isempty(arg) then
        txt = [];
        filename = tempname();
    elseif typeof(arg) == "string" then
        txt = arg;
        filename = tempname();
        if arg <> [] then
            mputl(arg, filename);
        end
    elseif typeof(arg) == "constant" then
        txt = "";
        for i=1:size(arg, '*')
            txt = [txt; strcat(repmat(ascii(96+i), 1, arg(i)))];
        end
        filename = tempname();
        mputl(txt, filename);
    end
end

function fd = open_create_file(txt)
    fd = mopen(create_file(txt), "rt");
endfunction

function check_mgetl(f, expected, varargin)
    txt = mgetl(f, varargin(:));
    assert_checkequal(txt, expected);
endfunction


ref = ["1 Scilab"; ..
        "2 Puffin"; ..
        "3 "; ..
        "4 Puffin"; ..
        "5 Scilab"];

// test simple
check_mgetl(create_file("a"), "a");
check_mgetl(create_file(ref), ref);

// test file argument: file descriptor
fd = open_create_file(ref);
check_mgetl(fd, [], 0)
mclose(fd);

// test line count argument: read all lines
check_mgetl(create_file(ref), ref, -1);
check_mgetl(create_file(ref), ref, -10);

// test line count argument: read zero lines
check_mgetl(create_file(ref), [], 0);

// test line count argument: read some lines
check_mgetl(create_file(ref), ref(1), 1);
check_mgetl(create_file(ref), ref(1:2), 2);
check_mgetl(create_file(ref), ref, 100);

// read all lines in several calls
fd = open_create_file(ref);
check_mgetl(fd, ref(1), 1);
check_mgetl(fd, ref(2:3), 2);
check_mgetl(fd, ref(4:5), 3);
mclose(fd);

// test empty file
check_mgetl(create_file([]), []);

// test file empty lines
filename = tempname();
fd = mopen(filename, "wb");
mfprintf(fd, ascii(10));
mclose(fd);
check_mgetl(filename, "");
txt = mgetl("SCI/modules/fileio/tests/unit_tests/text.txt");
assert_checkequal(size(txt), [6 1]);
assert_checkequal(txt(6), "");

// test file with different EOL (end of line)
ref_EOL = ["line with CR LF"; ..
    "line with LF"; ..
    "line with no EOL"];
filename = tempname();
fd = mopen(filename, "wb");
mfprintf(fd, "%s" + ascii(13) + ascii(10), ref_EOL(1));
mfprintf(fd, "%s" + ascii(10), ref_EOL(2));
mfprintf(fd, "%s", ref_EOL(3));
mclose(fd);
check_mgetl(filename, ref_EOL);


// test file with BOM
txt = mgetl("SCI/modules/fileio/tests/unit_tests/text-UTF8BOM.txt");
assert_checkequal(size(txt), [13 1]);
r = "Scilab is a numerical computational package developed since 1990 by researchers from the INRIA and the Ecole nationale des ponts et chaussees (ENPC). It is, since the creation of the Scilab consortium in May 2003, developed and maintained by the INRIA.";
assert_checkequal(txt(1), r);

// test when line size exceeeds reading buffer size (currently 4096 characters per line)
[filename, ref2] = create_file([5000, 5000]);
check_mgetl(filename, ref2);
[filename, ref2] = create_file([100, 5000, 15000]);
check_mgetl(filename, ref2);
[filename, ref2] = create_file([4095, 8191, 16383]);
check_mgetl(filename, ref2);
[filename, ref2] = create_file([4096, 8192, 16384]);
check_mgetl(filename, ref2);

// test error management
ierr = execstr("mgetl(1000, 1)", "errcatch");
assert_checkequal(ierr, 999);
ierr = execstr("mgetl(TMPDIR + ""notfound.txt"",1)", "errcatch");
assert_checkequal(ierr, 999);



