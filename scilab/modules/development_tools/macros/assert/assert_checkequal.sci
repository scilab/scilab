// Copyright (C) 2008-2009 - INRIA - Michael Baudin
// Copyright (C) 2010 - 2011 - DIGITEO - Michael Baudin
// Copyright (C) 2012 - 2016 - Scilab Enterprises
// Copyright (C) 2019 - Samuel GOUGEON
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function [flag,errmsg] = assert_checkequal ( computed , expected )
    //  Check that computed and expected are equal.
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

    //
    // Check sizes of variables
    if ( or(type(computed)==[16 17]) ) then
        ncom = length(computed)
    else
        try
            ncom = size(computed)
        catch   // non-sizeable objects: 1:$, iolib, sin, sind, etc
            ncom = -2
        end
    end
    if ( or(type(expected)==[16 17]) ) then
        nexp = length(expected)
    else
        try
            nexp = size(expected)
        catch
            nexp = -2
        end
    end
    if ( or(ncom <> nexp) ) then
        errmsg = sprintf ( gettext ( "%s: Incompatible input arguments #%d and #%d: Same sizes expected.\n") , "assert_checkequal" , 1 , 2 )
        error(errmsg)
    end

    // sparse or full real or complex matrices
    if or(type(computed) == [1 5])  then
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
        if cisreal & eisreal then
            [flag, k] = comparedoubles ( computed , expected )
        else
            [flag, k] = comparedoubles ( real(computed) , real(expected) )
            if flag then
                [flag ,k] = comparedoubles ( imag(computed) , imag(expected) )
            end
        end
        // k is the index of the first discrepancy (or [] is none)

    elseif or(typeof(computed)==["implicitlist" "fptr" "function"])
                                    // http://bugzilla.scilab.org/16104 C) D) E)
        flag = computed==expected
        if ~flag then
            if typeof(computed) == "implicitlist"
                errmsg = _("%s: Assertion failed: expected= %s  while computed= %s")
                errmsg = msprintf(errmsg,"assert_checkequal",string(expected),string(computed))

            elseif typeof(computed) == "function"
                c = macr2tree(computed).name+"()"
                e = macr2tree(expected).name+"()"
                errmsg = _("%s: Assertion failed: expected= %s  while computed= %s")
                errmsg = msprintf(errmsg,"assert_checkequal", e, c)

            else
                // no way to get the names of built-in functions
                errmsg = _("%s: Assertion failed: expected and computed are two distinct built-in functions.")
                errmsg = msprintf(errmsg,"assert_checkequal")
            end
            if lhs < 2 then
                assert_generror ( errmsg )
            end
        end
        return

    elseif type(computed) == 14   // library : http://bugzilla.scilab.org/16104#c1
        flag = and(string(computed)==string(expected))
        if ~flag then
            errmsg = gettext("%s: Assertion failed: expected= %s  while computed= %s")
            c = "lib@" + string(computed)(1)
            e = "lib@" + string(expected)(1)
            errmsg = msprintf(errmsg,"assert_checkequal", e, c)
            if lhs < 2 then
                assert_generror ( errmsg )
            end
        end
        return

    elseif type(computed)==15   // Simple lists
        b = computed==expected
        flag = and(b)
        if ~flag
            // computed<>expected can't be simply used due to // http://bugzilla.scilab.org/15293
            flag = %t
            for i = find(~b)
                tc = type(computed(i))
                te = type(expected(i))
                if tc==0 & te==0
                    continue
                elseif tc<>0 & te<>0 & isnan(computed(i)) & isnan(expected(i))
                    continue
                else
                    flag = %f
                    k = i
                    if tc==0
                        computed(k) = "(void)"
                    end
                    if te==0
                        expected(k) = "(void)"
                    end
                    break
                end
            end
        end

    else
        b = and(computed == expected)
        flag = b || isequal(computed, expected)
        if ~flag & ~b
            k = find(computed<>expected, 1);
        end
    end

    if flag then
        errmsg = ""

    else
        // Sets the message according to the type and size of the pair:
        if or(typeof(expected) == ["sparse", "boolean sparse"])
            e = full(expected(k))
            c = full(computed(k))
        elseif isdef("k","l")
            e = expected(k)
            c = computed(k)
        else
            try
            e = expected(1)
            c = computed(1)
            catch
            e = expected
            c = computed
            end
        end
        if type(computed)==9
            estr = msprintf("%s(uid:%d)", e.type, e.uid)
            cstr = msprintf("%s(uid:%d)", c.type, c.uid)
        else
            estr = string(e)
            cstr = string(c)
        end
        if isdef("k","l") & length(computed)>1
            estr = msprintf(_("expected(%d)= "),k) + estr
            cstr = msprintf(_("computed(%d)= "),k) + cstr
        else
            estr = _("expected= ") + estr
            cstr = _("computed= ") + cstr
        end
        //
        ierr = execstr("mdiff = string(mean(computed - expected))", "errcatch");
        if ( ierr == 0 ) then
            errmsg = msprintf(gettext("%s: Assertion failed: %s  while %s (mean diff = %s)"),"assert_checkequal",estr, cstr, mdiff)
        else
            errmsg = msprintf(gettext("%s: Assertion failed: %s  while %s"),"assert_checkequal", estr, cstr)
        end
        if lhs < 2 then
            // If no output variable is given, generate an error
            assert_generror ( errmsg )
        end
    end
endfunction
// ---------------------------------------------------------------------------
function [flag, k] = comparedoubles ( computed , expected )
    rand("seed",getdate("s"))
    joker = rand(1);
    while find(expected==joker | computed==joker,1)<>[]
        joker = rand(1);
    end
    computed(isnan(computed)) = joker;
    expected(isnan(expected)) = joker;
    k = find(expected<>computed,1);
    flag = k==[];
endfunction
