function netclose(win)
//Copyright INRIA
//Author : Serge Steer 2002
  old=xget('window')
  xset('window',win)
  seteventhandler("")
  w=string(win)
  execstr('global  EGdata_'+w+',ok=typeof(EGdata_'+w+')==''egdata''')
  if ~ok then
    execstr('clearglobal  EGdata_'+w)
    error('Bad edit_graph window number: '+string(win))
  end
  if ge_do_quit() then
    xdel(win)
  else
    seteventhandler('ge_eventhandler')   
  end
  if win<>old then xset('window',old),end

endfunction
