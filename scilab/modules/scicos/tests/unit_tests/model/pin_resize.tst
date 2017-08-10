// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2016 - Scilab Enterprises - Paul Bignier
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

loadXcosLibs();

o = scicos_block();

o.graphics.pin = [0; 0; 0; 0];
assert_checkequal(size(o.graphics.pin, "*"), 4);
assert_checkequal(size(o.graphics.in_implicit, "*"), 4);

o.graphics.pin = [0; 0];
assert_checkequal(size(o.graphics.pin, "*"), 2);
assert_checkequal(size(o.graphics.in_implicit, "*"), 2);

o.graphics.pin = [0; 0; 0];
assert_checkequal(size(o.graphics.pin, "*"), 3);
assert_checkequal(size(o.graphics.in_implicit, "*"), 3);

o.graphics.pin = 0;
assert_checkequal(size(o.graphics.pin, "*"), 1);
assert_checkequal(size(o.graphics.in_implicit, "*"), 1);


// Check that all the model items are freed
clear
