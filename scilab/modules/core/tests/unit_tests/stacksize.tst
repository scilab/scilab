// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Yann COLLETTE
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

currentstksize = stacksize();
stacksize('min');
minstksize = stacksize();
stacksize('max');
maxstksize = stacksize();
stacksize(ceil((maxstksize(1) + minstksize(1))/2));
stacksize(currentstksize(1));

// These commands should not hangs scilab
