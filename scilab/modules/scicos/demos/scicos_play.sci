function scicos_play(fil)
// Copyright INRIA
funcprot(0)

if exists('scicos_scicoslib')==0 then
  load("SCI/modules/scicos/macros/scicos_scicos/lib") ;
end

if exists('scicos_autolib')==0 then
  load("SCI/modules/scicos/macros/scicos_auto/lib") ;
end

if exists('scicos_utilslib')==0 then
  load("SCI/modules/scicos/macros/scicos_utils/lib") ;
end

global LineCount

//x_mess=funptr('x_message');clearfun('x_message')
x_dia=funptr('x_dialog');clearfun('x_dialog')
x_mdia=funptr('x_mdialog');clearfun('x_mdialog')
c_cho=funptr('x_choose');clearfun('x_choose')
xcli=funptr('xclick');clearfun('xclick')
xgetm=funptr('xgetmouse');clearfun('xgetmouse')
//xgetf=funptr('xgetfile');clearfun('xgetfile')


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
  exec(get_function_path(names(k)))
end


global %fig_counter
%fig_counter=0;
%scicos_short;
%scicos_short($+1,1)='x';
%scicos_short($,2)='GrabScope'
%scicos_menu;
%scicos_menu($)($+1)=['GrabScope'];


%scicos_short;
%scicos_short($+1,1)='y';
%scicos_short($,2)='Snapshot'
%scicos_menu;
%scicos_menu($)($+1)=['Snapshot'];

%scicos_short;
%scicos_short($+1,1)='w';
%scicos_short($,2)='GrabPalette'
%scicos_menu;
%scicos_menu($)($+1)=['GrabPalette'];


%scicos_short;
%scicos_short($+1,1)='z';
%scicos_short($,2)='Comment'
%scicos_menu;
%scicos_menu($)($+1)=['Comment'];


deff('Snapshot_()',..
     ['Cmenu=''Replot'''])
      
deff('GrabPalette_()',..
     ['global %fig_counter'
      '[c_i,c_x,c_y,c_w]=xclick()'
      'Cmenu=''Replot'''])      

deff('GrabScope_()',..
     ['global %fig_counter'
      '[c_i,c_x,c_y,c_w]=xclick()'
      'Cmenu=''Replot'''])

deff('Comment_()',..
     ['txt=x_dialog([''Comments''],'''');'
      'Cmenu=''Open/Set'''])



exec('SCI/demos/scicos/dialogs_play.sci')
execstr('tk_getfile=xgetfile')
execstr('tk_savefile=xgetfile')
tk_getcolor=getcolor

clear('%scicos_gui_mode_back')
if exists('%scicos_gui_mode') then
  %scicos_gui_mode_back=%scicos_gui_mode
end
%scicos_gui_mode=0
//%J=mopen(fil+".tex",'w')
%I=mopen(fil,'r');ierr=execstr('scicos();','errcatch');mclose(%I);
//mclose(%J)

if exists('%scicos_gui_mode_back') then
  %scicos_gui_mode=%scicos_gui_mode_back
end


//retore the primitives
//newfun('x_message',x_mess)
newfun('x_dialog',x_dia)
newfun('x_mdialog',x_mdia)
newfun('x_choose',c_cho)
newfun('xclick',xcli)
newfun('xgetmouse',xgetm)
//newfun('xgetfile',xgetf)
if ierr<>0 then 
 write(%io(2),'Error had occurred at line '+string(LineCount)+' of file '+fil)
end
endfunction
