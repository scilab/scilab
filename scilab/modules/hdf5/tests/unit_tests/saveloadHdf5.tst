// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2014 - Scilab Enterprises - Vladislav TRUBKIN
// Copyright (C) 2018 - Stéphane MOTTELET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->
//
// check function
function varNames=checkSaveLoad(varargin)
    varNames=list();
    oldVarNames=list();
    for i=1:length(varargin)
      varNames(i) = msprintf("%s%d",strsubst(typeof(varargin(i))," ","_"),i);
      execstr(msprintf("clear %s; %s = varargin(%d)",varNames(i),varNames(i),i));
      oldVarNames(i) = "old"+varNames(i);
    end
    for i=1:length(varargin)
      execstr(oldVarNames(i) + '=' + varNames(i));
    end
    fileNamea = TMPDIR + "/saveloadHdf5a.sod";
    fileNameb = TMPDIR + "/saveloadHdf5b.sod";
    // export to hdf5
    for i=1:length(varargin)
      execstr(oldVarNames(i) + '=' + varNames(i));
    end
    save(fileNamea, varNames(:));
    save(fileNameb, list2vec(varNames));
    // check that the "varName" has not been modified by export
    for i=1:length(varargin)
      execstr(msprintf("assert_checkequal(%s,%s)",varNames(i),oldVarNames(i)));
    end
    // reset ref value
    clear(list2vec(varNames))
    clear(list2vec(oldVarNames))
    for i=1:length(varargin)
      assert_checkequal(exists(varNames(i)), 0);
    end
    // import from hdf5
    load(fileNamea);
    for i=1:length(varargin)
      execstr(msprintf("assert_checkequal(%s,varargin(%d))",varNames(i),i));
    end
    clear(list2vec(varNames))
    load(fileNameb);
    for i=1:length(varargin)
      execstr(msprintf("assert_checkequal(%s,varargin(%d))",varNames(i),i));
    end
endfunction

mat1 = 1;
mat2 = 1:27;
mat3 = mat2';
mat4 = matrix(mat2,[9 3]);
mat5 = matrix(mat2,[3 1 3 3]);
//// Empty matrix
checkSaveLoad([]);
//// Double
checkSaveLoad(mat1); // scalar
checkSaveLoad(mat2); // vector
checkSaveLoad(mat3); // vector
checkSaveLoad(mat4); // matrix
checkSaveLoad(mat5); // hypermatrix
checkSaveLoad([],mat1,mat2,mat3,mat4,mat5); // all together

//// Double complex
checkSaveLoad(mat1+%i); // scalar
checkSaveLoad(mat2+%i); // vector
checkSaveLoad(mat3+%i); // vector
checkSaveLoad(mat4+%i); // matrix
checkSaveLoad(mat5+%i); // hypermatrix
checkSaveLoad(mat1+%i,mat2+%i,mat3+%i,mat4+%i,mat5+%i); // all together

//// String
checkSaveLoad(string(mat1)); // scalar
checkSaveLoad(string(mat2)); //vector
checkSaveLoad(string(mat3)); // vector
checkSaveLoad(string(mat4)); // matrix
checkSaveLoad(string(mat5)); // hypermatrix
checkSaveLoad(string(mat1),string(mat2),string(mat3),string(mat4),string(mat5)); // all together

//// Polynomials
p1 = poly(1:3, "s","coeff");
p2 = repmat(p1,8,1);
p3 = repmat(p1,1,8);
p4 = repmat(p1,2,4);
p5 = repmat(p1,2,2,2);
checkSaveLoad(p1); // single
checkSaveLoad(p2); // vector
checkSaveLoad(p3); // vector
checkSaveLoad(p4); // matrix
checkSaveLoad(p5); // hypermatrix
checkSaveLoad(p1,p2,p3,p4,p5); // all together

//// Boolean
bool1 = %t;
bool2 = modulo(1:27,2)>0;
bool3 = bool2';
bool4 = matrix(bool2,[9 3]);
bool5 = matrix(bool2,[3 1 3 3]);
checkSaveLoad(bool1); // scalar
checkSaveLoad(bool2); // vector
checkSaveLoad(bool3); // vector
checkSaveLoad(bool4); // boolrix
checkSaveLoad(bool5); // hyperboolrix
checkSaveLoad(bool1,bool2,bool3,bool4,bool5); // all together

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
    values=list();
    for i = 1:6
     values(i)=createIntValues(refValue, flag(i));
     checkSaveLoad(values) // individual int types
    end
    checkSaveLoad(values(:)); // all together
endfunction

// all flags for createIntValues
flag = [8, -8, 16, -16, 32, -32];
// scalar
checkIntValue(mat1, flag); // vector
checkIntValue(mat2, flag); //vector
checkIntValue(mat3, flag); // matrix
checkIntValue(mat4, flag); // hypermatrix

//// Sparse
checkSaveLoad(sparse([1, 2; 4, 5; 3, 10], [1, 2, 3]));
checkSaveLoad(sparse([1, 2; 4, 5; 3, 10], [1 + %i, 2 + 2*%i, 3 + 3*%i]));

//// Boolean sparse
valueRef = [%F, %F, %T, %F, %F
%T, %F, %F, %F, %F
%F, %F, %F, %F, %F
%F, %F, %F, %F, %T];
checkSaveLoad(sparse(valueRef));
checkSaveLoad(sparse([1, 1; 2, 2; 3, 3; 4, 4], [%t, %t, %t, %t]));

//// List
listNew = list();
// empty list
checkSaveLoad(listNew);
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
listNew(13) = [%s, %s^2; 1 + 3*%s^2, 1 + 2*%s + 4*%s^3];
// empty matrix in list
listNew(14) = [];
checkSaveLoad(listNew);
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
checkSaveLoad(listNew);
// nested list
listNew2 = list(listNew, listNew, list("Scilab", 7, %t));
checkSaveLoad(listNew2);
// tlist and mlist in the list
tlstRef = tlist(["random numbers"; "Name"; "Example"; "Example_2"], "Uniform", [%f, %t; %f, %t], [1, 2; 3, 4]);
mlstRef = mlist(["V", "name", "value"], ["a", "b"; "c", "d"], [1, 2; 3, 4]);
listNew = list("only one mlist", mlstRef);
checkSaveLoad(listNew);
listNew = list("only one tlist", tlstRef);
checkSaveLoad(listNew);
listNew = list(["mlist", "and", "tlist"], mlstRef, tlstRef);
checkSaveLoad(listNew);
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
checkSaveLoad(lstRef);
// hypermatrix in tlist
lstRef = tlist(["hmInTlist";
"Name";
"Example_1"
"Example_2"], "List", ones(1, 2, 3), matrix((1:8) == 0, [2 2 1 2]));
checkSaveLoad(lstRef);
// tlist in tlist
lstRef = tlist(["tlistInTlist";
"Name";
"Tlist";
"Poly";
"List"], "List", lstRef, poly([1, 2], "s", "c"), list(1, %t));
checkSaveLoad(lstRef);

//// Mlist
lstRef = mlist(["MLIST", "Name", "Value"], ["a", "b"; "c", "d"], [1, 2; 3, 4]);
checkSaveLoad(lstRef);
lstRef = mlist(["MLIST";
"String";
"Double";
"DoubleCplx";
"Int";
"Bool"], ["A", "B"; "C", "D"], valueRef, ...
complex(valueRef, 2), createIntValues(valueRef, 8), ...
[%f, %t; %f, %t]);
checkSaveLoad(lstRef);
// hypermatrix in mlist
lstRef = mlist(["hmInMlist";
"Name";
"Example_1"
"Example_2"], "List", ones(1, 2, 3), matrix((1:8) == 0, [2 2 1 2]));
checkSaveLoad(lstRef);

//// Undefined
valueRef = list(1, "two", "three");
valueRef(5) = "five";
valueRef(7) = 7;
checkSaveLoad(valueRef);

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
checkSaveLoad(data);

data2.data.data = 42;
data2.data.string = "Test";
data2.list = list(1,["a" "b"]);
checkSaveLoad(data2);

struct_ = [data, data, data2; data2, data2, data];
checkSaveLoad(struct_);

emptyStruct = struct();
checkSaveLoad(emptyStruct);

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
checkSaveLoad(Cell_);

emptyCell = cell();
checkSaveLoad(emptyCell);

// Heterogenous save
checkSaveLoad(mat3, %i+mat3, p3, bool3, listNew, struct_, Cell_);

