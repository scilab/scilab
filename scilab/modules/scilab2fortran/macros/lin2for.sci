// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt


function [txt]=lin2for(lst,ln)
//si lst est une liste representant une macro compilee,mac2for)
//restourne le code ascii de la macro
//!
count=0;level=0
[kt,count]=findlin(lst,ln)
nt=prod(size(kt))
for i=nt:-1:2
  lst=lst(kt(i))
end
setparam()

txt=ins2for(lst,1)
endfunction
