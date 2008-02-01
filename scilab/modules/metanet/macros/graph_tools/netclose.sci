function netclose(win)
//Copyright INRIA
//Author : Serge Steer 2002
  old=gcf()
  ge_win_handle=scf(win)
  seteventhandler("")
  w=string(win)
  execstr('global  EGdata_'+w+',ok=typeof(EGdata_'+w+')==''egdata''')
  if ~ok then
    execstr('clearglobal  EGdata_'+w)
    error('Bad edit_graph window number: '+string(win))
  end
  if ge_do_quit() then
    delete(ge_win_handle)
  else
    seteventhandler('ge_eventhandler')   
  end
  if ge_win_handle<>old then scf(old),end

endfunction
