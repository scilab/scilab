// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Michael Baudin
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
// optimsimplex_sort --
//   Sorts the simplex with increasing function value order so that
//   the smallest function value is at vertex #1.
// Arguments
//   <no arg>
//
function this = optimsimplex_sort ( this )
    [this.fv,is] = gsort(this.fv,"r","i");
    this.x = this.x ( is , : );
endfunction

