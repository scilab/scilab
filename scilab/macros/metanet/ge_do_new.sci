function ge_do_new(GraphList)
//Copyright INRIA
//Author : Serge Steer 2002
 w=string(win)
 execstr('global EGdata_'+w+'; edited=EGdata_'+w+'.Edited')
   if edited then
     ok=x_message(['Current graph is modified'
		'Do you really want to erase it'],['yes','no'])
     if ok==2 then return,end
   end
   if argn(2)<1 then GraphList=editgraph_diagram(),end
   
   execstr('EGdata_'+w+'.GraphList=GraphList')
   execstr('EGdata_'+w+'.Edited=%f;EGdata=EGdata_'+w)
   ge_clear_history()
   xbasc()
   ge_set_winsize()
   ge_drawobjs(GraphList),
endfunction
