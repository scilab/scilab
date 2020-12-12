// ============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2014 - Scilab Enterprises - Bruno JOFRET
//
//  This file is distributed under the same license as the Scilab package.
// ============================================================================

// <-- CLI SHELL MODE -->

// Test display of complex matrix, leave each line without semicolon

complex(ones(1,5), ones(1,5))

complex(ones(1,5), 2*ones(1,5))

complex(ones(1,5), zeros(1,5))

[1 + %i, 1 + 2*%i, 1 + 3*%i]

[1 + %i, 1 + 2*%i, 1 + 3*%i ; 1 + 3*%i, 1 + %i, 1+3*%i]

[1, 1+%i, 1 + 3*%i ; 1 + 3*%i, 1 + 3*%i, 1 + 3*%i]

[1, 1+%i, 1 + 3*%i ; 1 + 3*%i, 1 + %i, 1 + 3*%i]

[rand() + rand()*%i, rand()+rand()*%i;
 1+%i, 1+%i;
 1, 1+%i;
%i, 2 + 2*%i;
-2, -2 - 2*%i]

[ 1+%i, 1+%i;
 1, 1+%i;
%i, 2 + 2*%i;
-2, -2 - 2*%i]

[2+2*%i, 2+2*%i;]

[ 1+%i, 1+%i]

[1+%i, 1+%i;
1+%i, 1+%i;]

[%i, 1+%i]

[%i, 1+%i;
%i, 1+%i]