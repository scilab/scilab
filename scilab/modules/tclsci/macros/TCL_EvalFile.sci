// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008-2008 - INRIA - Bruno JOFRET
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
//
//
function result=TCL_EvalFile(TCLfile, interp)
  rhs=argn(2);
select rhs
 case 1
  result=TCL_EvalStr("source "+TCLfile)
 case 2
  result=TCL_EvalStr("source "+TCLfile, interp)
else
  error(msprintf(_("%s: Wrong number of input argument(s)"),"TCL_EvalFile"));
end
endfunction