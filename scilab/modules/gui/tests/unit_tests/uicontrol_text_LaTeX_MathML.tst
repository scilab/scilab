// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - Calixte DENIZET 
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

// <-- INTERACTIVE TEST -->

f=figure();
h=uicontrol(f,'Style','text','string',"$\Gamma(s)=\int_0^\infty t^{s-1}\mathrm{e}^{-t}\,\mathrm{d}t$");
h.fontsize=15;