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

test = TMPDIR + "/testLaTeXPreview.sci";
mputl(["// $\sum_{n=1}^{+\infty}\frac{1}{n^2}=\frac{\pi^2}{6}$"; ..
       "xtitle(''$\huge\int_\Omega f\,\mathrm{d}\mu$'')"; ..
       ""], test);
scinotes(test);

// Move the mouse on the LaTeX formulas
