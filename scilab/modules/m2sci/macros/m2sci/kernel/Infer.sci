function infer=Infer(varargin)
// Copyright INRIA
// Create a new inference tlist

fields=["infer","dims","type","contents"]

rhs=argn(2)

nargs=size(varargin)

// Infer(): all unknown
if nargs==0 then
  infer=tlist(fields,list(Unknown,Unknown),Type(Unknown,Unknown),Contents())
elseif nargs==2 then
  if typeof(varargin(1))<>"list" then
    error(msprintf(gettext("errors","m2sci_error_13"),typeof(varargin(1))));
  end
  if typeof(varargin(2))<>"type" then
    error(msprintf(gettext("errors","m2sci_error_14"),typeof(varargin(2))));
  end
  infer=tlist(fields,varargin(1),varargin(2),Contents())
elseif nargs==3 then // Should only be used for cells and structs
  if typeof(varargin(1))<>"list" then
    error(msprintf(gettext("errors","m2sci_error_13"),typeof(varargin(1))));
  end
  if typeof(varargin(2))<>"type" then
    error(msprintf(gettext("errors","m2sci_error_14"),typeof(varargin(2))));
  end
  if typeof(varargin(3))<>"contents" then
      error(msprintf(gettext("errors","m2sci_error_15"),typeof(varargin(3))));
  end
  infer=tlist(fields,varargin(1),varargin(2),varargin(3))
end
endfunction
