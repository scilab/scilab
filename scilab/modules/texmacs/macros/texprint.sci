// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// Copyright (C) DIGITEO - 2009 - Sylvestre LEDRU (tag this function as obsolete)
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function [tt]=texprint(a)
  warnobsolete("prettyprint","5.3");
  tt=prettyprint(a,"tex");
endfunction

