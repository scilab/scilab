// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Vincent Couvert <vincent.couvert@inria.fr>
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

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

        assert_checkequal(emptycell , cell());
        assert_checkequal(scalarcell , makecell([1 1], 1.23));
        assert_checkequal(rowcell , makecell([1 3], "abc", [1.23+7.89*%i 4.56-1.23*%i 7.89+4.56*%i], 1.23));
        assert_checkequal(colcell , makecell([3 1], [1.23+7.89*%i 4.56-1.23*%i 7.89+4.56*%i], "abc", 1.23));
        assert_checkequal(arraycell , makecell([2 3], "a", [1.23+7.89*%i 4.56-1.23*%i 7.89+4.56*%i], int8(1), "abc", 1.23, eye(100,100)));
        assert_checkequal(stringcell , makecell([2 3], "abc", "def", "ghi", "jkl", "mno", "pqr"));

        clear emptycell scalarcell rowcell colcell arraycell stringcell
    end

    //
    // TESTS FOR STRUCTURE ARRAYS
    //
    if binFormats(formatIndex) <> "-v4" then // Struct arrays can not be stored in Matlab 4 binary files

        sciemptystruct = struct();
        assert_checkequal(emptystruct , sciemptystruct);

        sciscalarstruct = struct("f1", 10, "ftwo", "Hello", "field3", int8(12));
        assert_checkequal(scalarstruct , sciscalarstruct);

        scirowstruct = struct("field1", 10, "field2", "Hello", "field3", int8(12));
        scirowstruct(1,2).field1 = "test";
        scirowstruct(1,2).field2 = eye(10, 10);
        scirowstruct(1,3).field2 = "a field contents";
        scirowstruct(1,3).field3 = 1.23+4.56*%i;
        assert_checkequal(rowstruct , scirowstruct);

        scicolstruct = struct("name", 10, "phone", "Hello", "address", int8(12));
        scicolstruct(2,1).name = "test";
        scicolstruct(2,1).phone = eye(10, 10);
        scicolstruct(3,1).phone = "a field contents";
        scicolstruct(3,1).address = 1.23+4.56*%i;
        assert_checkequal(colstruct , scicolstruct);

        sciarraystruct = struct("field1", 10, "field2", "Hello", "field3", int8(12));
        sciarraystruct(1,2).field1 = "test";
        sciarraystruct(1,2).field2 = eye(10, 10);
        sciarraystruct(1,3).field2 = "a field contents";
        sciarraystruct(1,3).field3 = 1.23+4.56*%i;
        sciarraystruct(2,1).name = "test";
        sciarraystruct(2,1).phone = eye(10, 10);
        sciarraystruct(3,1).phone = "a field contents";
        sciarraystruct(3,1).address = 1.23+4.56*%i;
        assert_checkequal(arraystruct , sciarraystruct);

        //
        // TESTS FOR STRUCTURE ARRAYS SPARSE
        //
        if binFormats(formatIndex) == "-v7" then
            assert_checkequal(structure.test,sparse([1 0; 2 0; 0 3]));
            clear structure
        end

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

    assert_checkequal(emptystringmatrix , "");
    assert_checkequal(stringscalar , "a");
    assert_checkequal(stringrowvector , ["abc"]);
    assert_checkequal(stringcolvector , ["a";"b";"c"]);
    assert_checkequal(stringmatrix , ["abc";"def"]);

    clear emptystringmatrix stringscalar stringrowvector stringcolvector stringmatrix

    //
    // TESTS FOR DOUBLE PRECISION ARRAYS
    //

    assert_checkequal(emptydoublematrix , []);

    assert_checkequal(realdoublescalar , 1.23);
    assert_checkequal(cplxdoublescalar , 1.23 + 4.56*%i);

    assert_checkequal(realdoublerowvector , [1.23 -4.56 7.89]);
    assert_checkequal(cplxdoublerowvector , [1.23+7.89*%i 4.56-1.23*%i 7.89+4.56*%i]);

    assert_checkequal(realdoublecolvector , [1.23;-4.56;7.89]);
    assert_checkequal(cplxdoublecolvector , [1.23+7.89*%i;4.56-1.23*%i;7.89+4.56*%i]);

    assert_checkequal(realdoublematrix , [1.23 -4.56 7.89;9.87 6.54 -3.21]);
    assert_checkequal(cplxdoublematrix , [1.23+7.89*%i 4.56-1.23*%i 7.89+4.56*%i;9.87+3.21*%i 6.54+9.87*%i 3.21-6.54*%i]);

    clear emptydoublematrix realdoublescalar cplxdoublescalar realdoublerowvector cplxdoublerowvector realdoublecolvector cplxdoublecolvector realdoublematrix cplxdoublematrix

    //
    // TESTS FOR SINGLE PRECISION ARRAYS
    //

    // TODO

    //
    // TESTS FOR SPARSE MATRICES
    //

    if binFormats(formatIndex) <> "-v4" then // Integers can not be stored in Matlab 4 binary files
        assert_checkequal(emptysparse , []);

        assert_checkequal(realscalarsparse , sparse(1.23));
        //assert_checkequal(cplxscalarsparse , sparse(1.23 + 4.56i));

        assert_checkequal(realrowvectorsparse , sparse([1.23 0 7.89]));
        //assert_checkequal(cplxrowvectorsparse , sparse([1.23+7.89i 0 7.89+4.56i]));

        assert_checkequal(realcolvectorsparse , sparse([1.23;0;7.89]));
        //assert_checkequal(cplxcolvectorsparse , sparse([1.23+7.89i;;7.89+4.56i]));

        assert_checkequal(realmatrixsparse , sparse([1.23 0 7.89;0 6.54 -3.21]));
        //assert_checkequal(cplxmatrixsparse , sparse([1.23+7.89i 0 7.89+4.56i;0 6.54+9.87i 3.21-6.54i]));
    end
    if binFormats(formatIndex) <> "-v4" then // Integers can not be stored in Matlab 4 binary files

        //
        // TESTS FOR 8-BITS SIGNED INTEGERS
        //

        assert_checkequal(emptyint8matrix , int8([]));
        assert_checkequal(int8scalar , int8(1));
        assert_checkequal(int8rowvector , int8([1 -4 7]));
        assert_checkequal(int8colvector , int8([1;-4;7]));
        assert_checkequal(int8matrix , int8([1 -4 7;-9 6 -3]));

        clear emptyint8matrix int8scalar int8rowvector int8colvector int8matrix

        //
        // TESTS FOR 16-BITS SIGNED INTEGERS
        //

        assert_checkequal(emptyint16matrix , int16([]));
        assert_checkequal(int16scalar , int16(1));
        assert_checkequal(int16rowvector , int16([1 -4 7]));
        assert_checkequal(int16colvector , int16([1;-4;7]));
        assert_checkequal(int16matrix , int16([1 -4 7;-9 6 -3]));

        clear emptyint16matrix int16scalar int16rowvector int16colvector int16matrix

        //
        // TESTS FOR 32-BITS SIGNED INTEGERS
        //

        assert_checkequal(emptyint32matrix , int32([]));
        assert_checkequal(int32scalar , int32(1));
        assert_checkequal(int32rowvector , int32([1 -4 7]));
        assert_checkequal(int32colvector , int32([1;-4;7]));
        assert_checkequal(int32matrix , int32([1 -4 7;-9 6 -3]));

        clear emptyint32matrix int32scalar int32rowvector int32colvector int32matrix

        // TODO: int64 tests ?

        //
        // TESTS FOR 8-BITS UNSIGNED INTEGERS
        //

        assert_checkequal(emptyuint8matrix , uint8([]));
        assert_checkequal(uint8scalar , uint8(1));
        //assert_checkequal(uint8rowvector , uint8([1 -4 7]));
        assert_checkequal(uint8rowvector , uint8([1 0 7]));
        //assert_checkequal(uint8colvector , uint8([1;-4;7]));
        assert_checkequal(uint8colvector , uint8([1;0;7]));
        //assert_checkequal(uint8matrix , uint8([1 -4 7;-9 6 -3]));
        assert_checkequal(uint8matrix , uint8([1 0 7;0 6 0]));

        clear emptyuint8matrix uint8scalar uint8rowvector uint8colvector uint8matrix

        //
        // TESTS FOR 16-BITS UNSIGNED INTEGERS
        //

        assert_checkequal(emptyuint16matrix , uint16([]));
        assert_checkequal(uint16scalar , uint16(1));
        //assert_checkequal(uint16rowvector , uint16([1 -4 7]));
        assert_checkequal(uint16rowvector , uint16([1 0 7]));
        //assert_checkequal(uint16colvector , uint16([1;-4;7]));
        assert_checkequal(uint16colvector , uint16([1;0;7]));
        //assert_checkequal(uint16matrix , uint16([1 -4 7;-9 6 -3]));
        assert_checkequal(uint16matrix , uint16([1 0 7;0 6 0]));

        clear emptyuint16matrix uint16scalar uint16rowvector uint16colvector uint16matrix

        //
        // TESTS FOR 32-BITS UNSIGNED INTEGERS
        //

        assert_checkequal(emptyuint32matrix , uint32([]));
        assert_checkequal(uint32scalar , uint32(1));
        //assert_checkequal(uint32rowvector , uint32([1 -4 7]));
        assert_checkequal(uint32rowvector , uint32([1 0 7]));
        //assert_checkequal(uint32colvector , uint32([1;-4;7]));
        assert_checkequal(uint32colvector , uint32([1;0;7]));
        //assert_checkequal(uint32matrix , uint32([1 -4 7;-9 6 -3]));
        assert_checkequal(uint32matrix , uint32([1 0 7;0 6 0]));

        clear emptyuint32matrix uint32scalar uint32rowvector uint32colvector uint32matrix

        // TODO: uint64 tests ?


        //
        // TESTS FOR ND-ARRAYS
        //
        assert_checkequal(emptyNDarray , []);
        assert_checkequal(realdoubleNDarray , matrix(1:24, 2, 3, 4));
        assert_checkequal(complexdoubleNDarray , matrix((1:24) + (1:24)*%i, 2, 3, 4));

        clear emptyNDarray realdoubleNDarray complexdoubleNDarray

    end

    //
    // MISC
    //
    assert_checkequal(eye100x100 , eye(100,100));

end

// TODO: read all variables from a single file ?
