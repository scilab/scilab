function show_nodes(p,sup)
//Copyright INRIA
//Author : Serge Steer 2002
  if argn(2)<2 then sup='no',end
  sup=sup=='sup'
  //get current editgraph window
  global EGcurrent
  if type(EGcurrent)<>1|size(EGcurrent,'*')<>1 then
    error('No current editgraph window defined, use netwindow')
  end
  win=EGcurrent;w=string(win)
  execstr(['global EGdata_'+w
	   'if typeof(EGdata_'+w+')==''egdata'' then';
	   'EGdata=EGdata_'+w
	   'GraphList=EGdata.GraphList'
	   'if ~sup then xbasc();ge_set_winsize();ge_drawobjs(GraphList),end'
	   'b=GraphList.node_border(p)'
	   'b(b==0)=GraphList.default_node_border'
	   'b=3*b'
	   'GraphList.node_border(p)=b'
	   'ge_drawnodes(p)'
	   'end'])
endfunction
