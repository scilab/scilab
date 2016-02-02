// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2006-2008 - INRIA - Pierre MARECHAL <pierre.marechal@inria.fr>
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function del_help_chapter(help_to_del,modulemode)

    // =========================================================================
    // Author : Pierre MARECHAL
    // Scilab team
    // Copyright INRIA
    // Date : August 1st 2006
    //
    // del_help_chapter(title,modulemode)
    //
    // Parameters
    //
    // title : a character string :
    //   - the help chapter title or
    //   - the module name
    //
    // Description
    //
    // This function deletes a entry in the helps list.
    // =========================================================================

    global %helps;
    global %helps_modules;

    // Check input arguments
    // -------------------------------------------------------------------------
    [lhs,rhs] = argn(0);
    if (rhs<1) | (rhs>2) then error(39); end
    if (rhs == 1) then modulemode=%F; end
    if type(help_to_del) <> 10 then error(55,1); end

    // Is the first parameter a module name ?
    // -------------------------------------------------------------------------

    is_module_name = %F;
    module_list    = getmodules();
    is_module_name = find(module_list == help_to_del) <> []

    // If yes, add the path
    // -------------------------------------------------------------------------

    if is_module_name then
        help_to_del = pathconvert(SCI+"/modules/"+help_to_del+"/help/"+getlanguage(),%F,%F);
    end

    // Now, check if help_to_del is present in the %helps_modules matrix
    // -------------------------------------------------------------------------

    if modulemode then

        k1 = find( %helps_modules(:,2) == help_to_del);
        if k1 <> [] then
            %helps_modules(k1,:) = [];
        end

    else

        k1 = find( %helps(:,2) == help_to_del);
        if k1 <> [] then
            %helps(k1,:) = [];
        end

    end

    if strindex(stripblanks(help_to_del),"(*)") == (length(stripblanks(help_to_del)) - 2) then

        help_to_del = strsubst(help_to_del,"(*)","");
        help_to_del = stripblanks(help_to_del);

        for i=1:100
            if modulemode then
                k2 = find( %helps_modules(:,2) == help_to_del+" ("+string(i)+")" );
                if k2 <> [] then
                    %helps_modules(k2,:) = [];
                end
            else
                k2 = find( %helps(:,2) == help_to_del+" ("+string(i)+")" );
                if k2 <> [] then
                    %helps(k2,:) = [];
                end
            end
        end
    end

endfunction
