// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function mputl(str,fd)
[lhs,rhs]=argn()
np=5000 //pack size

CR=ascii(13); LF=ascii(10); 
if MSDOS then
  newline = LF;
  str = strcat(str + newline);
 
else
  newline=LF;
  str=strcat(str,newline)+newline
  
end

n=length(str);
mp=floor(n/np);

if rhs==1 then //write to last opened file
  k1=1;
  for i=1:mp
    mput(ascii(part(str,k1:k1+np)),'uc');
    k1=k1+np+1;
  end  
  mput(ascii(part(str,k1:n)),'uc');
else
  if type(fd)==10 then // file given by its path
    opened=%t;
    fil=fd;
    [fd,ierr]=mopen(fil,'wb');
    if ierr~=0 then 
      error(msprintf(gettext("%s: Cannot open file %s.\n"),'mputl',fil));
    end
  else // file given by its logical unit
    opened=%f;
  end
  k1=1
  for i=1:mp
    mput(ascii(part(str,k1:k1+np)),'uc',fd);
    k1=k1+np+1;
  end  
  mput(ascii(part(str,k1:n)),'uc',fd);
  if opened then mclose(fd),end
end
endfunction
