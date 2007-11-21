//===============================
// A.C 2007
// blank unit tests
//===============================
disp(['xxx' blanks(20) 'yyy']);
//===============================
//LEN_MAX = 20000000; too slow
LEN_MAX = 2000;
r = blanks(LEN_MAX);
l = length(r);
if ( l <> LEN_MAX) then pause,end
//===============================
// check first and last characters
FIRST_CHAR = part(r,1);
LAST_CHAR = part(r,LEN_MAX);
if (FIRST_CHAR <> ' ') then pause,end  
if (LAST_CHAR <> ' ') then pause,end  
//===============================
