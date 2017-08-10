//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Jean-Baptiste Silvy
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
//

//
// This is the demonstration script of param3d1
// used when calling param3d1 without any parameter.
//

function [] = %_param3d1()
    t=0:0.1:5*%pi;
    param3d1(sin(t),cos(t),t/10,35,45,"X@Y@Z",[2,4]);
endfunction
