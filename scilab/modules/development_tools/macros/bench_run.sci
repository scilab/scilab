// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2007-2008 - INRIA - Pierre MARECHAL <pierre.marechal@inria.fr>
// Copyright (C) 2011-2011 - DIGITEO - Bruno JOFRET
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

//-----------------------------------------------------------------------------
// Launch benchmarks
//-----------------------------------------------------------------------------

function [modutests_names, elapsed_time, nb_iterations] = bench_run(varargin)

    lhs = argn(1);
    rhs = argn(2);

    global test_list;
    global test_count;

    test_list          = [];
    modutests_names    = "";
    test_count         = 0;
    boucle_for_time    = 0;

    just_list_tests    = %F;
    print_help         = %F;
    nb_run             = "10000";
    nb_run_override    = %f;

    elapsed_time       = [];
    nb_iterations      = [];

    xml_str            = "";

    // =======================================================
    // Gestion des tests à lancer
    // =======================================================

    if (rhs == 0) ..
        | ((rhs == 1) & (varargin(1)==[] | varargin(1)=="[]" | varargin(1) == "")) ..
        | (((rhs >= 2)) & (varargin(1)==[] | varargin(1)=="[]" | varargin(1) == "") & (varargin(2)==[] | varargin(2)=="[]" | varargin(2) == "")) then

        // No input argument
        // bench_run()
        // bench_run([])
        // => Launch each test of each module

        module_list = getmodules();
        module_list = gsort(module_list,"lr","i");
        for k=1:size(module_list,"*")
            bench_add_module(module_list(k));
        end

    elseif (rhs == 1) ..
        | ((rhs >= 2) & (varargin(2)==[] | varargin(2)=="[]" | varargin(2) == "")) ..

        // One input argument
        // bench_run(<module_name>)
        // bench_run([<module_name_1>,<module_name_2>])

        // varargin(1) = [<module_name_1>,<module_name_2>]

        module_mat = varargin(1);

        [nl,nc] = size(module_mat);

        // bench_run([<module_name_1>,<module_name_2>])

        for i=1:nl
            for j=1:nc
                if( with_module(module_mat(i,j)) ) then
                    bench_add_module(module_mat(i,j));
                else
                    if isdir(module_mat(i,j)) then
                        bench_add_dir(module_mat(i,j));
                    else
                        error(msprintf(gettext("%s: %s is not an installed module"), "bench_run", module_mat(i,j)));
                    end
                end
            end
        end

    elseif (rhs >= 2 & rhs <= 4) then

        // Two input arguments
        // bench_run(<module_name>,<test_name>)
        // bench_run(<module_name>,[<test_name_1>,<test_name_2>] )

        module_mat = varargin(1);
        test_mat   = varargin(2);
        bench_list_reduced = [];

        // get module and test lists
        bench_list = bench_list_tests(module_mat);
        // only keep relevant tests
        // after this loop bench_test_reduced contains the module and relevant tests
        for i = 1:size(test_mat, "*")
            found_tests = find(bench_list(:,2) == test_mat(i));
            if ~isempty(found_tests)
                bench_list_reduced = [bench_list_reduced; bench_list(found_tests, :)];
            else
                // At least one element in the test list is wrong
                // this is an error
                error(msprintf(_("%s: Wrong value for input argument #%d: test %s not found in the list of modules"), "bench_run", 2, test_mat(i)));
            end
        end

        for i=1:size(bench_list_reduced, "r") //loops over each row of bench_list_reduced
            bench_add_onebench(bench_list_reduced(i, 1), bench_list_reduced(i, 2));
        end
    else
        error(msprintf(gettext("%s: Wrong number of input argument(s): %d to %d expected.\n"), "bench_run", 0, 4));
    end

    // =======================================================
    // Gestion des options
    // =======================================================

    if rhs >= 3 then

        option_mat =  varargin(3);

        if grep(option_mat,"list") <> [] then
            just_list_tests    = %T;
        end

        if grep(option_mat,"help") <> [] then
            print_help         = %T;
        end

        nb_run_line = grep(option_mat,"/nb_run\s*=\s*/", "r")
        if ~isempty(nb_run_line) then
            nb_run_override = %t;
            stripped_option = option_mat(nb_run_line);
            idx_nonblank = strindex(stripped_option, "/[^ \t\b]/", "r");
            stripped_option = part(stripped_option, idx_nonblank);
            nb_run      = strsubst(stripped_option, "nb_run=","");
        end

    end

    if print_help then

        // =======================================================
        // Display help
        // =======================================================

        example = bench_examples();
        printf("%s\n",example);
        return;

    elseif just_list_tests then

        // =======================================================
        // Just list tests
        // =======================================================

        printf("\n");

        for i=1:test_count
            printf("   %03d - ",i);
            printf("[%s] %s\n",test_list(i,1),test_list(i,2));
        end

    else

        // =======================================================
        // Test launch
        // =======================================================

        // Protect user modes during tests
        saved_modes = mode();
        saved_ieee = ieee();
        saved_format = format();
        saved_warning = warning("query");
        saved_funcprot = funcprot();

        printf("\n");

        xml_str = [ xml_str ; "<benchmarks>" ];

        // Calcul de la durée de la boucle for en µs

        tic();
        for i = 1:1000000
        end
        timing = toc();
        boucle_for_time = timing * 1000;

        printf("            For Loop (as reference) ...........................      %4.2f ms [ 1000000 x]\n\n",boucle_for_time);

        // Creation of return values the size of test_count

        for i=1:test_count

            // Display
            printf("   %03d/%03d - ",i,test_count);
            printf("[%s] %s ",test_list(i,1),test_list(i,2));
            for j = length(test_list(i,2) + test_list(i,1)):45
                printf(".");
            end
            printf(" ");

            // Bench process
            [returned_time, nb_run_done] = bench_run_onebench(test_list(i,1), test_list(i,2), nb_run);

            // restore user modes inside the loop
            // Protects from tests that modify those settings
            mode(saved_modes);
            ieee(saved_ieee);
            format(saved_format([2 1]));
            warning(saved_warning);
            funcprot(saved_funcprot);

            elapsed_time = [elapsed_time; returned_time];
            nb_iterations = [nb_iterations; nb_run_done];

            // Display
            returned_time_str           = sprintf("%4.2f ms",returned_time);
            for j = length(returned_time_str):13
                printf(" ");
            end
            printf("%s [",returned_time_str);
            for j = length(nb_run_done):7
                printf(" ");
            end
            printf("%s x]\n",nb_run_done);

            // XML print
            xml_str = [ xml_str ; ..
            "    <bench>"; ..
            "        <module>"+test_list(i,1)+"</module>"; ..
            "        <id>"+test_list(i,2)+"</id>"; ..
            "        <duration>"+strsubst(returned_time_str," ms","")+"</duration>"; ..
            "        <nb_run>"+nb_run_done+"</nb_run>"; ..
            "    </bench>" ];

        end
    end

    modutests_names = test_list;
    nb_iterations = eval(nb_iterations);

    // XML management
    // ==============
    //      exportToFile can be
    //      * "", "[]" or []: default behaviour, write the output file in the TMPDIR/benchmarks
    //      path/to/directory/: export a timestamped xml file to the output directory
    //      path/to/directory/filename.xml: exports filename.xml to the directory
    //      get the current date to create a timestamp

    // Close the final tag for export
    xml_str = [ xml_str ; "</benchmarks>" ];
    if size(unique(modutests_names(:,1)), "r") == 1
        module_name = tokens(pathconvert(modutests_names(1, 1), %f, %f, "u"), "/"); // name of the only module tested
        module_name = module_name($);
    else
        module_name = "";
    end

    if (rhs == 4)
        exportToFile = varargin(4);
        if (isempty(exportToFile) | exportToFile == "[]")
            exportToFile = "";
        end
    else
        exportToFile = "";
    end
    [xml_file_name, ierr, fd_xml] = bench_file_output_path(exportToFile);

    if ierr == 0 then
        mputl(xml_str, fd_xml);
        mclose(fd_xml);
    end

    clearglobal test_list;
    clearglobal test_count;
    clearglobal boucle_for_time;


endfunction

//-----------------------------------------------------------------------------
// Pierre MARECHAL
// Scilab team
// Date : 28 oct. 2007
//
// => List all test files in the module <module_mat>
// => Add them to the test_mat matrix
//-----------------------------------------------------------------------------

function bench_add_module(module_mat,test_type)

    module_test_dir = SCI+"/modules/"+module_mat+"/tests/benchmarks";
    test_mat        = gsort(basename(listfiles(module_test_dir+"/*.tst")),"lr","i");

    nl = size(test_mat,"*");
    for i=1:nl
        bench_add_onebench(module_mat,test_mat(i));
    end

endfunction

function [bench_list] = bench_list_tests(module_mat)

    module_test_dir = [];
    bench_list= [];
    for i = 1:size(module_mat, "*")
        if with_module(module_mat(i))
            // module_mat(i) is a scilab module
            module_test_dir = [module_test_dir; SCI+"/modules/"+module_mat(i)+"/tests/benchmarks"];
        else
            // module_mat(i) is a directory
            module_test_dir = [module_test_dir; module_mat(i) + "/tests/benchmarks"];
        end
        test_mat        = gsort(basename(listfiles(module_test_dir(i) + "/*.tst")),"lr","i");
        bench_list = [bench_list; [repmat(module_mat(i), size(test_mat, "*"), 1), test_mat]];
    end
endfunction

//-----------------------------------------------------------------------------
// Pierre MARECHAL
// Scilab team
// Date : 28 oct. 2007
//
// => Add the test <test> to the test_mat matrix
//-----------------------------------------------------------------------------

function bench_add_onebench(module,test)

    global test_list;
    global test_count;

    test_count = test_count + 1;

    test_list( test_count , 1 ) = module;
    test_list( test_count , 2 ) = test;

endfunction

//-----------------------------------------------------------------------------
// Pierre MARECHAL
// Scilab team
// Date : 28 oct. 2007
//
// => Run one test
//-----------------------------------------------------------------------------

function [returned_time,nb_run_done] = bench_run_onebench(module, test, nb_run)
    // runs the benchmark for module
    returned_time = 0;

    if with_module(module)
        fullPath      = SCI+"/modules/"+module+"/tests/benchmarks/"+test;
    else
        fullPath = module + "/tests/benchmarks/" + test;
    end

    tstfile       = pathconvert(fullPath+".tst",%f,%f);
    scefile       = pathconvert(TMPDIR+"/"+test+".sce",%f,%f);

    // Get the tst file
    txt = mgetl(tstfile);

    // Check if the nb run is defined in the test

    check_nb_run_line = grep(txt,"<-- BENCH NB RUN :");

    nb_run_done   = nb_run;

    if (check_nb_run_line <> [] & ~nb_run_override) then
        nb_run_line   = txt(check_nb_run_line);
        nb_run_start  = strindex(nb_run_line,"<-- BENCH NB RUN :") + length("<-- BENCH NB RUN :");
        nb_run_end    = strindex(nb_run_line,"-->") - 1;
        nb_run        = stripblanks(part(nb_run_line,[nb_run_start:nb_run_end]));
        nb_run_done   = nb_run;
    end

    // get the <-- BENCH START --> and <-- BENCH END --> tags
    line_start = grep(txt,"<-- BENCH START -->");
    line_end   = grep(txt,"<-- BENCH END -->");

    // Get the context and the bench
    // Take the whole file as bench if the tags are not found
    if isempty(line_start) | isempty(line_end)
        context = "";
        bench = txt;
        after = ""
    else
        context    = txt([1:line_start-1]);
        bench      = txt([line_start+1:line_end-1]);
        after      = txt([line_end:$]);
    end

    // Remove blank lines
    context(find(context == "" )) = [];
    bench  (find(bench   == "" )) = [];

    tst_str = [ context ;
    "nb_run = "+nb_run+";";
    "tic();";
    "for __loop_iterator__ = 1:nb_run";
    bench;
    "end";
    "timing = toc();";
    after;
    "returned_time = timing * 1000;"]

    mputl(tst_str,scefile);
    exec(scefile);
    returned_time = returned_time - (boucle_for_time * nb_run / 1d6)
    return;

endfunction

//-----------------------------------------------------------------------------
// Pierre MARECHAL
// Scilab team
// Date : 28 oct. 2007
//
// => Check ref or generate ref
//-----------------------------------------------------------------------------

function example = bench_examples()

    example = [ sprintf("Examples :\n\n") ];

    example = [ example ; sprintf("// Launch all tests\n") ];
    example = [ example ; sprintf("// This may take some time...\n") ];
    example = [ example ; sprintf("// bench_run();\n") ];
    example = [ example ; sprintf("// bench_run([]);\n") ];
    example = [ example ; sprintf("// bench_run([],[]);\n") ];
    example = [ example ; "" ];
    example = [ example ; sprintf("// Test one or several module\n") ];
    example = [ example ; sprintf("bench_run(''core'');\n") ];
    example = [ example ; sprintf("bench_run(''core'',[]);\n") ];
    example = [ example ; sprintf("bench_run([''core'',''string'']);\n") ];
    example = [ example ; "" ];
    example = [ example ; sprintf("// Launch one or several test in a specified module\n") ];
    example = [ example ; sprintf("bench_run(''core'',[''trycatch'',''opcode'']);\n") ];
    example = [ example ; "" ];
    example = [ example ; sprintf("// With options\n") ];
    example = [ example ; sprintf("bench_run([],[],''list'');\n") ];
    example = [ example ; sprintf("bench_run([],[],''help'');\n") ];
    example = [ example ; sprintf("bench_run(""string"",[],''nb_run=100'');\n") ];
    example = [ example ; sprintf("// results in an output file in the local directory\n") ];
    example = [ example ; sprintf("bench_run(""string"",[],''nb_run=100'', ""my_output_file.xml"");\n") ];
    example = [ example ; sprintf("// results in an output directory TMPDIR/benchmarks/ is the default \n") ];
    example = [ example ; sprintf("bench_run(""string"",[],''nb_run=100'', TMPDIR);\n") ];
    example = [ example ; "" ];

endfunction

function bench_add_dir(directory)
    // Scans directory for tests/benchmarks and add the benchmarks
    module_test_dir = directory + "/tests/benchmarks";
    test_mat        = gsort(basename(listfiles(module_test_dir+"/*.tst")),"lr","i");

    nl = size(test_mat,"*");
    for i=1:nl
        bench_add_onebench(directory, test_mat(i));
    end
endfunction

function [xml_file_name, ierr, fd_xml] = bench_file_output_path(exportPath, module_name)
    if exportPath == ""
        // Default for export is TMPDIR/benchmarks/
        exportPath = TMPDIR + "/benchmarks";
        if ~isdir(exportPath)
            createdir(exportPath);
        end
    end

    // Create timestamp and scilab short version
    current_date = getdate();
    current_date = msprintf("%d-%02d-%02d_%02d%02d%02d", current_date(1), current_date(2), current_date(6), current_date(7), current_date(8), current_date(9));
    sciversion = getversion("scilab");
    sciversion = string(sciversion);
    sciversion = sciversion(1) + "." + sciversion(2) + "." + sciversion(3);

    // Manage a single module name separation
    if (module_name <> "")
        module_name_sep = module_name + "_";
    else
        module_name_sep = "";
    end

    if isdir(exportPath)
        // The exportPath is a directory
        // build the inside this directory
        xml_file_name = exportPath + "/bench_" + module_name_sep + sciversion + "_" + current_date +".xml";
        ierr = execstr("fd_xml = mopen(xml_file_name,''wt'');","errcatch");
    else
        // The exportPath is not a directory
        xml_file_name = exportPath;
        ierr = execstr("fd_xml = mopen(xml_file_name,''wt'');","errcatch");
    end
    if ierr <> 0 then
        [xml_file_alt, ierr, fd_xml] = bench_file_output_path("", module_name);
        msg = msprintf(_("%s: Cannot create file %s, created file %s instead.\n"), "bench_run", fullpath(xml_file_name), strsubst(fullpath(xml_file_alt), TMPDIR, "TMPDIR"));
        warning(msg);
    end
endfunction
