// Copyright INRIA



// length
if length('abd')<>3 then pause,end
if length(emptystr())<>0 then pause,end
if or(length(['abd';emptystr()])<>[3;0]) then pause,end
if or(length(string(ones(10,10)))<>1) then pause,end

//part
if part('abc',1)<>'a' then pause,end
if part('abc',[1 1])<>'aa' then pause,end
if part('abc',[1;1])<>'aa' then pause,end
if part('abc',[])<>emptystr() then pause,end
if part('abc',5)<>' ' then pause,end
if part('abc',5:6)<>'  ' then pause,end
if or(part(['abc';'x'],1)<>['a';'x']) then pause,end
if or(part(['abc';'x'],[1 1])<>['aa';'xx']) then pause,end
//if or(part(['abc';'x'],[1 2])<>['aa';'x ']) then pause,end

//string
if string(1)<>'1' then pause,end
if string(1.5)<>'1.5' then pause,end
if string([])<>[] then pause,end
if or(string(1:3)<>['1','2','3']) then pause,end
if or(string([1;2;3])<>['1';'2';'3']) then pause,end
if string('foo')<>'foo' then pause,end
deff('y=mymacro(x)','y=x+1');
[out,in,text]=string(mymacro);
if out<>'y'|in<>'x'|text<>[] then pause,end
mymacro=null();deff('y=mymacro(x)','y=x+1','n');
[out,in,text]=string(mymacro);
if out<>'y'|in<>'x'|text<>'y=x+1' then pause,end


//convstr
if convstr('ABC')<>'abc' then pause,end
if convstr('ABC','l')<>'abc' then pause,end
if convstr('ABC','u')<>'ABC' then pause,end
if convstr(['ABC';'x'])<>['abc';'x'] then pause,end
if convstr(['ABC';'x'],'l')<>['abc';'x'] then pause,end
if convstr(['ABC';'x'],'u')<>['ABC';'X'] then pause,end

if convstr('ABC')<>'abc' then pause,end
if convstr('ABC','l')<>'abc' then pause,end
if convstr('ABC','u')<>'ABC' then pause,end
if convstr(['ABC','x'])<>['abc','x'] then pause,end
if convstr(['ABC','x'],'l')<>['abc','x'] then pause,end
if convstr(['ABC','x'],'u')<>['ABC','X'] then pause,end
if convstr(emptystr())<>emptystr() then pause,end

//str2code
if or(str2code('abcdefghijklmnopqrstuvwxyz')<>(10:35)') then pause,end
if or(str2code('ABCDEFGHIJKLMNOPQRSTUVWXYZ')<>-(10:35)') then pause,end
if or(str2code('0123456789')<>(0:9)') then pause,end
if str2code(emptystr())<>[] then pause,end


//code2str 

if code2str(10:35)<>'abcdefghijklmnopqrstuvwxyz' then pause,end
if code2str(-(10:35))<>'ABCDEFGHIJKLMNOPQRSTUVWXYZ' then pause,end
if code2str(0:9)<>'0123456789' then pause,end
if code2str([])<>emptystr() then pause,end


//sort
[s]=sort(['abc','abd','aa','bxe']);
if or(s<>['aa','abc','abd','bxe']) then pause,end

[s,k]=sort(['abc','abd','aa','bxe']);
if or(s<>['aa','abc','abd','bxe']) then pause,end
if or(k<>[3 1 2 4])  then pause,end

if sort('abc')<>'abc' then pause,end

//strcat

if strcat(['abc','abd','aa','bxe'])<>'abcabdaabxe' then pause,end
if strcat(['abc','abd','aa','bxe'],',')<>'abc,abd,aa,bxe' then pause,end
if strcat('abc')<>'abc' then pause,end
if strcat('abc','sd')<>'abc' then pause,end

//strindex 
if or(strindex('abc,abd,aa,bxe',',')<>[4 8 11]) then pause,end
if or(strindex('abc',',')<>[]) then pause,end
if or(strindex('abc,abd,aa,bxe',',a')<>[4 8]) then pause,end
if or(strindex('abc,abd,aa,bxe','a')<>[1 5 9 10]) then pause,end
if strindex(emptystr(),'a')<>[] then pause,end

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

[pos,which]=strindex('abc,cbd,aa,bxe',[',a',',b']);
if or(pos<>[8 11])|or(which<>[1 2]) then pause,end
[pos,which]=strindex('abc,cbd,aa,bxe',[',a','bb']);
if or(pos<>[8])|or(which<>[1]) then pause,end

rand('u');str=strcat(string(round(8*rand(1,5000))));s= string(0:9);     
[pos,which]=strindex(str,s); 
if part(str,pos)<>strcat(s(which)) then pause,end

//strsubst
if strsubst('abc,abd,aa,bxe',',',';')<>'abc;abd;aa;bxe'  then pause,end
if strsubst('abc,abd,aa,bxe',',',emptystr())<>'abcabdaabxe'  then pause,end
if strsubst(',abc,abd,aa,bxe',',',emptystr())<>'abcabdaabxe'  then pause,end
if strsubst('abc',',',';')<>'abc'  then pause,end

//formal
if addf('1','1')<>'2' then pause,end
if addf('1','0')<>'1' then pause,end
if addf('0','1')<>'1' then pause,end
if addf('0','0')<>'0' then pause,end
if addf('1','-1')<>'0' then pause,end
if addf('-1','1')<>'0' then pause,end
if addf('-1','0')<>'-1' then pause,end
if addf('0','-1')<>'-1' then pause,end

if addf('1','a')<>'a+1' then pause,end
if addf('a','1')<>'a+1' then pause,end
if addf('a','0')<>'a' then pause,end
if addf('0','a')<>'a' then pause,end
if addf('a','-1')<>'a-1' then pause,end
if addf('-1','a')<>'a-1' then pause,end
if addf('a','b')<>'a+b' then pause,end
if addf('a+b','c')<>'a+b+c' then pause,end
if addf('c','a+b')<>'c+a+b' then pause,end
if addf('a+b','a+b')<>'a+b+a+b' then pause,end
if addf('a+b','a-b')<>'a+a' then pause,end
if addf('2*a+b','a-b')<>'2*a+a' then pause,end

if mulf('1','1')<>'1' then pause,end
if mulf('1','0')<>'0' then pause,end
if mulf('0','1')<>'0' then pause,end
if mulf('0','0')<>'0' then pause,end
if mulf('1','-1')<>'-1' then pause,end
if mulf('-1','1')<>'-1' then pause,end
if mulf('-1','0')<>'0' then pause,end
if mulf('0','-1')<>'0' then pause,end

if mulf('1','a')<>'a' then pause,end
if mulf('a','1')<>'a' then pause,end
if mulf('a','0')<>'0' then pause,end
if mulf('0','a')<>'0' then pause,end
if mulf('a','-1')<>'-a' then pause,end
if mulf('-1','a')<>'-a' then pause,end
if mulf('a','b')<>'a*b' then pause,end
if mulf('a+b','c')<>'(a+b)*c' then pause,end
if mulf('c','a+b')<>'c*(a+b)' then pause,end
if mulf('a+b','a+b')<>'(a+b)*(a+b)' then pause,end
if mulf('2*a+b','a-b')<>'(2*a+b)*(a-b)' then pause,end

