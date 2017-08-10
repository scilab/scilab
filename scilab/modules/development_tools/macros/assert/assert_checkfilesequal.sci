// Copyright (C) 2008-2009 - INRIA - Michael Baudin
// Copyright (C) 2010 - 2011 - DIGITEO - Michael Baudin
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function [flag,errmsg] = assert_checkfilesequal ( varargin )
    //   Check two files are equal.

    [lhs,rhs]=argn()
    if ( and ( rhs <> [ 2 3 ] ) ) then
        errmsg = sprintf ( gettext ( "%s: Wrong number of input arguments: %d to %d expected.") , "assert_checkfilesequal" , 2 , 3 )
        error(errmsg)
    end
    //
    // Get input arguments
    filecomp = varargin(1)
    fileref = varargin(2)
    if ( rhs <= 2 ) then
        compfun = []
    else
        compfun = varargin(3)
    end
    //
    // Check types of variables
    if ( typeof(filecomp) <> "string" ) then
        errmsg = sprintf ( gettext ( "%s: Wrong type for input argument #%d: Matrix of strings expected.\n") , "assert_checkfilesequal" , 1 )
        error(errmsg)
    end
    if ( typeof(fileref) <> "string" ) then
        errmsg = sprintf ( gettext ( "%s: Wrong type for input argument #%d: Matrix of strings expected.\n") , "assert_checkfilesequal" , 2 )
        error(errmsg)
    end
    if ( compfun <> [] ) then
        if ( and ( typeof(compfun) <> [ "function" "list" ] ) ) then
            errmsg = sprintf ( gettext ( "%s: Expected type ""%s"" or ""%s"" for input argument %s #%d, but got %s instead.") , "assert_checkfilesequal" , "function" , "list" , "compfun" , 3 , typeof(compfun) )
            error(errmsg)
        end
    end
    //
    // Check sizes of variables
    if ( size(filecomp,"*") <> 1 ) then
        errmsg = sprintf ( gettext ( "%s: Wrong size for input argument #%d: %d-by-%d matrix expected.\n") , "assert_checkfilesequal" , 1 , 1 , 1 )
        error(errmsg)
    end
    if ( size(fileref,"*") <> 1 ) then
        errmsg = sprintf ( gettext ( "%s: Wrong size for input argument #%d: %d-by-%d matrix expected.\n") , "assert_checkfilesequal" , 2 , 1 , 1 )
        error(errmsg)
    end
    //
    // Test if both files exist on disk
    if ( fileinfo(filecomp) == [] ) then
        flag = %f
        errmsg = sprintf ( gettext ( "%s: The file %s does not exist.\n") , "assert_checkfilesequal" , filecomp )
        if ( lhs < 2 ) then
            assert_generror ( errmsg )
        else
            return
        end
    end
    if ( fileinfo(fileref) == [] ) then
        flag = %f
        errmsg = sprintf ( gettext ( "%s: The file %s does not exist.\n") , "assert_checkfilesequal" , fileref )
        if ( lhs < 2 ) then
            assert_generror ( errmsg )
        else
            return
        end
    end
    //
    // Open files
    [fdc,err] = mopen(filecomp,"r")
    if ( err <> 0 ) then
        flag = %f
        errmsg = sprintf ( gettext ( "%s: Cannot open file %s.\n") , "assert_checkfilesequal" , filecomp )
        if ( lhs < 2 ) then
            assert_generror ( errmsg )
        else
            return
        end
    end
    [fdr,err] = mopen(fileref,"r")
    if ( err <> 0 ) then
        flag = %f
        errmsg = sprintf ( gettext ( "%s: Cannot open file %s.\n") , "assert_checkfilesequal" , fileref )
        if ( lhs < 2 ) then
            assert_generror ( errmsg )
        else
            return
        end
    end
    //
    // Get contents
    txtcomp = mgetl(fdc)
    txtref = mgetl(fdr)
    //
    // Compare contents
    if ( compfun <> [] ) then
        if ( typeof(compfun) == "function" ) then
            areeq = compfun ( txtcomp , txtref )
        else
            // compfun is a list
            cf = compfun(1)
            areeq = cf ( txtcomp , txtref , compfun(2:$) )
        end
    else
        areeq = ( txtcomp == txtref )
    end
    if ( areeq ) then
        flag = %t
        errmsg = ""
    else
        flag = %f
        errmsg = msprintf(gettext("%s: The content of computed file ""%s"" is different from the content of reference file ""%s""."), ..
        "assert_checkfilesequal",filecomp,fileref)
        // Do not generate the error now: let us close the files before!
    end
    //
    // Close the files
    err=mclose(fdc)
    if ( err <> 0 ) then
        flag = %f
        errmsg = sprintf ( gettext ( "%s: Cannot close file %s.\n") , "assert_checkfilesequal" , filecomp )
        // It may happen that we overwrite the content of the errmsg varaiable.
        // For example, we are there, while the file contents were different.
        // We consider that that not being able to close the file is a bigger issue,
    end
    err=mclose(fdr)
    if ( err <> 0 ) then
        errmsg = sprintf ( gettext ( "%s: Cannot close file %s.\n") , "assert_checkfilesequal" , fileref )
        // It may happen that we overwrite the content of the errmsg varaiable.
        // For example, we are there, while the file contents were different.
        // We consider that that not being able to close the file is a bigger issue,
    end

    if ( ~flag & lhs < 2 ) then
        // If no output variable is given, generate an error
        assert_generror ( errmsg )
    end
endfunction

