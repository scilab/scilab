// ============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2014 - Scilab Enterprises - Antoine ELIAS
//
//  This file is distributed under the same license as the Scilab package.
// ============================================================================

// <-- CLI SHELL MODE -->

function ret = bool(b)
    ret = (b == 1);
endfunction
types = list(int8, uint8, int16, uint16, int32, uint32, int64, uint64, double, bool);


ref =[  "int8","uint8","int16","uint16","int32","uint32","int64","uint64","int8","int8"; ...
        "uint8","uint8","uint16","uint16","uint32","uint32","uint64","uint64","uint8","uint8"; ...
        "int16","uint16","int16","uint16","int32","uint32","int64","uint64","int16","int16"; ...
        "uint16","uint16","uint16","uint16","uint32","uint32","uint64","uint64","uint16","uint16"; ...
        "int32","uint32","int32","uint32","int32","uint32","int64","uint64","int32","int32"; ...
        "uint32","uint32","uint32","uint32","uint32","uint32","uint64","uint64","uint32","uint32"; ...
        "int64","uint64","int64","uint64","int64","uint64","int64","uint64","int64","int64"; ...
        "uint64","uint64","uint64","uint64","uint64","uint64","uint64","uint64","uint64","uint64"; ...
        "int8","uint8","int16","uint16","int32","uint32","int64","uint64","constant","constant"; ...
        "int8","uint8","int16","uint16","int32","uint32","int64","uint64","constant","boolean"];

typesize = size(types);

//Scalar + Scalar
for i = 1 : typesize
    for j = 1 : typesize
        ScalarScalarComputed(i,j) = typeof(types(i)(1) + types(j)(1));
    end
end

assert_checkequal(ref, ScalarScalarComputed);

//Scalar + Matrix
for i = 1 : typesize
    for j = 1 : typesize
        ScalarMatrixComputed(i,j) = typeof(types(i)(1) + types(j)([1 2]));
    end
end

assert_checkequal(ref, ScalarMatrixComputed);

//Matrix + Scalar
for i = 1 : typesize
    for j = 1 : typesize
        MatrixScalarComputed(i,j) = typeof(types(i)([1 2]) + types(j)(1));
    end
end

assert_checkequal(ref, MatrixScalarComputed);

//Matrix + Matrix
for i = 1 : typesize
    for j = 1 : typesize
        MatrixMatrixComputed(i,j) = typeof(types(i)([1 2]) + types(j)([1 2]));
    end
end

assert_checkequal(ref, MatrixMatrixComputed);
