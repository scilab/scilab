//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Antoine ELIAS
// Copyright (C) 2015 - Scilab Enterprises - Clement DAVID
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

//import xcos diagram in Scilab environment
//parameters :
//xcosFile : xcos diagram file
//result : boolean
function result = importXcosDiagram(xcosFile)

    result = %f;
    rhs = argn(2);

    if(rhs == 1) then
        //save hdf5 file with xcos filename, just change extension
        [path,fname,extension] = fileparts(xcosFile);
        h5File = TMPDIR + filesep() + fname + ".sod";
        // open the file to check for permissions
        [a, err] = mopen(xcosFile, "r");
        if (err <> 0) then
            error(msprintf(gettext("Unable to open %s" + "\n"), xcosFile));
            return;
        end
        // construct a full path string
        fullPathName = get_absolute_file_path(fname + extension) + fname + convstr(extension, "l");
        mclose(a);
    else
        error(msprintf(gettext("%s: Wrong number of input argument(s): %d expected." + "\n"), "importXcosDiagram", 1));
        return;
    end

    // decode scilab managed file format
    [path,fname,extension] = fileparts(fullPathName);
    select extension
    case "sod"  then
        load(fullPathName, "scs_m");
    case "h5"   then
        load(fullPathName, "scs_m");
    case "cosf" then
        exec(fullPathName);
    case "cos"  then
        error(msprintf(gettext("%s: Not supported format %s.\n"), "importXcosDiagram", extension));
    else
        // on the Java side
        scs_m = xcosDiagramToScilab(fullPathName);
    end

    //return scs_m in Scilab environment
    result = %t;
    scs_m = resume(scs_m);
endfunction

