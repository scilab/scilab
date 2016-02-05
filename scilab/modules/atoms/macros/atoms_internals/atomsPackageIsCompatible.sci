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

// Returns True if the package "parent_name - version_name" can accept
// the version "child_version" of the needed package "child_name"
// No matter whether if "parent_name - parent_version" is
// installed or not

// Input arguments :
//
//   parent_name    : . String Array
//                    . mandatory
//
//   parent_version : . String Array
//                    . mandatory
//
//   child_name     : . Single String
//                    . mandatory
//
//   child_version  : . Single String
//                    . mandatory

// Output arguments :
//
//   result : . Boolean Array

function result = atomsPackageIsCompatible(parent_name,parent_version,child_name,child_version)

    rhs    = argn(2);
    result = [];

    // Check number of input arguments
    // =========================================================================
    if rhs <> 4 then
        error(msprintf(gettext("%s: Wrong number of input argument: %d expected.\n"),"atomsPackageIsCompatible",4));
    end

    // Check input parameter type
    // =========================================================================

    if type(parent_name) <> 10 then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: string expected.\n"),"atomsPackageIsCompatible",1));
    end

    if type(parent_version) <> 10 then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: string expected.\n"),"atomsPackageIsCompatible",2));
    end

    if type(child_name) <> 10 then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: string expected.\n"),"atomsPackageIsCompatible",3));
    end

    if type(child_version) <> 10 then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: string expected.\n"),"atomsPackageIsCompatible",4));
    end

    // Check input parameter dimension
    // =========================================================================

    if size(parent_name,"*") <> size(parent_version,"*") then
        error(msprintf(gettext("%s: Incompatible input arguments #%d and #%d: Same sizes expected.\n"),"atomsPackageIsCompatible",1,2));
    end

    if size(child_name,"*") <> 1 then
        error(msprintf(gettext("%s: Wrong size for input argument #%d: string expected.\n"),"atomsPackageIsCompatible",3));
    end

    if size(child_version,"*") <> 1 then
        error(msprintf(gettext("%s: Wrong size for input argument #%d: string expected.\n"),"atomsPackageIsCompatible",4));
    end

    // Loop on parents
    // =========================================================================

    for i=1:size(parent_name,"*")

        // Get the dependency field of this package
        // =========================================================================

        parent_deps = atomsToolboxDetails([parent_name(i),parent_version(i)],"Depends");

        if isempty(parent_deps)
            result = [ result ; %F ];
            continue;
        end

        // get the line corresponding to child_name
        // =========================================================================

        parent_deps_this_child = parent_deps(grep(parent_deps,"/\s"+child_name+"\s/","r"));

        if isempty(parent_deps_this_child) then
            result = [ result ; %F ];
            continue;
        end

        // get the line corresponding to child_name
        // =========================================================================

        this_dependency_tab     = stripblanks(strsplit(parent_deps_this_child,regexp(stripblanks(parent_deps_this_child),"/\s/")));

        this_dependency_dir     = this_dependency_tab(1);
        this_dependency_name    = this_dependency_tab(2);
        this_dependency_version = this_dependency_tab(3);

        // List versions of the dependency we can test

        if (this_dependency_dir == "~") | ..
            ( (this_dependency_dir == "=" ) & (this_dependency_version == child_version ) ) | ..
            ( (this_dependency_dir == ">=") & ( atomsVersionCompare(child_version,this_dependency_version) >= 0 ) ) | ..
            ( (this_dependency_dir == ">" ) & ( atomsVersionCompare(child_version,this_dependency_version) >  0 ) ) | ..
            ( (this_dependency_dir == "<=") & ( atomsVersionCompare(child_version,this_dependency_version) <= 0 ) ) | ..
            ( (this_dependency_dir == "<" ) & ( atomsVersionCompare(child_version,this_dependency_version) <  0 ) ) then

            result = [ result ; %T ];

        else
            result = [ result ; %F ];
        end

    end

    result = matrix(result,size(parent_name) );

endfunction
