// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - INRIA - Michael Baudin
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

//
// nmplot_display --
//   Display a Nelder-Mead engine
// Arguments
//
function this = nmplot_display ( this )
  mprintf("Nelder-Mead Plot Object\n");
  this.nmbase = neldermead_display ( this.nmbase )
  mprintf("Simplex data file : %s\n", this.simplexfn);
  mprintf("Fbar data file : %s\n", this.fbarfn);
  mprintf("Fopt data file : %s\n", this.foptfn);
  mprintf("Sigma data file : %s\n", this.sigmafn);
endfunction

