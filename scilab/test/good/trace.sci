
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function t=trace(a)
// trace - computes the trace of a matrix
  select type(a)
  case 1 then
    [m,n]=size(a)
    if m<>n then 
      error(msprintf(gettext("%s: Wrong size for input argument #%d: A square matrix expected.\n"),'trace',1));
    end
    t=sum(diag(a))
  case 2 then
    [m,n]=size(a)
    if m<>n then 
      error(msprintf(gettext("%s: Wrong size for input argument #%d: A square matrix expected.\n"),'trace',1));
    end
    t=sum(diag(a))
    //-compat next case retained for list /tlist compatibility
  case 15 then
    if a(1)=='r' then
      [m,n]=size(a)
      if m<>n then 
        error(msprintf(gettext("%s: Wrong size for input argument #%d: A square matrix expected.\n"),'trace',1));
      end
      t=sum(diag(a))
    else
      error(msprintf(gettext("%s: Wrong type for input argument #%d.\n"),'trace',1));
    end
  case 16 then
    if a(1)=='r' then
      [m,n]=size(a)
      if m<>n then 
        error(msprintf(gettext("%s: Wrong size for input argument #%d: A square matrix expected.\n"),'trace',1));
      end
      t=sum(diag(a))
    else
      error(msprintf(gettext("%s: Wrong type for input argument #%d.\n"),'trace',1));
    end   
  case 5 then
    [m,n]=size(a)
    if m<>n then 
      error(msprintf(gettext("%s: Wrong size for input argument #%d: A square matrix expected.\n"),'trace',1));
    end
    t=sum(diag(a))
  else
    error(msprintf(gettext("%s: Wrong type for input argument #%d.\n"),'trace',1));
  end
endfunction
