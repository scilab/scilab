function ge_do_defaults()
//Copyright INRIA
//Author : Serge Steer 2002

  execstr('global EGdata_'+w+';EGdata=EGdata_'+w)
  default_node_diam=EGdata.GraphList.default_node_diam
  default_node_border=EGdata.GraphList.default_node_border
  default_edge_width=EGdata.GraphList.default_edge_width
  default_edge_hi_width=EGdata.GraphList.default_edge_hi_width
  default_font_size=EGdata.GraphList.default_font_size
  
  [ok,default_node_diam,default_node_border, default_edge_width,default_font_size]=getvalue('Default parameters',..
		['Node diameter','Border node width','Edge width','font size'],..
		list('vec',1,'vec',1,'vec',1,'vec',1),..
                string([default_node_diam,default_node_border, default_edge_width,default_font_size]))
    if ok then
    EGdata.GraphList.default_node_diam = default_node_diam
    EGdata.GraphList.default_node_border = default_node_border
    EGdata.GraphList.default_edge_width =default_edge_width 
    EGdata.GraphList.default_font_size = default_font_size 
    
    execstr('EGdata_'+w+'=EGdata')
    GraphList=EGdata.GraphList
    xbasc()
    ge_set_winsize()
    ge_drawobjs(GraphList),
    end
endfunction
