// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - Yann COLLETTE <yann.collette@renault.com>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function Var = init_ising2d(Size, Proba)
    if (~isdef("Proba","local")) then
        Proba = 0.5;
    end
    Var = 2*(rand(Size,Size)<Proba)-1;
endfunction

