// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function hdl=%h_set(varargin)
  if (lstsize(varargin) == 3)
    hdl = varargin(1)
    str = varargin(2)
    val = varargin(3)
    for i=1:size(hdl,'*')
      set(hdl(i),str,val)
    end,

  elseif (lstsize(varargin) == 2)
    str = varargin(1)
    val = varargin(2)
    hdl=[]
    set(str,val)
  end,
endfunction


