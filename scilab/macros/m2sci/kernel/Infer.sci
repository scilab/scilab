function infer=Infer(varargin)
// Copyright INRIA
// Create a new inference tlist

fields=["infer","dims","type","contents"]

rhs=argn(2)

nargs=size(varargin)

// Infer(): all unknown
if nargs==0 then
  infer=tlist(fields,list(Unknown,Unknown),Type(Unknown,Unknown),[])
elseif nargs==2 then
  if typeof(varargin(1))<>"list" then
    error("Infer(): dims must be a list instead of a :"+typeof(varargin(1)))
  end
  if typeof(varargin(2))<>"type" then
    error("Infer(): type must be a ''type'' tlist instead of a :"+typeof(varargin(2)))
  end
  if varargin(2).vtype==Cell then
    infer=tlist(fields,varargin(1),varargin(2),cell())
  elseif varargin(2).vtype==Struct then
    infer=tlist(fields,varargin(1),varargin(2),struct())
  else
    infer=tlist(fields,varargin(1),varargin(2),[])
  end
elseif nargs==3 then // Should only be used for cells and structs
  if typeof(varargin(1))<>"list" then
    error("Infer(): dims must be a list instead of a :"+typeof(varargin(1)))
  end
  if typeof(varargin(2))<>"type" then
    error("Infer(): type must be a ''type'' tlist instead of a :"+typeof(varargin(2)))
  end
  if and(typeof(varargin(3))<>["st","ce"]) then
    if varargin(3)<>[] then 
      error("Infer(): contents must be a cell or a struct instead of a :"+typeof(varargin(3)))
    end
  end
  infer=tlist(fields,varargin(1),varargin(2),varargin(3))
end
endfunction
