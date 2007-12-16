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
