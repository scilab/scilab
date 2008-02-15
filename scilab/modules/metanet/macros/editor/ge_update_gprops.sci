
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Serge STEER <serge.steer@inria.fr>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function ge_update_gprops(e,propname,propvalue)
  for k=1:size(e,'*')
    if e(k).type=="Compound" then
      n=size(e(k).children,'*')
      for i=1:n
	set(e(k).children(i),propname,propvalue)
      end
    else
      set(e(k),propname,propvalue)
    end
  end
endfunction
