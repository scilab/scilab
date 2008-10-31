// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// Copyright (C) DIGITEO - 2008 - Allan CORNET
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function mputl(str,fd)

[lhs,rhs]=argn()

if ~or(rhs ==[1 2]) then
  error(999,msprintf(gettext("%s: Wrong number of input argument(s).\n"),'mputl'));
  return
end

if type(str) <> 10 then
  error(999,msprintf(gettext("%s: Wrong type for input argument: A string expected.\n"),'mputl'));
  return
end

mn = size(str);
if ~or(mn ==1) then
  error(999,msprintf(gettext("%s: Wrong size for input argument #%d: A 1-by-n or m-by-1 array expected.\n"),'mputl',1));
  return
end

opened = %f;

if rhs == 1 then //write to last opened file
  opened = %f;
  fd = -1;
else
  if type(fd) == 10 then // file given by its path
    opened = %t;
    fil = fd;
    [fd,ierr] = mopen(fil,'wt');
    if ierr ~= 0 then 
      error(999,msprintf(gettext("%s: Cannot open file %s.\n"),'mputl',fil));
    end
  else // file given by its logical unit
    opened = %f;
  end
end  

mn = size(str,'*');
for i = 1:mn
  mfprintf(fd, '%s\n',str(i));
end

if opened then mclose(fd),end

endfunction
