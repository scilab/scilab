
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function showprofile(fun)
lst=macr2lst(fun)
count=profile(lst)
count(:,2)=round(100*count(:,2))/100
count=string(count)

txt=fun2string(lst,'fun')
m=min(size(count,1),size(txt,1))
txt=txt(1:m);count=count(1:m,:)
txt=part(txt,1:max(length(txt)))
for k=1:3
  txt=txt+'|'+part(count(:,k),1:max(length(count(:,k))))
end
txt=txt+'|'
write(%io(2),txt,'(a)')
endfunction
