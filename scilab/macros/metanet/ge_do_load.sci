function [ok,GraphList,edited,fname]=ge_do_load(fname)
//Copyright INRIA
//Author : Serge Steer 2002

// Copyright INRIA
  GraphList=[]
  ok=%t
  if edited then
    ok=x_message(['Current graph is modified'
		  'Do you really want to erase it'],['yes','no'])
    if ok==2 then ok=%f,return,end
  end
  edited=%f
  if argn(2)<1 then fname=xgetfile('*.graph*'),end
  if fname<>emptystr() then
    [path,name,ext]=splitfilepath(fname)

    select ext
    case 'graph'
      ierr=execstr('GraphList=load_graph(fname)','errcatch')
      if(ierr<>0) then
	x_message(lasterror());ok=%f
	return
      end
      version=editgraph_ver()
      ok=%t
    case 'graphb'
      ierr=execstr('load(fname)','errcatch')
      if(ierr<>0) then
	x_message(lasterror());ok=%f
	return
      end
      version=editgraph_ver()
      ok=%t
    else
      x_message(['Only *.graph (formatted) or *.graphb (binary) files allowed'])
      ok=%f
      return
    end
    
    if ierr<>0 then
      x_message(fname+' cannot be loaded.') 
      ok=%f;return
    end

    for f=ge_node_fields()
      GraphList(f)=matrix(GraphList(f),1,-1)
    end
    for f=ge_arc_fields()
      GraphList(f)=matrix(GraphList(f),1,-1)
    end
    GraphList=ge_complete_defaults(GraphList)
    if version<>editgraph_ver()
      GraphList=ge_do_version(GraphList,editgraph_ver())
      edited=%t
    end
  else
    ok=%f
    return
  end
endfunction
