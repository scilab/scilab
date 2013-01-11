// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) XXXX-2008 - INRIA
// Copyright (C) 2009-2011 - DIGITEO - Allan CORNET
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt


function fprintf(fil, frmt, varargin)
// fprintf - Emulator of C language fprintf
//

  warnobsolete("mfprintf","5.5.0");

  [lhs, rhs] = argn(0);
  if rhs < 2 then
    error(999, msprintf(gettext("%s: Wrong number of input argument(s).\n"), "fprintf"));
  end

  if (type(frmt) <> 10) then
    error(999, msprintf(gettext("%s: Wrong type for input argument #%d: A String expected.\n"),"fprintf",2));
  end
  
  if size(frmt,"*")<>1 then
      error(999, msprintf(gettext("%s: Wrong size for input argument #%d: A String expected.\n"),"fprintf",2));
  end
  
  if (type(fil) == 1) then
  
    if size(fil,"*")<>1 then
      error(999, msprintf(gettext("%s: Wrong size for input argument #%d: A file descriptor expected.\n"),"fprintf",1));
    end
    
    [units,typ] = file();
    k = find( units == fil );
    if ((k == []) & (fil <> 0)) then
      error(999, msprintf(gettext("%s: Wrong value for input argument #%d: A valid file descriptor expected.\n"),"fprintf",1));
    else
      if ( typ(k) == 'F' ) then
        write(fil,sprintf(frmt,varargin(:)),'(a)')
      else // C Format
        mfprintf(fil,frmt,varargin(:))
      end
    end

  else
  
     if (type(fil) == 10) then

       if size(fil,"*")<>1 then
         error(999, msprintf(gettext("%s: Wrong size for input argument #%d: A filename expected.\n")),"fprintf",1);
       end
       
       [units, typ, filnams] = file();
       k = find( filnams == fil );
       if (k == []) then
         fd = mopen(fil,'at');
         mfprintf(fd,frmt,varargin(:))
       else
         if ( typ(k) == 'F' ) then
           write(units(k),sprintf(frmt,varargin(:)),'(a)')
         else
           mfprintf(units(k),frmt,varargin(:))
         end
         
       end
     else
       error(999, msprintf(gettext("%s: Wrong type for input argument #%d: A filename or file descriptor expected.\n"),"fprintf",1));
     end
     
  end

endfunction
