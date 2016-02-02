// Copyright (C) 2008 - INRIA - Michael Baudin
// Copyright (C) 2010 - DIGITEO - Michael Baudin
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

// <-- CLI SHELL MODE -->
// <-- ENGLISH IMPOSED -->

function flag = MY_assert_equal ( computed , expected )
    if computed==expected then
        flag = 1;
    else
        flag = 0;
    end
    if flag <> 1 then pause,end
endfunction

function checkassert ( flag , errmsg , ctype )
    if ( ctype == "success" ) then
        MY_assert_equal ( (flag==%t) & (errmsg==""), %t )
    else
        MY_assert_equal ( (flag==%f) & (errmsg<>""), %t )
    end
endfunction

format("v",10);

// Check error message when number of arguments is false
instr = "assert_checkfilesequal ( )";
ierr=execstr(instr,"errcatch");
MY_assert_equal ( ierr , 10000 );
//
instr = "assert_checkfilesequal ( ""foo.txt"" )";
ierr=execstr(instr,"errcatch");
MY_assert_equal ( ierr , 10000 );
//
instr = "[o1,o2,o3]=assert_checkfilesequal ( ""foo.txt"" , ""foo.txt"" )";
ierr=execstr(instr,"errcatch");
MY_assert_equal ( ierr , 999 );
//
//////////////////////////////////////////
//
// Check error message when type of arguments is false
instr = "assert_checkfilesequal ( ""foo.txt"" , %t )";
ierr=execstr(instr,"errcatch");
MY_assert_equal ( ierr , 10000 );
//
instr = "assert_checkfilesequal ( %t , ""foo.txt"" )";
ierr=execstr(instr,"errcatch");
MY_assert_equal ( ierr , 10000 );
//
instr = "assert_checkfilesequal ( %t , %t )";
ierr=execstr(instr,"errcatch");
MY_assert_equal ( ierr , 10000 );
//
//////////////////////////////////////////
//
// Check error message when size of arguments is false
instr = "assert_checkfilesequal ( [ ""foo.txt"" ""foo.txt"" ] , ""foo.txt"" )";
ierr=execstr(instr,"errcatch");
MY_assert_equal ( ierr , 10000 );
//
instr = "assert_checkfilesequal (  ""foo.txt"" , [ ""foo.txt"" ""foo.txt"" ] )";
ierr=execstr(instr,"errcatch");
MY_assert_equal ( ierr , 10000 );
//
//////////////////////////////////////////
//
// Prepare data for the tests
// fileref1 : three lines of text.
// filecomp1 : == fileref1
// filecomp2 : <> fileref1
//
// fileref1
fileref1 = fullfile(TMPDIR,"fileref.txt");
txt1 = [
"Line #1"
"Line #2"
"Line #3"
];
fd = mopen(fileref1,"w");
mputl(txt1,fd);
mclose(fd);
//
// filecomp1
filecomp1 = fullfile(TMPDIR,"filecomp1.txt");
fd = mopen(filecomp1,"w");
mputl(txt1,fd);
mclose(fd);
//
filecomp2 = fullfile(TMPDIR,"filecomp2.txt");
txt2 = [
"Line #1"
"Line #4"
"Line #3"
];
fd = mopen(filecomp2,"w");
mputl(txt2,fd);
mclose(fd);
//
//////////////////////////////////////////
//
// A test which pass
[flag,errmsg] = assert_checkfilesequal ( filecomp1 , fileref1 );
checkassert ( flag , errmsg , "success" );
//
// A test which pass.
// Replay it, to make sure that the files are correctly closed.
[flag,errmsg] = assert_checkfilesequal ( filecomp1 , fileref1 );
checkassert ( flag , errmsg , "success" );
//
// Failure: filecomp2 <> fileref1
[flag,errmsg] = assert_checkfilesequal ( filecomp2 , fileref1 );
checkassert ( flag , errmsg , "failure" );
//
// Failure: filecomp2 <> fileref1
// Replay it, to make sure that the files are correctly closed.
[flag,errmsg] = assert_checkfilesequal ( filecomp2 , fileref1 );
checkassert ( flag , errmsg , "failure" );
//
// Failure: fileref does not exist.
[flag,errmsg] = assert_checkfilesequal ( filecomp2 , "foo.txt" );
checkassert ( flag , errmsg , "failure" );
//
// Failure: filecomp does not exist.
[flag,errmsg] = assert_checkfilesequal ( "foo.txt" , fileref1 );
checkassert ( flag , errmsg , "failure" );
//
//////////////////////////////////////////
//
// A test with a comparison function which ignores comment lines.
//
// Define the filter
function otxt = myfilter ( itxt )
    nr = size(itxt,"r")
    // This is the pattern for a comment line of the form "// blabla"
    pattern = "/\/\/.*/"
    k = 1
    for i = 1 : nr
        start = regexp(itxt(i),pattern)
        if ( start == [] ) then
            otxt(k) = itxt(i)
            k = k + 1
        end
    end
endfunction
//
// Check that the filter works as expected.
itxt = [
"bla 1"
"// bla 2"
"bla 3"
"// bla 4"
"bla 5"
];
otxt = myfilter ( itxt );
etxt = [
"bla 1"
"bla 3"
"bla 5"
];
MY_assert_equal ( otxt , etxt );
//
// Define the comparison function
function areequal = mycompfun ( ctxt , etxt )
    ctxt = myfilter ( ctxt )
    etxt = myfilter ( etxt )
    areequal = ( ctxt == etxt )
endfunction

//
// Use the comparison function.
// fileref2 == filecomp3, given that comment lines are ignored.
// fileref2 <> filecomp4, given that comment lines are ignored.
// Notice that the comments are inserted at different positions in the files:
// sometimes at the begining, sometimes in the middle.
//
// Prepare data files
//
// fileref2
fileref2 = fullfile(TMPDIR,"fileref2.txt");
txt = [
"// bla 2"
"Line #1"
"// bla 2"
"Line #2"
"Line #3"
];
fd = mopen(fileref2,"w");
mputl(txt,fd);
mclose(fd);
//
// filecomp3
filecomp3 = fullfile(TMPDIR,"filecomp3.txt");
txt = [
"Line #1"
"// bla 5168"
"Line #2"
"Line #3"
"// bla oups"
];
fd = mopen(filecomp3,"w");
mputl(txt,fd);
mclose(fd);
//
// filecomp4
filecomp4 = fullfile(TMPDIR,"filecomp4.txt");
txt = [
"// bla 3"
"Line #1"
"Line #4"
"// bla 5168"
"Line #3"
"// bla oups"
];
fd = mopen(filecomp4,"w");
mputl(txt,fd);
mclose(fd);
//
// A test which pass
[flag,errmsg] = assert_checkfilesequal ( filecomp3 , fileref2 , mycompfun );
checkassert ( flag , errmsg , "success" );
//
// A test which fails
[flag,errmsg] = assert_checkfilesequal ( filecomp4 , fileref2 , mycompfun );
checkassert ( flag , errmsg , "failure" );
//
///////////////////////////////////////////////////////////////////
//
// A comparison function with an additional argument
//
// A test with a comparison function which ignores comment lines.
//
// Define the filter
function otxt = myfilter2 ( itxt , pattern )
    nr = size(itxt,"r")
    k = 1
    for i = 1 : nr
        start = regexp(itxt(i),pattern)
        if ( start == [] ) then
            otxt(k) = itxt(i)
            k = k + 1
        end
    end
endfunction
//
// Check that the filter works as expected.
// This is the pattern for a comment line of the form "// blabla"
pattern = "/\/\/.*/"
itxt = [
"bla 1"
"// bla 2"
"bla 3"
"// bla 4"
"bla 5"
];
otxt = myfilter2 ( itxt , pattern );
etxt = [
"bla 1"
"bla 3"
"bla 5"
];
MY_assert_equal ( otxt , etxt );
//
// Define the comparison function
function areequal = mycompfun2 ( ctxt , etxt , pattern )
    ctxt = myfilter2 ( ctxt , pattern )
    etxt = myfilter2 ( etxt , pattern )
    areequal = ( ctxt == etxt )
endfunction
//
// A test which pass
mycompfun3 = list ( mycompfun2 , pattern );
[flag,errmsg] = assert_checkfilesequal ( filecomp3 , fileref2 , mycompfun3 );
checkassert ( flag , errmsg , "success" );
//
// A test which fails
[flag,errmsg] = assert_checkfilesequal ( filecomp4 , fileref2 , mycompfun3 );
checkassert ( flag , errmsg , "failure" );

