function scicos_learn(fil)
// Copyright INRIA
funcprot(0);
comm='//'
load SCI/macros/scicos/lib

//rename dialog and mouse primitives
x_mess=funptr('x_message');clearfun('x_message');newfun('x_message1',x_mess);
x_dia=funptr('x_dialog');clearfun('x_dialog');newfun('x_dialog1',x_dia);
xcli=funptr('xclick');clearfun('xclick');newfun('xclick1',xcli);
xgetm=funptr('xgetmouse');clearfun('xgetmouse');newfun('xgetmouse1',xgetm)
x_mdia=funptr('x_mdialog');clearfun('x_mdialog');newfun('x_mdialog1',x_mdia);
c_cho=funptr('x_choose');clearfun('x_choose');newfun('x_choose1',c_cho);
xgetf=funptr('xgetfile');clearfun('xgetfile');newfun('xgetfile1',xgetf);
//these function are redefined below
getf SCI/demos/scicos/dialogs_learn.sci
tk_getfile=xgetfile
tk_getcolor=getcolor
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

uapp=mopen(fil,'w')
global MousePos
MousePos=[-100 -100 -1]
lines(0);
ierr=execstr('scicos();','errcatch')
mclose(uapp)
// kill the added primitives
clearfun('x_message1');clearfun('x_dialog1');clearfun('xclick1')
clearfun('xgetmouse1');clearfun('x_mdialog1');clearfun('x_choose1');
clearfun('xgetfile1');
//retore the primitives
newfun('x_message',x_mess)
newfun('x_dialog',x_dia)
newfun('x_mdialog',x_mdia)
newfun('x_choose',c_cho)
newfun('xclick',xcli)
newfun('xgetmouse',xgetm)
newfun('xgetfile',xgetf)
endfunction

