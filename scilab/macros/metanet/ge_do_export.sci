function ge_do_export()   
//Copyright INRIA
//Author : Serge Steer 2002

// export the contents of the editor to a regular graphic window for
// plotting and exports
//!
// Copyright INRIA
  old=xget('window')
  win=max(winsid()+1)
  execstr(['global EGdata_'+w
	   'EGdata_'+string(win)+'=EGdata_'+w
	   'GraphList=EGdata_'+w+'.GraphList'])

  xset('window',win)
  
  rect=ge_dig_bound(GraphList);
  d=max(GraphList.default_node_diam,max(GraphList.node_diam))
  if rect<>[] then
    rect(1:2)=rect(1:2)-d
    rect(3:4)=rect(3:4)+d
  else
    rect=[0,0,600,400];
  end
  xsetech(arect=[0,0,0,0])
  plot2d(0,0,frameflag=3,axesflag=0,rect=rect)
  ge_drawobjs(GraphList)
  xset('window',old)
endfunction
