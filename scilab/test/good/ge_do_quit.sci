function  ok=ge_do_quit(check_if_edited)
//Copyright INRIA
//Author : Serge Steer 2002
  if argn(2)<1 then check_if_edited=%t,end
  global EGcurrent
  w=string(win)
  execstr('global EGdata_'+w+'; edited=EGdata_'+w+'.Edited')
  if edited&check_if_edited then
    ok=x_message(['Current graph is modified'
		  'Really quit?'],['yes','no'])
    if ok==2 then ok=%f,return,end
  end
  if or(win==winsid()) then seteventhandler(''),end
  if win==EGcurrent then EGcurrent=[],end
  execstr('clearglobal EGdata_'+w+' EGhist_'+w)
  ok=%t
endfunction
