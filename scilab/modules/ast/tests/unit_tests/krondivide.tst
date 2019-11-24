// ============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
//
// Copyright (C) 2019 - Samuel GOUGEON
//
//  This file is distributed under the same license as the Scilab package.
// ============================================================================

// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->
//
// ---------------------------------------------
// <--  Unit test for ./. and .\. operators  -->
// ---------------------------------------------

rtol = 10*%eps;
objects = list(rand(1,1), rand(1,3), rand(4,1), rand(1,1,5), rand(2,3), rand(3,4,2));
n = length(objects);

// With arrays without zeros
// =========================
for i = 1:n
    o = objects(i);
    for j = 1:n
        o2 = objects(j);
        b = o .*. o2;
        assert_checkalmostequal(b ./. o2, o, rtol)
        assert_checkalmostequal(o .\. b, o2, rtol)
    end
end

// With a null array
// =================
z = list(zeros(1,1), zeros(1,3), zeros(4,1), zeros(1,1,5), zeros(2,3), zeros(3,4,2));
for i = 1:n
    o = objects(i);
    for j = 1:n
        o2 = z(j);
        b = o .*. o2;
        //assert_checkalmostequal(b ./. o2, o, rtol)  // => Nan | not managed
        assert_checkalmostequal(o .\. b, o2, rtol)

        b = o2 .*. o;
        assert_checkalmostequal(b ./. o, o2, rtol)
        //assert_checkalmostequal(o2 .\. b, o, rtol)  // => Nan | not managed
    end
end

// With arrays with zeros
// ======================
withZeros = list(1, 0:3, [2 0 1]', cat(3,3,0,2,0), [2 3 0 ; 1 0 2], ..
            matrix([0,3,1,2,0,2,3,0,3,3,3,2,3,1,0,0,3,3,1,0,3,3,0,0], [3,4,2]));
for i = 1:n
    o = objects(i);
    for j = 1:n
        // Without zeros .*. with zeros
        // ............................
        o2 = withZeros(j);
        b = o .*. o2;
        assert_checkalmostequal(b ./. o2, o, rtol)
        assert_checkalmostequal(o .\. b, o2, rtol)

        b = o2 .*. o;
        assert_checkalmostequal(b ./. o, o2, rtol)
        assert_checkalmostequal(o2 .\. b, o, rtol)

        // With zeros .*. with zeros
        // .........................
        o = withZeros(i);
        b = o .*. o2;
        assert_checkalmostequal(b ./. o2, o, rtol)
        assert_checkalmostequal(o .\. b, o2, rtol)

        b = o2 .*. o;
        assert_checkalmostequal(b ./. o, o2, rtol)
        assert_checkalmostequal(o2 .\. b, o, rtol)
    end
end
