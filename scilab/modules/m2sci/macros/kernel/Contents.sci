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
    error(msprintf(gettext("errors","index must be a list instead of a: %s."),typeof(varargin(1))))
  end
  if typeof(varargin(2))<>"list" then
    error(msprintf(gettext("errors","index must be a list instead of a: %s."),typeof(varargin(2))))
  end
  cont=tlist(fields,varargin(1),varargin(2))
else
  error(gettext("errors","Wrong number of inputs."));
end
endfunction
