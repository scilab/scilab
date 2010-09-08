// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ???? - INRIA - Serge STEER 
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function mtlb_error(varargin)
  
  if argn(2)==1 then
    m=varargin(1)
  else
    F=varargin(1)
    //check if it is an identifier or a message
    //if strindex(F,'%')==[] then //not a format
      if size(tokens(F,':'),'*')>1 then
	// an identifier, it is ignored
	m=msprintf(varargin(2:$))
      else
	m=msprintf(varargin(:))
      end
    //end
  end
  error(m)
endfunction
