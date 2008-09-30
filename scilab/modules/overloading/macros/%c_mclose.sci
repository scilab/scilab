// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
function %c_mclose(str)
if convstr(str)=='all' then
  file('close',file())
else
 [units,typ,nams]=file()
 k=find(nams==str)
 if k<>[] then
   file('close',k)
 else
   write(%io(2),'No such file opened file')
 end
end
endfunction
