
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2008 - INRIA - Serge STEER <serge.steer@inria.fr>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function txt=ge_get_info(info)
//This function may be redefined by the user to get a textual
//form of the informations associated with the current diagram
  if size(info)>0 then
    txt=info(1)
  else
    txt=[]
  end
endfunction
