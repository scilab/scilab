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

// Show information on a package

function atomsShow(package)

    // Load Atoms Internals lib if it's not already loaded
    // =========================================================================
    if ~ exists("atomsinternalslib") then
        load("SCI/modules/atoms/macros/atoms_internals/lib");
    end

    rhs = argn(2);

    // Check number of input arguments
    // =========================================================================

    if rhs <> 1 then
        error(msprintf(gettext("%s: Wrong number of input argument: %d expected.\n"),"atomsShow",1));
    end

    // Check input parameters type
    // =========================================================================

    if type(package) <> 10 then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: String array expected.\n"),"atomsShow",1));
    end

    if size(package(1,:),"*") > 2 then
        error(msprintf(gettext("%s: Wrong size for input argument #%d: 1x1 or 1x2 string matrix expected.\n"),"atomsShow",1));
    end

    // Remove leading and trailing parameters
    // =========================================================================
    package = stripblanks(package);

    // If the version is not mentioned, complete with an empty string
    // =========================================================================

    if size(package,"*") == 1 then
        package = [ package "" ];
    end

    // Check if it's a valid package
    // =========================================================================

    if ~atomsIsInstalled(package) & ~atomsIsPackage(package) then
        if isempty(package(2)) then
            module_full_name = package(1);
        else
            module_full_name = package(1)+" - "+package(2);
        end

        atomsError("error",msprintf(gettext("%s: The package %s is not available.\n"),"atomsShow",module_full_name));
    end

    // If version is not mentioned, the Most Recent Version is used
    // =========================================================================
    if isempty(package(2)) then
        package(1,2) = atomsGetMRVersion(package(1));
    end

    // Get the details of this package
    // =========================================================================

    details = atomsToolboxDetails(package);

    fields_map = [];
    fields_map = [ fields_map ; "Toolbox"        gettext("Package")        ];
    fields_map = [ fields_map ; "Title"          gettext("Title")          ];
    fields_map = [ fields_map ; "Summary"        gettext("Summary")        ];
    fields_map = [ fields_map ; "Version"        gettext("Version")        ];
    fields_map = [ fields_map ; "Depends"        gettext("Depend")         ];
    fields_map = [ fields_map ; "Category"       gettext("Category(ies)")  ];
    fields_map = [ fields_map ; "Author"         gettext("Author(s)")      ];
    fields_map = [ fields_map ; "Maintainer"     gettext("Maintainer(s)")  ];
    fields_map = [ fields_map ; "Entity"         gettext("Entity")         ];
    fields_map = [ fields_map ; "WebSite"        gettext("WebSite")        ];
    fields_map = [ fields_map ; "License"        gettext("License")        ];
    fields_map = [ fields_map ; "ScilabVersion"  gettext("Scilab Version") ];

    fields_map = [ fields_map ; "Status"         gettext("Status")         ];

    if atomsIsInstalled(package) then
        fields_map = [ fields_map ; "InstallAutomaticaly" gettext("Automatically Installed")];
        fields_map = [ fields_map ; "installPath"         gettext("Install Directory")];
    end

    fields_map = [ fields_map ; "Description"    gettext("Description")    ];

    // Show it
    // =========================================================================

    max_field_len = max( length(fields_map(:,2)) );
    fields_map(:,2) = justify(fields_map(:,2),"r");

    for i=1:size(fields_map(:,1),"*")

        value = "";

        //
        // Status
        //

        if fields_map(i,1)=="Status" then
            if atomsIsInstalled(package) then
                value = "Installed";
            else
                value = "Not installed";
            end
        end

        //
        // Automatically Installed ?
        //

        if fields_map(i,1)=="InstallAutomaticaly" then
            if atomsGetInstalledStatus(package) == "A" then
                value = "yes";
            else
                value = "no";
            end
        end

        //
        // Scilab Version
        //

        if fields_map(i,1)=="ScilabVersion" then
            if regexp( details(fields_map(i,1)) , "/^~/" , "o" )<>[] then
                value = "any";
            else
                value = details(fields_map(i,1));
            end
        end

        //
        // Dependences
        //

        if fields_map(i,1)=="Depends" then
            value = dep2str(details(fields_map(i,1)));
        end

        //
        // Other
        //

        if isempty(value) then
            value = details(fields_map(i,1));
        end

        for j=1:size(value,"*")

            if j==1 then
                mprintf("% "+string(max_field_len)+"s : %s\n",fields_map(i,2),value(j))

            else
                mprintf("% "+string(max_field_len)+"s   %s\n","",value(j))
            end
        end
    end

endfunction

// =============================================================================
// string = dep2str(string)
//
// Convert a technical dependence string (For ex. : ">= toolbox_1 1.3") to a
// display dependence string  (For ex. : "toolbox_1 (>= 1.3)" )
//
// =============================================================================

function str = dep2str(dep)

    str = [];

    if isempty(dep) then
        return;
    end

    for i=1:size(dep,"*")

        this_dep = dep(i);

        // direction part
        this_dep         = stripblanks(this_dep);
        direction_length = regexp(this_dep,"/\s/","o");
        direction        = stripblanks(part(this_dep,1:direction_length-1));

        // name part
        this_dep         = stripblanks(part(this_dep,direction_length+1:length(this_dep)));
        name_length      = regexp(this_dep,"/\s/","o");
        name             = part(this_dep,1:name_length-1);

        // version part
        version          = stripblanks(part(this_dep,name_length:length(this_dep)));

        this_str         = name+" ";

        if direction == "~" then
            this_str = this_str + "(Any version)";
        else
            this_str = this_str + "("+direction+" "+version+")";
        end

        str = [ str ; this_str ];

    end

endfunction
