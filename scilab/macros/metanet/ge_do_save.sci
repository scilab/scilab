function ok=ge_do_save(GraphList,path)   
//Copyright INRIA
//Author : Serge Steer 2002

// saves data structures GraphList 
//!
// Copyright INRIA


  ok=%f
  GraphList.node_number=size(GraphList.node_x,'*')
  
  if GraphList.tail==[] then
    x_message(['Graph with no arcs cannot be saved'])
    ok=%f
    return
  end

  if GraphList.node_name==[] then
    GraphList.node_name=string(1:size(GraphList.node_x,'*'))
  else
    GraphList.node_name=stripblanks(GraphList.node_name)
    GraphList.edge_name=stripblanks(GraphList.edge_name)
    k=find(GraphList.edge_name=='')
    if k<>[] then
      ge_hilite_arcs(k,GraphList)
      x_message(['Hilited arcs have no name'
		 'Graph has not been saved'])
      ok=%f
      return
    end
    conflicts=ge_check_names(GraphList)
    if conflicts(1)<>[] then
      ge_hilite_nodes(conflicts(1),GraphList)
      x_message(['Hilited nodes have no name'
		 'Graph has not been saved'])
      ok=%f
      return
    elseif size(conflicts)>1 then
      ge_hilite_nodes(conflicts(2),GraphList)
      x_message(['Hilited nodes have identical name'
		 'Graph has not been saved'])
      ok=%f
      return
    end
    
  end
  fname=path
  if fileinfo(fname)<>[] then
    if MSDOS then
      unix_s('del '+fname)
    else
      unix_s('rm '+fname)
    end
  end
  
  ok=execstr('save_graph(GraphList,fname)','errcatch')==0
  if ~ok then
    x_message([lasterror();'';'Graph has not been saved'])
  end
  ok=%t
  save_graph(GraphList,fname)
endfunction
