
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2008 - INRIA
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function p=metanet_module_path()
  //returns the path where the metanet toolbox is located
  execstr('w=string('+whereis('metanet_module_path')+')')
  p=w(1)
  k=strindex(p,['/' '\'])
  p=part(p,1:k($-2))
endfunction
