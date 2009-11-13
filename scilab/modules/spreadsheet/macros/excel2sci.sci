
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function mat = excel2sci(fname, sep)
  warnobsolete("read_csv", "5.3");
  
  if argn(2)<2 then 
    sep=',';
  end
  
  mat = read_csv(fname, sep);
endfunction
