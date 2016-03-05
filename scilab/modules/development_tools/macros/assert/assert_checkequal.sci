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

function [flag,errmsg] = assert_checkequal ( computed , expected )
    //  Check that computed and expected are equal.

    function flag = comparedoubles ( computed , expected )
        [cnonan , cnumb] = mythrownan(computed)
        [enonan , enumb] = mythrownan(expected)
        if ( and(enonan == cnonan) & and(enumb == cnumb) ) then
            flag = %t
        else
            flag = %f
        end
    endfunction

    function [nonan,numb] = mythrownan(x)
        //
        //
        // Copyright (C) 2000 - INRIA - Carlos Klimann
        // Copyright (C) 2010 - 2011 - DIGITEO - Michael Baudin
        //This  function  returns  in  vector  nonan  the  values
        //(ignoring the NANs) of a  vector or matrix x and in the
        //corresponding places of vector  numb the indexes of the
        //value.
        //
        //For  a  vector  or matrix  x,  [nonan,numb]=thrownan(x)
        //considers x, whatever his dimensions are, like a linear
        //vector (columns  first).
        //
        //
        [lhs,rhs]=argn(0)
        if ( rhs<>1 ) then
            error(msprintf(gettext("%s: Wrong number of input argument: %d expected.\n"),"thrownan",1))
        end
        numb=find(bool2s(~isnan(x)))
        nonan=x(~isnan(x))
    endfunction

    [lhs,rhs]=argn()
    if ( rhs <> 2 ) then
        errmsg = sprintf ( gettext ( "%s: Wrong number of input arguments: %d expected.\n") , "assert_checkequal" , 2 )
        error(errmsg)
    end

    // Check types of variables
    if ( typeof(computed) <> typeof(expected) ) then
        errmsg = sprintf ( gettext ( "%s: Incompatible input arguments #%d and #%d: Same types expected.\n" ) , "assert_checkequal" , 1 , 2 )
        error(errmsg)
   end

    // Check hypermat type
    if (typeof(computed) == "hypermat") then
        // Check on first element type
        if (typeof(computed(1)) <> typeof(expected(1))) then
            errmsg = sprintf ( gettext ( "%s: Incompatible input arguments #%d and #%d: Same types expected.\n" ) , "assert_checkequal" , 1 , 2 )
            error(errmsg)
        end
    end


    //
    // Check sizes of variables
    if ( or(type(computed)==[16 17]) ) then
        ncom = length(computed)
    else
        ncom = size(computed)
    end
    if ( or(type(expected)==[16 17]) ) then
        nexp = length(expected)
    else
        nexp = size(expected)
    end
    if ( or(ncom <> nexp) ) then
        errmsg = sprintf ( gettext ( "%s: Incompatible input arguments #%d and #%d: Same sizes expected.\n") , "assert_checkequal" , 1 , 2 )
        error(errmsg)
    end
    //
    if ( type(computed) == 1 & type(expected) == 1 ) then
        // These are two matrices of doubles
        cisreal = isreal(computed)
        eisreal = isreal(expected)
        if ( cisreal & ~eisreal ) then
            errmsg = sprintf ( gettext ( "%s: Computed is real, but expected is complex.") , "assert_checkequal" )
            error(errmsg)
        end
        if ( ~cisreal & eisreal ) then
            errmsg = sprintf ( gettext ( "%s: Computed is complex, but expected is real.") , "assert_checkequal" )
            error(errmsg)
        end
        if ( cisreal & eisreal ) then
            flag = comparedoubles ( computed , expected )
        else
            flagreal = comparedoubles ( real(computed) , real(expected) )
            if ( flagreal ) then
                flagimag = comparedoubles ( imag(computed) , imag(expected) )
                flag = flagimag
            else
                flag = %f
            end
        end
    else
        if ( and ( computed == expected ) ) then
            flag = %t
        else
            flag = %f
        end
    end

    if ( flag == %t ) then

        errmsg = ""
    else
        // Change the message if the matrix contains more than one value
        if ( size(expected,"*") == 1 ) then
            if ( or(typeof(expected) == ["sparse", "boolean sparse"])) then
                val = full(expected)
            else
                val = expected
            end
            estr = string(val)
        else
            if ( or(typeof(expected) == ["sparse", "boolean sparse"])) then
                val = full(expected(1))
            else
                val = expected(1)
            end
            estr = "[" + string(val) + " ...]"
        end
        if ( size(computed,"*") == 1 ) then
            if ( or(typeof(computed) == ["sparse", "boolean sparse"])) then
                val = full(computed)
            else
                val = computed
            end
            cstr = string(val)
        else
            if ( or(typeof(computed) == ["sparse", "boolean sparse"])) then
                val = full(computed(1))
            else
                val = computed(1)
            end
            cstr = "[" + string(val) + " ...]"
        end
        ierr = execstr("mdiff = string(mean(computed - expected))", "errcatch");
        if ( ierr == 0 ) then
            errmsg = msprintf(gettext("%s: Assertion failed: expected = %s while computed = %s (mean diff = %s)"),"assert_checkequal",estr,cstr,mdiff)
        else
            errmsg = msprintf(gettext("%s: Assertion failed: expected = %s while computed = %s"),"assert_checkequal",estr,cstr)
        end
        if ( lhs < 2 ) then
            // If no output variable is given, generate an error
            assert_generror ( errmsg )
        end
    end
endfunction


