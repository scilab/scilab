function GraphList=ge_do_default_names(GraphList)
//Copyright INRIA
//Author : Serge Steer 2002
  r=x_message(['This operation will replaces ALL node and arc names'
	       'by their default values.'
	       'Do you really want to do it?'],['yes','no'])
  if r==1 then
    
    GraphList.nodes.graphics.name=string(1:GraphList.nodes.number)
    GraphList.edges.graphics.name=string(1:size(GraphList.edges.head,'*'))

    NodeId=GraphList.nodes.graphics.display;
    ArcId=GraphList.edges.graphics.display;
    ge_axes_handle=gca();
    gindex=ge_axes_handle.user_data
    drawlater()
    if ArcId==2 then
      hedges=gindex.edge;
      for k=1:size(hedges,'*')
	e=hedges(k)
	e.children(1).text=string(k)
      end
    end
    if NodeId==2 then
      hnodes=gindex.node;
      for k=1:size(hnodes,'*')
	e=hnodes(k)
	e.children(1).text=string(k)
      end
    end
    drawnow();show_pixmap()
  end
endfunction
