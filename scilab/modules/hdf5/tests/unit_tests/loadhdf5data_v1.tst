// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011 - DIGITEO - Sylvestre LEDRU
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

// Load previously saved data (check backward compatibility)


///// Double
load(SCI+"/modules/hdf5/tests/sample_scilab_data/matricedoublescalar_v1.sod");
assert_checkequal(ascalar,42);

load(SCI+"/modules/hdf5/tests/sample_scilab_data/matricedouble_v1.sod");
assert_checkequal(a,[2,3;4,5]);


///// Double complex
load(SCI+"/modules/hdf5/tests/sample_scilab_data/matricedoublecomplex_v1.sod");
assert_checkequal(b,[2+%i,3+%i*2;4+%i*%pi,5]);

load(SCI+"/modules/hdf5/tests/sample_scilab_data/matricedoublecomplexscalar_v1.sod");
assert_checkequal(bscalar,42+%i);


///// String
load(SCI+"/modules/hdf5/tests/sample_scilab_data/matricestring_v1.sod");
assert_checkequal(c,["my matrix","of strings";"is","saved in Scilab data format"]);

load(SCI+"/modules/hdf5/tests/sample_scilab_data/matricestringscalar_v1.sod");
assert_checkequal(cscalar,["my single string"]);

// Polynomials
load(SCI+"/modules/hdf5/tests/sample_scilab_data/polynomials_v1.sod");
p_reference=poly([1 2],"s") ;
assert_checkequal(p,p_reference);

load(SCI+"/modules/hdf5/tests/sample_scilab_data/polynomialscoef_v1.sod");
q_reference=poly([1 42],"s","c");
assert_checkequal(q,q_reference);


// Boolean matrices
load(SCI+"/modules/hdf5/tests/sample_scilab_data/booleanscalar_v1.sod");
assert_checkequal(bm,%t);
load(SCI+"/modules/hdf5/tests/sample_scilab_data/booleanmatrix_v1.sod");
assert_checkequal(bmm,[%t,%f; %f, %t]);

// Sparse
load(SCI+"/modules/hdf5/tests/sample_scilab_data/sparsematrix_v1.sod");
assert_checkequal(sp,sparse([1,2;4,5;3,10],[1,2,3]));

load(SCI+"/modules/hdf5/tests/sample_scilab_data/emptysparse_v1.sod");
assert_checkequal(emptysparse,sparse([]));

// boolean sparse
dense=[%F, %F, %T, %F, %F
%T, %F, %F, %F, %F
%F, %F, %F, %F, %F
%F, %F, %F, %F, %T];
sp_reference=sparse(dense);
load(SCI+"/modules/hdf5/tests/sample_scilab_data/booleansparse_v1.sod");
assert_checkequal(sp,sp_reference);

// List
load(SCI+"/modules/hdf5/tests/sample_scilab_data/list_v1.sod");
assert_checkequal(ls,list(2,%i,"f",ones(3,3)));
ls(2) =  list( %t, [32,42]); // alter ls to add the list
load(SCI+"/modules/hdf5/tests/sample_scilab_data/listnested_v1.sod");
assert_checkequal(lsnested,ls);

// tlist
lst_reference  =  tlist(["random numbers";"Name";"Example"], "Uniform",ones(1,2, 3));
load(SCI+"/modules/hdf5/tests/sample_scilab_data/tlist_v1.sod");
assert_checkequal(lst,lst_reference);

// mlist
M_ref=mlist(["V","name","value"],["a","b";"c" "d"],[1 2; 3 4]);
load(SCI+"/modules/hdf5/tests/sample_scilab_data/mlist_v1.sod");
// Commented because of bug 9531
//assert_checkequal(M_ref,M);

// hypermatrices
load(SCI+"/modules/hdf5/tests/sample_scilab_data/hypermatrix_v1.sod");
assert_checkequal(HM,matrix(1:24, [2 3 2 2]));

load(SCI+"/modules/hdf5/tests/sample_scilab_data/hypermatrixcomplex_v1.sod");
HMC_reference(1,1,1,1:2)=[1+%i 2*2*%i];
assert_checkequal(HMC,HMC_reference);

// int

emptyint8matrix_ref = int8([]);
int8scalar_ref = int8(1);
int8rowvector_ref = int8([1 -4 7]);
int8colvector_ref = int8([1;-4;7]);
int8matrix_ref = int8([1 -4 7;-9 6 -3]);
load(SCI+"/modules/hdf5/tests/sample_scilab_data/int8_v1.sod");
assert_checkequal(emptyint8matrix, emptyint8matrix_ref);
assert_checkequal(int8scalar, int8scalar_ref);
assert_checkequal(int8rowvector, int8rowvector_ref);
assert_checkequal(int8colvector, int8colvector_ref);
assert_checkequal(int8matrix, int8matrix_ref);


emptyint16matrix_ref = int16([]);
int16scalar_ref = int16(1);
int16rowvector_ref = int16([1 -4 7]);
int16colvector_ref = int16([1;-4;7]);
int16matrix_ref = int16([1 -4 7;-9 6 -3]);
load(SCI+"/modules/hdf5/tests/sample_scilab_data/int16_v1.sod");
assert_checkequal(emptyint16matrix, emptyint16matrix_ref);
assert_checkequal(int16scalar, int16scalar_ref);
assert_checkequal(int16rowvector, int16rowvector_ref);
assert_checkequal(int16colvector, int16colvector_ref);
assert_checkequal(int16matrix, int16matrix_ref);


emptyint32matrix_ref = int32([]);
int32scalar_ref = int32(1);
int32rowvector_ref = int32([1 -4 7]);
int32colvector_ref = int32([1;-4;7]);
int32matrix_ref = int32([1 -4 7;-9 6 -3]);
load(SCI+"/modules/hdf5/tests/sample_scilab_data/int32_v1.sod");
assert_checkequal(emptyint32matrix, emptyint32matrix_ref);
assert_checkequal(int32scalar, int32scalar_ref);
assert_checkequal(int32rowvector, int32rowvector_ref);
assert_checkequal(int32colvector, int32colvector_ref);
assert_checkequal(int32matrix, int32matrix_ref);


emptyuint8matrix_ref = uint8([]);
uint8scalar_ref = uint8(1);
uint8rowvector_ref = uint8([1 4 7]);
uint8colvector_ref = uint8([1;4;7]);
uint8matrix_ref = uint8([1 4 7;9 6 3]);
load(SCI+"/modules/hdf5/tests/sample_scilab_data/uint8_v1.sod");
assert_checkequal(emptyuint8matrix, emptyuint8matrix_ref);
assert_checkequal(uint8scalar, uint8scalar_ref);
assert_checkequal(uint8rowvector, uint8rowvector_ref);
assert_checkequal(uint8colvector, uint8colvector_ref);
assert_checkequal(uint8matrix, uint8matrix_ref);


emptyuint16matrix_ref = uint16([]);
uint16scalar_ref = uint16(1);
uint16rowvector_ref = uint16([1 4 7]);
uint16colvector_ref = uint16([1;4;7]);
uint16matrix_ref = uint16([1 4 7;9 6 3]);
load(SCI+"/modules/hdf5/tests/sample_scilab_data/uint16_v1.sod");
assert_checkequal(emptyuint16matrix, emptyuint16matrix_ref);
assert_checkequal(uint16scalar, uint16scalar_ref);
assert_checkequal(uint16rowvector, uint16rowvector_ref);
assert_checkequal(uint16colvector, uint16colvector_ref);
assert_checkequal(uint16matrix, uint16matrix_ref);


emptyuint32matrix_ref = uint32([]);
uint32scalar_ref = uint32(1);
uint32rowvector_ref = uint32([1 4 7]);
uint32colvector_ref = uint32([1;4;7]);
uint32matrix_ref = uint32([1 4 7;9 6 3]);
load(SCI+"/modules/hdf5/tests/sample_scilab_data/uint32_v1.sod");
assert_checkequal(emptyuint32matrix, emptyuint32matrix_ref);
assert_checkequal(uint32scalar, uint32scalar_ref);
assert_checkequal(uint32rowvector, uint32rowvector_ref);
assert_checkequal(uint32colvector, uint32colvector_ref);
assert_checkequal(uint32matrix, uint32matrix_ref);


// undefined
undefinedelement_ref=list(2,%i,"f",ones(3,3));
undefinedelement_ref(6)="toto";
load(SCI+"/modules/hdf5/tests/sample_scilab_data/undefinedelement_v1.sod");
assert_checkequal(undefinedelement, undefinedelement_ref);


// void
voidelement_ref=list(1,,3);
load(SCI+"/modules/hdf5/tests/sample_scilab_data/voidelement_v1.sod");
// Commented because of bug 9529
//assert_checkequal(voidelement, voidelement_ref);

