function  netwindow(win)
//Copyright INRIA
//Author : Serge Steer 2002

  global EGcurrent
  execstr('global  EGdata_'+w+',ok=typeof(EGdata_'+w+')==''egdata''')
  if ~ok then
    error('Bad editgraph window number: '+string(win))
  end
  EGcurrent=win 
endfunction

