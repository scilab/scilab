function GraphList=ge_do_settings(GraphList)
//Copyright INRIA
//Author : Serge Steer 2002

  default_node_diam=GraphList.default_node_diam
  default_node_border=GraphList.default_node_border
  default_edge_width=GraphList.default_edge_width
  default_edge_hi_width=GraphList.default_edge_hi_width
  default_font_size=GraphList.default_font_size
  if GraphList.directed==0 then directed='no',else directed='yes',end
  while %t 
    [ok,default_node_diam,default_node_border,default_edge_width,default_font_size,directed]=getvalue('Default parameters',..
						  ['Default Node diameter','Default Border node width',..
		    'Default Edge width','Default font size','Is graph directed (yes/no)'],..
		    list('vec',1,'vec',1,'vec',1,'vec',1,'str',-1),..
		    [string([default_node_diam,default_node_border,default_edge_width,default_font_size]),directed])
    if ~ok then return,end
    mess=[]
    if default_node_diam <0 then 
      mess=[mess;'Default Node diameter must be positive']
    end
    if default_node_border <0 then 
      mess=[mess;'Default Node border must be positive']
    end
    if default_edge_width <0 then 
      mess=[mess;'Default edge width must be positive']
    end
    if default_font_size <0 then 
      mess=[mess;'Default font size must be positive']
    end
    directed=convstr(stripblanks(directed))
    if and(directed<>['yes','no']) then
       mess=[mess;'The answer for ""Is graph directed"" must be ""yes"" or ""no""']
    end
    if mess<>[] then
      x_message(mess)
    else
      break
    end
  end
  GraphList.default_node_diam = default_node_diam
  GraphList.default_node_border = default_node_border
  GraphList.default_edge_width =default_edge_width 
  GraphList.default_font_size = default_font_size 
  GraphList.directed=find(directed==['no','yes'])-1
endfunction
