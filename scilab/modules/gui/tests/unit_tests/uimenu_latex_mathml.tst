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
mlatex=uimenu(f,'label', '$\LaTeX$');
ml1=uimenu(mlatex,'label', '$\int_0^\infty\mathrm{e}^{-x^2}\,dx$');
ml2=uimenu(mlatex,'label', '$\frac\sqrt{\pi}2$');
mmathml=uimenu(f,'label', 'MathML');
mm1=uimenu(mmathml,'label', '<msup><mn>x</mn><mi>2</mi></msup>');
mm2=uimenu(mmathml,'label', '<mrow><msup><mn>a</mn><mi>2</mi></msup><mo>+</mo><msup><mn>b</mn><mi>2</mi></msup><mo>=</mo><msup><mn>c</mn><mi>2</mi></msup></mrow>');

