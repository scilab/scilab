// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008-2009 - INRIA - Michael Baudin
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt


//
// nmplot_cget --
//   Get the value for the given key.
//   If the key is unknown, generates an error.
//
function value = nmplot_cget (this,key)
  select key
  case "-simplexfn" then
    value = this.simplexfn;
  case "-fbarfn" then
    value = this.fbarfn;
  case "-foptfn" then
    value = this.foptfn;
  case "-sigmafn" then
    value = this.sigmafn;
  else
    this.nmbase = neldermead_configure ( this.nmbase , key , value )
  end
endfunction
