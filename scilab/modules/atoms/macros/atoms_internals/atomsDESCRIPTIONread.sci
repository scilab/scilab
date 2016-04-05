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

// Return the full description of
//  - TOOLBOXES file present in the differents repositories
//  - DESCRIPTION file present in one package

// DESCRIPTION
// |
// |-- packages                              [1x1 struct]
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
// |-- categories                            [1x1 struct]
// |   |-- Optimization                      [1x1 struct]
// |   |   |-- Linear
// |   |   `-- General
// |   `-- ..
// |
// |-- categories_flat                       [1x1 struct]
//     |-- Optimization - Linear
//     |   |-- packages
//     |   |   `-- ["simplex" "1.0" ; "lolimot" "0.9"]
//     |   `-- label
//     |       `-- [ "Optimization" "Linear" ]
//     |-- Optimization
//     |   |-- packages
//     |   |   `-- ["simplex" "1.0" ; "lolimot" "0.9"]
//     |   `-- label
//     |       `-- [ "Optimization" "Linear" ]
//     `-- Education
//         |-- packages
//         |   `-- ["module_lycee" "1.0" ; "module_lycee" "1.1"]
//         `-- label
//             `-- [ "Education" ]



function description_out = atomsDESCRIPTIONread(file_in,additional)

    // Check input parameters
    // =========================================================================

    rhs  = argn(2);

    if and(rhs <> [1 2])  then
        error(msprintf(gettext("%s: Wrong number of input argument: %d to %d expected.\n"),"atomsDESCRIPTIONread",1,2));
    end

    if regexp( file_in,"/(TOOLBOXES|DESCRIPTION)/") == [] then
        error(msprintf(gettext("%s: Wrong value for input argument #%d: String that contains ''TOOLBOXES'' or ''DESCRIPTION'' expected.\n"),"atomsDESCRIPTIONread",1));
    end

    if rhs < 2 then
        additional = struct();
    else
        if type(additional) <> 17 then
            error(msprintf(gettext("%s: Wrong type for input argument #%d: matrix oriented typed list expected.\n"),"atomsDESCRIPTIONread",2));
        end
    end

    // Init the output argument
    // =========================================================================

    packages         = struct();
    categories_flat  = struct();
    categories       = struct();
    description_out  = struct();

    description_out("packages")        = packages;
    description_out("categories")      = categories;
    description_out("categories_flat") = categories_flat;

    // Operating system detection + Architecture detection
    // =========================================================================
    [OSNAME, ARCH, LINUX, MACOSX, SOLARIS,BSD] = atomsGetPlatform();

    // Start Read the file
    // =========================================================================

    if ~isfile(file_in) then
        error(msprintf(gettext("%s: The file ""%s"" does not exist.\n"), ..
        "atomsDESCRIPTIONread", ..
        file_in));
    end

    lines_in         = mgetl(file_in);
    current_toolbox  = struct();
    current_field    = "";

    if isempty(lines_in) then
        error(msprintf(gettext("%s: The file ""%s"" is empty.\n"), ..
        "atomsDESCRIPTIONread", ..
        file_in));
    end

    winId = atomsOpenProgressBar(_("Updating Atoms modules database..."), %t)
    for i=1:(size(lines_in,"*")+1)

        atomsUpdateProgressBar(winId, i / size(lines_in,"*"));

        // First case : new field, or file all read
        if ((i == (size(lines_in,"*")+1)) | (regexp(lines_in(i),"/^[a-zA-Z0-9]*:\s/","o") == 1)) then

            // subcase of First case: new toolbox, or file all read: register the latest toolbox
            if ((i == (size(lines_in,"*")+1)) | (regexp(lines_in(i),"/^Toolbox:\s/","o") == 1)) then

                if and(isfield(current_toolbox,["Toolbox";"Version"])) then

                    if  ~ isfield(packages,current_toolbox("Toolbox")) then
                        // This is the first version of the package
                        this_toolbox = struct();
                    else
                        // Get the version list of this package
                        this_toolbox = packages(current_toolbox("Toolbox"));
                    end

                    // Register the current toolbox : Check the mandatory fields
                    missingfield = atomsCheckFields( current_toolbox );
                    if ~ isempty(missingfield) then
                        atomsCloseProgressBar(winId);
                        error(msprintf(gettext("%s: The file ""%s"" is not well formated, the toolbox ""%s - %s"" does not contain the %s field\n"), ..
                        "atomsDESCRIPTIONread",..
                        file_in,current_toolbox("Toolbox"),..
                        current_toolbox("Version"),..
                        missingfield));
                    end

                    // Register the current toolbox : Check the scilab version
                    // comptability
                    if atomsIsCompatible(current_toolbox("ScilabVersion")) then
                        if isfield(current_toolbox,"PackagingVersion") then
                            current_toolbox("Version") = current_toolbox("Version") + "-" + current_toolbox("PackagingVersion");
                            this_toolbox(current_toolbox("Version")) = current_toolbox;
                        else
                            this_toolbox(current_toolbox("Version")) = current_toolbox;
                        end
                    end

                    // Register the current toolbox : Fill the packages struct
                    packages(current_toolbox("Toolbox")) = this_toolbox;

                    if i == (size(lines_in,"*")+1) then
                        break
                    end                
                end

                // Reset the current_toolbox struct
                current_toolbox  = struct();
            end

            // process field
            current_field_length           = regexp(lines_in(i),"/:\s/","o")
            current_field                  = part(lines_in(i),1:current_field_length-1);
            current_value                  = part(lines_in(i),current_field_length+2:length(lines_in(i)));

            // process binary files
            if regexp(current_field,"/^(windows|linux|macosx|solaris|bsd)(32|64)?(Url|Name|Md5|Sha1|Id)$/","o")<>[] then
                // This field doesn't concern this platform => Next line
                if regexp(current_field,"/^"+OSNAME+ARCH+"/","o")==[] then
                    continue;
                else
                    current_field = "binary"+part(current_field,length(OSNAME+ARCH)+1:length(current_field));
                end
            end

            // process URLs
            if isfield(additional,"repository") & ..
                ( regexp(current_field,"/^(source|binary|windows|linux|macosx|solaris|bsd)(32|64)?Url$/","o")<>[] | current_field=="URL" ) & ..
                regexp(current_value,"/^(https?|ftps?|file):\/\//","o")==[] then
                current_value = additional("repository") + current_value;
            end

            current_toolbox(current_field) = current_value;

            // Category management
            if current_field == "Category" then
                if ~ isfield(categories_flat,current_value) then
                    [categories,categories_flat] = atomsCreateCategory(categories,categories_flat,current_value)
                end
                if and(isfield(current_toolbox,["Toolbox";"Version"])) then
                    categories_flat = atomsAddPackage2Cat( categories_flat , [current_toolbox("Toolbox") current_toolbox("Version")],current_value);
                else
                    atomsCloseProgressBar(winId);
                    error(msprintf(gettext("%s: name and version are not both defined\n"),"atomsDESCRIPTIONread"));
                end
            end

            continue;
        end

        // Second case : Current field continuation
        if regexp(lines_in(i),"/^\s/","o") == 1 then
            current_value = part(lines_in(i),2:length(lines_in(i)));
            current_toolbox(current_field)($+1) =  current_value;

            // Category management
            if current_field == "Category" then
                if ~ isfield(categories_flat,current_value) then
                    [categories,categories_flat] = atomsCreateCategory(categories,categories_flat,current_value)
                end
                if and(isfield(current_toolbox,["Toolbox";"Version"])) then
                    categories_flat = atomsAddPackage2Cat( categories_flat , [current_toolbox("Toolbox") current_toolbox("Version")],current_value);
                else
                    atomsCloseProgressBar(winId);
                    error(msprintf(gettext("%s: name and version are not both defined\n"),"atomsDESCRIPTIONread"));
                end
            end

            continue;
        end

        // Third case : blank line
        if length(lines_in(i)) == 0 then
            continue;
        end

        // Fourth case: comment
        if regexp(lines_in(i),"/^\/\//","o") == 1 then
            continue;
        end

        // Else Error
        atomsCloseProgressBar(winId);
        error(msprintf(gettext("%s: The file ''%s'' is not well formated at line %d\n"),"atomsDESCRIPTIONread",file_in,i));

    end

    description_out("packages")        = packages;
    description_out("categories")      = categories;
    description_out("categories_flat") = categories_flat;
    atomsCloseProgressBar(winId);

endfunction

// =============================================================================
// atomsCreateCategory
// =============================================================================

function [cat_out , cat_flat_out ] = atomsCreateCategory(cat_in,cat_flat_in,cat_id)

    category_main = "";
    category_sub  = "";
    cat_flat_out  = cat_flat_in;
    cat_out       = cat_in;

    // Build the skeleton of the category
    cat_struct             = struct();
    cat_struct("label")    = [];
    cat_struct("packages") = [];
    cat_struct("is_main")  = %T;

    // Is this category a main category or a sub category

    pattern_index = regexp(cat_id,"/\s-\s/","o");

    if pattern_index <> [] then

        // Sub category
        category_main         = part(cat_id,1:pattern_index-1);
        category_sub          = part(cat_id,pattern_index+3:length(cat_id) );
        cat_struct("label")   = [ category_main  category_sub ];
        cat_struct("is_main") = %F;

    else
        // Main category
        category_main = cat_id;
        cat_struct("label")   = [ category_main ];
        cat_struct("is_main") = %T;

    end

    cat_flat_out(cat_id)  = cat_struct;

    if isfield(categories,category_main) then
        if category_sub <> "" then
            subcategories          = cat_out(category_main);
            subcategories          = [ subcategories ; category_sub ];
            cat_out(category_main) = subcategories;
        end
    else
        if category_sub == "" then
            cat_out(category_main) = [];
        else
            cat_out(category_main) = category_sub;
        end
    end

    if ~cat_struct("is_main") & ~isfield(cat_flat_out,category_main) then
        [cat_out , cat_flat_out ] = atomsCreateCategory(cat_out,cat_flat_out,category_main)
    end

endfunction

// =============================================================================
// atomsAddPackage2Cat
// =============================================================================

function cat_flat_out = atomsAddPackage2Cat( cat_flat_in , package , category)

    cat_flat_out  = cat_flat_in;

    if ~ isfield( cat_flat_out , category ) then
        error(msprintf(gettext("%s: Wrong value for input argument #%d: ''%s'' is not a registered category"),"atomsAddPackage2Cat",2,category));
    end

    cat_struct             = cat_flat_out(category);
    package_mat            = [ cat_struct("packages") ; package ];
    cat_struct("packages") = package_mat;
    cat_flat_out(category) = cat_struct;

    if ~ cat_struct("is_main") then
        label_mat    = cat_struct("label");
        cat_flat_out = atomsAddPackage2Cat( cat_flat_out , package , label_mat(1))
    end

endfunction

// =============================================================================
// atomsCheckFields
// =============================================================================

function field = atomsCheckFields( module )

    field = "";

    mandatory = [             ..
    "Toolbox"           ; ..
    "Title"             ; ..
    "Summary"           ; ..
    "Version"           ; ..
    "Author"            ; ..
    "Maintainer"        ; ..
    "Category"          ; ..
    "Entity"            ; ..
    "License"           ; ..
    "ScilabVersion"     ; ..
    "Depends"           ; ..
    "Date"              ];

    for i=1:size(mandatory,"*")
        if ~ isfield(module,mandatory(i)) then
            field = mandatory(i);
            return;
        end
    end

endfunction
