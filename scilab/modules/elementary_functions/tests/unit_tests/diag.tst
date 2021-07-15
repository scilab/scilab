// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2021 - ESI Group - Clement DAVID
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.
// ----------------------------------------------------------------------------

// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->
// <-- IMPOSED ENGLISH -->
//
// --------------------------
// Unit tests for diag()
// --------------------------

// Double
assert_checkequal(diag(1), 1);
assert_checkequal(diag([1 ; 2]), [1 0 ; 0 2]);
assert_checkequal(diag([1 ; 2 ; 3]), [1 0 0 ; 0 2 0 ; 0 0 3]);
assert_checkequal(diag(diag([1 ; 2])), [1 ; 2]);
assert_checkequal(diag(diag([1 ; 2 ; 3])), [1 ; 2 ; 3]);

assert_checkequal(diag(1, 1), [0 1 ; 0 0]);
assert_checkequal(diag([1 ; 2], 1), [0 1 0 ; 0 0 2 ; 0 0 0]);
assert_checkequal(diag([1 ; 2 ; 3], 1), [0 1 0 0 ; 0 0 2 0 ; 0 0 0 3 ; 0 0 0 0]);

// Sparse
assert_checkequal(diag(sparse(1)), sparse(1));
assert_checkequal(diag(sparse([1 ; 2])), sparse([1 0 ; 0 2]));
assert_checkequal(diag(sparse([1 ; 2 ; 3])), sparse([1 0 0 ; 0 2 0 ; 0 0 3]));
assert_checkequal(diag(sparse(diag([1 ; 2]))), sparse([1 ; 2]));
assert_checkequal(diag(sparse(diag([1 ; 2 ; 3]))), sparse([1 ; 2 ; 3]));

assert_checkequal(diag(sparse(1), 1), sparse([0 1 ; 0 0]));
assert_checkequal(diag(sparse([1 ; 2]), 1), sparse([0 1 0 ; 0 0 2 ; 0 0 0]));
assert_checkequal(diag(sparse([1 ; 2 ; 3]), 1), sparse([0 1 0 0 ; 0 0 2 0 ; 0 0 0 3 ; 0 0 0 0]));

// http://bugzilla.scilab.org/16664
diag(spzeros(2,2))