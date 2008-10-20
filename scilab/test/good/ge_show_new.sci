function ge_show_new(GraphList,scale)
//Copyright INRIA
//Author : Serge Steer 2002
  if argn(2)<2 then scale=1,end
  w=string(win)
  execstr('global EGdata_'+w+'; edited=EGdata_'+w+'.Edited')
  if edited then
    if x_message(['Current graph is modified'
		  'Do you really want to erase it'],['yes','no'])==2 then
      return
    end
  end
  execstr('EGdata_'+w+'.Zoom=scale')
  execstr('EGdata_'+w+'.GraphList=GraphList')
  execstr('EGdata_'+w+'.Edited=%f;EGdata=EGdata_'+w)
  ge_clear_history()
  ge_do_replot(GraphList)
endfunction
