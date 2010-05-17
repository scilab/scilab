// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - INRIA - Michael Baudin
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

//
// optimbase_outstruct --
//   Returns a tlist with basic optimization fields.
//   This struct may be enriched by children (specialize) optimization methods.
// Arguments
//   this : the current object
//
function  data = optimbase_outstruct ( this )
  if this.outputcommand == "" then
    errmsg = msprintf( gettext ( "%s: No output command is defined.") , "optimbase_outstruct" )
    error(errmsg)
  else
    data = tlist(["T_OPTDATA",...
      "x","fval","iteration","funccount"]);
    data.x = this.xopt;
    data.fval = this.fopt;
    data.iteration = this.iterations;
    data.funccount = this.funevals;
  end
endfunction

