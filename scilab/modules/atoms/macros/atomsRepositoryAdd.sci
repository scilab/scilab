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

// Add an URL to the list of repositories, and returns

function nbAdd = atomsRepositoryAdd(url,section)

    // Load Atoms Internals lib if it's not already loaded
    // =========================================================================
    if ~ exists("atomsinternalslib") then
        load("SCI/modules/atoms/macros/atoms_internals/lib");
    end

    // Check write access on allusers zone
    // =========================================================================
    ATOMSALLUSERSWRITEACCESS = atomsAUWriteAccess();

    rhs                    = argn(2);
    nbAdd                = 0;
    repositories         = []; // Column vector that contains user repositories
    currentRepositories  = atomsRepositoryList();
    currentRepositories  = currentRepositories(:,1);
    // from https://gist.github.com/dperini/729294
    // with unicode support, private and local address removed
    valid_url_pattern    = "/^" +..
    ..// protocol identifier
    "(?:(?:https?|ftp):\/\/)" +..
    ..// user:pass authentication
    "(?:\S+(?::\S*)?@)?" +..
    "(?:" +..
    ..// IP address dotted notation octets
    ..// excludes loopback network 0.0.0.0
    ..// excludes reserved space >= 224.0.0.0
    ..// excludes network & broacast addresses
    ..// (first & last IP address of each class)
    "(?:[1-9]\d?|1\d\d|2[01]\d|22[0-3])" +..
    "(?:\.(?:1?\d{1,2}|2[0-4]\d|25[0-5])){2}" +..
    "(?:\.(?:[1-9]\d?|1\d\d|2[0-4]\d|25[0-4]))" +..
    "|" +..
    ..// host name
    "(?:(?:[a-z0-9]+-?)*[a-z0-9]+)" +..
    ..// domain name
    "(?:\.(?:[a-z0-9]+-?)*[a-z0-9]+)*" +..
    ..// TLD identifier
    "(?:\.(?:[a-z]{2,}))" +..
    ")" +..
    ..// port number
    "(?::\d{2,5})?" +..
    ..// resource path
    "(?:\/[^\s]*)?" +..
    "$/i";


    // Check number of input arguments
    // =========================================================================

    if rhs < 1 | rhs > 2 then
        error(msprintf(gettext("%s: Wrong number of input argument: %d to %d expected.\n"),"atomsRepositoryAdd",1,2));
    end

    // Check URLs specified as first input argument
    // =========================================================================

    if type(url) <> 10 then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: String array expected.\n"),"atomsRepositoryAdd",1));
    end

    for i=1:size(url,"*")
        if (part(url(i),1:7) <> "file://") & (~ regexp(url(i),valid_url_pattern,"o") == 1) then
            error(msprintf(gettext("%s: Wrong value for input argument #%d: This ("+url(i)+") is not a valid URL.\n"),"atomsRepositoryAdd",1));
        end
    end

    // Allusers/user management
    //   - If section is equal to "allusers", The repository will added for all users
    //       → SCI/.atoms  : ATOMS system files
    //   -  If section is equal to "user", The repository will added only for the current user
    //       → SCIHOME/atoms : location of the modules & ATOMS system files
    // =========================================================================

    if rhs <= 1 then
        if ATOMSALLUSERSWRITEACCESS then
            section = "allusers";
        else
            section = "user";
        end

    else

        // Process the 2nd input argument : section
        // Allusers can be equal to "user" or "allusers"

        if type(section) <> 10 then
            error(msprintf(gettext("%s: Wrong type for input argument #%d: string expected.\n"),"atomsRepositoryAdd",2));
        end

        if and(section<>["user","allusers"]) then
            error(msprintf(gettext("%s: Wrong value for input argument #%d: ''user'' or ''allusers'' expected.\n"),"atomsRepositoryAdd",2));
        end

        // Check if we have the write access
        if (section=="allusers") & ~ATOMSALLUSERSWRITEACCESS then
            error(msprintf(gettext("%s: You haven''t write access on this directory : %s.\n"),"atomsRepositoryAdd",2,pathconvert(SCI+"/.atoms")));
        end

    end

    // Define the path of the file that will record the change according to
    // the "section" value
    // =========================================================================

    atoms_directory = atomsPath("system",section);

    // Does the atoms_directory exist, if not create it
    // =========================================================================

    if ~ isdir(atoms_directory) then
        mkdir(atoms_directory);
    end

    // Does the SCIHOME/atoms/repositories exist, if yes load it
    // =========================================================================

    if fileinfo(atoms_directory+"repositories") <> [] then
        repositories = mgetl(atoms_directory+"repositories");
    end

    // Loop on each URL specified as input argument
    // =========================================================================

    for i=1:size(url,"*")
        // Add the URL only if it doesn't already exist
        if find( currentRepositories == url(i) ) == [] then
            repositories = [ repositories ; url(i) ];
            nbAdd = nbAdd + 1;
        end
    end

    // Apply changes
    // =========================================================================

    if nbAdd > 0 then

        mputl(repositories, atoms_directory+"repositories");

        // Force reload the different distant TOOLBOXES files
        atomsDESCRIPTIONget(%T);

    end

endfunction
