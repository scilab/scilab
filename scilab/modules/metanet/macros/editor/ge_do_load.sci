function [ok,GraphList,edited,fname]=ge_do_load(fname)
//Copyright INRIA
//Author : Serge Steer 2002
  GraphList=[];ok=%t
  if edited then
    ok=x_message(['Current graph is modified'
		  'Do you really want to erase it'],['yes','no'])
    if ok==2 then ok=%f,return,end
  end

  if argn(2)<1 then fname=tk_getfile('*.graph*'),end
  if fname==emptystr() then ok=%f,return,end// Canceled by user
  [GraphList,edited,msg]=load_graph(fname)
  if msg<>[] then //an error occured
    message(msg)
    ok=%f
    return
  end
  [path,name,ext]=splitfilepath(fname)
  GraphList.name=name;
  ok=%t

endfunction

