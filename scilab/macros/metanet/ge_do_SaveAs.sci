function [GraphList,ok]=ge_do_SaveAs(GraphList)
//Copyright INRIA
//Author : Serge Steer 2002

//
// Copyright INRIA
  tit='Select a file path'
  fname=xgetfile('*.graph',emptystr(),tit)
  fname=stripblanks(fname)
  if fname==emptystr() then return,end

  [path,name,ext]=splitfilepath(fname)
  GraphList.node_number=size(GraphList.node_x,'*')
  
   if GraphList.tail==[] then
    x_message(['Graph with no arcs cannot be saved'])
    ok=%f
    return
  end

  if GraphList.node_name==[] then
    GraphList.node_name=string(1:size(GraphList.node_x,'*'))
  else
    GraphList.edge_name=stripblanks(GraphList.edge_name)
    k=find(GraphList.edge_name=='')
    if k<>[] then
      ge_hilite_arcs(k,GraphList)
      x_message(['Hilited arcs have no name'
		 'Graph has not been saved'])
      ok=%f
      return
    end
    GraphList.node_name=stripblanks(GraphList.node_name)
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
  
  
  select ext
    case 'graph' then
    if fileinfo(fname)<>[] then
      r=x_message(['File '+fname+ ' already exists'
		 'erase it'],['Yes','No'])
      if r==2 then ok=%f,return,end
      if MSDOS then
	unix_s('del '+fname)
      else
	unix_s('rm '+fname)
      end
    end
    path_s=GraphList.name(2);
    GraphList.name= GraphList.name(1)
    ok=execstr('save_graph(GraphList,fname)','errcatch')==0
    GraphList.name(2)=path_s
    if ok then 
      nf=length(fname)
      GraphList.name(1)=name
      GraphList.name(2)=part(fname,1:nf-6)
    else
      x_message([lasterror();'';'Graph has not been saved'])
    end
    
  else
    message('Only *.graph  files allowed');
    ok=%f
    return
  end
endfunction
