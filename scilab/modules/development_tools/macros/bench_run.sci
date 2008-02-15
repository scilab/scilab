
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2007-2008 - INRIA - Pierre MARECHAL <pierre.marechal@inria.fr>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

//-----------------------------------------------------------------------------
// Launch benchmarks
//-----------------------------------------------------------------------------

function bench_run(varargin)
	
	lhs = argn(1);
	rhs = argn(2);
	
	global test_list;
	global test_count;
	global boucle_for_time;
	
	test_list          = [];
	test_count         = 0;
	boucle_for_time    = 0;
	
	just_list_tests    = %F;
	print_help         = %F;
	nb_run             = "10000";
	
	xml_str            = "";
	
	// =======================================================
	// Gestion des tests à lancer
	// =======================================================
	
	if (rhs == 0) ..
		| ((rhs == 1) & (varargin(1)==[])) ..
		| (((rhs == 2)|(rhs == 3)) & (varargin(1)==[]) & (varargin(2)==[])) then
		
		// No input argument
		// bench_run()
		// bench_run([])
		// => Launch each test of each module
		
		module_list = getmodules();
		module_list = gsort(module_list,"lr","i");
		for k=1:size(module_list,'*')
			bench_add_module(module_list(k));
		end
	
	elseif (rhs == 1) ..
		| ((rhs == 2) & (varargin(2)==[])) ..
		| ((rhs == 3) & (varargin(2)==[])) then
		
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
					error(sprintf(gettext("%s is not an installed module"),module_mat(i,j)));
				end
			end
		end
		
	elseif (rhs == 2) | (rhs == 3) then
		
		// Two input arguments
		// bench_run(<module_name>,<test_name>)
		// bench_run(<module_name>,[<test_name_1>,<test_name_2>] )
		
		// varargin(1) = <module_name> ==> string 1x1
		// varargin(2) = <test_name_1> ==> mat nl x nc
		
		module     = varargin(1);
		test_mat   = varargin(2);
		
		if ((or(size(module) <> [1,1])) & (test_mat <> [])) then
			example = bench_examples();
			err     = ["" ; gettext("error : Input argument sizes are not valid") ; "" ; example ];
			printf("%s\n",err);
			return;
		end
		
		[nl,nc] = size(test_mat);
		
		for i=1:nl
			for j=1:nc
				
				if (fileinfo(SCI+"/modules/"+module+"/tests/benchmarks/"+test_mat(i,j)+".tst")<>[]) then
					bench_add_onebench(module,test_mat(i,j));
				else
					error(sprintf(gettext("The test ""%s"" is not available from the ""%s"" module"),test_mat(i,j),module));
				end
				
			end
		end
	else
		error(gettext('Number of parameters incorrect.'));
	end
	
	// =======================================================
	// Gestion des options
	// =======================================================
	
	if rhs == 3 then
		
		option_mat =  varargin(3);
		
		if grep(option_mat,"list") <> [] then
			just_list_tests    = %T;
		end
		
		if grep(option_mat,"help") <> [] then
			print_help         = %T;
		end
		
		if grep(option_mat,"nb_run=") <> [] then
			nb_run_line = grep(option_mat,"nb_run=");
			nb_run      = strsubst(option_mat(nb_run_line),"nb_run=","");
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
		
		printf("\n");
		
		xml_str = [ xml_str ; "<benchmarks>" ];
		
		// Calcul de la durée de la boucle for en µs
		
		timer();
		for i = 1:1000000
		end
		timing = timer();
		boucle_for_time = timing;
		
		printf("             Boucle For .......................................        %1.2f %ss [ 1000000 x]\n\n",boucle_for_time,ascii(181));
		
		for i=1:test_count
			
			// Display
			printf("   %03d/%03d - ",i,test_count);
			printf("[%s] %s ",test_list(i,1),test_list(i,2));
			for j = length(test_list(i,2) + test_list(i,1)):45
				printf(".");
			end
			printf(" ");
			
			// Bench process
			[returned_time,nb_run_done] = bench_run_onebench(test_list(i,1),test_list(i,2),nb_run);
			
			// Display
			returned_time_str           = sprintf("%4.2f %ss",returned_time,ascii(181));
			for j = length(returned_time_str):13
				printf(' ');
			end
			printf("%s [",returned_time_str);
			for j = length(nb_run_done):7
				printf(' ');
			end
			printf("%s x]\n",nb_run_done);
			
			// XML print
			xml_str = [ xml_str ; ..
				"    <bench>"; ..
				"        <module>"+test_list(i,1)+"</module>"; ..
				"        <id>"+test_list(i,2)+"</id>"; ..
				"        <duration>"+strsubst(returned_time_str," "+ascii(181)+"s","")+"</duration>"; ..
				"        <nb_run>"+nb_run_done+"</nb_run>"; ..
				"    </bench>" ];
			
		end
		
	end
	
	// XML management
	xml_str = [ xml_str ; "</benchmarks>" ];
	xml_file_name = SCI+"/bench_"+getversion()+"_"+date()+".xml";
	mputl(xml_str,xml_file_name);
	
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

function [returned_time,nb_run_done] = bench_run_onebench(module,test,nb_run)
	
	returned_time = 0;
	
	fullPath      = SCI+"/modules/"+module+"/tests/benchmarks/"+test;
	
	tstfile       = pathconvert(fullPath+".tst",%f,%f);
	scefile       = pathconvert(fullPath+".sce",%f,%f);
	datfile       = pathconvert(fullPath+".dat",%f,%f);
	
	// Get the tst file
	txt = mgetl(tstfile);
	
	// Check if the nb run is defined in the test
	
	check_nb_run_line = grep(txt,"<-- BENCH NB RUN :");
	
	nb_run_done   = nb_run;
	
	if check_nb_run_line <> [] then
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
	context    = txt([1:line_start-1]);
	bench      = txt([line_start+1:line_end-1]);
	
	// Remove blank lines
	context(find(context == '' )) = [];
	bench  (find(bench   == '' )) = [];
	
	tst_str = [ context ;
				"nb_run = "+nb_run+";";
				"timer();";
				"for i = 1:nb_run";
				bench;
				"end";
				"timing = timer();";
				"returned_time = timing * 1000000 / nb_run;"]
	
	mputl(tst_str,scefile);
	exec(scefile);
	returned_time = returned_time - boucle_for_time;
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
	example = [ example ; sprintf("bench_run();\n") ];
	example = [ example ; sprintf("bench_run([]);\n") ];
	example = [ example ; sprintf("bench_run([],[]);\n") ];
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
	example = [ example ; sprintf("bench_run([],[],''nb_run=2000'');\n") ];
	example = [ example ; "" ];
	
endfunction
