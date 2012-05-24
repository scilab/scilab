// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2007-2008 - INRIA
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

//===============================
// unit tests tokens
//===============================
if tokens('This is a character string') <> ['This'; 'is'; 'a'; 'character'; 'string'] then pause,end
if tokens('SCI/demos/scicos','/') <> ['SCI';'demos';'scicos'] then pause,end
if tokens('y=a+b*2',['=','+','*']) <> ['y';'a';'b';'2'] then pause,end
if length(tokens('',['=','+','*'])) <> 0 then pause,end
//===============================
if tokens(' ',' ') <> [] then pause,end
//===============================
s = tokens(' '+ascii(9)+' ',ascii(9));
if  s <> [' '; ' '] then pause,end
//===============================
s = tokens(' '+ascii(9)+' ',' ');
if  s <> ascii(9) then pause,end
//===============================
s = tokens(' '+ascii(9)+' ',ascii(9));
if  s <> [' '; ' '] then pause,end
//===============================
s = tokens(' '+ascii(9)+' ',' ');
if  s <> ascii(9) then pause,end
//===============================
str = "世界您好";
ch = "界";
r = tokens(str,ch);
ref = ["世";"您好"];
if and(ref <> r) then pause,end
//===============================
str = "азеазея";
ch = "з";
r = tokens(str,ch);
ref = ["а";"еа";"ея"];
if and(ref <> r) then pause,end
//===============================
str = "ハロー・ワールド";
ch = "ワ";
r = tokens(str,ch);
ref = ["ハロー・";"ールド"];
if and(ref <> r) then pause,end
//===============================
str = "เฮลโลเวิลด์";
ch = "ฮ";
r = tokens(str,ch);
ref = ["เ";"ลโลเวิลด์ "];
if and(ref <> r) then pause,end
//===============================
str = "حريات وحقوق";
ch = "و";
r = tokens(str,ch);
ref = ["حريات";"حق";"ق"];
if and(ref <> r) then pause,end
//===============================
str = "תוכנית";
ch = "ו";
r = tokens(str,ch);
ref = ["ת";"כנית"];
if and(ref <> r) then pause,end
//===============================