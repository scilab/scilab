// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008-2009 - INRIA - Michael Baudin
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

//
// nmplot_configure --
//   Configure nmplot and returns the modified object.
//
function this = nmplot_configure (this,key,value)
  select key
  case "-simplexfn" then
    this.simplexfn = value;
  case "-fbarfn" then
    this.fbarfn = value;
  case "-foptfn" then
    this.foptfn = value;
  case "-sigmafn" then
    this.sigmafn = value;
  case "-outputcommand" then
    errmsg = msprintf(gettext("%s: Unknown key %s"), "nmplot_configure", key)
    error(errmsg)
  else
    this.nmbase = neldermead_configure ( this.nmbase , key , value )
  end
endfunction
