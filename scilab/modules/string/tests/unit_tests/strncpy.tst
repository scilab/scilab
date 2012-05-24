// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2007-2008 - INRIA
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

//===============================
str1 = "Scilab string 1";
str2 = "Scilab string 2";
if strncpy(str1,0) <> strncpy(str2,0) then pause,end
//===============================
if (strncpy(str1,0) <> '') then pause,end
//===============================
if (strncpy(str1,length(str1)) <> str1) then pause,end
//===============================
STR = [str1,str2;str2,str1];
if strncpy(STR,length(STR)) <> STR then pause,end
//===============================
REF = ['Sc','Sc';'Sc','Sc'];
if strncpy(STR,2) <> REF then pause,end
//===============================
if strncpy('I try to crash scilab',-1) <> '' then pause,end
//===============================
REF = ['S','Sc';'Sci','Scil'];
if strncpy(STR,[1,2;3,4]) <> REF then pause,end
//===============================
STR = 'scilab is The open source platform for numerical computation.';
if strncpy(STR,6)<> 'scilab' then pause,end;
//===============================
STR1 = strncpy(STR,6);
STR2 = strncpy(STR,8);
STR3 = strncpy(STR,10);
STR4 = strncpy(STR,12);
STR5 = strncpy(STR,14);
STR6 = strncpy(STR,16);
STRS = [STR1,STR2,STR3;STR4,STR5,STR6];
REF = [STR1,STR1,STR1;STR1,STR1,STR1];
if strncpy(STRS,6) <> REF then pause,end
//===============================
STRS = [STR,STR,STR;STR,STR,STR];
STR1 = strncpy(STR,1);
STR2 = strncpy(STR,2);
STR3 = strncpy(STR,3);
STR4 = strncpy(STR,4);
STR5 = strncpy(STR,5);
STR6 = strncpy(STR,6);
REF = [STR1,STR2,STR3;STR4,STR5,STR6];
if strncpy(STRS,[1,2,3;4,5,6])<> REF then pause,end
//===============================
tab_ref = [
"世界您好",
"азеазея",
"ハロー・ワールド",
"เฮลโลเวิลด์",
"حريات وحقوق",
"תוכנית"];

for i=1:size(tab_ref,'*')
 r = strncpy(tab_ref(i),i);
 if r <> part(tab_ref(i),1:i) then pause,end
end
