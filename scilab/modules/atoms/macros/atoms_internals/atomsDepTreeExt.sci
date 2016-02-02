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

// Input arguments :

//   name : . technical name of the package
//          . single string
//          . mandatory

//   version : . version of the package
//             . single string
//             . optional

//   tree_in : . Tree that will be concatenated in the output tree
//             . struct
//             . optional


// Output arguments :

//   tree_out : . Dependency tree of the package
//              . struct
//              . mandatory
//              . Example :
//                   tree_out  =
//                   toolbox_5 - 1.0: [1x1 struct]
//                   toolbox_4 - 1.0: [1x1 struct]
//                   toolbox_2 - 1.3: [1x1 struct]
//                   toolbox_1 - 1.9: [1x1 struct]

//   version_out : . version of the package
//                 . single string
//                 . optional

function [tree_out,version_out] = atomsDepTreeExt(name,version)

    lhs = argn(1);
    rhs = argn(2);

    // Check number of input arguments
    // =========================================================================

    if (rhs < 1) | (rhs > 2) then
        error(msprintf(gettext("%s: Wrong number of input argument: %d to %d expected.\n"),"atomsDepTreeExt",1,2));
    end

    // Check input parameters type
    // =========================================================================

    if type(name) <> 10 then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: string expected.\n"),"atomsDepTreeExt",1));
    end

    if (rhs>=2) & (type(version) <> 10) then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: string expected.\n"),"atomsDepTreeExt",2));
    end

    // Check input parameters dimensions
    // =========================================================================

    if size(name) <> 1 then
        error(msprintf(gettext("%s: Wrong size for input argument #%d: string expected.\n"),"atomsDepTreeExt",1));
    end

    if (rhs>=2) & (size(name)<>1) then
        error(msprintf(gettext("%s: Wrong size for input argument #%d: string expected.\n"),"atomsDepTreeExt",1));
    end

    // If version not define, version is the list of version compatible with
    // the current version of Scilab
    // =========================================================================

    if (rhs<2) | ((rhs>=2) & (version=="")) then
        version = atomsCompatibleVersions(name);
    end

    // Loop on versions
    // =========================================================================

    for i=1:size(version,"*")

        this_package_details = atomsToolboxDetails([name,version(i)]);
        tree_out(name+" - "+version(i)) = this_package_details;

        if lhs>1 then
            version_out = version(i);
        end

        // Now, loop on dependencies
        // =========================================================================

        this_package_deptree = struct();

        if isfield(this_package_details,"Depends") & (this_package_details("Depends") ~= "") then
            dependencies = this_package_details("Depends");
        else
            dependencies = [];
        end

        for j=1:size(dependencies,"*")

            this_dependency_success = %F;

            // Split dependencies to get
            //  - direction ("=",">=",">","<=","<","~")
            //  - dependence name
            //  - dependence version (optional)

            this_dependency_tab     = stripblanks(strsplit(dependencies(j),regexp(stripblanks(dependencies(j)),"/\s/")));

            this_dependency_dir     = this_dependency_tab(1);
            this_dependency_name    = this_dependency_tab(2);
            this_dependency_version = this_dependency_tab(3);

            // List versions of the dependency we can test

            if this_dependency_dir     == "="  then
                this_dependency_list = atomsGetVersions(this_dependency_name,this_dependency_version,this_dependency_version,%T,%T);

            elseif this_dependency_dir == "~"  then
                this_dependency_list = atomsGetVersions(this_dependency_name);

            elseif this_dependency_dir == ">=" then
                this_dependency_list = atomsGetVersions(this_dependency_name,this_dependency_version,"",%T,%T);

            elseif this_dependency_dir == "<=" then
                this_dependency_list = atomsGetVersions(this_dependency_name,"",this_dependency_version,%T,%T);

            elseif this_dependency_dir == ">" then
                this_dependency_list = atomsGetVersions(this_dependency_name,this_dependency_version,"",%F,%F);

            elseif this_dependency_dir == "<" then
                this_dependency_list = atomsGetVersions(this_dependency_name,"",this_dependency_version,%F,%F);

            end

            for k=1:size(this_dependency_list,"*")

                tree = atomsDepTreeExt(this_dependency_name,this_dependency_list(k));

                // Dependency Tree fails

                if (type(tree) == 4) & (~ tree) then
                    continue;
                end

                // Dependency Tree OK

                if type(tree) == 17 then
                    this_package_deptree    = atomsCatTree(this_package_deptree,tree);
                    this_dependency_success = %T;
                    break;
                end

            end

            if ~  this_dependency_success then
                tree_out = %F;
                break;
            end

        end

        if type(tree_out)==17 then
            this_package_details("DependencyTree") = this_package_deptree;
            tree_out(name+" - "+version(i))        = this_package_details;
            return;
        end

    end

endfunction
