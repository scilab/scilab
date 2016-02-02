// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2007-2008 - INRIA - Pierre MARECHAL
// Copyright (C) 2009 - DIGITEO - Pierre MARECHAL
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

// Parameters
//
// title      : A string array - Chapter title
// path       : A string array - Chapter path
// modulemode : A boolean      - %F by default
//                             - if TRUE, the chpater is consider as internal module online help,
//                               otherwise, it's consider as external online help,

// Description
//
// This function adds a new entry in the helps list. The help chapter files are to
// be located in a single directory. If the given path already exists in the helps
// list, nothing is done. The function checks if the directory exist.

function ok = add_help_chapter(helptitle,path,modulemode)

    ok = [];

    // Check input arguments
    // =========================================================================

    [lhs,rhs] = argn(0);

    // Input arguments number
    // -------------------------------------------------------------------------

    if rhs < 2 | rhs > 3 then
        error(msprintf(gettext("%s: Wrong number of input argument: %d to %d expected.\n"),"add_help_chapter",2,3));
    end

    // Input arguments types
    // -------------------------------------------------------------------------

    if type(helptitle) <> 10 then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: String array expected.\n"),"add_help_chapter",1));
    end

    if type(path) <> 10 then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: String array expected.\n"),"add_help_chapter",2));
    end

    if (rhs>2) & (type(modulemode) <> 4) then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: A boolean expected.\n"),"add_help_chapter",3));
    end

    // Input arguments dimensions
    // -------------------------------------------------------------------------

    if or( size(helptitle) <> size(path) ) then
        error(msprintf(gettext("%s: Incompatible input arguments #%d and #%d: Same sizes expected.\n"),"add_help_chapter",1,2));
    end

    // Default value
    // =========================================================================

    if rhs < 3 then
        modulemode = %F;
    end

    if modulemode then
        global %helps_modules;
        this_help = %helps_modules;
    else
        global %helps;
        this_help = %helps;
    end

    // Save the current path
    // =========================================================================
    current_directory = pwd();

    // Loop on "path"
    // =========================================================================
    path = pathconvert(path,%F);

    for i=1:size(path,"*")

        ok(i) = %F;

        // Get the absolute path of "path"
        // ---------------------------------------------------------------------

        if ~isdir(path(i)) then
            chdir(current_directory);
            error(msprintf(gettext("%s: Wrong value for input argument #%d: An existing directory expected.\n"),"add_help_chapter",2));
        end

        chdir(path(i));

        if getos() == "Windows" then
            path(i) = getlongpathname(pwd());
        else
            path(i) = pwd();
        end

        // Check if the path is already added
        // ---------------------------------------------------------------------

        if find( this_help(:,1) == path(i)) <> [] then
            continue;
        end

        this_help = [ this_help ; path(i) helptitle(i) ];

        ok(i) = %T;
    end

    // Go to the original location
    // =========================================================================
    chdir(current_directory);

    // Reshape ok
    // =========================================================================
    ok = matrix(ok,size(path));

    // That's all
    // =========================================================================
    if modulemode then
        %helps_modules = this_help;
    else
        %helps = this_help;
    end

endfunction
