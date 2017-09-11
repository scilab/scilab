// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Pierre MARECHAL <pierre.marechal@inria.fr>
// Copyright (C) 2017 - Samuel GOUGEON
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->

// Test 1
// =============================================================================
// Test 1.1
A = floor(2^52 * rand(20,20));
for i=1:size(A,"*");
    B = strcat(string(bitget(A(i), 1:52)));
    C = strrev(dec2bin(A(i),52));
    assert_checkequal(B, C);
end

// Test 1.2
for i = [1 2 4 8 11 12 14 18]               // Loop on int types
    m = 8 * modulo(i,10) - 1 + 1*(i>10);    // Maximal bit rank
    m = min(m, 52);   // due to dec2base() limitation to 2^52
    A = iconvert(floor( 2^m * rand(20,20)), i);
    for k = 1:size(A,"*");
        B = strcat(string(bitget(A(k), 1:m)));
        C = strrev(dec2bin(A(k), m));
        assert_checkequal(B, C);
    end
end

// Test 2
// =============================================================================
A0 = bin2dec(["0001" "0010";"0011" "0100"]);
B =         [    1      0 ;    1      0 ];
C =         [   0      1  ;   1      0  ];
for i = [0 1 2 4 8 11 12 14 18]
    A = iconvert(A0,i);
    assert_checktrue(and( bitget(A,1)==B));
    assert_checktrue(and( bitget(A,2)==C));
end

// Test 3: about bitmax
// =============================================================================
k = [0 1 2 4 8 11 12 14 18];
bitmax = [1024 7 15 31 63 8 16 32 64];
for i = 1:length(k)
    n = iconvert(123, k(i));
    assert_checkequal(execstr("bitget(n, bitmax(i));","errcatch"), 0);
    assert_checkfalse(execstr("bitget(n, bitmax(i)+1);" ,"errcatch")==0);
    assert_checkfalse(execstr("bitget(n, 0);" ,"errcatch")==0);
end

// Tests about input / output types
// =============================================================================
it = [0 1 2 4 8 11 12 14 18];
for i = it
    n = abs(iconvert([1 10 100 271 1000 3467 34567], i));
    for j = it
        r = bitget(n, iconvert([1:7], j));
        assert_checkequal(inttype(r), i);
    end
end

// Tests about input / output sizes
// =============================================================================
b = [1 3 4 6 7];
n = sum(2^(b-1));

// x scalar, pos matrix:
r = bitget(n, [1 2 3 ; 5 6 7]);
assert_checkequal(size(r), [2 3]);
assert_checkequal(r, [1 0 1 ; 0 1 1]);

// x matrix, pos scalar:
r = bitget(n*[1 2 4 ; 8 16 32]', 6);
assert_checkequal(size(r), [3 2]);
assert_checkequal(r, [1 0 1 ; 1 0 1]');

// x and pos matrices of identical sizes
x = cumsum(2^(b-1));
x = [x ; x];        // 2x5
pos = [1 3 5 7 9 ; 8 6 4 2 1];
r = bitget(x, pos);
assert_checkequal(size(r), [2 5]);
assert_checkequal(r, [1 1 0 0 0 ; 0 0 1 0 1]);

// x and pos are arrays with mismatching sizes
x = [39  8  4  44  52  5  6  14  64 39  12  4  62  29  12  50  39  29];
x = matrix(x,2,3,3);
//(:,:,1)
//   39.   4.    52.
//   8.    44.   5.
//(:,:,2)
//   6.    64.   12.
//   14.   39.   4.
//(:,:,3)
//   62.   12.   39.
//   29.   50.   29.
x = sum(2.^(x-1),3);
ref = [
  %nan   %nan  0.   1.
   0.    1.    0.   0.
   %nan  %nan  1.   0.
   0.    0.    0.   1.
   0.    0.    1.   1.
   1.    0.    0.   0.
];
assert_checkequal(bitget(x, [5 8 12 39]), ref);

// Bits extraction from decimal numbers > 2^52
// =============================================================================
assert_checkequal(execstr("bitget(123 , 54);","errcatch"), 0);
assert_checkequal(execstr("bitget(123 , 1000);","errcatch"), 0);

// The extracted values of lower bits below %eps must all be %nan:
assert_checktrue(and(isnan(bitget(2^70 , 1:17))));
assert_checkequal(bitget(1+2.^[51 53 ; 54 5], [2 1 ; 2 1]), [0 %nan ; %nan 1]);

// We build a random integer with known bits #0-99
// Beware of the bug http://bugzilla.scilab.org/15276
i = matrix(1:100, 10, 10);
bv = grand(10, 10, "uin", 0, 1)  // Bits values
bp = bv.*(i-1);                  // Related powers
v = sum(2 .^bp);
// We now extract all its bits with bitget()
bg = bitget(v, i);
// We select extracted bits that have a known value
s = ~isnan(bg);
// We built the related number. Lower bits are ignored:
va = sum(2 .^(bg(s).*(i(s)-1)));
// Is it equal to v?  Yes: bitget() works like a charm on huge decimal numbers!
assert_checkequal(va, v);


