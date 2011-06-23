// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008-2009 - INRIA - Michael Baudin
// Copyright (C) 2010 - DIGITEO - Allan CORNET
// Copyright (C) 2011 - DIGITEO - Michael Baudin
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function neldermead_summary(nm)
function str = _strvec ( x )
    str = strcat(string(x)," ")
endfunction

  xopt = neldermead_get(nm,"-xopt")
  fopt = neldermead_get(nm,"-fopt")
  funevals = neldermead_get(nm,"-funevals")
  iter = neldermead_get(nm,"-iterations")
  status = neldermead_get(nm,"-status")
  restartnb = neldermead_get(nm,"-restartnb")
  mprintf("Xopt: [%s]\n",_strvec(xopt));
  mprintf("Fopt: %s\n",string(fopt));
  mprintf("Function evaluations: %d\n",funevals);
  mprintf("Iterations: %d\n",iter);
  mprintf("Status: %s\n",status);
  mprintf("Restartnb: %d\n",restartnb)
endfunction 
