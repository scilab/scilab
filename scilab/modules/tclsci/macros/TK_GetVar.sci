// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2005 - INRIA - Allan CORNET
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function ret=TK_GetVar(Var)
  warning(msprintf(gettext("%s: Obsolete function. Please use %s."),"TK_GetVar","TCL_GetVar"));
  ret=TCL_GetVar(Var)
endfunction