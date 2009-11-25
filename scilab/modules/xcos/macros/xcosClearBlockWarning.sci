//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009-2009 - DIGITEO - Bruno JOFRET
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
//
//

function xcosClearBlockWarning(k, win)
  for i = k'
    if typeof(scs_m.objs(i)) == "Block"
      warnBlockByUID(scs_m.objs(i).doc(1), "");
    end
  end 

endfunction
