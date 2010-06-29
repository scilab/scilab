// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function whereami()
// whereami - display calling tree 
//!
[linn,mac]=where()
nn=prod(size(linn))
lm=max(length(mac))
txt=[]    
for k=2:nn
  if mac(k)=='exec' then
    txt=[txt;
	msprintf(gettext("%s called at line %s of %s"),part(mac(k-1),1:lm), string(linn(k)), "exec_file")]
  elseif mac(k)=='execstr' then
    txt=[txt;
        	msprintf(gettext("%s called at line %s of %s instruction."),part(mac(k-1),1:lm), string(linn(k)),"execstr")]
  elseif mac(k)=='pause' then
     txt=[txt;
        msprintf(gettext("%s called under %s"),part(mac(k-1),1:lm),"pause") ]
  else
    txt=[txt;
        msprintf(gettext("%s called at line %s of macro %s"), part(mac(k-1),1:lm), string(linn(k)),mac(k))]
  end
end
write(%io(2),txt)
endfunction
