// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - SE - Sylvestre Ledru
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- JIT TEST -->
//

a=2+2
a=a+3
a
a=a+2+3+23+23
a

b=2*2
b=a*3

c=2-1
c=2*3-1


for i=1:20:1, a=a+i, end
a // 59
