// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Pierre MARECHAL <pierre.marechal@inria.fr>
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// Test 1
// =============================================================================

if execstr("getmd5(123)","errcatch")                         == 0 then pause, end
if execstr("getmd5(''"+TMPDIR+"/phantom_file'')","errcatch") == 0 then pause, end


// Test 2
// =============================================================================

A = "";
if getmd5(A,"string") <> "d41d8cd98f00b204e9800998ecf8427e" then pause, end


// Test 3
// =============================================================================

A = "$$$$$$$$$$$";
if getmd5(A,"string") <> "d4b8e88a8b72f9ce207063d7b0a17787" then pause, end

// Test 4
// =============================================================================

A = [ 'abcd' 'efgh' ; 'ijkl' 'mnop' ];
B = [ 'e2fc714c4727ee9395f324cd2e7f331f' '1f7690ebdd9b4caf8fab49ca1757bf27' ; ..
      '09a0877d04abf8759f99adec02baf579' 'e132e96a5ddad6da8b07bba6f6131fef' ];

if getmd5(A,"string") <> B then pause, end

// Test 5
// =============================================================================

A = [ 'abcd' 'efgh' 'ijkl' 'mnop' ];
B = [ 'e2fc714c4727ee9395f324cd2e7f331f' '1f7690ebdd9b4caf8fab49ca1757bf27' ..
      '09a0877d04abf8759f99adec02baf579' 'e132e96a5ddad6da8b07bba6f6131fef' ];

if getmd5(A,"string") <> B then pause, end

// Test 6
// =============================================================================

A = [ 'abcd' ; 'efgh' ; 'ijkl' ; 'mnop' ];
B = [ 'e2fc714c4727ee9395f324cd2e7f331f' ; '1f7690ebdd9b4caf8fab49ca1757bf27' ; ..
      '09a0877d04abf8759f99adec02baf579' ; 'e132e96a5ddad6da8b07bba6f6131fef' ];

if getmd5(A,"string") <> B then pause, end

// Test 7
// =============================================================================

if or(size(getmd5( SCI+'/modules/core/etc/'+['core.start' 'core.quit'])) <> [1 2]) then pause, end

// Test 8
// ============================================================================= 
cd(TMPDIR);

if MSDOS then
	unix_w(jre_path()+"\bin\java.exe -cp "+SCI+"\modules\localization\tests\unit_tests CreateDir");
else
	unix_w(jre_path()+"/bin/java -classpath "+SCI+"/modules/localization/tests/unit_tests CreateDir");
end

tab_ref = [
"世界您好",
"азеазея",
"ハロー・ワールド",
"เฮลโลเวิลด์",
"حريات وحقوق",
"프로그램",
"תוכנית"];

v_ref = [
'67737ef45b718d9e151929715a2a9557',
'0e68dfdd1701d509731099cb245b05f0',
'3451966ed3a31bf8ebc5ff621e54ed46',
'090d128546828182ff488b8dcf24792c',
'cc4031dc819e21b7428480ddcf8761d3',
'c654f8795440063a2bed82518065ecf0',
'd4c784f6566873efb0bb25c858b28fc8'];

for i = 1 : size(tab_ref,'*')
	fz = TMPDIR + filesep() + "dir_" + tab_ref(i) + filesep() + "file_" + tab_ref(i);
	if getmd5(fz) <> v_ref(i) then pause,end
end
cd(TMPDIR); 
