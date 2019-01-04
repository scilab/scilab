// ===================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2019 - Samuel GOUGEON
//
//  This file is distributed under the same license as the Scilab package.
// ===================================================================

// <-- ENGLISH IMPOSED -->
// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->

assert_checkequal(bitset(uint8(9), 5), uint8(25));
assert_checkequal(bitset(uint8(25), 5, 0), uint8(9));
assert_checkequal(bitset(25, 5, 0), 9);

a = [170,82,24,89,92,59,220,141];
assert_checkequal(bitset(a, ones(1:8)), [171,83,25,89,93,59,221,141]);

// x matrix ; pos scalar
assert_checkequal(bitset([0 1;2 3], 3), [4 5;6 7]);
// x matrix ; pos = same sizes
assert_checkequal(bitset([0 1;2 3], [3 4;5 6]), [4 9; 18 35]);

// =====================
// Element-wise settings
// =====================
dataTypes = list(double, int8, uint8, int16, uint16, int32, uint32, int64, uint64);
shifts = [1000, 1, 1, 9, 9, 25, 25, 57, 57];

// With default bits values v = ones()
// ===================================
// Only one bit
// ------------
pos = cat(3, [0 1 2 ; 3 2 1], [1 3 1 ; 2 1 3]);
X = zeros(2, 3, 2);
for i = 1:9         // Loop over inttypes
    iconv = dataTypes(i);
    x = iconv(X);
    s = shifts(i); sm1 = s - 1;
    // on a row
    y = bitset(x(:)', s+pos(:)');
    assert_checkequal(y/(2^sm1), iconv(2.^pos(:)'));
    // on a column
    y = bitset(x(:), s+pos(:));
    assert_checkequal(y/(2^sm1), iconv(2.^pos(:)));
    // on a matrix
    y = bitset(x(:,:,1), s+pos(:,:,1));
    assert_checkequal(y/(2^sm1), iconv(2.^pos(:,:,1)));
    // on a hypermatrix
    y = bitset(x, s+pos);
    assert_checkequal(y/(2^sm1), iconv(2.^pos));
end

// Several bits
// ------------
pos = matrix(permute(grand(12,"prm",0:6)(:,1:3),[1 4 3 2]), 2,3,2,-1);
// 12 series of 3 random bits pos among 0:6 with no duplicate in triplets
X = zeros(2,3,2);
for i = 1:9         // Loop over inttypes
    iconv = dataTypes(i);
    x = iconv(X);
    s = shifts(i); sm1 = s - 1;
    // on a scalar
    p = [3 2 5 1];
    y = bitset(x(1), s+p);
    assert_checkequal(y/(2^sm1), iconv(sum(2.^p)));
    // on a row
    p = matrix(pos,1,12,-1);
    y = bitset(x(:)', s+p);
    assert_checkequal(y/(2^sm1), iconv(sum(2.^p, 3)));
    // on a column
    p = matrix(pos,12,1,-1);
    y = bitset(x(:), s+p);
    assert_checkequal(y/(2^sm1), iconv(sum(2.^p, 3)));
    // on a matrix
    p = matrix(pos(:,:,1,:),2,3,-1);
    y = bitset(x(:,:,1), s+p);
    assert_checkequal(y/(2^sm1), iconv(sum(2.^p, 3)));
    // on a hypermatrix
    y = bitset(x, s+pos);
    assert_checkequal(y/(2^sm1), iconv(sum(2.^pos, 4)));
end

// ===========================
// With explicit bits values v
// ===========================
X = matrix([grand(1,3,"uin",0,9) grand(1,9,"uin",10,100)],2,3,2);

// Only one bit assigned per input target
// --------------------------------------
pos = cat(3, [0 1 2 ; 3 2 1], [1 3 1 ; 2 1 3])+1;
B = matrix(bitget(X(:)', pos(:)'), 2,3,2);      // current bits values
for v = list(0, 1, grand(2,3,2,"uin",0,1))  // new bits values to assign
    // Preparing the expected answer
    rep = X;    // Initialization, OK where B==V
    if v==0
        w = B==1; if or(w), rep(w) = X(w) - 2.^(pos(w)-1); end
    elseif v==1
        w = B==0; if or(w), rep(w) = X(w) + 2.^(pos(w)-1); end
    else
        w = B>v; if or(w), rep(w) = X(w) - 2.^(pos(w)-1); end
        w = B<v; if or(w), rep(w) = X(w) + 2.^(pos(w)-1); end
    end
    // Tests
    for i = 1:9         // Loop over inttypes
        iconv = dataTypes(i);
        x = iconv(X);
        // on a row
        y = bitset(x(:)', pos(:)', v(:)');
        assert_checkequal(y, iconv(rep(:)'));
        // on a column
        y = bitset(x(:), pos(:), v(:));
        assert_checkequal(y, iconv(rep(:)));
        // on a matrix
        y = bitset(x(:,:,1), pos(:,:,1), v(:,:,1));
        assert_checkequal(y, iconv(rep(:,:,1)));
        // on a hypermatrix
        y = bitset(x, pos, v);
        assert_checkequal(y, iconv(rep));
    end
end
// Several bits assigned per input
// -------------------------------
n = 3;
pos = matrix(permute(grand(12,"prm",0:3)(:,1:n),[1 4 3 2]), 2,3,2,-1)+1;
// 12 series of n random bits pos among 0:3 with no duplicate in triplets
V = grand(2,3,2,n,"uin",0,1);   // New bits values to assign
B = zeros(pos);                 // current bits values:
// Computing the expected answer
rep = X;
for dim = 1:n
    p = pos(:,:,:,dim);
    b = bitget(X, p);
    v = V(:,:,:,dim);
    w = b>v; if or(w), rep(w) = rep(w) - 2.^(p(w)-1); end
    w = b<v; if or(w), rep(w) = rep(w) + 2.^(p(w)-1); end
end
// Tests
for i = 1:9         // Loop over inttypes
    iconv = dataTypes(i);
    x = iconv(X);
    // on a row
    y = bitset(x(:)', matrix(pos,1,-1,n), matrix(V,1,-1,n));
    assert_checkequal(y, iconv(rep(:)'));
    // on a column
    y = bitset(x(:), matrix(pos,-1,1,n), matrix(V,-1,1,n));
    assert_checkequal(y, iconv(rep(:)));
    // on a matrix
    y = bitset(x(:,:,1), matrix(pos(:,:,1,:),2,3,-1), matrix(V(:,:,1,:),2,3,-1));
    assert_checkequal(y, iconv(rep(:,:,1)));
    // on a hypermatrix
    y = bitset(x, pos, V);
    assert_checkequal(y, iconv(rep));
end


// ===============
// Errors handling
// ===============
msg = "bitset: Wrong number of input arguments: 2 or 3 expected.";
assert_checkerror("bitset()", msg);
assert_checkerror("bitset(1)", msg);
assert_checkerror("bitset(1,1,1,1)", "Wrong number of input arguments.");

msg = "bitset: Argument #1: Non-negative real integers expected.";
assert_checkerror("bitset(%z, 1)", msg);
assert_checkerror("bitset(%t, 1)", msg);
assert_checkerror("bitset(-1, 1)", msg);
assert_checkerror("bitset(int8(-30), 1)", msg);
assert_checkerror("bitset(1+0*%i, 1)", msg);
assert_checkerror("bitset(1.5, 1)", msg);

msg = "bitset: Argument #2: integers > 0 expected.";
assert_checkerror("bitset([10 20], %t)", msg);
assert_checkerror("bitset([10 20], 1+0*%z)", msg);
assert_checkerror("bitset([10 20], 1+0*%i)", msg);
assert_checkerror("bitset([10 20], 0)", msg);
assert_checkerror("bitset([10 20], 0.5)", msg);
assert_checkerror("bitset([10 20], 1.5)", msg);

msg = "bitset: Argument #2: Integers <= %d expected.\n";
assert_checkerror("bitset([10 20], 1200)", msprintf(msg, 1024));
assert_checkerror("bitset(  int8([10 20]),  8)", msprintf(msg, 7));
assert_checkerror("bitset( uint8([10 20]),  9)", msprintf(msg, 8));
assert_checkerror("bitset(uint16([10 20]), 17)", msprintf(msg, 16));
assert_checkerror("bitset( int16([10 20]), 16)", msprintf(msg, 15));
assert_checkerror("bitset( int32([10 20]), 32)", msprintf(msg, 31));
assert_checkerror("bitset(uint32([10 20]), 33)", msprintf(msg, 32));
assert_checkerror("bitset(uint64([10 20]), 65)", msprintf(msg, 64));
assert_checkerror("bitset( int64([10 20]), 64)", msprintf(msg, 63));

msg = "bitset: Arguments #1 and #2: Incompatible sizes.";
assert_checkerror("bitset([10 20], [1 3 ; 2 4])", msg);
assert_checkerror("bitset([1 2 3 4], [1 3 ; 1 2])", msg);

msg = "bitset: Argument #2 is longer than 7. Please check against duplicate values.";
assert_checkerror("bitset(int8([10 20]), [1 2 2 3 4 4 5 6 6], [1 0 0 0 1 1 0 1 0])", msg);

msg = "bitset: Argument #3: Must be in the set {0,1}.";
assert_checkerror("bitset([10 20], 3, 2)", msg);
assert_checkerror("bitset([10 20], 3, 0.5)", msg);
assert_checkerror("bitset([10 20], 3, -3)", msg);

msg = "bitset: Arguments #2 and #3: Incompatible sizes.";
assert_checkerror("bitset([10 20], 3, [1 0])", msg);
assert_checkerror("bitset([10 20], [3 6], [0 1 1])", msg);
