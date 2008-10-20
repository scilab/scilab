function show_arcs(p,sup)
//Copyright INRIA
//Author : Serge Steer 2002
  if argn(2)<2 then sup='no',end
  sup=sup=='sup'
  //get current editgraph window
  global EGcurrent
  if type(EGcurrent)<>1|size(EGcurrent,'*')<>1 then
    error('No current edit_graph window defined, use netwindow')
  end
  win=EGcurrent;
  old=xget('window');
  xset('window',win);w=string(win)
  execstr(['global EGdata_'+w
	   'if typeof(EGdata_'+w+')==''egdata'' then';
	   'EGdata=EGdata_'+w
	   'GraphList=EGdata.GraphList'
	   'if ~sup then xbasc();ge_set_winsize();ge_drawobjs(GraphList),end'
	   'b=GraphList.edge_width(p)'
	   'b(b==0)=GraphList.default_edge_width'
	   'b=3*b'
	   'GraphList.edge_width(p)=b'
	   'ge_drawarcs(p)'
	   'end'])
  xset('window',old)
endfunction

