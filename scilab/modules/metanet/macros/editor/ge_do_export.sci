function ge_do_export(GraphList)   
//Copyright INRIA
//Author : Serge Steer 2002

// export the contents of the editor to a regular graphic window for
// plotting and exports
//!
// Copyright INRIA
  old=xget('window')
  win=max(winsid()+1)

  xset('window',win)
  
  rect=ge_dig_bound(GraphList);
  d=max(GraphList.default_node_diam,max(GraphList.node_diam))
  if rect<>[] then
    rect(1:2)=rect(1:2)-d
    rect(3:4)=rect(3:4)+d
  else
    rect=[0,0,600,400];
  end
  if get('figure_style')=='old' then
    xsetech(arect=[0,0,0,0])
    plot2d(0,0,frameflag=3,axesflag=0,rect=rect)
    ge_drawobjs(GraphList)
  else
    drawlater()
    f=gcf();//f.axes_size=[rect(3)-rect(1);rect(4)-rect(2)];
    ax=gca();ax.data_bounds=[rect(1) rect(2);rect(3),rect(4)];
    ax.margins=[0,0,0,0]
    ge_drawobjs(GraphList)
    drawnow()
  end
  xset('window',old)
endfunction
