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

// The description struct looks like that :

// DESCRIPTION
// |
// |-- packages
// |   |-- toolbox_1                         [1x1 struct]
// |   |   |-- 2.0                           [1x1 struct]
// |   |   |   |-- Toolbox: "toolbox_2"
// |   |   |   |-- Title: "Toolbox Test 2"
// |   |   |   |-- Version: "2.0"
// |   |   |   `-- ..
// |   |   `-- 1.0                           [1x1 struct]
// |   |   |   |-- Toolbox: "toolbox_2"
// |   |   |   |-- Title: "Toolbox Test 2"
// |   |   |   |-- Version: "1.0"
// |   |   |   `-- ..
// |   |-- module_lycee
// |   `-- ..
// |
// |-- categories
// |-- categories_flat

function atomsDESCRIPTIONwrite(description_in,file_out)

    // Check input parameters number
    // =========================================================================

    rhs  = argn(2);

    if rhs <> 2 then
        error(msprintf(gettext("%s: Wrong number of input argument: %d expected.\n"),"atomsDESCRIPTIONwrite",2));
    end

    // Check input parameters type
    // =========================================================================

    if type(description_in) <> 17 then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: Struct expected.\n"),"atomsDESCRIPTIONwrite",1));
    end

    if type(file_out) <> 10 then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: string expected.\n"),"atomsDESCRIPTIONwrite",1));
    end

    // Check input parameters size
    // =========================================================================

    if size(file_out,"*") <> 1 then
        error(msprintf(gettext("%s: Wrong size for input argument #%d: string expected.\n"),"atomsDESCRIPTIONwrite",2));
    end

    // Build the string matrix
    // =========================================================================

    str_mat = [];

    if isfield(description_in,"packages") then

        package_packages   = description_in("packages");
        package_names      = getfield(1,package_packages);
        package_names(1:2) = [];

        for i=1:size(package_names,"*")

            package_versions_struct = package_packages(package_names(i));

            if type(package_versions_struct) <> 17 then
                error(msprintf(gettext("%s: Wrong value for input argument #%d: The matrix oriented typed list is not well formatted.\n"),"atomsDESCRIPTIONwrite",1));
            end

            package_versions      = getfield(1,package_versions_struct);
            package_versions(1:2) = [];

            for j=1:size(package_versions,"*")

                str_mat = [ str_mat ; "// =============================================================================" ];

                this_package = package_versions_struct(package_versions(j));

                this_package_fields      = getfield(1,this_package);
                this_package_fields(1:2) = [];

                for k=1:size(this_package_fields,"*")

                    this_field = this_package_fields(k);
                    this_value = this_package(this_package_fields(k));

                    if type(this_value)<>10 then
                        continue;
                    end

                    if (this_field=="Version") & ~isempty(strindex(this_value(1),"-")) then
                        str_mat = [ str_mat ; ..
                        sprintf("Version: %s",part(this_value(1),1:strindex(this_value(1),"-")-1)) ; ..
                        sprintf("PackagingVersion: %s",part(this_value(1),strindex(this_value(1),"-")+1:length(this_value(1)))) ];
                        continue;
                    elseif(this_field=="PackagingVersion")
                        continue;
                    else
                        str_mat = [ str_mat ; this_field+": "+this_value(1) ];
                    end

                    if size(this_value,"*") == 1 then
                        continue;
                    end

                    for l=2:size(this_value,"*")
                        str_mat = [ str_mat ; " "+this_value(l) ];
                    end

                end

            end

        end
    end

    // Put the string matrix in the wanted file
    // =========================================================================

    if isempty(str_mat) then
        mdelete(file_out);

    elseif ~ mputl(str_mat,file_out) then
        error(msprintf(gettext("%s: The file ''%s'' cannot be written.\n"),"atomsDESCRIPTIONwrite",file_out));

    end

endfunction
