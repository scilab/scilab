// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2018 - ESI - Antoine ELIAS
// Copyright (C) 2018 - Stéphane MOTTELET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->

res(2,3,4) = 0;
computed = list([], 1, 10, list(2,3), list(2,3,4));
expected = list([], 0, 0, [0 0 0;0 0 0], res);

for i = 1:size(computed)
    assert_checkequal(zeros(computed(i)(:)), expected(i));
end

data = rand(4, 3, 2) * 1000;
dataz = zeros(data);

// boolean
ref = isinf(dataz);
assert_checkequal(zeros(1,0, "boolean"), []);
assert_checkequal(zeros([], "boolean"), []);
assert_checkequal(zeros(data, "boolean"), ref);
assert_checkequal(zeros(4,3,2, "boolean"), ref);
assert_checkequal(zeros(isinf(data)), dataz);

// int8
ref = int8(dataz);
assert_checkequal(zeros(1,0, "int8"), []);
assert_checkequal(zeros([], "int8"), []);
assert_checkequal(zeros(data, "int8"), ref);
assert_checkequal(zeros(4,3,2, "int8"), ref);
assert_checkequal(zeros(int8(data)), dataz);

// uint8
ref = uint8(dataz);
assert_checkequal(zeros(1,0, "uint8"), []);
assert_checkequal(zeros([], "uint8"), []);
assert_checkequal(zeros(data, "uint8"), ref);
assert_checkequal(zeros(4,3,2, "uint8"), ref);
assert_checkequal(zeros(uint8(data)), dataz);

// int16
ref = int16(dataz);
assert_checkequal(zeros(1,0, "int16"), []);
assert_checkequal(zeros([], "int16"), []);
assert_checkequal(zeros(data, "int16"), ref);
assert_checkequal(zeros(4,3,2, "int16"), ref);
assert_checkequal(zeros(int16(data)), dataz);

// uint16
ref = uint16(dataz);
assert_checkequal(zeros(1,0, "uint16"), []);
assert_checkequal(zeros([], "uint16"), []);
assert_checkequal(zeros(data, "uint16"), ref);
assert_checkequal(zeros(4,3,2, "uint16"), ref);
assert_checkequal(zeros(uint16(data)), dataz);

// int32
ref = int32(dataz);
assert_checkequal(zeros(1,0, "int32"), []);
assert_checkequal(zeros([], "int32"), []);
assert_checkequal(zeros(data, "int32"), ref);
assert_checkequal(zeros(4,3,2, "int32"), ref);
assert_checkequal(zeros(int32(data)), dataz);

// uint32
ref = uint32(dataz);
assert_checkequal(zeros(1,0, "uint32"), []);
assert_checkequal(zeros([], "uint32"), []);
assert_checkequal(zeros(data, "uint32"), ref);
assert_checkequal(zeros(4,3,2, "uint32"), ref);
assert_checkequal(zeros(uint32(data)), dataz);

// int64
ref = int64(dataz);
assert_checkequal(zeros(1,0, "int64"), []);
assert_checkequal(zeros([], "int64"), []);
assert_checkequal(zeros(data, "int64"), ref);
assert_checkequal(zeros(4,3,2, "int64"), ref);
assert_checkequal(zeros(int64(data)), dataz);

// uint64
ref = uint64(dataz);
assert_checkequal(zeros(1,0, "uint64"), []);
assert_checkequal(zeros([], "uint64"), []);
assert_checkequal(zeros(data, "uint64"), ref);
assert_checkequal(zeros(4,3,2, "uint64"), ref);
assert_checkequal(zeros(uint64(data)), dataz);

