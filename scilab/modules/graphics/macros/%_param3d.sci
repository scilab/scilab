//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Jean-Baptiste Silvy
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
//
//

//
// This is the demonstration script of param3d
// used when calling param3d without any parameter.
//

function [] = %_param3d()
  t=0:0.1:5*%pi;
  param3d(sin(t),cos(t),t/10,35,45,'X@Y@Z',[2,4]);
endfunction
