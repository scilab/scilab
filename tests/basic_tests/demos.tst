// Copyright INRIA
mode(-1)
//to Check all the demos
funcprot(0)
global LineCount
load SCI/macros/scicos/lib
x_mess=funptr('x_message');clearfun('x_message')
x_dia=funptr('x_dialog');clearfun('x_dialog')
x_mdia=funptr('x_mdialog');clearfun('x_mdialog')
c_cho=funptr('x_choose');clearfun('x_choose')
xcli=funptr('xclick');clearfun('xclick')
xgetm=funptr('xgetmouse');clearfun('xgetmouse')
xgetf=funptr('xgetfile');clearfun('xgetfile')

deff('[]=mode(x)','x=x')
deff('[]=halt(  )',' ')
getf('SCI/macros/util/x_matrix.sci')
getf('SCI/macros/util/getvalue.sci')
getf('SCI/macros/auto/scicos.sci')

//reload the functions using the redefined primitive functions
names=[
    'scicos'
    'getvalue';
    'choosefile';
    'do_addnew';
    'do_block';
    'do_color';
    'do_copy';
    'do_copy_region';
    'do_delete';
    'do_delete_region';
    'do_help';
    'do_move';
    'do_stupidmove';
    'do_palettes';
    'do_replace';
    'do_run';
    'do_SaveAs';
    'do_tild';
    'do_view';
    'getlink';
    'prt_align';
    'cosclick';
    'SaveasInterfFunc_'
    'do_addnew'
    'do_export'
    'do_load' 
    'do_exit'
    'x_matrix']
for k=1:size(names,'r')
  getf(get_function_path(names(k)))
end

getf('SCI/macros/scicos_blocks/Misc/scifunc_block.sci')
lines(0)
clearfun('lines')
deff('x=lines(x)','x=0 ')
getf('SCI/tests/demos/dialogs.sci')
execstr('message=x_message')
execstr('dialog=x_dialog')
execstr('tk_getcolor=getcolor')
//I=%io(1)
if MSDOS then
  O=file('open',TMPDIR+'\sciout','unknown')
else
  O=file('open','/dev/null','unknown')
end
//exec('SCI/tests/demos/demolist.sce',-1)
if MSDOS then
// Windows
  sel=[1:8] 
else
// UNIX
  sel=[1:8] 
end

dialogs='demo'+string(sel)+'.dialogs'
for dia=dialogs
  I=file('open','SCI/tests/demos/'+dia,'old');
  %IO=[I,O];
  ierr=execstr('exec(''SCI/demos/alldems.dem'')','errcatch')
  if ierr<>0 then 
    write(%io(2),'Error had occurred at line '+LineCount+' of file '+fil)
  end
  file('close',I)
end
file('close',O)
//retore the primitives
newfun('x_message',x_mess)
newfun('x_dialog',x_dia)
newfun('x_mdialog',x_mdia)
newfun('x_choose',c_cho)
newfun('xclick',xcli)
newfun('xgetmouse',xgetm)
newfun('xgetfile',xgetf)
