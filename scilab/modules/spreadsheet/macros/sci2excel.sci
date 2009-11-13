
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function sci2excel(var, fname, sep, dec)
  warnobsolete("write_csv", "5.3");
  
  if exists('sep','local') == 0 then
    sep = ascii(9);
  end
  
  if exists('dec','local') == 0 then
    dec = ',';
  end
  
  write_csv(var, fname, sep, dec);
endfunction
