// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <!-- ENGLISH IMPOSED -->

// comp

funcprot(0);

deff("[a]=foo()","a=1","n")
comp(foo);
if foo()<>1 then pause,end
