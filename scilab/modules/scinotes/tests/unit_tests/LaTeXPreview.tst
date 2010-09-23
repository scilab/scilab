// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - Calixte DENIZET
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

// <-- INTERACTIVE TEST -->

test = TMPDIR + "/testLaTeXPreview.sci";
mputl(["// $\sum_{n=1}^{+\infty}\frac{1}{n^2}=\frac{\pi^2}{6}$"; ..
       "xtitle(''$\huge\int_\Omega f\,\mathrm{d}\mu$'')"; ..
       ""], test);
scinotes(test);

// Move the mouse on the LaTeX formulas
