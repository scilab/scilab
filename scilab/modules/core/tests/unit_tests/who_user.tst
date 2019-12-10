// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2018 - Samuel GOUGEON
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->
// who_user

clear
a    = issparse(1);
%expo = 2;              // starting like the predefined %e
v = who_user(%f);
assert_checkequal(v,["%expo" "a" "issparse"]');
