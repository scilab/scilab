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
     ['global %fig_counter'
      '%fig_counter=%fig_counter+1;'
      'scs_m1=scs_m;scs_m1.props.title(1)='' '' '
      'do_export(scs_m1,''fig''+string(%fig_counter));'
      'rect_=dig_bound(scs_m)'
      'rect_=min((rect_(3)-rect_(1))*12/400,12)'
      'txti=[''\begin{center}'';''\includegraphics[angle=270,width=''+string(rect_)+''cm]{fig''+string(%fig_counter)+''.eps}'';''\end{center}'']'
      'txti=[''  '';txti;''  '']'
      '//mputl(txti,%J)'
      'Cmenu=''Replot'''])

deff('GrabPalette_()',..
     ['global %fig_counter'
      '[c_i,c_x,c_y,c_w]=xclick()'
      '%fig_counter=%fig_counter+1;'
      'dr=driver();driver(''Pos'');'
      'fname=''fig''+string(%fig_counter)'
      'xinit(fname)';
      'xtape(''replay'',c_w);xend();driver(dr)'
      'if MSDOS then'
      '  comm=pathconvert(SCI+''\bin\BEpsf'',%f,%f,''w'')'
      '  rep=unix_g('''''"''+comm+'''''" ''+fname)'
      'else'
      '  rep=unix_g(SCI+''/bin/BEpsf ''+fname)'
      'end'
      'txt2=x_dialog([''Enter caption''],'''');'
      'txti=[''\begin{figure}\begin{center}'';''\fbox{\includegraphics[angle=0,width=12cm]{fig''+string(%fig_counter)+''.eps}}'';''\end{center}\caption{\label{labyy''+string(%fig_counter)+''}''+txt2+''}'';''\end{figure}'']'
      'txti=[''  '';txti;''  '']'
      '//mputl(txti,%J)'
      'Cmenu=''Replot'''])

deff('GrabScope_()',..
     ['global %fig_counter'
      '[c_i,c_x,c_y,c_w]=xclick()'
      'xset(''default'')'
      '%fig_counter=%fig_counter+1;'
      'dr=driver();driver(''Pos'');'
      'fname=''fig''+string(%fig_counter)'
      'xinit(fname)';
      'xtape(''replay'',c_w);xend();driver(dr)'
      'if MSDOS then'
      '  comm=pathconvert(SCI+''\bin\BEpsf'',%f,%f,''w'')'
      '  rep=unix_g('''''"''+comm+'''''" ''+fname)'
      'else'
      '  rep=unix_g(SCI+''/bin/BEpsf ''+fname)'
      'end'
      'txt2=x_dialog([''Enter caption''],'''');'
      'txti=[''\begin{figure}\begin{center}'';''\includegraphics[angle=0,width=12cm]{fig''+string(%fig_counter)+''.eps}'';''\end{center}\caption{\label{labyy''+string(%fig_counter)+''}''+txt2+''}'';''\end{figure}'']'
      'txti=[''  '';txti;''  '']'
      '//mputl(txti,%J)'
      'Cmenu=''Replot'''])


deff('Comment_()',..
     ['txt=x_dialog([''Comments''],'''');'
      '//mputl(txt,%J)'
      'Cmenu=''Open/Set'''])



getf('SCI/demos/scicos/dialogs_play.sci')
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
newfun('x_message',x_mess)
newfun('x_dialog',x_dia)
newfun('x_mdialog',x_mdia)
newfun('x_choose',c_cho)
newfun('xclick',xcli)
newfun('xgetmouse',xgetm)
newfun('xgetfile',xgetf)
if ierr<>0 then 
 write(%io(2),'Error had occurred at line '+string(LineCount)+' of file '+fil)
end
endfunction
