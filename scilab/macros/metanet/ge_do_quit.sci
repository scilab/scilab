function  ge_do_quit
//Copyright INRIA
//Author : Serge Steer 2002
  global EGcurrent
  w=string(win)
  execstr('global EGdata_'+w+'; edited=EGdata_'+w+'.Edited')
  if edited then
    ok=x_message(['Current graph is modified'
		  'Do you really want to delete it'],['yes','no'])
    if ok==2 then return,end
  end
  if win==EGcurrent then EGcurrent=[],end
  xdel(win)
  execstr('clearglobal EGdata_'+w')
endfunction
