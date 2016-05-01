// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2007 - INRIA - Allan CORNET
// Copyright (C) 2012 - Scilab Enterprises - Antoine ELIAS
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.


function files= listfiles(paths,flag,flagexpand)

    // list the files in a set of directories
    // files= listfiles(paths [,flag,flagexpand])
    // paths           : a string matrix giving a set of pathnames (eventually ended by a pattern built with  * )
    // flag,flagexpand : boolean optional parameters. (default value  %t ).
    // files           : a string matrix

    [lhs,rhs]=argn(0)
    if rhs < 1  then paths="./" ; end
    if rhs <= 1 then flag = %t ; end
    if rhs <= 2 then flagexpand = %t ; end
    files=[];

    // Stripblanks paths
    paths = stripblanks(paths);

    // list files of the current directory
    paths(find(paths=="")) = "./";

    if flag == %t then
        paths = pathconvert(paths,%f,flagexpand);
    end

    // list files of the root directory
    if getos() <> "Windows" then
        paths(find(paths=="")) = "/";
    end

    DIR_SEPARATOR=filesep();

    bMulti = %f;
    if size(paths,"*") > 1 then
        bMulti = %t;
    end

    for i=1:size(paths,"*")  // Loop on the file/path list
        [path,fname,extension]=fileparts(paths(i)); // Retrieve the information
        // about the file
        if isdir(paths(i)) then // It is a directory then returns all the file in the dir
            path = paths(i) + filesep();
            fname="*";
            if bMulti then
                if getos() == "Windows" // Don't want this case under Linux/Unix
                    // Windows will return the file toto even if you provided toto.*
                    extension=".*";
                end
            else
                extension="";
            end
        else
            // It is a file
            if path == "" then path="./",end;
            if getos() == "Windows" // Don't want this case under Linux/Unix
                // Windows will return the file toto even if you provided toto.*
                if extension == "" then extension=".*",end;
            end

            if fname == "" then fname="*",end;
        end

        filesi=findfiles(path,fname+extension);

        if filesi<>[] then
            filesi=filesi($:-1:1);
            // prepend with the path, if required
            // when listing a full directory, path is not prepended
            dirs=paths(i);
            if part(dirs,length(dirs))<>DIR_SEPARATOR then
                if isdir(dirs) then
                    if bMulti then //yes
                        with_dir=%t;
                        dirs = dirs + filesep();
                    else
                        with_dir=%f;
                    end
                else                //no
                    k=strindex(dirs,DIR_SEPARATOR);
                    if k==[] then
                        with_dir=%f;
                    else
                        dirs=part(dirs,1:k($));
                        with_dir=%t;
                    end
                end
            else
                with_dir=%f;
            end // if part(dirs,length(dirs) ...

            if with_dir then
                filesi = dirs+filesi;
            end

            files=[files;filesi];
        end // if filesi<>[] then
    end // for i=1:size(paths,'*')
endfunction
