//===============================
// unit tests grep
// INRIA 2007
//===============================
txt=['find matches of a string in a vector of strings'
     'search position of a character string in an other string'
     'Compare Strings'];
//===============================
if grep(txt,'vide')<>[] then pause,end
if grep([],'vide')<>[] then pause,end
if grep('','vide')<>[] then pause,end
e='';if execstr('grep(txt,e)','errcatch')==0 then pause,end
//===============================
if grep(txt,'strings')<>1 then pause,end
if or(grep(txt,['strings' 'Strings'])<>[1 3]) then pause,end
//===============================
[r,w]=grep(txt,['strings' 'Strings']);
if or(w<>[1 2])|or(r<>[1 3]) then pause,end
//===============================
e=['strings' '']; if execstr('[r,w]=grep(txt,e)','errcatch')==0 then pause,end;
//===============================
if grep([],' ') <> [] then pause,end
//===============================
if grep('AB',['A','B']) <> 1 then pause,end
//===============================
if grep('AB',['A';'B']) <> 1 then pause,end
//===============================
if grep(['AB','B'],['A';'B']) <> 1 then pause,end
//===============================
if grep(['AB','B'],['A';'B';'C']) <> 1 then pause,end 
//===============================
fd = mopen(SCI+'/modules/string/tests/unit_tests/text.txt','r');
txt = mgetl( fd );
mclose( fd );
//===============================
if grep(txt,'scilab') <> [] then pause,end 
//===============================
ref = [ 1., 3., 5., 32., 48., 83., 110., 114., 162., 175., 177., 272., 365., 390., 402., 404., 408.];
if grep(txt,'Scilab') <> ref then pause,end 
//===============================
