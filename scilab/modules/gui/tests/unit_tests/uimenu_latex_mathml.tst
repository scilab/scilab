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
mlatex=uimenu(f,'label', '$\LaTeX$');
ml1=uimenu(mlatex,'label', '$\int_0^\infty\mathrm{e}^{-x^2}\,dx$');
ml2=uimenu(mlatex,'label', '$\frac\sqrt{\pi}2$');
mmathml=uimenu(f,'label', 'MathML');
mm1=uimenu(mmathml,'label', '<msup><mn>x</mn><mi>2</mi></msup>');
mm2=uimenu(mmathml,'label', '<mrow><msup><mn>a</mn><mi>2</mi></msup><mo>+</mo><msup><mn>b</mn><mi>2</mi></msup><mo>=</mo><msup><mn>c</mn><mi>2</mi></msup></mrow>');

