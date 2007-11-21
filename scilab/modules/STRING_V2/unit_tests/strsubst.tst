//===============================
// unit tests strsubst
// INRIA 2007
//===============================
if strsubst('abc,abd,aa,bxe',',',';')<>'abc;abd;aa;bxe'  then pause,end
//===============================
if strsubst('abc,abd,aa,bxe',',',emptystr())<>'abcabdaabxe'  then pause,end
//===============================
if strsubst(',abc,abd,aa,bxe',',',emptystr())<>'abcabdaabxe'  then pause,end
//===============================
if strsubst('abc',',',';')<>'abc'  then pause,end
//===============================
if strsubst('SCI/demos/scicos','SCI','.') <> './demos/scicos'  then pause,end
//===============================
if strsubst('SCI/demos/scicos','/',' ') <> 'SCI demos scicos'  then pause,end
//===============================
if strsubst([],'/',' ') <> [] then pause,end
//===============================
if strsubst([],' ','') <> [] then pause,end
//===============================
if strsubst('',' ','') <> '' then pause,end
//===============================
if strsubst("2","3","1") <> "2" then pause,end
//===============================
if strsubst(["2","3"],"3","1") <> ['2' '1'] then pause,end
//===============================
r = strsubst(['Scilab','Mathlab';'Octlab','Rlab'],'lab','');
ref  = ['Sci','Math';'Oct','R'];
if (r <> ref) then  pause,end
//===============================

