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

// Load the toolboxes that are marked "autoload"

function result = atomsAutoload()

    result = [];

    // Load Atoms Internals lib if it's not already loaded
    // =========================================================================
    if ~ exists("atomsinternalslib") then
        load("SCI/modules/atoms/macros/atoms_internals/lib");
    end

    // If the autoload system is disabled, no need to continue
    // =========================================================================
    if atomsGetConfig("autoload") == "False" then
        return;
    end

    // Check write access on allusers zone
    // =========================================================================
    ATOMSALLUSERSWRITEACCESS = atomsAUWriteAccess();

    // Save the initial path
    // =========================================================================
    ATOMSINITIALPATH = pwd();

    // Tell atomsLoad() we are in an atomsAutoload() session
    // =========================================================================
    ATOMSAUTOLOAD = %T;

    // Check input parameters
    // =========================================================================
    rhs = argn(2);

    if rhs > 0 then
        error(msprintf(gettext("%s: Wrong number of input arguments: %d expected.\n"),"atomsAutoload",0))
    end

    // Get the list of packages to load
    // =========================================================================

    packages = atomsAutoloadLoad("all");

    // If the list is empty, quit the function
    // =========================================================================
    if isempty(packages) then
        return;
    end

    // Libraries to resume
    // =========================================================================
    libs_resume = [];

    // Get the list of lib [before]
    // =========================================================================
    libs_before = librarieslist();

    // This case can happen :
    //  → Administrator install a package
    //  → User add it to its autoload list.
    //  → Then administrator remove the package
    // =========================================================================

    if or( ~ atomsIsInstalled( [packages(:,1) packages(:,2)] )) then

        // One or more package are not installed : Remove them from the autoload
        // list

        if ATOMSALLUSERSWRITEACCESS then
            section = "all";
        else
            section = "user";
        end

        for i=1:size(packages(:,1),"*")
            if ~ atomsIsInstalled([packages(i,1) packages(i,2)],packages(i,3)) then
                atomsAutoloadDel(packages(i,:),section);
            end
        end

    end

    if or(getscilabmode() == ["NW";"STD"]) then
        global demolist; // Demos list is defined in scilab.start
        global demolistlock;
        if isempty(demolist) then
            if isempty(demolistlock) then
                demolistlock = %t;
                // we load scilab demos only when it is required
                modules = getmodules();
                for i=1:size(modules,"*")
                    if isfile("SCI/modules/"+modules(i)+"/demos/" + modules(i) + ".dem.gateway.sce") then
                        exec("SCI/modules/"+modules(i)+"/demos/" + modules(i) + ".dem.gateway.sce",-1);
                    end
                end
                clear demolistlock;
                clearglobal demolistlock;
            end
        end
    end

    // Load the wanted packages
    // =========================================================================
    result = atomsLoad([packages(:,1) packages(:,2)]);

    // Get the list of lib [after]
    // =====================================================================
    libs_after = librarieslist();

    // Loop on libs_after
    // =====================================================================
    for i=1:size(libs_after,"*")

        if find(libs_after(i) == libs_before) == [] then
            libs_resume = [ libs_resume ; libs_after(i) ];
        end
    end

    // If libs_resume is empty, the job is done
    // =========================================================================
    if isempty(libs_resume) then
        return;
    end

    // Build the resume cmd
    // =========================================================================

    resume_cmd = "[";

    for i=1:size(libs_resume,"*")
        resume_cmd = resume_cmd + libs_resume(i);
        if i<size(libs_resume,"*") then
            resume_cmd = resume_cmd + ",";
        else
            resume_cmd = resume_cmd + "] = resume(";
        end
    end

    for i=1:size(libs_resume,"*")
        resume_cmd = resume_cmd + libs_resume(i);
        if i<size(libs_resume,"*") then
            resume_cmd = resume_cmd + ",";
        else
            resume_cmd = resume_cmd + ");";
        end
    end

    // Exec the resume cmd
    // =========================================================================
    execstr(resume_cmd,"errcatch");

    // Go to the initial location
    // =========================================================================
    chdir(ATOMSINITIALPATH);

endfunction
