// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2007-2008 - INRIA - Pierre MARECHAL <pierre.marechal@inria.fr>
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

// =============================================================================
// Clean tests
// =============================================================================

function test_clean(varargin)

    lhs = argn(1);
    rhs = argn(2);

    global test_list;
    global test_count;
    global displayed_txt;

    // test type
    test_types        = ["unit_tests","nonreg_tests"];
    test_types_keeped = "";

    test_count         = 0;

    displayed_txt      = "";

    // =======================================================
    // Gestion des types de tests à lancer
    // =======================================================

    if rhs < 3 then
        test_types_keeped = "all_tests";
    else
        option_mat =  varargin(3);

        if ((grep(option_mat,"unit_tests")<>[]) & (grep(option_mat,"nonreg_tests")<>[])) ..
            | (grep(option_mat,"all_tests")<>[]) then
            test_types_keeped = "all_tests";

        elseif grep(option_mat,"unit_tests") <> [] then
            test_types_keeped = "unit_tests";

        elseif  grep(option_mat,"nonreg_tests") <> [] then
            test_types_keeped = "nonreg_tests";

        else
            test_types_keeped = "all_tests";

        end

    end

    // =======================================================
    // Gestion des tests à lancer
    // =======================================================

    if (rhs == 0) ..
        | ((rhs == 1) & (varargin(1)==[])) ..
        | (((rhs == 2)|(rhs == 3)) & (varargin(1)==[]) & (varargin(2)==[])) then

        // No input argument
        // test_clean()
        // test_clean([])
        // => Launch each test of each module

        module_list = getmodules();
        module_list = gsort(module_list,"lr","i");
        for k=1:size(module_list,"*")
            test_clean_module(module_list(k),test_types_keeped);
        end

    elseif (rhs == 1) ..
        | ((rhs == 2) & (varargin(2)==[])) ..
        | ((rhs == 3) & (varargin(2)==[])) then

        // One input argument
        // test_clean(<module_name>)
        // test_clean([<module_name_1>,<module_name_2>])

        // varargin(1) = [<module_name_1>,<module_name_2>]

        module_mat = varargin(1);

        [nl,nc] = size(module_mat);

        // test_clean([<module_name_1>,<module_name_2>])

        for i=1:nl
            for j=1:nc
                if( with_module(module_mat(i,j)) ) then
                    test_clean_module(module_mat(i,j),test_types_keeped);
                else
                    error(sprintf(gettext("%s is not an installed module"),module_mat(i,j)));
                end
            end
        end

    elseif (rhs == 2) | (rhs == 3) then

        // Two input arguments
        // test_clean(<module_name>,<test_name>)
        // test_clean(<module_name>,[<test_name_1>,<test_name_2>] )

        // varargin(1) = <module_name> ==> string 1x1
        // varargin(2) = <test_name_1> ==> mat nl x nc

        module     = varargin(1);
        test_mat   = varargin(2);

        if ((or(size(module) <> [1,1])) & (test_mat <> [])) then
            example = test_examples();
            err     = ["" ; msprintf(gettext("%s: Wrong size for input argument."),"test_clean") ; "" ; example ];
            printf("%s\n",err);
            return;
        end

        [nl,nc] = size(test_mat);

        for i=1:nl
            for j=1:nc

                if (fileinfo(SCI+"/modules/"+module+"/tests/unit_tests/"+test_mat(i,j)+".dia")<>[]) ..
                    & ( (test_types_keeped=="all_tests") | (test_types_keeped=="unit_tests") ) then
                    test_clean_onetest(module,test_mat(i,j),"unit_tests");

                elseif (fileinfo(SCI+"/modules/"+module+"/tests/nonreg_tests/"+test_mat(i,j)+".dia")<>[]) ..
                    & ( (test_types_keeped=="all_tests") | (test_types_keeped=="nonreg_tests") ) then
                    test_clean_onetest(module,test_mat(i,j),"nonreg_tests");

                else
                    error(sprintf(gettext("The test ""%s"" is not available from the ""%s"" module"),test_mat(i,j),module));

                end
            end
        end
    else
        error(msprintf(gettext("%s: Wrong number of input arguments."),"test_clean"));
    end

    // =======================================================
    // Test clean
    // =======================================================

    for i=1:test_count
        printf("   %03d/%03d - ",i,test_count);
        printf("[%s] %s.dia\n",test_list(i,1),test_list(i,2));
        test_clean_dia(test_list(i,1),test_list(i,2),test_list(i,3));
    end

endfunction

//-----------------------------------------------------------------------------
// Pierre MARECHAL
// Scilab team
// Date : 28 oct. 2007
//
// => List all test files in the module <module_mat>
// => Add them to the test_mat matrix
//-----------------------------------------------------------------------------

function test_clean_module(module_mat,test_type)

    if (test_type == "all_tests") | (test_type == "unit_tests") then

        module_test_dir = SCI+"/modules/"+module_mat+"/tests/unit_tests";
        test_mat        = gsort(basename(listfiles(module_test_dir+"/*.dia")),"lr","i");

        nl = size(test_mat,"*");
        for i=1:nl
            test_clean_onetest(module_mat,test_mat(i),"unit_tests");
        end

    end

    if (test_type == "all_tests") | (test_type == "nonreg_tests") then

        module_test_dir = SCI+"/modules/"+module_mat+"/tests/nonreg_tests";
        test_mat        = gsort(basename(listfiles(module_test_dir+"/*.dia")),"lr","i");

        nl = size(test_mat,"*");
        for i=1:nl
            test_clean_onetest(module_mat,test_mat(i),"nonreg_tests");
        end

    end

endfunction

//-----------------------------------------------------------------------------
// Pierre MARECHAL
// Scilab team
// Date : 28 oct. 2007
//
// => Add the test <test> to the test_mat matrix
//-----------------------------------------------------------------------------

function test_clean_onetest(module,test,test_type)

    global test_list;
    global test_count;

    test_count = test_count + 1;
    test_list( test_count , 1 ) = module;
    test_list( test_count , 2 ) = test;
    test_list( test_count , 3 ) = test_type;

endfunction

//-----------------------------------------------------------------------------
// Pierre MARECHAL
// Scilab team
// Date : 15 jan. 2008
//
// => remove dia file
//-----------------------------------------------------------------------------

function test_clean_dia(module,test,test_type)
    mdelete(SCI+"/modules/"+module+"/tests/"+test_type+"/"+test+".dia");
endfunction
