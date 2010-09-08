// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function count=profile(fun)
if type(fun)==13 then
  lst=macr2lst(fun)
else
  lst=fun
end
if lst(5)(1)<>'25' then
  error(msprintf(gettext("%s: The function has not been built for profiling"),"profile"))
end
count=get_profile(lst,4)
count=count(1:$-1,:)
count(:,2)=count(:,2)/1000000	
endfunction
