//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Jean-Baptiste Silvy
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
//
//

//
// This is the demonstration script of champ
// used when calling champ without any parameter.
//

function [] = %_champ()
    champ(1:10,1:10,rand(10,10),rand(10,10),1.0);
endfunction
