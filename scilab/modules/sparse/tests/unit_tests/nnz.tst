// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2019 - Stéphane MOTTELET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->


a = testmatrix('magi',4);
a(a>8) = 0;
as = sparse(a);
ab = a > 0;
asb = sparse(ab);
ap = a*(1+%s)

// double (sparse and full)
assert_checkequal(nnz(as),8)
assert_checkequal(nnz(a),8)

// bool (sparse and full)
assert_checkequal(nnz(asb),8)
assert_checkequal(nnz(ab),8)

// int
assert_checkequal(nnz(uint8(a)),8)
assert_checkequal(nnz(int8(a)),8)
assert_checkequal(nnz(uint16(a)),8)
assert_checkequal(nnz(int16(a)),8)
assert_checkequal(nnz(uint32(a)),8)
assert_checkequal(nnz(int32(a)),8)
assert_checkequal(nnz(uint64(a)),8)
assert_checkequal(nnz(int64(a)),8)

// polynomial (via overloading)
assert_checkequal(nnz(ap),8)


