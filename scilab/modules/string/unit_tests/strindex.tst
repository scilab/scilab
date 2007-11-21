//===============================
// unit tests strindex
// INRIA 2007
//===============================
if or(strindex('abc,abd,aa,bxe',',')<>[4 8 11]) then pause,end
if or(strindex('abc',',')<>[]) then pause,end
if or(strindex('abc,abd,aa,bxe',',a')<>[4 8]) then pause,end
if or(strindex('abc,abd,aa,bxe','a')<>[1 5 9 10]) then pause,end
if strindex(emptystr(),'a')<>[] then pause,end
//===============================
[pos,which]=strindex('abc,abd,aa,bxe',',');
if or(pos<>[4 8 11])|or(which<>[1 1 1]) then pause,end
[pos,which]=strindex('abc',',');
if or(pos<>[])|or(which<>[]) then pause,end
[pos,which]=strindex('abc,abd,aa,bxe',',a');
if or(pos<>[4 8])|or(which<>[1 1]) then pause,end
[pos,which]=strindex('abc,abd,aa,bxe','a');
if or(pos<>[1 5 9 10])|or(which<>[1 1 1 1]) then pause,end
[pos,which]=strindex('','a');
if or(pos<>[])|or(which<>[]) then pause,end
//===============================
[pos,which]=strindex('abc,cbd,aa,bxe',[',a',',b']);
if or(pos<>[8 11])|or(which<>[1 2]) then pause,end
[pos,which]=strindex('abc,cbd,aa,bxe',[',a','bb']);
if or(pos<>[8])|or(which<>[1]) then pause,end
//===============================
rand('u');str=strcat(string(round(8*rand(1,5000))));s= string(0:9);     
[pos,which]=strindex(str,s); 
if part(str,pos)<>strcat(s(which)) then pause,end
//===============================
ref = [4 10];
k=strindex('SCI/demos/scicos','/');
if (k <> ref ) then pause,end
//===============================
ref = 1;
k=strindex('SCI/demos/scicos','SCI/');
if (k <> ref ) then pause,end
//===============================
ref = [];
k=strindex('SCI/demos/scicos','!');
if (k <> ref ) then pause,end
//===============================
ref = [1 2 3 4];
k=strindex('aaaaa','aa');
if (k <> ref ) then pause,end
//===============================
ref = [1 11];
k=strindex('SCI/demos/scicos',['SCI','sci'])
if (k <> ref ) then pause,end
//===============================
refk = [2 4 8];
refw = [1 3 4];
[k,w]=strindex('1+3*abc/2.33',['+','-','*','/']);
if (k <> refk ) then pause,end
if (w <> refw ) then pause,end
//===============================
fd = mopen(SCI+'/modules/string/unit_tests/text.txt','r');
txt = mgetl( fd );
mclose( fd );
//===============================
if (strindex(txt(1),'scilab') <> [])  then pause,end
if (strindex(txt(1),'Scilab') <> [1 216])  then pause,end
[ind1,ind2] = strindex(txt(1),'Scilab');
//===============================
if (ind1 <> [1 1]) then pause,end
if (ind2 <> [1 216]) then pause,end
//===============================
if (strindex('',' ') <> [])  then pause,end
//===============================
if (strindex([],' ') <> [])  then pause,end
//===============================
