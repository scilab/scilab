// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA - Farid BELAHCENE
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function   bool=iscellstr(c)

// This function returns 1 if input argument is a cell of strings, else it returns 0.  
// F.B

if argn(2)<>1 then
  error(msprintf(gettext("%s: Wrong number of input arguments: %d expected"),"iscellstr",1));
else
  if isempty(c) then
    bool=%F;
  else
    if typeof(c)=="ce"
      bool=%T;
      for i=1:size(c,"*")
        if typeof(c(i).entries)<>"string" then
          bool=%F;
          break
        end
      end
    else
      bool=%F;
    end
  end
end

endfunction
