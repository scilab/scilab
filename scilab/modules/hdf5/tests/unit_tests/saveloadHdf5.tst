//<-- CLI SHELL MODE -->
// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2014 - Scilab Enterprises - Vladislav TRUBKIN
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//

// check function
function checkValue(value)
    clear "varName";
    varName = value;
    fileName = TMPDIR + "/saveloadHdf5.sod";
    // export to hdf5
    oldVarName = varName;
    save(fileName, "varName");
    // check that the "varName" has not been modified by export
    assert_checkequal(oldVarName, varName);
    // reset ref value
    clear "varName" "oldVarName";
    assert_checkequal(exists("varName"), 0);
    // import from hdf5
    load(fileName);
    assert_checkequal(varName, value);
endfunction

//// Empty matrix
checkValue([]);

//// Double
// scalar
checkValue(77);
// vector
checkValue([1, 2, 3, 4, 5]);
checkValue([1; 2; 3; 4; 5]);
// matrix
checkValue([1, 2, 3; 4, 5, 6]);

//// Double complex
// scalar
checkValue(1 + %i*2);
// vector
checkValue([1 + %i, 2 + %i, 3 + %i*2, 4 + %i*3, 5 + %i*4]);
checkValue([1 + %i; 2 + %i; 3 + %i*2; 4 + %i*3; 5 + %i*4]);
// matrix
checkValue([1 + %i, 2 + %i, 3 + %i*2; 4 + %i*3, 5 + %i*4, 6 + %i]);

//// String
// single
checkValue("Single String");
// vector
checkValue(["a", "b", "c"]);
checkValue(["a"; "b"; "c"]);
// matrix
checkValue(["a", "b", "c"; "d", "e", "f"]);

//// Polynomials
s = poly(0, "s");
// single
checkValue(poly([1, 2], "s", "c"));
// vector
checkValue([s, s^2, 1 + 3*s^2, 1 + 2*s + 4*s^3]);
checkValue([s; s^2; 1 + 3*s^2; 1 + 2*s + 4*s^3]);
// matrix
checkValue([s, s^2; 1 + 3*s^2, 1 + 2*s + 4*s^3]);

//// Boolean
// single
checkValue(%t);
// vector
checkValue([%t, %t, %f, %t, %t]);
checkValue([%t; %t; %f; %t; %t]);
// matrix
checkValue([%t, %t; %f, %f]);

//// Integer
clear "createIntValues";
function intValues = createIntValues(value, flag)
    select flag
    case 8 // int8
        intValues = int8(value);
        break;
    case -8 // uint8
        intValues = uint8(value);
        break;
    case 16
        intValues = int16(value);
        break;
    case -16
        intValues = uint16(value);
        break;
    case 32
        intValues = int32(value);
        break;
    case -32
        intValues = uint32(value);
        break;
    end
endfunction

clear "checkIntValue";
function checkIntValue(refValue, flag)
    for i = 1:6
        checkValue(createIntValues(refValue, flag(i)));
    end
endfunction

// all flags for createIntValues
flag = [8, -8, 16, -16, 32, -32];
// scalar
checkIntValue(5, flag);
// vector
checkIntValue([1, 2, 3, 4, 5], flag);
checkIntValue([1, 2, 3, 4, 5]', flag);
// matrix
checkIntValue([1, 2, 3; 4, 5, 6], flag);

//// Sparse
checkValue(sparse([1, 2; 4, 5; 3, 10], [1, 2, 3]));
checkValue(sparse([1, 2; 4, 5; 3, 10], [1 + %i, 2 + 2*%i, 3 + 3*%i]));

//// Boolean sparse
valueRef = [%F, %F, %T, %F, %F
%T, %F, %F, %F, %F
%F, %F, %F, %F, %F
%F, %F, %F, %F, %T];
checkValue(sparse(valueRef));
checkValue(sparse([1, 1; 2, 2; 3, 3; 4, 4], [%t, %t, %t, %t]));

//// List
listNew = list();
// empty list
checkValue(listNew);
// double in list
listNew(1) = 111;
listNew(2) = [1, 2, 3];
listNew(3) = [1, 2, 3]';
// string in list
listNew(4) = ["a", "b"; "c", "d"];
// double complex in list
listNew(5) = [complex(1, 2), complex(1, 3); complex(1, 4), complex(1, 5)];
// boolean in list
listNew(6) = [%t, %t, %t; %f, %f, %f];
// integer in list
valueRef = ones(4, 3);
for i = 7:12
    listNew(i) = createIntValues(valueRef, flag(i - 6));
end
// polynom in list
listNew(13) = [s, s^2; 1 + 3*s^2, 1 + 2*s + 4*s^3];
// empty matrix in list
listNew(14) = [];
checkValue(listNew);
// list in list
listNew = list();
listNew(1) = 111;
listNew(2) = list([1, 2, 3]);
valueRef = ones(4, 3);
for i = 3:8
    listNew(i) = list(createIntValues(valueRef, flag(i - 2)));
end
listNew(9) = list(string([1, 2, 3; 4, 5, 6]));
listNew(10) = list([%f, %f; %t, %t]);
checkValue(listNew);
// nested list
listNew2 = list(listNew, listNew, list("Scilab", 7, %t));
checkValue(listNew2);
// tlist and mlist in the list
tlstRef = tlist(["random numbers"; "Name"; "Example"; "Example_2"], "Uniform", [%f, %t; %f, %t], [1, 2; 3, 4]);
mlstRef = mlist(["V", "name", "value"], ["a", "b"; "c", "d"], [1, 2; 3, 4]);
listNew = list("only one mlist", mlstRef);
checkValue(listNew);
listNew = list("only one tlist", tlstRef);
checkValue(listNew);
listNew = list(["mlist", "and", "tlist"], mlstRef, tlstRef);
checkValue(listNew);
// sparse in the list
listNew = list(sparse([1, 2; 4, 5; 3, 10], [1, 2, 3]), ...
sparse([1, 1; 2, 2; 3, 3; 4, 4], [%t, %t, %t, %t]), ...
sparse([1, 2; 4, 5; 3, 10], [1 + %i, 2 + 2*%i, 3 + 3*%i]));
//// Tlist
valueRef = ones(2, 2);
lstRef = tlist(["TLIST";
"String";
"Double";
"DoubleCplx";
"Int";
"Bool"], ["A", "B"; "C", "D"], valueRef, ...
complex(valueRef, 2), createIntValues(valueRef, 16), ...
[%f, %t; %f, %t]);
checkValue(lstRef);
// hypermatrix in tlist
lstRef = tlist(["hmInTlist";
"Name";
"Example_1"
"Example_2"], "List", ones(1, 2, 3), matrix((1:8) == 0, [2 2 1 2]));
checkValue(lstRef);
// tlist in tlist
lstRef = tlist(["tlistInTlist";
"Name";
"Tlist";
"Poly";
"List"], "List", lstRef, poly([1, 2], "s", "c"), list(1, %t));
checkValue(lstRef);

//// Mlist
lstRef = mlist(["MLIST", "Name", "Value"], ["a", "b"; "c", "d"], [1, 2; 3, 4]);
checkValue(lstRef);
lstRef = mlist(["MLIST";
"String";
"Double";
"DoubleCplx";
"Int";
"Bool"], ["A", "B"; "C", "D"], valueRef, ...
complex(valueRef, 2), createIntValues(valueRef, 8), ...
[%f, %t; %f, %t]);
checkValue(lstRef);
// hypermatrix in mlist
lstRef = mlist(["hmInMlist";
"Name";
"Example_1"
"Example_2"], "List", ones(1, 2, 3), matrix((1:8) == 0, [2 2 1 2]));
checkValue(lstRef);

//// Hypermatrix
// double
checkValue(ones(1, 2, 3, 4));
checkValue(complex(ones(1, 2, 3, 4), 2));
checkValue(rand(3, 3));
// integer
for i = flag
    checkValue(createIntValues(ones(1, 2, 3, 4), i));
end
// boolean
checkValue(matrix((1:8) == 0, [2 2 1 2]));

//// Undefined
valueRef = list(1, "two", "three");
valueRef(5) = "five";
valueRef(7) = 7;
checkValue(valueRef);

//// Void
l = list(1, , 3);

// l == l return [%t %f %t],
// we can't use assertcheck_equal(computed, expected)
fileName = TMPDIR + "/saveloadHdf5.sod";
// export to hdf5
old_l = l;
save(fileName, "l");
// check that the "l" has not been modified by export
assert_checkequal(old_l == l, [%t %f %t]);
// reset ref value
clear l old_l
assert_checktrue(exists("l")==0);
// import from hdf5
load(fileName);
assert_checkequal(l == list(1, , 3), [%t %f %t]);

// Struct
data.data.data = 0;
data.data.string = "Scilab";
data.list = list(1,["S" "E"]);
checkValue(data);

data2.data.data = 42;
data2.data.string = "Test";
data2.list = list(1,["a" "b"]);
checkValue(data2);

struct_ = [data, data, data2; data2, data2, data];
checkValue(struct_);

emptyStruct = struct();
checkValue(emptyStruct);

// Cell
Cell_ = cell(2,2,2);
Cell_{1} = "scilab";
Cell_{2} = 42;
Cell_{3} = list(1,["a" "b"]);
Cell_{4} = int8(2);
Cell_{5} = 5;
Cell_{6} = 6;
Cell_{7} = poly(1:3,"s");
Cell_{8} = "Scilab 6";
checkValue(Cell_);

emptyCell = cell();
checkValue(emptyCell);
