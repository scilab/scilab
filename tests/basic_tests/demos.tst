// Copyright INRIA
mode(-1)
//to Check all the demos
funcprot(0);lines(0)
global LineCount
deff('[]=mode(x)','x=x')
deff('[]=halt(  )',' ')
deff('x=lines(x)','x=0 ')
getf('SCI/tests/demos/dialogs.sci')
execstr('message=x_message')
execstr('dialog=x_dialog')
execstr('tk_getcolor=getcolor')

if MSDOS then
  O=file('open',TMPDIR+'\sciout','unknown')
  sel=[1:8] 
else
  O=file('open','/dev/null','unknown')
  sel=[1:8] 
end

dialogs='demo'+string(sel)+'.dialogs'
for dia=dialogs
  I=file('open','SCI/tests/demos/'+dia,'old');
  %IO=[I,O];
  //ierr=execstr('exec(''SCI/demos/alldems.dem'')','errcatch')
  ierr=0;exec('SCI/demos/alldems.dem')
  if ierr<>0 then 
     write(%io(2),'Error had occurred at line '+sci2exp(LineCount)+' of file '+dia)
  end
  file('close',I)
end

file('close',O)
