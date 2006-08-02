function cont=Contents(varargin)
// Copyright INRIA
// Create a new inference tlist

fields=["contents","index","data"]

nargs=size(varargin)

// Contents: all unknown
if nargs==0 then
  cont=tlist(fields,list(),list())
elseif nargs==2 then
  if typeof(varargin(1))<>"list" then
    error("Contents(): index must be a list instead of a :"+typeof(varargin(1)))
  end
  if typeof(varargin(2))<>"list" then
    error("Contents(): data must be a list instead of a :"+typeof(varargin(2)))
  end
  cont=tlist(fields,varargin(1),varargin(2))
else
  error("Wrong number of inputs");
end
endfunction
