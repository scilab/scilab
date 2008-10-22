// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ???? - INRIA - Scilab
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function cont=Contents(varargin)
// Create a new inference tlist

fields=["contents","index","data"]

nargs=size(varargin)

// Contents: all unknown
if nargs==0 then
  cont=tlist(fields,list(),list())
elseif nargs==2 then
  if typeof(varargin(1))<>"list" then
    error(msprintf(gettext("index must be a list instead of a: %s."),typeof(varargin(1))))
  end
  if typeof(varargin(2))<>"list" then
    error(msprintf(gettext("index must be a list instead of a: %s."),typeof(varargin(2))))
  end
  cont=tlist(fields,varargin(1),varargin(2))
else
  error(gettext("Wrong number of inputs."));
end
endfunction
