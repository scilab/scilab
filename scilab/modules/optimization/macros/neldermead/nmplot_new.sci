// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008-2009 - INRIA - Michael Baudin
// Copyright (C) 2009-2011 - DIGITEO - Michael Baudin
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
// nmplot_new --
//   Creates a new Nelder - Mead plot object.
//
function newobj = nmplot_new ()
    newobj = tlist(["TNMPLOT",...
    "simplexfn","fbarfn","foptfn","sigmafn","nmbase",...
    "simplexhandle","fbarhandle","fopthandle","sigmahandle"]);
    newobj.simplexfn = "";
    newobj.fbarfn = "";
    newobj.foptfn = "";
    newobj.sigmafn = "";
    newobj.simplexhandle = 0;
    newobj.fbarhandle = 0;
    newobj.fopthandle = 0;
    newobj.sigmahandle = 0;
    //
    // Create and configure parent class
    //
    newobj.nmbase = neldermead_new();
endfunction

