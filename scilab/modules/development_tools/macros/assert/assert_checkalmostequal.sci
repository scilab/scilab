// Copyright (C) 2008 - 2009 - INRIA - Michael Baudin
// Copyright (C) 2009 - 2011 - DIGITEO - Michael Baudin
// Copyright (C) 2012 - Michael Baudin
// Copyright (C) 2017 - Samuel GOUGEON
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function [flag,errmsg] = assert_checkalmostequal ( varargin )

    function [kpinf , kninf , knan , kreg , xreg] = infnanindices ( x )
        // Returns the indices where
        //  * kpinf : x(kpinf) == +%inf,
        //  * kninf : x(kninf) == -%inf,
        //  * knan : x(knan) is a %nan
        //  * kreg : x(kreg) is not an infinity, not a nan
        //  * xreg = x(kreg)
        // These 4 sets of indices have no intersection.
        //
        // Example :
        // x = [1 2 3 -%inf %inf %nan %inf %nan -%inf 4 5 6]
        // [kpinf , kninf , knan , kreg , xreg] = infnanindices ( x )
        // xreg = [1 2 3 4 5 6]
        // kreg = [1 2 3 10 11 12]
        // knan = [6 8]
        // kninf = [4 9]
        // kpinf = [5 7]

        kpinf = find(x==%inf)
        kninf = find(x==-%inf)
        knan = find(isnan(x))
        kreg = find(abs(x)<>%inf & ~isnan(x))
        xreg = x(kreg)
    endfunction

    function areequal = assert_arealmostequal ( computed , expected , reltol , abstol , comptype )
        //
        // Decompose the expected value into nan indices, inf indices and regular indices
        // This allows to solve the following issue:
        // if computed is %inf and expected is %inf, the difference is %nan,
        // which makes the computations fail.
        if ( computed == [] & expected == []) then
            areequal = %t
            return
        end
        [kcpinf , kcninf , kcnan , kcreg , creg] = infnanindices ( computed )
        [kepinf , keninf , kenan , kereg , ereg] = infnanindices ( expected )
        //
        if ( comptype == "matrix" ) then
            areclose = ( norm ( creg - ereg ) <= reltol * max(norm(ereg),norm(creg) ) + abstol )
        else
            if (creg==[]&ereg==[]) then
                areclose=%t
            elseif (creg<>[]&ereg==[]) then
                areclose=%f
            elseif (creg==[]&ereg<>[]) then
                areclose=%f
            else
                entries = ( abs(creg-ereg) <= reltol * max(abs(ereg),abs(creg)) + abstol )
                // Compute the global condition from the entries conditions
                areclose = and(entries)
            end
        end
        // The regular values must be almost equal and
        // * the +%inf must be at the same place,
        // * the -%inf must be at the same place,
        // * the %nan must be at the same place.

        areequal = ( areclose & and(kcpinf == kepinf) & and(kcninf == keninf) & and(kcnan == kenan) )
    endfunction

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
    // ============================================================================================

    //  Check that computed and expected are numerically close.

    [lhs,rhs]=argn()
    if ( and(rhs <> [2 3 4 5] ) ) then
        errmsg = gettext("%s: Wrong number of input arguments: %d to %d expected.")
        error(msprintf(errmsg, "assert_checkalmostequal" , 2 , 5 ))
    end
    //
    // Get arguments
    computed = varargin(1)
    expected = varargin(2)
    reltol = assert_argindefault ( rhs , varargin , 3 , sqrt(%eps) )
    abstol = assert_argindefault ( rhs , varargin , 4 , 0 )
    comptype = assert_argindefault ( rhs , varargin , 5 , "element" )
    //
    // Check types of variables
    if ( and(typeof(computed) <> ["constant" "sparse" "polynomial"]) ) then
        errmsg = gettext("%s: Wrong type for input argument #%d: Matrix expected.\n")
        error(msprintf(errmsg, "assert_checkalmostequal", 1))
    end
    if ( and(typeof(expected) <> ["constant" "sparse" "polynomial"]) ) then
        errmsg = gettext("%s: Wrong type for input argument #%d: Matrix expected.\n")
        error(msprintf (errmsg, "assert_checkalmostequal" , 2 ))
    end
    if typeof(computed)=="polynomial" & typeof(expected)~="polynomial" then
        errmsg = gettext("%s: Wrong type for input argument #%d: Polynomial expected.\n")
        error(msprintf (errmsg, "assert_checkalmostequal" , 2 ))
    end
    if ( typeof(reltol) <> "constant" ) then
        errmsg = gettext("%s: Wrong type for input argument #%d: Matrix expected.\n")
        error(sprintf(errmsg, "assert_checkalmostequal", 3 ))
    end
    if ( typeof(abstol) <> "constant" ) then
        errmsg = gettext("%s: Wrong type for input argument #%d: Matrix expected.\n")
        error(sprintf(errmsg, "assert_checkalmostequal", 4 ))
    end
    if ( typeof(comptype) <> "string" ) then
        errmsg = gettext("%s: Wrong type for input argument #%d: Matrix of strings expected.\n")
        error(sprintf(errmsg, "assert_checkalmostequal", 5))
    end
    //
    // Check sizes of variables
    if ( size(reltol,"*") <> 1 ) then
        errmsg = gettext("%s: Wrong size for input argument #%d: %d-by-%d matrix expected.\n")
        error(msprintf(errmsg, "assert_checkalmostequal", 3, 1, 1))
    end
    if ( size(abstol,"*") <> 1 ) then
        errmsg = gettext("%s: Wrong size for input argument #%d: %d-by-%d matrix expected.\n")
        error(msprintf(errmsg, "assert_checkalmostequal", 4, 1, 1))
    end
    if ( size(comptype,"*") <> 1 ) then
        errmsg = gettext("%s: Wrong size for input argument #%d: %d-by-%d matrix expected.\n")
        error(msprintf(errmsg, "assert_checkalmostequal", 5, 1, 1))
    end
    //
    // Check values of variables
    if ( reltol < 0 ) then
        errmsg = gettext("%s: Wrong value for input argument #%d: Must be > %d.\n")
        error(msprintf(errmsg, "assert_checkalmostequal", 3, 0))
    end
    if ( abstol < 0 ) then
        errmsg = gettext("%s: Wrong value for input argument #%d: Must be > %d.\n")
        error(msprintf(errmsg, "assert_checkalmostequal", 4, 0))
    end
    if ( and ( comptype <> ["matrix" "element"] ) ) then
        errmsg = gettext("%s: Wrong value for input argument #%d: Must be in the set  {%s}.\n")
        error(msprintf(errmsg, "assert_checkalmostequal", 5, """matrix"",""element"""))
    end
    //
    // Proceed...
    ncom = size(computed)
    nexp = size(expected)
    if ( or(ncom <> nexp) ) then
        errmsg = gettext("%s: Incompatible input arguments #%d and #%d: Same sizes expected.\n")
        error(msprintf(errmsg, "assert_checkalmostequal", 1, 2))
    end
    if typeof(computed)=="polynomial" then
        if or(degree(computed) <> degree(expected) ) then
            errmsg = gettext("%s: Arguments #%d and #%d: Same degrees expected.\n")
            error(msprintf(errmsg, "assert_checkalmostequal", 1, 2))
        end
        computed0 = computed;
        expected0 = expected;
        computed = coeff(computed(:));
        expected = coeff(expected(:));
    end
    //
    areequal_re = assert_arealmostequal ( real(computed) , real(expected) , reltol , abstol , comptype )
    if ( areequal_re ) then
        areequal = assert_arealmostequal ( imag(computed) , imag(expected) , reltol , abstol , comptype )
    else
        areequal = %f
    end
    //
    if ( areequal ) then
        flag = %t
        errmsg = ""
    else
        flag = %f
        if isdef("computed0","l") then
            computed = computed0;
            expected = expected0;
        end
        // Change the message if the matrix contains more than one value
        if ( size(expected,"*") == 1 ) then
            estr = string(expected)
        else
            estr = "[" + string(expected(1)) + " ...]"
        end
        if ( size(computed,"*") == 1 ) then
            cstr = string(computed)
        else
            cstr = "[" + string(computed(1)) + " ...]"
        end
        relstr = string(reltol)
        absstr = string(abstol)
        errmsg = gettext("%s: Assertion failed: expected = %s while computed = %s")
        errmsg = msprintf(errmsg, "assert_checkalmostequal", estr, cstr)
        if ( lhs < 2 ) then
            // If no output variable is given, generate an error
            assert_generror ( errmsg )
        end
    end
endfunction
