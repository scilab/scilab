// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2005-2007 - INRIA - Allan CORNET
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

//------------------------------------------------------------------------------
function [HomeDirectory,ScilabHomeDirectory] = sethomedirectory(varargin)
  nv = size(varargin);
	ScilabHomeDirectory = SCIHOME;
	HomeDirectory = home;

  if nv == 1 then // to set Scilab home with a path
    warnobsolete("SCIHOME","5.3");
  end
endfunction
//------------------------------------------------------------------------------
