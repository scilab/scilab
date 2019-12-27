
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - Yann COLLETTE <yann.collette@renault.com>
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

// <-- CLI SHELL MODE -->

[Crossed_Indiv1, Crossed_Indiv2] = crossover_ga_default(1,-1,[]);

if (Crossed_Indiv1<-1) | (Crossed_Indiv1>1) | (Crossed_Indiv2<-1) | (Crossed_Indiv2>1) then pause,end
