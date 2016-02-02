// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Pierre MARECHAL <pierre.marechal@scilab.org>
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

// Internal function

// Return  1 if version_1 is superior than version_2
// Return -1 if version_2 is superior than version_1
// Return  0 if version_1 and version_2 are equal

// version can be an array

function result = atomsVersionCompare( version_1 , version_2 )
    rhs    = argn(2);

    // Check number of input arguments
    // =========================================================================

    if rhs <> 2 then
        error(msprintf(gettext("%s: Wrong number of input argument: %d expected.\n"),"atomsVersionCompare",2));
    end

    // Check input parameters type
    // =========================================================================

    if type(version_1) <> 10 then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: string expected.\n"),"atomsVersionCompare",1));
    end

    if type(version_2) <> 10 then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: string expected.\n"),"atomsVersionCompare",2));
    end

    // Check input parameters size
    // =========================================================================

    if size(version_2,"*") <> 1 then
        error(msprintf(gettext("%s: Wrong size for input argument #%d: string expected.\n"),"atomsVersionCompare",2));
    end

    // Check input parameters values
    // =========================================================================

    for i=1:size(version_1,"*")

        if (length(version_1(i)) > 1) & (regexp(version_1(i),"/^[0-9]([0-9\.])*[0-9](\-[0-9]([0-9])*)?$/") == []) then
            error(msprintf(gettext("%s: Wrong value for input argument #%d: This is not a valid version.\n"),"atomsVersionCompare",1));
        end

        if (length(version_1(i)) == 1) & (regexp(version_1(i),"/^[0-9]$/") == []) then
            error(msprintf(gettext("%s: Wrong value for input argument #%d: This is not a valid version.\n"),"atomsVersionCompare",1));
        end

    end

    // version_2 == -1 means no more recent version found
    if version_2 == "-1" then
        result = ones(version_1);
        return
    end


    if (length(version_2) > 1) & (regexp(version_2,"/^[0-9]([0-9\.])*[0-9](\-[0-9]([0-9])*)?$/") == []) then
        error(msprintf(gettext("%s: Wrong value for input argument #%d: This is not a valid version.\n"),"atomsVersionCompare",2));
    end

    if (length(version_2) == 1) & (regexp(version_2,"/^[0-9]$/") == []) then
        error(msprintf(gettext("%s: Wrong value for input argument #%d: This is not a valid version.\n"),"atomsVersionCompare",2));
    end

    // Init the result matrix
    // =========================================================================
    result = zeros(version_1);

    // Now : action
    // =========================================================================

    // Split version and packaging version

    if strindex(version_2,"-")==[] then
        version_2_pack = 0;
    else
        version_2_tmp  = strsubst( strsplit(version_2,strindex(version_2,"-")) , "-" , "" );
        version_2      = version_2_tmp(1);
        version_2_pack = strtod(version_2_tmp(2));
    end

    // Split the version

    if regexp(version_2,"/\./","o") == [] then
        version_2_mat = strtod(version_2);
    else
        version_2_mat = strtod(strsubst( strsplit(version_2,strindex(version_2,".")) , "." , "" ));
    end

    version_2_mat_size = size(version_2_mat,"*");

    for i=1:size(version_1,"*")

        // Split version and packaging version

        if strindex(version_1(i),"-")==[] then
            version_1_pack = 0;
        else
            version_1_tmp  = strsubst( strsplit(version_1(i),strindex(version_1(i),"-")) , "-" , "" );
            version_1(i)   = version_1_tmp(1);
            version_1_pack = strtod(version_1_tmp(2));
        end

        // split the version

        if regexp(version_1(i),"/\./","o") == [] then
            version_1_mat = strtod(version_1(i));
        else
            version_1_mat = strtod(strsubst( strsplit(version_1(i),strindex(version_1(i),".")) , "." , "" ));
        end

        version_1_mat_size = size(version_1_mat,"*");

        if version_1_mat_size>=version_2_mat_size then
            for j=1:version_1_mat_size

                if result(i) <> 0 then
                    break;
                end

                if j > version_2_mat_size then
                    if version_1_mat(j) > 0 then
                        // Version_1 is superior than version_2
                        result(i) = 1;
                        break;
                    end
                    continue;
                end

                if version_1_mat(j) > version_2_mat(j) then
                    // Version_1 is superior than version_2
                    result(i) = 1;
                    break;
                end

                if version_2_mat(j) > version_1_mat(j) then
                    // Version_2 is superior than version_1
                    result(i) = -1;
                    break;
                end

            end
        else
            for j=1:version_2_mat_size

                if result(i) <> 0 then
                    break;
                end

                if j > version_1_mat_size then
                    if version_2_mat(j) > 0 then
                        // Version_2 is superior than version_1
                        result(i) = -1;
                        break;
                    end
                    continue;
                end

                if version_2_mat(j) > version_1_mat(j) then
                    // Version_2 is superior than version_1
                    result(i) = -1;
                    break;
                end

                if version_1_mat(j) > version_2_mat(j) then
                    // Version_1 is superior than version_2
                    result(i) = 1;
                    break;
                end

            end

        end

        // Last test : check if the two version are equivalent
        // If yes : compare the packaging versions

        if result(i) == 0 then

            if version_1_pack > version_2_pack then
                result(i) = 1;

            elseif version_1_pack < version_2_pack then
                result(i) = -1;

            else
                result(i) = 0;

            end

        end

    end

endfunction
