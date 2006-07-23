function scr=metanet(path,winsize)
// Copyright INRIA
  disp("""metanet"" is obsolete. Use ""edit_graph"" instead")
  if argn(2)<1 then
    scr=edit_graph() 
  elseif argn(2)==1 then
    scr=edit_graph(path) 
  else
    scr=edit_graph(path,1,winsize) 
  end
endfunction
