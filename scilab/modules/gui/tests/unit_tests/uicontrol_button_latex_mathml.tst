// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - Calixte DENIZET 
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

// <-- INTERACTIVE TEST -->

f=figure();
h=uicontrol(f,"style","pushbutton","string","$\scalebox{2}{\LaTeX\ x^2}$");
h.Position = [20, 40, 120, 40];
hh=uicontrol(f,"style","pushbutton","string","<mrow><mtext>MathML</mtext><mphantom><mi>a</mi></mphantom><msup><mi>x</mi><mn>2</mn></msup></mrow>");
hh.Position = h.Position + [140, 0, 0, 0];