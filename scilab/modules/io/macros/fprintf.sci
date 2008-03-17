// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) XXXX-2008 - INRIA
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt


function fprintf(fil,frmt,varargin)
// fprintf - Emulator of C language fprintf
//!
if type(fil)==1 then
  [units,typ] =file()
  k=find(units==fil)
  if k==[] then
    write(fil,sprintf(frmt,varargin(:)),'(a)')
  elseif typ(k)=='F' then
    write(fil,sprintf(frmt,varargin(:)),'(a)')
  else
    mfprintf(fil,frmt,varargin(:))
  end
else
  write(fil,sprintf(frmt,varargin(:)),'(a)')
end
endfunction
