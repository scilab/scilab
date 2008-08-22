// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Vincent Couvert <vincent.couvert@inria.fr>
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// This Scilab script loads MAT-files to test the MATIO module

clear all

//
// Loads all MAT-files
//

binFormats = ["-v4";"-v6";"-v7"];
for formatIndex = 1:size(binFormats, "*")
  matFiles = gsort(listfiles("SCI/modules/matio/tests/unit_tests/*" + binFormats(formatIndex) + ".mat"), "r", "i");
  for fileIndex=1:size(matFiles, "*")
    disp(fileparts(matFiles(fileIndex), "fname") + fileparts(matFiles(fileIndex), "extension"));
    loadmatfile(matFiles(fileIndex));
  end

  //
  // TESTS FOR CELL ARRAYS
  //
  if binFormats(formatIndex) <> "-v4" then // Cell arrays can not be stored in Matlab 4 binary files
    
    if or(emptycell <> cell()) then pause, end
    if or(scalarcell <> makecell([1 1], 1.23)) then pause, end
    if or(rowcell <> makecell([1 3], "abc", [1.23+7.89*%i 4.56-1.23*%i 7.89+4.56*%i], 1.23)) then pause, end
    if or(colcell <> makecell([3 1], [1.23+7.89*%i 4.56-1.23*%i 7.89+4.56*%i], "abc", 1.23)) then pause, end
    if or(arraycell <> makecell([2 3], "a", [1.23+7.89*%i 4.56-1.23*%i 7.89+4.56*%i], int8(1), "abc", 1.23, eye(100,100))) then pause, end
    if or(stringcell <> makecell([2 3], "abc", "def", "ghi", "jkl", "mno", "pqr")) then pause, end
  
    clear emptycell scalarcell rowcell colcell arraycell stringcell
  end
  
  //
  // TESTS FOR STRUCTURE ARRAYS
  //
  if binFormats(formatIndex) <> "-v4" then // Struct arrays can not be stored in Matlab 4 binary files
    
    sciemptystruct = struct();
    if or(emptystruct <> sciemptystruct) then pause, end
    
    sciscalarstruct = struct("f1", 10, "ftwo", "Hello", "field3", int8(12));
    if or(scalarstruct <> sciscalarstruct) then pause, end
    
    scirowstruct = struct("field1", 10, "field2", "Hello", "field3", int8(12));
    scirowstruct(1,2).field1 = "test";
    scirowstruct(1,2).field2 = eye(10, 10);
    scirowstruct(1,3).field2 = "a field contents";
    scirowstruct(1,3).field3 = 1.23+4.56*%i;
    if or(rowstruct <> scirowstruct) then pause, end
    
    scicolstruct = struct("name", 10, "phone", "Hello", "address", int8(12));
    scicolstruct(2,1).name = "test";
    scicolstruct(2,1).phone = eye(10, 10);
    scicolstruct(3,1).phone = "a field contents";
    scicolstruct(3,1).address = 1.23+4.56*%i;
    if or(colstruct <> scicolstruct) then pause, end
    
    sciarraystruct = struct("field1", 10, "field2", "Hello", "field3", int8(12));
    sciarraystruct(1,2).field1 = "test";
    sciarraystruct(1,2).field2 = eye(10, 10);
    sciarraystruct(1,3).field2 = "a field contents";
    sciarraystruct(1,3).field3 = 1.23+4.56*%i;
    sciarraystruct(2,1).name = "test";
    sciarraystruct(2,1).phone = eye(10, 10);
    sciarraystruct(3,1).phone = "a field contents";
    sciarraystruct(3,1).address = 1.23+4.56*%i;
    if or(arraystruct <> sciarraystruct) then pause, end
    
    clear emptystruct scalarstruct rowstruct colstruct arraystruct
    clear sciemptystruct sciscalarstruct scirowstruct scicolstruct sciarraystruct
  
  end

  //
  // TESTS FOR OBJECTS
  //

  // TODO

  //
  // TESTS FOR CHARACTER ARRAYS
  //

  if or(emptystringmatrix <> "") then pause, end
  if or(stringscalar <> "a") then pause, end
  if or(stringrowvector <> ["abc"]) then pause, end
  if or(stringcolvector <> ["a";"b";"c"]) then pause, end
  if or(stringmatrix <> ["abc";"def"]) then pause, end

  clear emptystringmatrix stringscalar stringrowvector stringcolvector stringmatrix

  //
  // TESTS FOR DOUBLE PRECISION ARRAYS
  //

  if or(emptydoublematrix <> []) then pause, end

  if or(realdoublescalar <> 1.23) then pause, end
  if or(cplxdoublescalar <> 1.23 + 4.56*%i) then pause, end

  if or(realdoublerowvector <> [1.23 -4.56 7.89]) then pause, end
  if or(cplxdoublerowvector <> [1.23+7.89*%i 4.56-1.23*%i 7.89+4.56*%i]) then pause, end

  if or(realdoublecolvector <> [1.23;-4.56;7.89]) then pause, end
  if or(cplxdoublecolvector <> [1.23+7.89*%i;4.56-1.23*%i;7.89+4.56*%i]) then pause, end

  if or(realdoublematrix <> [1.23 -4.56 7.89;9.87 6.54 -3.21]) then pause, end
  if or(cplxdoublematrix <> [1.23+7.89*%i 4.56-1.23*%i 7.89+4.56*%i;9.87+3.21*%i 6.54+9.87*%i 3.21-6.54*%i]) then pause, end

  clear emptydoublematrix realdoublescalar cplxdoublescalar realdoublerowvector cplxdoublerowvector realdoublecolvector cplxdoublecolvector realdoublematrix cplxdoublematrix

  //
  // TESTS FOR SINGLE PRECISION ARRAYS
  //

  // TODO

  //
  // TESTS FOR SPARSE MATRICES
  //

  if binFormats(formatIndex) <> "-v4" then // Integers can not be stored in Matlab 4 binary files
  if or(emptysparse <> []) then pause, end

  if or(realscalarsparse <> sparse(1.23)) then pause, end
  //if or(cplxscalarsparse <> sparse(1.23 + 4.56i)) then pause, end
  
  if or(realrowvectorsparse <> sparse([1.23 0 7.89])) then pause, end
  //if or(cplxrowvectorsparse <> sparse([1.23+7.89i 0 7.89+4.56i])) then pause, end
  
  if or(realcolvectorsparse <> sparse([1.23;0;7.89])) then pause, end
  //if or(cplxcolvectorsparse <> sparse([1.23+7.89i;;7.89+4.56i])) then pause, end
  
  if or(realmatrixsparse <> sparse([1.23 0 7.89;0 6.54 -3.21])) then pause, end
  //if or(cplxmatrixsparse <> sparse([1.23+7.89i 0 7.89+4.56i;0 6.54+9.87i 3.21-6.54i])) then pause, end
end
  if binFormats(formatIndex) <> "-v4" then // Integers can not be stored in Matlab 4 binary files
    
    //
    // TESTS FOR 8-BITS SIGNED INTEGERS
    //

    if or(emptyint8matrix <> int8([])) then pause, end
    if or(int8scalar <> int8(1)) then pause, end
    if or(int8rowvector <> int8([1 -4 7])) then pause, end
    if or(int8colvector <> int8([1;-4;7])) then pause, end
    if or(int8matrix <> int8([1 -4 7;-9 6 -3])) then pause, end

    clear emptyint8matrix int8scalar int8rowvector int8colvector int8matrix

    //
    // TESTS FOR 16-BITS SIGNED INTEGERS
    //

    if or(emptyint16matrix <> int16([])) then pause, end
    if or(int16scalar <> int16(1)) then pause, end
    if or(int16rowvector <> int16([1 -4 7])) then pause, end
    if or(int16colvector <> int16([1;-4;7])) then pause, end
    if or(int16matrix <> int16([1 -4 7;-9 6 -3])) then pause, end

    clear emptyint16matrix int16scalar int16rowvector int16colvector int16matrix

    //
    // TESTS FOR 32-BITS SIGNED INTEGERS
    //

    if or(emptyint32matrix <> int32([])) then pause, end
    if or(int32scalar <> int32(1)) then pause, end
    if or(int32rowvector <> int32([1 -4 7])) then pause, end
    if or(int32colvector <> int32([1;-4;7])) then pause, end
    if or(int32matrix <> int32([1 -4 7;-9 6 -3])) then pause, end

    clear emptyint32matrix int32scalar int32rowvector int32colvector int32matrix

    // TODO: int64 tests ?

    //
    // TESTS FOR 8-BITS UNSIGNED INTEGERS
    //

    if or(emptyuint8matrix <> uint8([])) then pause, end
    if or(uint8scalar <> uint8(1)) then pause, end
    //if or(uint8rowvector <> uint8([1 -4 7])) then pause, end
    if or(uint8rowvector <> [1 0 7]) then pause, end
    //if or(uint8colvector <> uint8([1;-4;7])) then pause, end
    if or(uint8colvector <> [1;0;7]) then pause, end
    //if or(uint8matrix <> uint8([1 -4 7;-9 6 -3])) then pause, end
    if or(uint8matrix <> [1 0 7;0 6 0]) then pause, end

    clear emptyuint8matrix uint8scalar uint8rowvector uint8colvector uint8matrix

    //
    // TESTS FOR 16-BITS UNSIGNED INTEGERS
    //

    if or(emptyuint16matrix <> uint16([])) then pause, end
    if or(uint16scalar <> uint16(1)) then pause, end
    //if or(uint16rowvector <> uint16([1 -4 7])) then pause, end
    if or(uint16rowvector <> [1 0 7]) then pause, end
    //if or(uint16colvector <> uint16([1;-4;7])) then pause, end
    if or(uint16colvector <> [1;0;7]) then pause, end
    //if or(uint16matrix <> uint16([1 -4 7;-9 6 -3])) then pause, end
    if or(uint16matrix <> [1 0 7;0 6 0]) then pause, end

    clear emptyuint16matrix uint16scalar uint16rowvector uint16colvector uint16matrix

    //
    // TESTS FOR 32-BITS UNSIGNED INTEGERS
    //

    if or(emptyuint32matrix <> uint32([])) then pause, end
    if or(uint32scalar <> uint32(1)) then pause, end
    //if or(uint32rowvector <> uint32([1 -4 7])) then pause, end
    if or(uint32rowvector <> [1 0 7]) then pause, end
    //if or(uint32colvector <> uint32([1;-4;7])) then pause, end
    if or(uint32colvector <> [1;0;7]) then pause, end
    //if or(uint32matrix <> uint32([1 -4 7;-9 6 -3])) then pause, end
    if or(uint32matrix <> [1 0 7;0 6 0]) then pause, end

    clear emptyuint32matrix uint32scalar uint32rowvector uint32colvector uint32matrix

    // TODO: uint64 tests ?

  
    //
    // TESTS FOR ND-ARRAYS
    //
    sciemptyNDarray = hypermat([0 0 0], []);
    if or(emptyNDarray.dims <> sciemptyNDarray.dims) | or(emptyNDarray.entries <> sciemptyNDarray.entries) then pause, end
    if or(realdoubleNDarray <> matrix(1:24, 2, 3, 4)) then pause, end
    if or(complexdoubleNDarray <> matrix((1:24) + (1:24)*%i, 2, 3, 4)) then pause, end
    
    clear sciemptyNDarray emptyNDarray realdoubleNDarray complexdoubleNDarray
  
  end

  //
  // MISC
  //
  if or(eye100x100 <> eye(100,100)) then pause, end
  
end

// TODO: read all variables from a single file ?
