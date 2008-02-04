//===============================
// unit tests emptystr
// Allan CORNET INRIA 2007
//===============================
s = emptystr();
if (s <> '') then pause,end;
//===============================
s = emptystr([]);
if (s <> []) then pause,end;
//===============================
s = emptystr("Scilab");
d = size(s);
if (d <> [1 1]) then pause,end;
if (s <> '') then pause,end;
//===============================
MS = ["Scilab";"String"];
s = emptystr(MS);
d = size(s);
if (d <> size(MS)) then pause,end;
if or(s <> '') then pause,end;
//===============================
M = 5;
N = 3;
s = emptystr(M,N);
d = size(s);
if (d <> [M N]) then pause,end;
if or(s <> '') then pause,end;
//===============================
M = 200;
N = 300;
s = emptystr(M,N);
d = size(s);
if (d <> [M N]) then pause,end;
if or(s <> '') then pause,end;
//===============================
s = emptystr(0,0);
if (s <> []) then pause,end;
//===============================
s = emptystr(0);
d = size(s);
if (d <> [1 1]) then pause,end;
if (s <> '') then pause,end;
//===============================
M = 200;
s = emptystr(M);
d = size(s);
if (d <> [1 1]) then pause,end;
if (s <> '') then pause,end;
//===============================
if ( size(emptystr(0,1)) <> [ 0 0 ] ) then pause,end	
//===============================
