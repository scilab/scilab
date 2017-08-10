// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - DIGITEO - Vincent COUVERT <vincent.couvert@scilab.org>
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.
//

function testOk = m2sciTestExecution(functionName)

    mfilesPath = "SCI/modules/m2sci/tests/unit_tests/mfiles/";

    exec("SCI/modules/m2sci/tests/unit_tests/utils/sci_m2sciUnknownDims.sci", -1);
    exec("SCI/modules/m2sci/tests/unit_tests/utils/sci_m2sciUnknownType.sci", -1);

    mfile2sci(mfilesPath + functionName + ".m", TMPDIR, %F, %F, 0, %T);

    loadmatfile(mfilesPath + functionName + ".mat");

    // Rename all res* variable to matres*
    allVars = who("local");
    numberOfMatRes = 0;
    for kVar = 1:size(allVars, "*")
        if part(allVars(kVar), 1:3)=="res" then
            execstr("mat" + allVars(kVar) + "=" + allVars(kVar));
            execstr("clear " + allVars(kVar));
            numberOfMatRes = numberOfMatRes + 1;
        end
    end

    exec("SCI/modules/m2sci/tests/unit_tests/utils/m2sciUnknownDims.sci", -1);
    exec("SCI/modules/m2sci/tests/unit_tests/utils/m2sciUnknownType.sci", -1);

    exec(TMPDIR + filesep() + functionName + ".sci", -1);

    // Check that Scilab defined as many res* as Matlab
    allVars = who("local");
    allResNames = [];
    for kVar = 1:size(allVars, "*")
        if part(allVars(kVar), 1:3)=="res" then
            allResNames($+1) = allVars(kVar);
        end
    end

    if size(allResNames, "*")<>(numberOfMatRes/2) then
        disp("Wrong number of results: " + string(size(allResNames, "*")) + " <> " + string(numberOfMatRes/2));
        testOk = %F;
        return
    end

    exec("SCI/modules/m2sci/tests/unit_tests/utils/m2sciParseInfos.sci", -1);
    exec("SCI/modules/m2sci/tests/unit_tests/utils/m2sciCompareResults.sci", -1);
    for kRes = 1: size(allResNames, "*")
        resName = allResNames(kRes);

        // For each result, compare the information
        [matSize, matType, matProp] = m2sciParseInfos(evstr("mat" + resName + "_Infos"));

        sciRes = evstr(resName);
        sciSize = size(sciRes);

        if type(sciRes)==10 then
            sciType = "String";
            sciProp = "Real";
        elseif type(sciRes)==4 then
            sciType = "Boolean";
            sciProp = "Real";
        elseif type(sciRes)==6 then
            sciType = "Sparse";
            sciProp = "Real";
        elseif type(sciRes)==5 then
            if isreal(sciRes,0) then
                sciType = "Sparse";
                sciProp = "Real";
            else
                sciType = "Sparse";
                sciProp = "Complex";
            end
        else
            if isreal(sciRes,0) then
                sciType = "Double";
                sciProp = "Real";
            else
                sciType = "Double";
                sciProp = "Complex";
            end
        end

        // Verify size between Scilab and Matlab
        if sciType=="String" then
            if or(sciSize<>matSize) & sciSize(matSize<>sciSize)<>1 then
                // Error
                disp(resName + ": " + sci2exp(sciSize) + " <> " + sci2exp(matSize))
                testOk = %F
                return
            elseif or(sciSize<>matSize) then
                // WARNING: Error for string length
                disp(resName + ": " + sci2exp(sciSize) + " <> " + sci2exp(matSize));
            end
        else
            if or(sciSize<>matSize) then
                disp(resName + ": " + sci2exp(sciSize) + " <> " + sci2exp(matSize));
                if prod(sciSize)==0 & prod(matSize)==0 then // Empty matrix size
                    // WARNING: Error for empty matrix
                else
                    // Error
                    testOk = %F
                    return
                end
            end
        end

        // Verify type between Scilab and Matlab
        if sciType<>matType then
            disp(resName + ": " + string(sciType) + " <> " + string(matType));
            if sciType<>"constant" & matType<>"Boolean" then
                // Error
                testOk = %F
                return
            else
                // WARNING: Error for empty matrix of boolean
            end
        end

        // Verify prop between Scilab and Matlab
        if sciProp<>matProp then
            disp(resName + ": " + string(sciProp) + " <> " + string(matProp));
            testOk = %F;
            return
        end

        // Compare results
        if ~m2sciCompareResults(sciRes, evstr("mat" + resName)) then
            disp([resName + " (scilab): " + sci2exp(sciRes, 0); resName + " (matlab): " + sci2exp(evstr("mat" + resName), 0)])
            testOk = %F
            return
        end
    end

    testOk = %T

endfunction

