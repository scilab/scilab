function scicos_play(fil)
// Copyright INRIA
funcprot(0)
load SCI/macros/scicos/lib

global LineCount

x_mess=funptr('x_message');clearfun('x_message')
x_dia=funptr('x_dialog');clearfun('x_dialog')
x_mdia=funptr('x_mdialog');clearfun('x_mdialog')
c_cho=funptr('x_choose');clearfun('x_choose')
xcli=funptr('xclick');clearfun('xclick')
xgetm=funptr('xgetmouse');clearfun('xgetmouse')
xgetf=funptr('xgetfile');clearfun('xgetfile')


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

getf('SCI/demos/scicos/dialogs_play.sci')
execstr('tk_getfile=xgetfile')
tk_getcolor=getcolor

%I=mopen(fil,'r');ierr=execstr('scicos();','errcatch');mclose(%I);

//retore the primitives
newfun('x_message',x_mess)
newfun('x_dialog',x_dia)
newfun('x_mdialog',x_mdia)
newfun('x_choose',c_cho)
newfun('xclick',xcli)
newfun('xgetmouse',xgetm)
newfun('xgetfile',xgetf)
if ierr<>0 then 
 write(%io(2),'Error had occurred at line '+LineCount+' of file '+fil)
end
endfunction
