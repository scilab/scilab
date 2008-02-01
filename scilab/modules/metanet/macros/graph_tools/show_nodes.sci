function show_nodes(p,sup,leg)
//Copyright INRIA
//Author : Serge Steer 2002
  if argn(2)<2 then sup='no',end
  if exists('leg','local')==0 then leg='',end
  opt=find(['Number','Name','Demand','Label']==leg)
  if opt==[] then opt=0,end
  sup=sup=='sup'
  //get current editgraph window
  global EGcurrent
  if type(EGcurrent)<>1|size(EGcurrent,'*')<>1 then
    error('No current edit_graph window defined, use netwindow')
  end
  win=EGcurrent;w=string(win)
  old=xget('window');xset('window',win)
  execstr(['global EGdata_'+w
	   'if typeof(EGdata_'+w+')==''egdata'' then';
	   'EGdata=EGdata_'+w
	   'EGdata.NodeId=opt'
	   'GraphList=EGdata.GraphList'
	   'if ~sup then xbasc();ge_set_winsize();ge_drawobjs(GraphList),end'
	   'b=GraphList.node_border(p)'
	   'b(b==0)=GraphList.default_node_border'
	   'b=3*b'
	   'GraphList.node_border(p)=b'
	   'ge_drawnodes(p)'
	   'end'])
   xset('window',old)
endfunction
