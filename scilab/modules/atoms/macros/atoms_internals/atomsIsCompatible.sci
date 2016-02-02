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

// Returns True if the version string is compatible with the current scilab version
// else, returns false

function result = atomsIsCompatible(version_str)

    rhs = argn(2);

    // Check number of input arguments
    // =========================================================================
    if rhs <> 1 then
        error(msprintf(gettext("%s: Wrong number of input argument: %d expected.\n"),"atomsIsCompatible",1));
    end

    // Check input parameter type
    // =========================================================================
    if type(version_str) <> 10 then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: string expected.\n"),"atomsIsCompatible",1));
    end

    // Check input parameter dimension
    // =========================================================================

    if size(version_str,"*") <> 1 then
        error(msprintf(gettext("%s: Wrong size for input argument #%d: string expected.\n"),"atomsIsCompatible",1));
    end

    // If version_str == 'any', no need to continue
    // =========================================================================

    if version_str == "any" then
        result = %T;
        return;
    end

    // Check input parameters value
    // =========================================================================

    if regexp(version_str,"/^(>=|>|<=|<|=|==|~)\s/","o") == [] then
        error(msprintf(gettext("%s: Wrong value for input argument #%d: This is not a valid version/dependency.\n"),"atomsIsCompatible",1));
    end

    // Get scilab version (Only the first digit)
    // =========================================================================
    sciversion_mat = getversion("scilab");
    sciversion     = strcat(string(sciversion_mat(1:3)) + ".");
    sciversion     = strsubst(sciversion,"/\.$/","","r");

    // Get the dependency operator and version
    // =========================================================================
    operator_length  = regexp(version_str,"/\s/","o");
    dep_operator     = part(version_str,1:operator_length-1);
    if dep_operator == "~" then
        result = %T;
        return;
    end
    dep_version      = stripblanks(part(version_str,operator_length+1:length(version_str)));

    // Compare sciversion and the specified version
    // =========================================================================
    comparison       = atomsVersionCompare(dep_version,sciversion);

    // Verdict
    // =========================================================================

    if (dep_operator == "==" | dep_operator == "=")  & comparison<>0 then
        result = %F;
        return;
    end

    if (dep_operator == ">=") & comparison>0 then
        result = %F;
        return;
    end

    if (dep_operator == "<=") & comparison<0 then
        result = %F;
        return;
    end

    if (dep_operator == ">" ) & comparison>=0 then
        result = %F;
        return;
    end

    if (dep_operator == "<" ) & comparison<=0 then
        result = %F;
        return;
    end

    result = %T;

endfunction
