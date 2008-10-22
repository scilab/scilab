// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function kdf=tokenpos(str,sep)
//return a vector of indices giving the beginning of the included
//tokens
  if argn(2)<2 then sep=[' ',ascii(9)],end

  if size(str,'*')<> 1 then 
    error(msprintf(gettext("%s: Wrong size for input argument #%d: A character string expected.\n"),"tokenpos",1));
  end
  if or(length(sep)<>1) then
    error(msprintf(gettext("%s: Wrong size for input argument #%d: A character or vector of characters expected.\n"),"tokenpos",2));
  end
  str=sep(1)+str+sep(1)
  k1=strindex(str,sep)
  i=find(k1(2:$)-k1(1:$-1)>1)
  if i<>[] then
    kdf=[k1(i);k1(i+1)-2]'
  else
    kdf=[]
  end
endfunction
