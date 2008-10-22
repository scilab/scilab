// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
//

// @OBSOLETE

function [x,lagr,f]=linpro(p,C,b,ci,cs,mi,x0,imp)
  msg = [gettext('This function has been moved to a external contribution.'); ..
        gettext('Please download quapro toolboxe.'); ..
        gettext('http://www.scilab.org/contrib/index_contrib.php?page=download.php');];
 warning(msg);
endfunction
