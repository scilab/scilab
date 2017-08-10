// Copyright (C) 2010 - 2011 - DIGITEO - Michael Baudin
// Copyright (C) 2012 - DIGITEO - Allan CORNET
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function [flag,errmsg] = assert_checkerror ( varargin )
    function argin = assert_argindefault ( rhs , vararglist , ivar , default )
        // Returns the value of the input argument #ivar.
        // If this argument was not provided, or was equal to the
        // empty matrix, returns the default value.
        if ( rhs < ivar ) then
            argin = default
        else
            if ( vararglist(ivar) <> [] ) then
                argin = vararglist(ivar)
            else
                argin = default
            end
        end
    endfunction

    //  Check that an instruction produces the expected error.
    [lhs,rhs]=argn()
    if ( rhs < 2 ) then
        errmsg = sprintf ( gettext ( "%s: Wrong number of input argument: At least %d expected.\n") , "assert_checkerror" , 2 )
        error(errmsg)
    end
    //
    // Get arguments
    instr = varargin(1);
    expectedmsg = varargin(2);
    expectederrnb = assert_argindefault ( rhs , varargin , 3 , [] );
    //
    // Check types of variables
    if ( typeof(instr) <> "string" ) then
        errmsg = sprintf ( gettext ( "%s: Wrong type for input argument #%d: Matrix of strings expected.\n") , "assert_checkerror" , 1 )
        error(errmsg)
    end
    if (expectedmsg<>[]) then
        if ( typeof(expectedmsg) <> "string" ) then
            errmsg = sprintf ( gettext ( "%s: Wrong type for input argument #%d: Matrix of strings expected.\n") , "assert_checkerror" , 2 )
            error(errmsg)
        end
    end
    if ( typeof(expectederrnb) <> "constant" ) then
        errmsg = sprintf ( gettext ( "%s: Wrong type for input argument #%d: Matrix expected.\n") , "assert_checkerror" , 3 )
        error(errmsg)
    end
    //
    // Check sizes of variables
    if ( size(instr,"*") <> 1 ) then
        errmsg = sprintf ( gettext ( "%s: Wrong size for input argument #%d: %d-by-%d matrix expected.\n") , "assert_checkerror" , 1 , 1 , 1 )
        error(errmsg)
    end
    if (expectedmsg<>[]) then
        if ((size(expectedmsg, "r") > 1) & (size(expectedmsg, "c") > 1))
            errmsg = sprintf ( gettext ( "%s: Wrong size for input argument #%d: A string vector expected.\n") , "assert_checkerror" , 2 );
            error(errmsg)
        end
    end
    if ( expectederrnb <> [] ) then
        if ( size(expectederrnb,"*") <> 1 ) then
            errmsg = sprintf ( gettext ( "%s: Wrong size for input argument #%d: %d-by-%d matrix expected.\n") , "assert_checkerror" , 3 , 1 , 1 )
            error(errmsg)
        end
    end
    //
    // Check values of variables
    if (expectedmsg<>[]) then
        if ( expectederrnb <> [] ) then
            if ( expectederrnb < 0 ) then
                errmsg = sprintf ( gettext ( "%s: Wrong value for input argument #%d: Non-negative integers expected.\n"  ) , "assert_checkerror" , 3 )
                error(errmsg)
            end
        end
    else
        // If the message is empty (arg #2), check that the error
        // number is not empty (arg #3).
        if ( expectederrnb == [] ) then
            errmsg = sprintf ( gettext ( "%s: Wrong size for input argument #%d: Non-empty matrix expected.\n"  ) , "assert_checkerror" , 3 )
            error(errmsg)
        end
    end
    //
    // Proceed...
    compierr = execstr(instr,"errcatch")
    //
    // Check that an error is produced
    if ( compierr == 0 ) then
        localstr = gettext ( "%s: No error was produced while evaluating ""%s"".")
        errmsg = sprintf ( localstr , "assert_checkerror" , instr )
        error(errmsg)
    end
    //
    // Get the error
    compmsg = lasterror();
    //
    // Initialize output arguments
    flag = %t
    errmsg = ""
    //
    // Localize the message, if necessary.
    if ( rhs >= 4 ) then
        localmsg = gettext(expectedmsg)
        instr = "expectedmsg = msprintf(localmsg, varargin(4:$))"
        ierr = execstr(instr,"errcatch")
        if ( ierr <> 0 ) then
            fmterrmsg = lasterror();
            localstr = gettext ( "%s: Error while formatting the error message: ""%s""")
            errmsg = sprintf ( localstr , "assert_checkerror" , fmterrmsg )
            error(errmsg)
        end
    end
    //
    // Check the error message
    if (expectedmsg <> []) then
        isdifferentmsg = or(expectedmsg <> compmsg);

        if (isdifferentmsg) then
            flag = %f
            if size(compmsg, "*") <> 1 then
                compmsg = sci2exp(compmsg);
            end
            if size(expectedmsg, "*") <> 1 then
                expectedmsg = sci2exp(expectedmsg);
            end
            localstr = gettext("%s: Assertion failed: expected error message = ""%s"" while computed error message = ""%s"".")
            errmsg = msprintf(localstr,"assert_checkerror", expectedmsg, compmsg)
            if ( lhs < 2 ) then
                // If no output variable is given, generate an error
                error ( errmsg )
            else
                return
            end
        end
    end
    if ( expectederrnb <> [] ) then
        //
        // Check the error number
        if ( expectederrnb <> compierr ) then
            flag = %f
            localstr = gettext("%s: Assertion failed: expected error number = %d while computed error number = %d.")
            errmsg = msprintf(localstr,"assert_checkerror",expectederrnb,compierr)
            if ( lhs < 2 ) then
                // If no output variable is given, generate an error
                error ( errmsg )
            else
                return
            end
        end
    end
endfunction

