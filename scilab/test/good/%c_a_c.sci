// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// Copyright (C) DIGITEO - 2009 - Allan CORNET
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function r=%c_a_c(a,b)
// overload string added to a string
// see strops.f

  if (type(a)<> 10 | type(b)<> 10) then
    if ( type(a)<> 10 ) then
      error(msprintf(_("%s: Wrong type for input argument #%d: String expected.\n"),"%%c_a_c",1));
    end

    if ( type(b)<> 10 ) then
      error(msprintf(_("%s: Wrong type for input argument #%d: String expected.\n"),"%%c_a_c",2));
    end
  end

  if size(b,'*') == 1 then
    b = b(ones(a));
  elseif size(a,'*') == 1 then
    a = a(ones(b));
  end

  r = a + b;
endfunction
