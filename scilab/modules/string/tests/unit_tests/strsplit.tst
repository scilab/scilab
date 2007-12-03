//===============================
// unit tests strsplit
// INRIA 2007
//===============================
STR_SPLITED = ['strsplit splits';'a string';'into';'a vector of strings'];
STR = 'strsplit splits a string into a vector of strings';
INDICES = [15 25 30];
R = stripblanks(strsplit(STR,INDICES));
if R <> STR_SPLITED then pause,end
//===============================
if strsplit([],[1 1 1]) <> []  then pause,end
//===============================
if execstr('strsplit([],[3 2 1])','errcatch') <> 99  then pause,end
if execstr('strsplit('',[0 1])','errcatch') <> 31  then pause,end
if execstr('strsplit([])','errcatch') <> 39  then pause,end
//===============================
