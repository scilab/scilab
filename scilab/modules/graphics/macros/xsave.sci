// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function xsave(fil,num)
  if argn(2)<1 then
   error(msprintf(gettext("%s: Wrong number of input argument(s): At least %d expected.\n"), "xsave", 1));
  end
  if argn(2)<2 then
    f=gcf()
    preservecur=%f
  else
    preservecur=winsid()<>[]
    if preservecur then old=gcf();end
    f=scf(num);
  end
  save(fil,f)
  if preservecur then scf(old),end
endfunction
