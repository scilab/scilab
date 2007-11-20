//===============================
// unit tests length
// Allan CORNET INRIA 2007
//===============================
l = length([]);
if (l <> 0) then pause,end;
//===============================
N = 321;
l = length(N);
if (l <> 1) then pause,end;
//===============================
N = 321;
M = 32;
PARAMIN = [N,M;M,N];
l = length(PARAMIN);
if (l <> size(PARAMIN,'*') ) then pause,end;
//===============================
STRING = "Scilab";
l = length(STRING);
if (l <> 6 ) then pause,end;
//===============================
STRING1 = "Scilab";
STRING2 = "Strings";
PARAMIN = [STRING1,STRING2;STRING2,STRING1];
l = length(PARAMIN);
LS1 = length(STRING1);
LS2 = length(STRING2);
if (LS1 <> 6 ) then pause,end;
if (LS2 <> 7 ) then pause,end;
if (l <> [ LS1, LS2; LS2 , LS1 ]) then pause,end
//===============================
if length('abd')<>3 then pause,end
if length(emptystr())<>0 then pause,end
if or(length(['abd';emptystr()])<>[3;0]) then pause,end
if or(length(string(ones(10,10)))<>1) then pause,end
//===============================

