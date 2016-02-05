// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2012 - Scilab Enterprises - Antoine ELIAS
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

//trad(inputFile) -> convert inputFile in place.
//trad(inputFile, backupFile) -> convert inputFile in place and save exiting file in backupFile
//if backupFile is empty [] or "" use inputFile with extention .old

function status = trad(inputFile, backupFile)
    if exists("backupFile") then
        bBackup = %t;
        if backupFile == [] | backupFile == "" then
            [filePath, fileName, fileExt] = fileparts(inputFile);
            backupFile = filePath + fileName + ".old";
        end

        [fd,err] = mopen(backupFile, "a");
        if err <> 0 then
            error(999, msprintf(gettext("%s: Unable to create backup file ""%s""\n"), "trad", backupFile));
        end
        mclose(fd);
    else
        bBachup = %f;
    end
    if ~isfile(inputFile) then
        error(msprintf("file %s not find", inputFile));
    end

    printf("\nStarting convertion of ""%s""\n\n", inputFile);

    if bBackup then
        printf("Backup in ""%s""\n", backupFile);
        copyfile(inputFile, backupFile);
    end
    inputFile = pathconvert(inputFile, %f);
    //check if astyle exist on computer
    cmd = "astyle --pad-header --suffix=none --pad-oper --indent-col1-comments --indent-switches --add-brackets --style=bsd --formatted ";
    if getos() == "Windows" then
        cmd = SCI + "/tools/astyle/" + cmd + inputFile;
    else
        cmd = cmd + inputFile;
    end

    host(cmd);
    src = mgetl(inputFile);

    result = src;
    result = sed(result, "/CheckRhs\((.*), (.*)\)(.*)/", "CheckInputArgument(pvApiCtx, \1, \2)\3");
    result = sed(result, "/CheckLhs\((.*), (.*)\)(.*)/", "CheckOutputArgument(pvApiCtx, \1, \2)\3");
    result = sed(result, "/PutLhsVar\(\)(.*)/", "ReturnArguments(pvApiCtx)\1");


    printf("Header operations");
    //add api_scilab include
    index = grep(result, "/#include [""<]api_scilab.h["">]/", "r");
    if index == [] then
        //try to replace #include "stack*.h" to #include "api_scilab.h"
        result = sed(result, "/#include ""stack(.*).h""/", "#include ""api_scilab.h""");

        //remove all #include "api_scilab.h" but one
        index = grep(result, "/#include [""<]api_scilab.h["">]/", "r");
        if size(index, "*") > 1 then
            result(index(2:$)) = [];
        end
    end

    //add localization header
    index = grep(result, "/#include [""<]localization.h[""<]/", "r");
    if index == [] then
        index = grep(result, "/#include [""<]api_scilab.h["">]/", "r");
        result = [result(1:index) ; "#include ""localization.h""" ; result((index+1):$)];
    end

    //add Scierror header
    index = grep(result, "/#include [""<]Scierror.h[""<]/", "r");
    if index == [] then
        index = grep(result, "/#include [""<]api_scilab.h["">]/", "r");
        result = [result(1:index) ; "#include ""Scierror.h""" ; result((index+1):$)];
    end

    printf(":\tOK\n");


    //add SciErr sciErr declaration
    index = grep(result, "SciErr sciErr;");
    if index == [] then
        //find gateway prototype and add declaration 2 lines lower
        index = grep(result, "/int ([C2F\(]?.*[\)]?)\(([\s]*)char(.*), unsigned long (.*)\)/", "r");
        if index <> [] then
            for i = 1:size(index, "*")
                result = [result(1 : (index(i) + 1)); "    SciErr sciErr;"; result((index(i) + 2):$)];
            end
        else
            error(999, msprintf(gettext("%s: unable to find gateway prototype in file %s"), "trad", inputFile));
        end
    end

    //double
    printf("Double operations");
    result = GetDouble(result);
    result = AllocDouble(result)
    result = CreateDouble(result);
    result = GetDoubleComplex(result);
    result = AllocDoubleComplex(result)
    result = CreateDoubleComplex(result);
    printf(":\tOK\n");

    //string
    printf("String operations");
    result = GetSingleString(result);
    result = GetStringMatrix(result);
    result = AllocSingleString(result);
    result = CreateSingleString(result);
    result = CreateStringMatrix(result);
    printf(":\tOK\n");

    //bool
    printf("Boolean operations");
    result = GetBoolMatrix(result);
    result = AllocBoolMatrix(result);
    result = CreateBoolMatrix(result);
    printf(":\tOK\n");

    //pointer
    printf("Pointer operations");
    result = GetPointer(result)
    result = CreatePointer(result)
    result = AllocPointer(result)
    printf(":\tOK\n");

    //handle
    printf("Handle operations");
    result = GetHandleMatrix(result)
    result = CreateHandleMatrix(result)
    result = AllocHandleMatrix(result)
    printf(":\tOK\n");

    //tools
    printf("Tool operations");
    result = replaceCheckScalar(result);
    result = replaceCheckSquare(result);
    result = replaceCheckVector(result);
    result = replaceCheckRow(result);
    result = replaceCheckColumn(result);
    result = replaceCheckDims(result);
    result = replaceCheckLength(result);
    result = replaceCheckSameDims(result);
    result = replaceCheckDimProp(result);
    result = replaceCheckOneDim(result);
    printf(":\tOK\n");

    result = replaceVarType(result);
    result = replaceGetType(result);

    result = sed(result, "/LhsVar\((.*)\) = (.*);/", "AssignOutputVariable(pvApiCtx, \1) = \2;");
    result = sed(result, "/ Rhs /", " nbInputArgument(pvApiCtx) ");
    result = sed(result, "/\(Rhs([ ,])/", "(nbInputArgument(pvApiCtx)\1");
    result = sed(result, "/ Lhs /", " nbOutputArgument(pvApiCtx) ");
    result = sed(result, "/\(Lhs([ ,])/", "(nbOutputArgument(pvApiCtx)\1");


    printf("Move declarations");

    index = grep(result, "//@MOVE@");

    dec = result(index);
    dec = strsubst(dec, " //@MOVE@", "");
    dec = ["";
    "//WARNING ALL NEW DECALRATIONS ARE HERE IF YOUR HAVE MANY FUNCTIONS";
    "//IN THE FILE YOU HAVE PROBABLY TO MOVE DECLARATIONS IN GOOD FUNCTIONS";
    dec];
    result(index) = [];

    sciErrPos = grep(result, "SciErr sciErr;");
    result = [result(1:sciErrPos) ; dec ; "" ; result(sciErrPos+1:$)];
    printf("\nConvertion finished\n");
    mputl(result, inputFile);

    //call astyle again
    host(cmd);

    status = %t;
endfunction


function result = sed(str, findExp, replaceExp)

    result = str;
    index = grep(result, findExp, "r");
    while index <> []
        idx = index(1);
        [startPos, endPos, match, captured] = regexp(result(idx), findExp);

        if captured <> [] then
            //multiple matches on the same line, YOUHOU !
            for i=1:size(captured, "r")
                replace = replaceExp;
                for j = 1:size(captured, "c")
                    replace = strsubst(replace, "\" + string(j), captured(i,j));
                end

                if size(replace, "*") > 1 & (startPos <> 1 | endPos <> length(result(idx))) then
                    //replace partial line by multiline expression
                    replace(1) = part(result(idx), 1:startPos) + " " + replace(1);
                    replace($) = replace($) + " " + part(result(idx), (endPos+1):length(result(idx)));

                    result = [result(1:(idx-1)); replace; result((idx+1):$)];
                elseif size(replace, "*") > 1 then
                    //replace entire line by multiline expression
                    result = [result(1:(idx-1)); replace; result((idx+1):$)];
                else
                    //replace partial line by 1-line expression
                    result(idx) = strsubst(result(idx), match(i), replace);
                end
            end
        end

        //update index with new "file"
        index = grep(result, findExp, "r");
    end
endfunction

//double
function result = GetDouble(str)
    replaceExp = mgetl("SCI/tools/api_scilab_trad/Double_Matrix_Get.txt");
    findExp = "/([\s]*)GetRhsVar\([\s]*(.*)[\s]*,[\s]*MATRIX_OF_DOUBLE_DATATYPE[\s]*,[\s]*&(.*)[\s]*,[\s]*&(.*)[\s]*,[\s]*&([\S]*)[\s]*\);(.*)/";

    result = commonDouble(str, findExp, replaceExp);

    replaceExp = mgetl("SCI/tools/api_scilab_trad/Double_Matrix_GetAsInteger.txt");
    findExp = "/([\s]*)GetRhsVar\([\s]*(.*)[\s]*,[\s]*MATRIX_OF_INTEGER_DATATYPE[\s]*,[\s]*&(.*)[\s]*,[\s]*&(.*)[\s]*,[\s]*&([\S]*)[\s]*\);(.*)/";

    result = commonDouble(result, findExp, replaceExp);
endfunction

function result = CreateDouble(str)
    replaceExp = mgetl("SCI/tools/api_scilab_trad/Double_Matrix_Create.txt");
    findExp = "/([\s]*)CreateVarFromPtr\([\s]*(.*)[\s]*,[\s]*MATRIX_OF_DOUBLE_DATATYPE[\s]*,[\s]*&(.*)[\s]*,[\s]*&(.*)[\s]*,[\s]*&([\S]*)[\s]*\);(.*)/";

    result = commonDouble(str, findExp, replaceExp);

    replaceExp = mgetl("SCI/tools/api_scilab_trad/Double_Matrix_CreateAsInteger.txt");
    findExp = "/([\s]*)CreateVarFromPtr\([\s]*(.*)[\s]*,[\s]*MATRIX_OF_INTEGER_DATATYPE[\s]*,[\s]*&(.*)[\s]*,[\s]*&(.*)[\s]*,[\s]*&([\S]*)[\s]*\);(.*)/";

    result = commonDouble(result, findExp, replaceExp);
endfunction

function result = AllocDouble(str)
    replaceExp = mgetl("SCI/tools/api_scilab_trad/Double_Matrix_Alloc.txt");
    findExp = "/([\s]*)CreateVar\([\s]*(.*)[\s]*,[\s]*MATRIX_OF_DOUBLE_DATATYPE[\s]*,[\s]*&(.*)[\s]*,[\s]*&(.*)[\s]*,[\s]*&([\S]*)[\s]*\);(.*)/";

    result = commonDouble(str, findExp, replaceExp);

    replaceExp = mgetl("SCI/tools/api_scilab_trad/Double_Matrix_AllocAsInteger.txt");
    findExp = "/([\s]*)CreateVar\([\s]*(.*)[\s]*,[\s]*MATRIX_OF_INTEGER_DATATYPE[\s]*,[\s]*&(.*)[\s]*,[\s]*&(.*)[\s]*,[\s]*&([\S]*)[\s]*\);(.*)/";

    result = commonDouble(result, findExp, replaceExp);
endfunction

function result = commonDouble(str, findExp, replaceExp)
    result = str;
    index = grep(result, findExp, "r");
    while index <> []
        i = index(1);
    [start, end, match, captured] = regexp(result(i), findExp);
    replace = replaceExp;

    if size(captured, "c") >= 5 then
        //try to replace (i)stk(\5*) by (\5*).
        if captured(5) <> "" then
            //hstk(\5*) -> (long long*)(\5*)
            result = sed(result, "/hstk\(" + captured(5) + "(.*[^)]?)\)/", "(long long*)(" + captured(5) + "\1)");
            result = sed(result, "/istk\(" + captured(5) + "(.*[^)]?)\)/", "(int*)(" + captured(5) + "\1)");
            result = sed(result, "/[^(csz]?stk\(" + captured(5) + "(.*[^)]?)\)/", "(" + captured(5) + "\1)");
        end
    end

    for j = 1:size(captured, "c")
        replace = strsubst(replace, "\" + string(j), captured(j));
    end

    if size(replace, "*") > 1 then
        result = [result(1:(i-1)); replace; result((i+1):$)];
    else
        result(i) = strsubst(str(i), match, replace);
    end

    index = grep(result, findExp, "r");
end
endfunction

function result = GetDoubleComplex(str)
    //MATRIX_OF_DOUBLE_DATATYPE
    replaceExp = mgetl("SCI/tools/api_scilab_trad/DoubleComplex_Matrix_Get.txt");
    findExp = "/([\s]*)GetRhsCVar\([\s]*(.*)[\s]*,[\s]*MATRIX_OF_DOUBLE_DATATYPE[\s]*,[\s]*&(.*)[\s]*,[\s]*&(.*)[\s]*,[\s]*&(.*)[\s]*,[\s]*&([\S]*)[\s]*[\s]*,[\s]*&([\S]*)[\s]*\);(.*)/";

    result = commonDoubleComplex(str, findExp, replaceExp);

    //MATRIX_OF_INTEGER_DATATYPE
    replaceExp = mgetl("SCI/tools/api_scilab_trad/DoubleComplex_Matrix_GetAsInteger.txt");
    findExp = "/([\s]*)GetRhsCVar\([\s]*(.*)[\s]*,[\s]*MATRIX_OF_INTEGER_DATATYPE[\s]*,[\s]*&(.*)[\s]*,[\s]*&(.*)[\s]*,[\s]*&(.*)[\s]*,[\s]*&([\S]*)[\s]*[\s]*,[\s]*&([\S]*)[\s]*\);(.*)/";

    result = commonDoubleComplex(result, findExp, replaceExp);
endfunction

function result = CreateDoubleComplex(str)
    replaceExp = mgetl("SCI/tools/api_scilab_trad/DoubleComplex_Matrix_Create.txt");
    findExp = "/([\s]*)CreateCVarFromPtr\([\s]*(.*)[\s]*,[\s]*MATRIX_OF_DOUBLE_DATATYPE[\s]*,[\s]*&(.*)[\s]*,[\s]*&(.*)[\s]*,[\s]*&(.*)[\s]*,[\s]*&([\S]*)[\s]*[\s]*,[\s]*&([\S]*)[\s]*\);(.*)/";

    result = commonDoubleComplex(str, findExp, replaceExp);

    replaceExp = mgetl("SCI/tools/api_scilab_trad/DoubleComplex_Matrix_CreateAsInteger.txt");
    findExp = "/([\s]*)CreateCVarFromPtr\([\s]*(.*)[\s]*,[\s]*MATRIX_OF_INTEGER_DATATYPE[\s]*,[\s]*&(.*)[\s]*,[\s]*&(.*)[\s]*,[\s]*&(.*)[\s]*,[\s]*&([\S]*)[\s]*,[\s]*&([\S]*)[\s]*\);(.*)/";

    result = commonDoubleComplex(result, findExp, replaceExp);
endfunction

function result = AllocDoubleComplex(str)
    replaceExp = mgetl("SCI/tools/api_scilab_trad/DoubleComplex_Matrix_Alloc.txt");
    findExp = "/([\s]*)CreateCVar\([\s]*(.*)[\s]*,[\s]*MATRIX_OF_DOUBLE_DATATYPE[\s]*,[\s]*&(.*)[\s]*,[\s]*&(.*)[\s]*,[\s]*&(.*)[\s]*,[\s]*&([\S]*)[\s]*,[\s]*&([\S]*)[\s]*\);(.*)/";

    result = commonDoubleComplex(str, findExp, replaceExp);

    replaceExp = mgetl("SCI/tools/api_scilab_trad/DoubleComplex_Matrix_AllocAsInteger.txt");
    findExp = "/([\s]*)CreateCVar\([\s]*(.*)[\s]*,[\s]*MATRIX_OF_INTEGER_DATATYPE[\s]*,[\s]*&(.*)[\s]*,[\s]*&(.*)[\s]*,[\s]*&(.*)[\s]*,[\s]*&([\S]*)[\s]*,[\s]*&([\S]*)[\s]*\);(.*)/";

    result = commonDoubleComplex(result, findExp, replaceExp);
endfunction

function result = commonDoubleComplex(str, findExp, replaceExp)
    result = str;
    index = grep(result, findExp, "r");
    while index <> []
        i = index(1);
    [start, end, match, captured] = regexp(result(i), findExp);
    replace = replaceExp;

    if size(captured, "c") >= 6 then
        //try to replace stk(\6*) by (\6*).
        if captured(6) <> "" then
            result = sed(result, "/hstk\(" + captured(5) + "(.*[^)]?)\)/", "(long long*)(" + captured(5) + "\1)");
            result = sed(result, "/istk\(" + captured(5) + "(.*[^)]?)\)/", "(int*)(" + captured(5) + "\1)");
            result = sed(result, "/[^(csz]?stk\(" + captured(5) + "(.*[^)]?)\)/", "(" + captured(5) + "\1)");
        end
    end

    if size(captured, "c") >= 7 then
        //try to replace stk(\7*) by (\7*).
        if captured(7) <> "" then
            result = sed(result, "/hstk\(" + captured(7) + "(.*[^)]?)\)/", "(long long*)(" + captured(7) + "\1)");
            result = sed(result, "/istk\(" + captured(7) + "(.*[^)]?)\)/", "(int*)(" + captured(7) + "\1)");
            result = sed(result, "/[^(csz]?stk\(" + captured(7) + "(.*[^)]?)\)/", "(" + captured(7) + "\1)");
        end
    end

    for j = 1:size(captured, "c")
        replace = strsubst(replace, "\" + string(j), captured(j));
    end

    if size(replace, "*") > 1 then
        result = [result(1:(i-1)); replace; result((i+1):$)];
    else
        result(i) = strsubst(str(i), match, replace);
    end

    index = grep(result, findExp, "r");
end
endfunction


//string
function result = GetSingleString(str)
    replaceExp = mgetl("SCI/tools/api_scilab_trad/String_Single_Get.txt");
    findExp = "/([\s]*)GetRhsVar\([\s]*(.*)[\s]*,[\s]*STRING_DATATYPE[\s]*,[\s]*&(.*)[\s]*,[\s]*&(.*)[\s]*,[\s]*&([\S]*)[\s]*\);(.*)/";

    result = commonString(str, findExp, replaceExp);
endfunction

function result = GetStringMatrix(str)
    replaceExp = mgetl("SCI/tools/api_scilab_trad/String_Matrix_Get.txt");
    findExp = "/([\s]*)GetRhsVar\([\s]*(.*)[\s]*,[\s]*MATRIX_OF_STRING_DATATYPE[\s]*,[\s]*&(.*)[\s]*,[\s]*&(.*)[\s]*,[\s]*&([\S]*)[\s]*\);(.*)/";

    result = commonString(str, findExp, replaceExp);
endfunction

function result = AllocSingleString(str)
    replaceExp = mgetl("SCI/tools/api_scilab_trad/String_Single_Alloc.txt");
    findExp = "/([\s]*)CreateVar\([\s]*(.*)[\s]*,[\s]*STRING_DATATYPE[\s]*,[\s]*&(.*)[\s]*,[\s]*&(.*)[\s]*,[\s]*&([\S]*)[\s]*\);(.*)/";

    result = commonString(str, findExp, replaceExp);
endfunction

function result = CreateSingleString(str)
    replaceExp = mgetl("SCI/tools/api_scilab_trad/String_Single_Create.txt");
    findExp = "/([\s]*)CreateVarFromPtr\([\s]*(.*)[\s]*,[\s]*STRING_DATATYPE[\s]*,[\s]*&(.*)[\s]*,[\s]*&(.*)[\s]*,[\s]*&([\S]*)[\s]*\);(.*)/";

    result = commonString(str, findExp, replaceExp);
endfunction

function result = CreateStringMatrix(str)
    replaceExp = mgetl("SCI/tools/api_scilab_trad/String_Matrix_Create.txt");
    findExp = "/([\s]*)CreateVarFromPtr\([\s]*(.*)[\s]*,[\s]*MATRIX_OF_STRING_DATATYPE[\s]*,[\s]*&(.*)[\s]*,[\s]*&(.*)[\s]*,[\s]*&([\S]*)[\s]*\);(.*)/";

    result = commonString(str, findExp, replaceExp);
endfunction

function result = commonString(str, findExp, ReplaceExp)
    result = str;
    index = grep(result, findExp, "r");
    while index <> []
        i = index(1);
    [start, end, match, captured] = regexp(result(i), findExp);
    replace = replaceExp;

    if size(captured, "c") >= 5 then
        //try to replace stk(\5*) by (\5*).
        if captured(5) <> "" then
            result = sed(result, "/cstk\(" + captured(5) + "(.*[^)]?)\)/", "(" + captured(5) + "\1)");
        end
    end

    for j = 1:size(captured, "c")
        replace = strsubst(replace, "\" + string(j), captured(j));
    end

    if size(replace, "*") > 1 then
        result = [result(1:(i-1)); replace; result((i+1):$)];
    else
        result(i) = strsubst(str(i), match, replace);
    end

    index = grep(result, findExp, "r");
end
endfunction

//////////////
//   bool   //
//////////////
function result = GetBoolMatrix(str)
    replaceExp = mgetl("SCI/tools/api_scilab_trad/Bool_Matrix_Get.txt");
    findExp = "/([\s]*)GetRhsVar\([\s]*(.*)[\s]*,[\s]*MATRIX_OF_BOOLEAN_DATATYPE[\s]*,[\s]*&(.*)[\s]*,[\s]*&(.*)[\s]*,[\s]*&([\S]*)[\s]*\);(.*)/";

    result = commonBool(str, findExp, replaceExp);
endfunction

function result = AllocBoolMatrix(str)
    replaceExp = mgetl("SCI/tools/api_scilab_trad/Bool_Matrix_Alloc.txt");
    findExp = "/([\s]*)CreateVar\([\s]*(.*)[\s]*,[\s]*MATRIX_OF_BOOLEAN_DATATYPE[\s]*,[\s]*&(.*)[\s]*,[\s]*&(.*)[\s]*,[\s]*&([\S]*)[\s]*\);(.*)/";

    result = commonBool(str, findExp, replaceExp);
endfunction

function result = CreateBoolMatrix(str)
    replaceExp = mgetl("SCI/tools/api_scilab_trad/Bool_Matrix_Create.txt");
    findExp = "/([\s]*)CreateVarFromPtr\([\s]*(.*)[\s]*,[\s]*MATRIX_OF_BOOLEAN_DATATYPE[\s]*,[\s]*&(.*)[\s]*,[\s]*&(.*)[\s]*,[\s]*&([\S]*)[\s]*\);(.*)/";

    result = commonBool(str, findExp, replaceExp);
endfunction

function result = commonBool(str, findExp, ReplaceExp)
    result = str;
    index = grep(result, findExp, "r");
    while index <> []
        i = index(1);
    [start, end, match, captured] = regexp(result(i), findExp);
    replace = replaceExp;

    if size(captured, "c") >= 5 then
        //try to replace stk(\5*) by (\5*).
        if captured(5) <> "" then
            result = sed(result, "/istk\(" + captured(5) + "(.*[^)]?)\)/", "(" + captured(5) + "\1)");
        end
    end

    for j = 1:size(captured, "c")
        replace = strsubst(replace, "\" + string(j), captured(j));
    end

    if size(replace, "*") > 1 then
        result = [result(1:(i-1)); replace; result((i+1):$)];
    else
        result(i) = strsubst(str(i), match, replace);
    end

    index = grep(result, findExp, "r");
end
endfunction

/////////////////
//   pointer   //
/////////////////
function result = GetPointer(str)
    replaceExp = mgetl("SCI/tools/api_scilab_trad/Pointer_Get.txt");
    findExp = "/([\s]*)GetRhsVar\([\s]*(.*)[\s]*,[\s]*SCILAB_POINTER_DATATYPE[\s]*,[\s]*&(.*)[\s]*,[\s]*&(.*)[\s]*,[\s]*&([\S]*)[\s]*\);(.*)/";

    result = commonPointer(str, findExp, replaceExp);
endfunction

function result = CreatePointer(str)
    replaceExp = mgetl("SCI/tools/api_scilab_trad/Pointer_Create.txt");
    findExp = "/([\s]*)CreateVarFromPtr\([\s]*(.*)[\s]*,[\s]*SCILAB_POINTER_DATATYPE[\s]*,[\s]*&(.*)[\s]*,[\s]*&(.*)[\s]*,[\s]*([\S]*)[\s]*\);(.*)/";

    result = commonPointer(str, findExp, replaceExp);
endfunction

function result = AllocPointer(str)
    replaceExp = mgetl("SCI/tools/api_scilab_trad/Pointer_Alloc.txt");
    findExp = "/([\s]*)CreateVar\([\s]*(.*)[\s]*,[\s]*SCILAB_POINTER_DATATYPE[\s]*,[\s]*&(.*)[\s]*,[\s]*&(.*)[\s]*,[\s]*&([\S]*)[\s]*\);(.*)/";

    result = commonPointer(str, findExp, replaceExp);
endfunction

function result = commonPointer(str, findExp, ReplaceExp)
    result = str;
    index = grep(result, findExp, "r");
    while index <> []
        i = index(1);
    [start, end, match, captured] = regexp(result(i), findExp);
    replace = replaceExp;

    if size(captured, "c") >= 5 then
        //try to replace stk(\5*) by (\5*).
        if captured(5) <> "" then
            result = sed(result, "/[^(cisz]?stk\(" + captured(5) + "(.*[^)]?)\)/", "(" + captured(5) + "\1)");
        end
    end

    for j = 1:size(captured, "c")
        replace = strsubst(replace, "\" + string(j), captured(j));
    end

    if size(replace, "*") > 1 then
        result = [result(1:(i-1)); replace; result((i+1):$)];
    else
        result(i) = strsubst(str(i), match, replace);
    end

    index = grep(result, findExp, "r");
end
endfunction

////////////////
//   handle   //
////////////////
function result = GetHandleMatrix(str)
    replaceExp = mgetl("SCI/tools/api_scilab_trad/Handle_Matrix_Get.txt");
    findExp = "/([\s]*)GetRhsVar\([\s]*(.*)[\s]*,[\s]*GRAPHICAL_HANDLE_DATATYPE[\s]*,[\s]*&(.*)[\s]*,[\s]*&(.*)[\s]*,[\s]*&([\S]*)[\s]*\);(.*)/";

    result = commonHandle(str, findExp, replaceExp);
endfunction

function result = CreateHandleMatrix(str)
    replaceExp = mgetl("SCI/tools/api_scilab_trad/Handle_Matrix_Create.txt");
    findExp = "/([\s]*)CreateVarFromPtr\([\s]*(.*)[\s]*,[\s]*GRAPHICAL_HANDLE_DATATYPE[\s]*,[\s]*&(.*)[\s]*,[\s]*&(.*)[\s]*,[\s]*([\S]*)[\s]*\);(.*)/";

    result = commonHandle(str, findExp, replaceExp);
endfunction

function result = AllocHandleMatrix(str)
    replaceExp = mgetl("SCI/tools/api_scilab_trad/Handle_Matrix_Alloc.txt");
    findExp = "/([\s]*)CreateVar\([\s]*(.*)[\s]*,[\s]*GRAPHICAL_HANDLE_DATATYPE[\s]*,[\s]*&(.*)[\s]*,[\s]*&(.*)[\s]*,[\s]*&([\S]*)[\s]*\);(.*)/";

    result = commonHandle(str, findExp, replaceExp);
endfunction

function result = commonHandle(str, findExp, ReplaceExp)
    result = str;
    index = grep(result, findExp, "r");
    while index <> []
        i = index(1);
    [start, end, match, captured] = regexp(result(i), findExp);
    replace = replaceExp;

    if size(captured, "c") >= 5 then
        //try to replace stk(\5*) by (\5*).
        if captured(5) <> "" then
            result = sed(result, "/[h]?stk\(" + captured(5) + "(.*[^)]?)\)/", "(" + captured(5) + "\1)");
        end
    end

    for j = 1:size(captured, "c")
        replace = strsubst(replace, "\" + string(j), captured(j));
    end

    if size(replace, "*") > 1 then
        result = [result(1:(i-1)); replace; result((i+1):$)];
    else
        result(i) = strsubst(str(i), match, replace);
    end

    index = grep(result, findExp, "r");
end
endfunction

///////////////
//   tools   //
///////////////
function result = replaceCheckScalar(str)
    replaceExp = mgetl("SCI/tools/api_scilab_trad/Tools_Check_Scalar.txt");
    findExp = "/([\s]*)CheckScalar\([\s]*(.*)[\s]*,[\s]*(.*)[\s]*,[\s]*(.*)\);/";

    result = sed(str, findExp, replaceExp);
endfunction

function result = replaceCheckSquare(str)
    replaceExp = mgetl("SCI/tools/api_scilab_trad/Tools_Check_Square.txt");
    findExp = "/([\s]*)CheckSquare\([\s]*(.*)[\s]*,[\s]*(.*)[\s]*,[\s]*(.*)\);/";

    result = sed(str, findExp, replaceExp);
endfunction

function result = replaceCheckVector(str)
    replaceExp = mgetl("SCI/tools/api_scilab_trad/Tools_Check_Vector.txt");
    findExp = "/([\s]*)CheckVector\([\s]*(.*)[\s]*,[\s]*(.*)[\s]*,[\s]*(.*)\);/";

    result = sed(str, findExp, replaceExp);
endfunction

function result = replaceCheckRow(str)
    replaceExp = mgetl("SCI/tools/api_scilab_trad/Tools_Check_Row.txt");
    findExp = "/([\s]*)CheckRow\([\s]*(.*)[\s]*,[\s]*(.*)[\s]*,[\s]*(.*)\);/";

    result = sed(str, findExp, replaceExp);
endfunction

function result = replaceCheckColumn(str)
    replaceExp = mgetl("SCI/tools/api_scilab_trad/Tools_Check_Column.txt");
    findExp = "/([\s]*)CheckColumn\([\s]*(.*)[\s]*,[\s]*(.*)[\s]*,[\s]*(.*)\);/";

    result = sed(str, findExp, replaceExp);
endfunction

function result = replaceCheckDims(str)
    replaceExp = mgetl("SCI/tools/api_scilab_trad/Tools_Check_Dims.txt");
    findExp = "/([\s]*)CheckDims\([\s]*(.*)[\s]*,[\s]*(.*)[\s]*,[\s]*(.*)[\s]*,[\s]*(.*)[\s]*,[\s]*(.*)\);/";

    result = sed(str, findExp, replaceExp);
endfunction

function result = replaceCheckLength(str)
    replaceExp = mgetl("SCI/tools/api_scilab_trad/Tools_Check_Length.txt");
    findExp = "/([\s]*)CheckLength\([\s]*(.*)[\s]*,[\s]*(.*)[\s]*,[\s]*(.*)\);/";

    result = sed(str, findExp, replaceExp);
endfunction

function result = replaceCheckSameDims(str)
    replaceExp = mgetl("SCI/tools/api_scilab_trad/Tools_Check_SameDims.txt");
    findExp = "/([\s]*)CheckSameDims\([\s]*(.*)[\s]*,[\s]*(.*)[\s]*,[\s]*(.*)[\s]*,[\s]*(.*)[\s]*,[\s]*(.*)[\s]*,[\s]*(.*)\);/";

    result = sed(str, findExp, replaceExp);
endfunction

function result = replaceCheckDimProp(str)
    replaceExp = mgetl("SCI/tools/api_scilab_trad/Tools_Check_DimProp.txt");
    findExp = "/([\s]*)CheckDimProp\([\s]*(.*)[\s]*,[\s]*(.*)[\s]*,[\s]*(.*)\);/";

    result = sed(str, findExp, replaceExp);
endfunction

function result = replaceCheckOneDim(str)
    replaceExp = mgetl("SCI/tools/api_scilab_trad/Tools_Check_OneDim.txt");
    findExp = "/([\s]*)CheckOneDim\([\s]*(.*)[\s]*,[\s]*(.*)[\s]*,[\s]*(.*)[\s]*,[\s]*(.*)\);/";

    result = sed(str, findExp, replaceExp);
endfunction

function result = replaceVarType(str)
    result = str

    //three cases
    //first
    //VarType(x) == sci_xx
    result = sed(result, "/[\s]*VarType\([\s]*(.*)[\s]*\)[\s]*==[\s]*([^);]*)[\s]*/", "(checkInputArgumentType(pvApiCtx, \1, \2))");
    //second
    //VarType(x) != sci_xx
    result = sed(result, "/[\s]*VarType\([\s]*(.*)[\s]*\)[\s]*!=[\s]*([^);]*)[\s]*/", "(!checkInputArgumentType(pvApiCtx, \1, \2))");
    //third
    //VarType(x)
    result = sed(result, "/VarType\([\s]*(.*)[\s]*\)/", "getInputArgumentType(pvApiCtx, \1)");
endfunction

function result = replaceGetType(str)
    result = str
    //three cases
    //first
    //GetType(x) == sci_xx
    result = sed(result, "/[\s]*GetType\([\s]*(.*)[\s]*\)[\s]*==[\s]*([^);]*)[\s^)]*/", "checkInputArgumentType(pvApiCtx, \1, \2)");
    //second
    //GetType(x) != sci_xx
    result = sed(result, "/[\s]*GetType\([\s]*(.*)[\s]*\)[\s]*!=[\s]*([^);]*)[\s^)]*/", "(!checkInputArgumentType(pvApiCtx, \1, \2))");
    //third
    //GetType(x)
    result = sed(result, "/GetType\((.*)\)/", "getInputArgumentType(pvApiCtx, \1)");
endfunction


