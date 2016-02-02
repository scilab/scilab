// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - INRIA - Michael Baudin
// Copyright (C) 2009-2010 - DIGITEO - Michael Baudin
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

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

