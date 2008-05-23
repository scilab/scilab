// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2005 - INRIA - Allan CORNET 
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function []=wavwrite(y,Fs,nbits,wavefile)
  lhs=argn(1);
  rhs=argn(2);
  
  Fs_pref = 22050;
  nbits_pref = 16;
   
  if rhs<2|rhs>4 then
    error(msprintf(gettext("%s: Wrong number of input arguments: %d to %d expected.\n"),'wavwrite',2,3));
  elseif rhs<3 then
     wavefile = Fs;
     Fs = Fs_pref;
     nbits = nbits_pref;
  elseif rhs<4 then
     wavefile = nbits;
     nbits = nbits_pref;
  end
  
  if (nbits<>16) then
    error(msprintf(gettext("%s: An error occurred: %s\n"),'wavwrite',gettext('only 16 bits for the encoding.')));
  end
  
  savewave(wavefile,y,Fs);
  
endfunction
//----------------------------------------------------------
