//<-- CLI SHELL MODE -->
// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2015 - Scilab Enterprises - Antoine ELIAS
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//

clear;
function writeData(filename, data)
    save(filename, data);
endfunction

function checkBinary(file1, file2)
    //open 1st file
    md5_1 = getmd5(file1);
    md5_2 = getmd5(file2);

    assert_checkequal(md5_1, md5_2);

    //mdelete(file1);
    //mdelete(file2);
endfunction

function writeAllData(path, radical)
    d = matrix(1:100, [10, 10]);
    data(:,:,1) = d - d * %i;
    data(:,:,2) = -d;
    data(:,:,3) = d * 2 + 4*d*%i;
    data(:,:,4) = -d * 2;
    data(:,:,5) = d - d * %i;
    data(:,:,6) = -d
    data(:,:,7) = d * 2 + 4*d*%i;
    data(:,:,8) = -d * 2;
    data(:,:,9) = d - d * %i;
    data(:,:,10) = -d;

    //write double
    d = data;
    writeData(path + "double" + radical + ".sod", "d");
    //write string
    s = string(data);
    writeData(path + "string" + radical + ".sod", "s");
    //write boolean
    b = real(data) > 0;
    writeData(path + "boolean" + radical + ".sod", "b");
    //write ints
    i8 = int8(data);
    writeData(path + "int8" + radical + ".sod", "i8");
    i16 = int16(data);
    writeData(path + "int16" + radical + ".sod", "i16");
    i32 = int32(data);
    writeData(path + "int32" + radical + ".sod", "i32");
    i64 = int64(data);
    writeData(path + "int64" + radical + ".sod", "i64");
    ui8 = uint8(data);
    writeData(path + "uint8" + radical + ".sod", "ui8");
    ui16 = uint16(data);
    writeData(path + "uint16" + radical + ".sod", "ui16");
    ui32 = uint32(data);
    writeData(path + "uint32" + radical + ".sod", "ui32");
    ui64 = uint64(data);
    writeData(path + "uint64" + radical + ".sod", "ui64");
    //write polynom
    p = data * %s;
    writeData(path + "poly" + radical + ".sod", "p");
    //write list
    l = list(d,s,b,i8,i16,i32,i64,ui8,ui16,ui32,ui64,p);
    writeData(path + "list" + radical + ".sod", "l");
endfunction

function checkAllData(path, radical1, radical2)
    //check double
    checkBinary(path + "double" + radical1 + ".sod", path + "double" + radical2 + ".sod");
    //check string
    checkBinary(path + "string" + radical1 + ".sod", path + "string" + radical2 + ".sod");
    //check boolean
    checkBinary(path + "boolean" + radical1 + ".sod", path + "boolean" + radical2 + ".sod");
    //check ints
    checkBinary(path + "int8" + radical1 + ".sod", path + "int8" + radical2 + ".sod");
    checkBinary(path + "int16" + radical1 + ".sod", path + "int16" + radical2 + ".sod");
    checkBinary(path + "int32" + radical1 + ".sod", path + "int32" + radical2 + ".sod");
    checkBinary(path + "int64" + radical1 + ".sod", path + "int64" + radical2 + ".sod");
    checkBinary(path + "uint8" + radical1 + ".sod", path + "uint8" + radical2 + ".sod");
    checkBinary(path + "uint16" + radical1 + ".sod", path + "uint16" + radical2 + ".sod");
    checkBinary(path + "uint32" + radical1 + ".sod", path + "uint32" + radical2 + ".sod");
    checkBinary(path + "uint64" + radical1 + ".sod", path + "uint64" + radical2 + ".sod");
    //check poly
    checkBinary(path + "poly" + radical1 + ".sod", path + "poly" + radical2 + ".sod");
    //check list
    checkBinary(path + "list" + radical1 + ".sod", path + "list" + radical2 + ".sod");
endfunction

path = TMPDIR + "/";
radical1 = "1";
radical2 = "2";

writeAllData(path, radical1);
sleep(2000);
writeAllData(path, radical2);

checkAllData(path, radical1, radical2);

