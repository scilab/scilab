// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// stacksize

a = stacksize();
a = a(1);
stacksize(a+1000);
y =stacksize();

if y(1)<>a+1000 then pause,end
