// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - Calixte DENIZET 
// 
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

// <-- INTERACTIVE TEST -->

f=figure();
h=uicontrol(f,"style","pushbutton","string","$\scalebox{2}{\LaTeX\ x^2}$");
h.Position = [20, 40, 120, 40];
hh=uicontrol(f,"style","pushbutton","string","<mrow><mtext>MathML</mtext><mphantom><mi>a</mi></mphantom><msup><mi>x</mi><mn>2</mn></msup></mrow>");
hh.Position = h.Position + [140, 0, 0, 0];