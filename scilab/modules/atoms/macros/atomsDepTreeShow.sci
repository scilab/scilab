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

// End user function

// Input arguments :

//   name : . technical name of the package
//          . single string
//          . mandatory

//   version : . version of the package
//             . single string
//             . optional

// Output arguments :

function atomsDepTreeShow(package)

    // Load Atoms Internals lib if it's not already loaded
    // =========================================================================
    if ~ exists("atomsinternalslib") then
        load("SCI/modules/atoms/macros/atoms_internals/lib");
    end

    rhs = argn(2);

    // Check number of input arguments
    // =========================================================================

    if rhs <> 1 then
        error(msprintf(gettext("%s: Wrong number of input argument: %d expected.\n"),"atomsDepTreeShow",1));
    end

    // Check input parameters type
    // =========================================================================

    if type(package) <> 10 then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: String array expected.\n"),"atomsDepTreeShow",1));
    end

    if size(package(1,:),"*") > 2 then
        error(msprintf(gettext("%s: Wrong size for input argument #%d: 1x1 or 1x2 string matrix expected.\n"),"atomsDepTreeShow",1));
    end

    // Get the dependency tree
    // =========================================================================

    if size(package(1,:),"*") == 2 then
        tree = atomsDepTreeExt(package(1),package(2));
    else
        tree = atomsDepTreeExt(package(1));
    end

    situation = struct();
    situation("current_level")  = 1;

    mprintf("\n");
    atomsDispTree(tree,situation)

endfunction


function atomsDispTree(tree,situation)

    fields      = getfield(1,tree);
    fields(1:2) = [];

    current_level = situation("current_level");
    situation("level"+string(current_level)+"_number") = size(fields,"*");
    situation("level"+string(current_level)+"_pos")    = 1;

    for i=1:size(fields,"*")

        this_package_details = tree(fields(i));

        for j=1:current_level-1
            if situation("level"+string(j)+"_pos") < situation("level"+string(j)+"_number") then
                mprintf("|   ");
            else
                mprintf("    ");
            end
        end

        if current_level == 1 then
            mprintf("    ");
        elseif situation("level"+string(current_level)+"_pos") == situation("level"+string(current_level)+"_number") then
            mprintf("`-- ");
        else
            mprintf("|-- ");
        end

        mprintf("%s - %s\n",this_package_details("Toolbox"),this_package_details("Version"));

        if isfield(this_package_details,"DependencyTree") then

            next_situation                  = situation;
            next_situation("current_level") = current_level + 1;
            this_package_deptree            = this_package_details("DependencyTree");
            atomsDispTree(this_package_deptree,next_situation);

        end

        situation("level"+string(current_level)+"_pos") = situation("level"+string(current_level)+"_pos") + 1;

    end

endfunction
