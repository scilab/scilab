
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - Yann COLLETTE <yann.collette@renault.com>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

// <-- CLI SHELL MODE -->

[Crossed_Indiv1, Crossed_Indiv2] = crossover_ga_binary('11111111','00000000',[]);

if (length(Crossed_Indiv1)~=8) | (length(Crossed_Indiv2)~=8) then pause,end
