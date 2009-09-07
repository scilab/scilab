// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2007-2008 - INRIA
// Copyright (C) 2009 - DIGITEO - Allan CORNET
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- JVM NOT MANDATORY -->

//===============================
// unit tests strsplit
//===============================
STR_SPLITED = ['strsplit splits';'a string';'into';'a vector of strings'];
STR = 'strsplit splits a string into a vector of strings';
INDICES = [15 25 30];
R = stripblanks(strsplit(STR,INDICES));
if R <> STR_SPLITED then pause,end
//===============================
if strsplit([],[1 1 1]) <> []  then pause,end
//===============================
// Scilab 4.x 
//if execstr('strsplit([],[3 2 1])','errcatch') <> 99  then pause,end
// Scilab 5.x
if strsplit([],[3 2 1]) <> []  then pause,end
//===============================
if execstr('strsplit('',[0 1])','errcatch') <> 31  then pause,end
// Scilab 4.x 
//if execstr('strsplit([])','errcatch') <> 39  then pause,end
// Scilab 5.1
//if execstr('strsplit([])','errcatch') <> 77  then pause,end
// Scilab 5.2
if execstr('strsplit([])','errcatch') <> 0  then pause,end
if (strsplit([]) <> []) then pause,end
//===============================
ref_1 = ["toto"];
ref_2 = [];
[r_1, r_2] = strsplit("toto","a");
if ~and(ref_1 == r_1) then pause,end
if ~and(ref_2 == r_2) then pause,end
//===============================
// request 663
ref_1 = ["a";"b";"c";"d";"e";"f"];
ref_2 = ["";"";"";"";"";""];
[r_1, r_2] = strsplit("abcdef");
if ~and(ref_1 == r_1) then pause,end
if ~and(ref_2 == r_2) then pause,end
if strcat(r_1,"") <> "abcdef" then pause,end
//===============================
[r_1, r_2] = strsplit("abcdef","");
[r_3, r_4] = strsplit("abcdef");
if ~and(r_1 == r_3) then pause,end
if ~and(r_2 == r_4) then pause,end
//===============================
ref_1 = ["a";"bcdef"];
ref_2 = [""];
[r_1, r_2] = strsplit("abcdef","",1);
if ~and(ref_1 == r_1) then pause,end
if ~and(ref_2 == r_2) then pause,end
if strcat(r_1,"") <> "abcdef" then pause,end
//===============================
ref_1 = ["abcdef";"ghijkl";"mnopqr";"stuvw";"xyz"];
ref_2 = [",";",";",";","];
[r_1, r_2] = strsplit("abcdef,ghijkl,mnopqr,stuvw,xyz",",");
if ~and(ref_1 == r_1) then pause,end
if ~and(ref_2 == r_2) then pause,end
if strcat(r_1,",") <> "abcdef,ghijkl,mnopqr,stuvw,xyz" then pause,end
//===============================
ref_1 = ["abc";"";"";"def";"";"ghijkl";"";"mno"];
ref_2 = [":";":";":";":";":";":";":"];
[r_1, r_2] = strsplit("abc:::def::ghijkl::mno",":");
if ~and(ref_1 == r_1) then pause,end
if ~and(ref_2 == r_2) then pause,end
if strcat(r_1,":") <> "abc:::def::ghijkl::mno" then pause,end
//===============================
ref_1  = ["abcdef";"ghijkl";"mnopqr";"stuvw";"xyz"];
ref_2  = ["~~~";"~~~";"~~~";"~~~"] ;
[r_1, r_2] = strsplit("abcdef~~~ghijkl~~~mnopqr~~~stuvw~~~xyz","~~~");
if ~and(ref_1 == r_1) then pause,end
if ~and(ref_2 == r_2) then pause,end
if strcat(r_1,"~~~") <> "abcdef~~~ghijkl~~~mnopqr~~~stuvw~~~xyz" then pause,end
//===============================
ref_1 = ["abcdef";"ghijkl";"mnopqr";"stuvw";"xyz"];
ref_2 = ["2";"3";"6";"7"];
[r_1, r_2] = strsplit("abcdef2ghijkl3mnopqr6stuvw7xyz","/\d+/");
if ~and(ref_1 == r_1) then pause,end
if ~and(ref_2 == r_2) then pause,end
//===============================
ref_2 = ["scilab";"scilab"];
ref_1 = ["";" a numerical tools ";"oraty"];
[r_1, r_2] = strsplit("scilab a numerical tools scilaboraty","/scilab/");
if ~and(ref_1 == r_1) then pause,end
if ~and(ref_2 == r_2) then pause,end
//===============================
ref_1  = ["abcdef";"ghijkl";"mnopqr";"stuvw";"xyz"];
ref_2  = [ascii(9);ascii(9);ascii(9);ascii(9)];
[r_1, r_2] = strsplit("abcdef"+ascii(9)+"ghijkl" + ascii(9)+"mnopqr"+ascii(9)+"stuvw" + ascii(9)+"xyz","/\t/") ;
if ~and(ref_1 == r_1) then pause,end
if ~and(ref_2 == r_2) then pause,end
if strcat(r_1,ascii(9)) <> "abcdef"+ascii(9)+"ghijkl" + ascii(9)+"mnopqr"+ascii(9)+"stuvw" + ascii(9)+"xyz" then pause,end
//===============================
ref_1  = ["server.name";"scilab.org"];
ref_2  = "       = ";
linestr = "server.name       = scilab.org";
[r_1, r_2] = strsplit(linestr, "/\s*=\s*/");
if ~and(ref_1 == r_1) then pause,end
if ~and(ref_2 == r_2) then pause,end
if strcat(r_1,r_2) <> "server.name       = scilab.org" then pause,end
//===============================
ierr = execstr("strsplit(''root:x:0:0:root:/root:/bin/bash'','':'',0)","errcatch");
if ierr <> 999 then pause,end
//===============================
ref_1 = ["root";"x:0:0:root:/root:/bin/bash"];
ref_2 = ":";
[r_1, r_2] = strsplit("root:x:0:0:root:/root:/bin/bash",":",1);
if ~and(ref_1 == r_1) then pause,end
if ~and(ref_2 == r_2) then pause,end
//===============================
ref_1 = ["root";"x";"0";"0";"root";"/root:/bin/bash"];
ref_2 = [":";":";":";":";":"];
[r_1, r_2] = strsplit("root:x:0:0:root:/root:/bin/bash",":",5);
if ~and(ref_1 == r_1) then pause,end
if ~and(ref_2 == r_2) then pause,end
//===============================
[r_1, r_2] = strsplit("root:x:0:0:root:/root:/bin/bash",":",7);
[r_3, r_4] = strsplit("root:x:0:0:root:/root:/bin/bash",":",50);
if ~and(r_1 == r_3) then pause,end
if ~and(r_2 == r_4) then pause,end
//===============================
ref_1 = ["abc";""];
ref_2 = ',';
[r_1, r_2] = strsplit("abc,",",");
if ~and(ref_1 == r_1) then pause,end
if ~and(ref_2 == r_2) then pause,end
//===============================
ref_1 = ["abc";""];
ref_2 = ',';
[r_1, r_2] = strsplit("abc,",",",1);
if ~and(ref_1 == r_1) then pause,end
if ~and(ref_2 == r_2) then pause,end
//===============================
[r_1, r_2] = strsplit("abc,",",",1);
[r_3, r_4] = strsplit("abc,",",",10);
if ~and(r_1 == r_3) then pause,end
if ~and(r_2 == r_4) then pause,end
//===============================
ref_1 = ["abc";"def";"ijk";"";"lmo"];
ref_2 = [",";":";",";":"];
[r_1, r_2] = strsplit("abc,def:ijk,:lmo","/:|,/");
if ~and(ref_1 == r_1) then pause,end
if ~and(ref_2 == r_2) then pause,end
//===============================
ref_1 = ["abc";"def";"ijk";"";"lmo"];
ref_2 = [",";":";",";":"];
[r_1, r_2] = strsplit("abc,def:ijk,:lmo",[":";","]);
if ~and(ref_1 == r_1) then pause,end
if ~and(ref_2 == r_2) then pause,end
//===============================
ref_1 = ["abc";"def";"ijk,:lmo"];
ref_2 = [",";":"];
[r_1, r_2] = strsplit("abc,def:ijk,:lmo",[":";","],2);
if ~and(ref_1 == r_1) then pause,end
if ~and(ref_2 == r_2) then pause,end
//===============================
v = "世界您好";
c = "您";
[r_1,r_2] = strsplit(v,c);
if strcat(r_1,r_2) <> v then pause,end
//===============================
v = "азеаея";
c = "з";
[r_1,r_2] = strsplit(v,c);
if strcat(r_1,r_2) <> v then pause,end
//===============================
v = "ድቅስድስግ";
c = "ቅ";
[r_1,r_2] = strsplit(v,c);
if strcat(r_1,r_2) <> v then pause,end
//===============================
v = "ハロー・ワールド";
c = "ド";
[r_1,r_2] = strsplit(v,c);
if strcat(r_1,r_2) <> v then pause,end
//===============================
v = "תוכנית";
c = "י";
[r_1,r_2] = strsplit(v,c);
if strcat(r_1,r_2) <> v then pause,end
//===============================
