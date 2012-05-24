// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011 - DIGITEO - Michael Baudin
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

function [flag,errmsg] = assert_equal ( computed , expected )
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
    //
    // Check types of variables
    if ( typeof(computed) <> typeof(expected) ) then
        errmsg = sprintf ( gettext ( "%s: Incompatible input arguments #%d and #%d: Same types expected.\n" ) , "assert_checkequal" , 1 , 2 )
        error(errmsg)
    end
    //
    // Check sizes of variables
    ncom = size(computed)
    nexp = size(expected)
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
            estr = string(expected)
        else
            estr = "[" + string(expected(1)) + " ...]"
        end
        if ( size(computed,"*") == 1 ) then
            cstr = string(computed)
        else
            cstr = "[" + string(computed(1)) + " ...]"
        end
        errmsg = msprintf(gettext("%s: Assertion failed: expected = %s while computed = %s"),"assert_checkequal",estr,cstr)
        if ( lhs < 2 ) then
            // If no output variable is given, generate an error
            error ( errmsg )
        end
    end
endfunction




c = complex(1,2);
assert_equal([real(c);imag(c)],[1;2]);
//
c = complex([1 2],[3 4]);
assert_equal([real(c);imag(c)],[1 2;3 4]);
//
c = complex(%inf,%inf);
assert_equal([real(c);imag(c)],[%inf;%inf]);
//
c = complex(%inf,%nan);
assert_equal([real(c);imag(c)],[%inf;%nan]);
//
c = complex(%nan,%nan);
assert_equal([real(c);imag(c)],[%nan;%nan]);
//
c = complex(%nan,%inf);
assert_equal([real(c);imag(c)],[%nan;%inf]);
//
c = complex(0,%nan);
assert_equal([real(c);imag(c)],[0;%nan]);
//
c = complex(0,%inf);
assert_equal([real(c);imag(c)],[0;%inf]);
//
c = complex(1);
assert_equal([real(c);imag(c)],[1;0]);
//
c = complex([1 2]);
assert_equal([real(c);imag(c)],[1 2;0 0]);
//
c = complex([1 2 3], 4);
assert_equal([real(c);imag(c)],[1 2 3;4 4 4]);
//
c = complex(1, [2 3 4]);
assert_equal([real(c);imag(c)],[1 1 1;2 3 4]);
