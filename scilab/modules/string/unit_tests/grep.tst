
//grep

txt=['find matches of a string in a vector of strings'
     'search position of a character string in an other string'
     'Compare Strings'];

if grep(txt,'vide')<>[] then pause,end
if grep([],'vide')<>[] then pause,end
if grep('','vide')<>[] then pause,end
e='';if execstr('grep(txt,e)','errcatch')==0 then pause,end

if grep(txt,'strings')<>1 then pause,end
if or(grep(txt,['strings' 'Strings'])<>[1 3]) then pause,end

[r,w]=grep(txt,['strings' 'Strings']);
if or(w<>[1 2])|or(r<>[1 3]) then pause,end

e=['strings' '']; if execstr('[r,w]=grep(txt,e)','errcatch')==0 then pause,end;
