// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011-2012 - DIGITEO - Allan CORNET
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.
// =============================================================================
function generatef2csln(defaultSLNpath)

    // generates scilab_f2c.sln from scilab.sln
    // scilab_f2c.sln for visual studio express 2010

    C_GUID = "{8BC9CEB8-8B4A-11D0-8D11-00A0C91BC942}";
    FORTRAN_GUID = "{6989167D-11E4-40FE-8C1A-2192A86A7E90}";
    VFPROJ = "_f.vfproj";
    VCPROJ = "_f2c.vcxproj";
    SLN_NAME_F2C = "Scilab_f2c.sln";
    SLN_NAME = "Scilab.sln";

    if ~isdef("defaultSLNpath") then
        defaultSLNpath = SCI;
    else
        if (type(defaultSLNpath) <> 10) then
            error(999, msprintf(gettext("%s: Wrong type for input argument #%d: A String expected.\n"),"generatef2csln",1));
        end
    end

    if ~isdir(defaultSLNpath) then
        error(999, msprintf(gettext("%s: Wrong value for input argument #%d: A valid path expected.\n"),"generatef2csln",1));
    end

    fullFilenameSLN = fullfile(defaultSLNpath, SLN_NAME);
    if ~isfile(fullFilenameSLN) then
        error(999, msprintf(gettext("%s: Wrong value for input argument #%d: %s not found.\n"),"generatef2csln",1,SLN_NAME));
    end

    SLN = mgetl(fullFilenameSLN);
    F2CSLN = strsubst(SLN, FORTRAN_GUID, C_GUID);
    F2CSLN = strsubst(F2CSLN, VFPROJ, VCPROJ);
    mputl(F2CSLN, fullfile(defaultSLNpath, SLN_NAME_F2C));
endfunction
// =============================================================================
// main
generatef2csln();
// =============================================================================
