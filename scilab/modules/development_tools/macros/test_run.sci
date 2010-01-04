// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2007-2008 - INRIA - Pierre MARECHAL <pierre.marechal@inria.fr>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

// test_run  --
//   Launch unit tests.
//   Search for .tst files in the unit test and non-regression test library
//   execute them, and display a report about success of failures.
//   The .tst files are searched in directories SCI+"/modules/*/tests/unit_tests"
//   and SCI+"/modules/*/tests/nonreg_tests".
//   Whenever a test is executed, a .dia file is generated which contains 
//   the full list of commands executed along with message which appears in the
//   console. When the script is done, the .dia file is compared with 
//   the .dia.ref file which is expected to be in the same directory 
//   as the .tst file. If the two file are different, the test fails.
//   Special tags may be inserted in the .tst file, which help to 
//   control the processing of the corresponding test. These tags
//   are expected to be found in Scilab comments.
//   These are the available tags :
//     <-- INTERACTIVE TEST -->
//       This test will be skipped because it is interactive.
//     <-- NOT FIXED -->
//       This test will be skipped because it is a known, but unfixed bug.
//     <-- TEST WITH GRAPHIC -->
//       This test will not be executed if the option "mode_nwni" is used.
//     <-- NO TRY CATCH -->
//     <-- NO CHECK ERROR OUTPUT -->
//       The error output file is not checked
//     <-- NO CHECK REF -->
//       The .dia and the .dia.ref files are not compared.
//     <-- ENGLISH IMPOSED -->
//       This test will be executed with the -l en_US option.
//     <-- FRENCH IMPOSED -->
//       This test will be executed with the -l fr_FR option.
//     <-- JVM NOT MANDATORY -->
//       This test will be executed with the nwni mode by default.
//     <-- WINDOWS ONLY -->
//       If the operating system isn't Windows, the test is skipped.
//     <-- UNIX ONLY -->
//       If the operating system isn't an unix OS, the test is skipped.
//     <-- LINUX ONLY -->
//       If the operating system isn't Linux, the test is skipped.
//     <-- MACOSX ONLY -->
//       If the operating system isn't MacOSX, the test is skipped.
//
//   Each test is executed in a separated process, created with the "host" command.
//   That enables the current command to continue, even if the test as
//   created an unstable environment. It also enables the tests to be 
//   independent from one another.
//
// Arguments:
//   modulename, optional : a string or a vector of strings, where 
//     each string is a the name of a module to test
//   testname, optional : a string, a vector or a matrix of strings, where 
//     each string is a the name of a test. For example, if testname is "foo",
//     the associated test file is "foo.tst".
//   options, optional : a string or a vector of strings, where options can
//     have the following values :
//       'no_check_ref' : does not check if the .dia and .dia.ref are equal
//       'no_check_error_output'
//       'create_ref' : create the .dia.ref file and does not check if the .dia and .dia.ref are equal
//       'list' : does not process the tests but displays a list of available tests
//       'mode_nw' : add the "-nw" option to the launch
//       'mode_nwni' : add the "-nwni" option to the launch
//       'help' : display some examples about how to use this command
//       "nonreg_tests" : runs only the non-regression tests, skipping unit tests
//       "unit_tests" : runs only the unit tests, skipping non-regression tests
//
// =============================================================================
// Launch tests
// =============================================================================
// Examples :
//
// Launch all tests
// test_run();
// test_run([]);
// test_run([],[]);
//
// Test one or several module
// test_run('core');
// test_run('core',[]);
// test_run(['core','string']);
//
// Launch one or several test in a specified module
// test_run('core',['trycatch','opcode']);
//
// With options
// test_run([],[],'no_check_ref');
// test_run([],[],'no_check_error_output');
// test_run([],[],'create_ref');
// test_run([],[],'list');
// test_run([],[],'mode_nw');
// test_run([],[],'mode_nwni');
// test_run([],[],'help');
// test_run([],[],['no_check_error_output','create_ref']);


function test_run(varargin)
	
	lhs = argn(1);
	rhs = argn(2);
	
	// test type
	type_filter  = "all_tests"; // By default, lauch nonreg tests AND unitary tests
	skip_mat     = [];          // The list of test to skip
	
	// =========================================================================
	// Print test_run help
	// =========================================================================
	
	if (rhs >= 3) & (~ isempty(grep(varargin(3),"help"))) then
		example = test_examples();
		printf("%s\n",example);
		return;
	end
	
	// =======================================================
	// Gestion des types de tests à lancer
	// =======================================================
	
	global MACOSX;
	global LINUX;
	
	if ~MSDOS then
		OSNAME = unix_g('uname');
		MACOSX = (strcmpi(OSNAME,"darwin") == 0);
		LINUX  = (strcmpi(OSNAME,"linux") == 0);
	else
		MACOSX = %F;
		LINUX  = %F;
	end
	
	global testsuite;
	testsuite = testsuite_new();
	
	if rhs >= 3 then
		
		option_mat = varargin(3);
		
		if (or(option_mat == "unit_tests") & or(option_mat == "nonreg_tests")) | (or(option_mat == "all_tests") ) then
			type_filter = "all_tests";
			
		elseif or(option_mat == "unit_tests") then
			type_filter = "unit_tests";
			
		elseif or(option_mat == "nonreg_tests") then
			type_filter = "nonreg_tests";
		end
		
		if or(option_mat == "skip_tests") then
			skip_mat = varargin(2);
		end
		
	end
	
	// =======================================================
	// Stacksize management
	// =======================================================
	
	gstacksize(10000000);
	stacksize(10000000);
	
	// =======================================================
	// Gestion des tests à lancer
	// =======================================================
	
	if (rhs == 0) ..
				| ((rhs == 1) & (varargin(1)==[])) ..
				| (((rhs == 2)|(rhs == 3)) & (varargin(1)==[]) & (varargin(2)==[])) then
		
		// No input argument
		// test_run()
		// test_run([])
		// => Launch each test of each module
		
		module_list = getmodules();
		module_list = gsort(module_list,"lr","i");
		
		for i=1:size(module_list,"*")
			module    = module_new();
			module    = module_set_name(module,module_list(i))
			module    = module_set_path(module,pathconvert(SCI+"/modules/"+module.name,%F));
			tests     = get_tests_from_module(module,type_filter,skip_mat);
			testsuite = testsuite_add_tests( testsuite, tests );
		end
		
	elseif (rhs == 1) ..
				| ((rhs == 2) & (varargin(2)==[])) ..
				| ((rhs == 3) & (varargin(2)==[])) ..
				| ( ~ isempty(skip_mat)) then
		
		// One input argument
		// test_run(<module_name>)
		// test_run([<module_name_1>,<module_name_2>])
		
		// varargin(1) = [<module_name_1>,<module_name_2>]
		
		module_mat = varargin(1);
		
		// test_run([<module_name_1>,<module_name_2>])
		
		for i=1:size(module_mat,"*")
			
			module = module_new();
			module = module_set_name(module,module_mat(i))
			
			 // It's a scilab internal module
			if with_module(module.items(1)) then
				module = module_set_path(module,pathconvert(SCI+"/modules/"+module.items(1),%F));
				
			// It's an external module
			elseif isdir(module.items(1)) then
				module = module_set_path(module,pathconvert(module.items(1),%F));
			
			// It's an error
			else
				error(sprintf(gettext("%s is not an installed module or toolbox"),module_mat(i)));
			end
			
			tests = get_tests_from_module(module,type_filter,skip_mat);
			
			testsuite = testsuite_add_tests( testsuite , tests);
			
		end
		
	elseif (rhs == 2) | (rhs == 3) then
		
		// Two input arguments
		// test_run(<module_name>,<test_name>)
		// test_run(<module_name>,[<test_name_1>,<test_name_2>] )
		
		// varargin(1) = <module_name> ==> string 1x1
		// varargin(2) = <test_name_1> ==> mat nl x nc
		
		module_in  = varargin(1);
		test_mat   = varargin(2);
		
		if ((or(size(module_in) <> [1,1])) & (test_mat <> [])) then
			
			example = test_examples();
			err     = ["" ; msprintf(gettext("%s: Wrong size for input argument."),"test_run") ; "" ; example ];
			printf("%s\n",err);
			return;
		end
		
		module = module_new();
		module = module_set_name(module,module_in)
		
		// It's a scilab internal module
		if with_module(module.items(1)) then
			module = module_set_path(module,pathconvert(SCI+"/modules/"+module.items(1),%F));
			
		// It's an external module
		elseif isdir(module.items(1)) then
			module = module_set_path(module,pathconvert(module.items(1),%F));
		
		// It's an error
		else
			error(sprintf(gettext("%s is not an installed module or toolbox"),module.items(1)));
		end
		
		for i=1:size(test_mat,"*")
			
			types    = ["unit_tests";"nonreg_tests"]
			my_tests = list();
			
			for j=1:size(types,"*")
				
				if (type_filter<>"all_tests") & (type_filter<>types(j)) then
					continue;
				end
				
				// Get the list of directories where search
				
				test_directory_path = module.path + "/tests/" + types(j);
				for k=2:size(module.items,"*")
					test_directory_path = test_directory_path + "/" + module.items(k);
				end
				
				// The main directory doesn't exist, continue the loop
				
				if ~ isdir(test_directory_path) then
					continue;
				end
				
				test_main_directory = directory_new();
				test_main_directory = directory_set_type(test_main_directory,types(j));
				test_main_directory = directory_set_module(test_main_directory,module);
				test_main_directory = directory_set_path(test_main_directory,test_directory_path);
				
				test_directories = get_directories(test_main_directory);
				
				for k=1:size(test_directories)
					
					this_directory = test_directories(k);
					
					test = st_new();
					test = st_set_name(   test , test_mat(i) );
					test = st_set_module( test , this_directory.module );
					test = st_set_type(   test , types(j) );
					test = st_set_path(   test , pathconvert( this_directory.path + "/" + test.name + ".tst" , %F ) );
					
					if ~ isempty( fileinfo(test.path) ) then
						test = st_analyse(test);
						my_tests($+1) = test;
					end
					
				end
				
			end
			
			if size(my_tests) == 0 then
				error(sprintf(gettext("The test ""%s"" is not available from the ""%s"" module"),test_mat(i),module.name));
			end
			
			testsuite = testsuite_add_tests(testsuite,my_tests);
			
		end
		
	else
		error(msprintf(gettext('%s: Wrong number of input arguments.'),"test_run"));
	end
	
	// =========================================================================
	// Gestion des options
	// =========================================================================
	
	if rhs >= 3 then
		
		// Mode
		
		if grep(option_mat,"mode_nw") <> [] then
			testsuite = testsuite_set_WM(testsuite,"NW");
		end
		
		if grep(option_mat,"mode_nwni") <> [] then
			testsuite = testsuite_set_WM(testsuite,"NWNI");
		end
		
		// Reference
		
		if grep(option_mat,"no_check_ref") <> [] then
			testsuite = testsuite_set_reference(testsuite,"skip")
		end
		
		if grep(option_mat,"create_ref") <> [] then
			testsuite = testsuite_set_reference(testsuite,"create")
		end
		
		// Error Output
		
		if grep(option_mat,"no_check_error_output") <> [] then
			testsuite = testsuite_set_EO(testsuite,"skip")
		end
		
	end
	
	// =========================================================================
	// List test
	// =========================================================================
	
	if (rhs >= 3) & (~ isempty(grep(option_mat,"list"))) then
		testsuite_list(testsuite);
		clearglobal testsuite;
		return;
	
	// =========================================================================
	// Test launch
	// =========================================================================
	
	else
		testsuite_run(testsuite);
		clearglobal testsuite;
		return;
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

function my_tests = get_tests_from_module(module,type_filter,skip_mat)
	
	my_types = ["unit_tests","nonreg_tests"];
	
	// 1st action : build the list of directories to explore
	// test_type : {"all_tests"|"unit_tests"|"nonreg_tests"}
	
	// Example of a directory :
	//   path: "/home/robert/scilab/modules/time/tests/unit_tests/date"
	//   type: "unit_tests"
	//   module: "time|date"
	
	my_directories = list();
	
	for i=1:size(my_types,"*")
		
		if (type_filter == "all_tests") | (type_filter == my_types(i)) then	
			
			directory = directory_new();
			directory = directory_set_type(directory,my_types(i));
			directory = directory_set_module(directory,module);
			
			directory_path = module.path+"/tests/"+my_types(i);
			module_items   = module.items;
			
			for j=2:size(module_items,"*")
				directory_path = directory_path + "/" + module_items(j);
			end
			
			directory = directory_set_path(directory,directory_path);
			
			if isdir(directory.path) then
				my_directories = lstcat( my_directories , get_directories(directory) );
			end
		end
	end
	
	// 2nd action : Build the list of tests
	// Example of a test :
	//   name: "bug_793"
	//   path: "/home/robert/scilab/modules/time/tests/nonreg_tests/bug_793.tst"
	//   type: "nonreg_tests"
	//   module: "time"
	
	my_tests = list();
	
	for i=1:length(my_directories)
		
		directory = my_directories(i);
		tests_in  = get_tests_from_directory(directory.path);
		tests_out = list();
		
		for j=1:length(tests_in)
			test = tests_in(j);
			if or(test.name==skip_mat) then 
				continue;
			end
			test = st_set_type(test,directory.type);
			test = st_set_module(test,directory.module);
			test = st_analyse(test);
			tests_out($+1) = test;
		end
		
		my_tests = lstcat( my_tests , tests_out );
		
	end
	
endfunction



//-----------------------------------------------------------------------------
// Pierre MARECHAL
// Scilab team
// Date : 28 août 2009
//
// List all test file in the directory
//-----------------------------------------------------------------------------

function tests = get_tests_from_directory(directory)
	
	tests = list();
	
	if isdir(directory) then
		test_mat = gsort(basename(listfiles(directory+"/*.tst")),"lr","i");
		for i=1:size(test_mat,"*")
			this_test  = st_new();
			this_test  = st_set_name(this_test,test_mat(i));
			this_test  = st_set_path(this_test,pathconvert(directory)+test_mat(i)+".tst");
			tests($+1) = this_test;
		end
	end
	
endfunction

//-----------------------------------------------------------------------------
// Pierre MARECHAL
// Scilab team
// Date : 28 août 2009
//
// => Get subdirectories of a directory
// 
// directories is a list of "directory" object (See directory interface)
//-----------------------------------------------------------------------------

function directories = get_directories(directory)
	
	this_directory_type   = directory.type;
	
	directories           = list();
	directories($+1)      = directory;
	
	items = gsort(listfiles(directory.path),"lr","i");
	
	for i=1:size(items,"*")
		if isdir(pathconvert(directory.path) + items(i)) then
			
			new_module = module_new();
			new_module = module_set_name( new_module , directory.module.name +  "|" + items(i) );
			new_module = module_set_path( new_module , pathconvert(directory.path) + items(i) );
			
			new_dir = directory_new();
			new_dir = directory_set_path(new_dir,pathconvert(directory.path) + items(i));
			new_dir = directory_set_module(new_dir,new_module);
			new_dir = directory_set_type(new_dir,directory.type) 
			
			directories = lstcat( directories , get_directories(new_dir) );
		end
	end
	
endfunction


//-----------------------------------------------------------------------------
// Pierre MARECHAL
// Scilab team
// Date : 8 novembre 2007
//
// => remove header from the diary txt
//
//-----------------------------------------------------------------------------

function dia_out = remove_headers(dia_in)
	
	dia_out = dia_in;
	
	body_start = grep(dia_out,"// <-- HEADER END -->");
	
	if body_start<>[] then
		dia_out(1:body_start(1)) = [];
	end
	
	body_end   = grep(dia_out,"// <-- FOOTER START -->");
	
	if body_end<>[] then
		[dia_nl,dia_nc] = size(dia);
		dia_out(body_end(1):dia_nl) = [];
	end
	
	return;
	
endfunction

//-----------------------------------------------------------------------------
// Pierre MARECHAL
// Scilab team
// Date : 28 oct. 2007
//
// => Check ref or generate ref
//-----------------------------------------------------------------------------

function example = test_examples()
	
	example = [ sprintf("Examples :\n\n") ];

	
	example = [ example ; sprintf("// Launch all tests\n") ];
	example = [ example ; sprintf("test_run();\n") ];
	example = [ example ; sprintf("test_run([]);\n") ];
	example = [ example ; sprintf("test_run([],[]);\n") ];
	example = [ example ; "" ];
	example = [ example ; sprintf("// Test one or several module\n") ];
	example = [ example ; sprintf("test_run(''core'');\n") ];
	example = [ example ; sprintf("test_run(''core'',[]);\n") ];
	example = [ example ; sprintf("test_run([''core'',''string'']);\n") ];
	example = [ example ; "" ];
	example = [ example ; sprintf("// Launch one or several test in a specified module\n") ];
	example = [ example ; sprintf("test_run(''core'',[''trycatch'',''opcode'']);\n") ];
	example = [ example ; "" ];
	example = [ example ; sprintf("// With options\n") ];
	example = [ example ; sprintf("test_run([],[],''no_check_ref'');\n") ];
	example = [ example ; sprintf("test_run([],[],''no_check_error_output'');\n") ];
	example = [ example ; sprintf("test_run([],[],''create_ref'');\n") ];
	example = [ example ; sprintf("test_run([],[],''list'');\n") ];
	example = [ example ; sprintf("test_run([],[],''help'');\n") ];
	example = [ example ; sprintf("test_run([],[],[''no_check_ref'',''mode_nw'']);\n") ];
	example = [ example ; "" ];
	
endfunction

// =============================================================================
// Pierre MARECHAL
// Scilab team
// Date : 31 août 2009
//
// singletest interface
// =============================================================================

// constructor
// -----------------------------------------------------------------------------

function st = st_new()
	
	st = tlist([ "T_SINGLETEST"   ..
				 "name"           ..
				 "type"           ..
				 "path"           ..
				 "module"         ..
				 "skip"           ..
				 "content"        ..
				 "interactive"    ..
				 "notyetfixed"    ..
				 "reopened"       ..
				 "platform"       ..
				 "language"       ..
				 "jvm_mandatory"  ..
				 "graphic"        ..
				 "mode"           ..    // NW, NWNI, GUI
				 "reference"      ..    // check, create, skip
				 "error_output"   ..    // check, skip
				 "try_catch"      .. 
				 "path_dia"       ..    // diary file
				 "path_dia_ref"   ..    // reference file
				 "tmp_tst"        ..    // diary file
				 "tmp_dia"        ..    // reference file
				 "tmp_res"        ..    // diary file
				 "tmp_err"        ..    // reference file
				 "status"         ..    // status
				 "cmd"            ..    // command to launch
				 ] );
				 
	// Default values
	st.skip          = %F;
	st.interactive   = %F;
	st.notyetfixed   = %F;
	st.reopened      = %F;
	st.jvm_mandatory = %T;
	st.graphic       = %F;
	st.mode          = "";
	st.platform      = "all";
	st.language      = "any";
	st.try_catch     = %T;
	st.error_output  = "check";
	st.reference     = "check";
	
	st.path_dia      = "";
	st.path_dia_ref  = "";

	st.tmp_tst       = "";
	st.tmp_dia       = "";
	st.tmp_res       = "";
	st.tmp_err       = "";
	
	st.cmd           = "";
	
	st.content       = "";
	
	st.status        = status_new();
	
endfunction

// setters
// -----------------------------------------------------------------------------

function st = st_set_name(st,name)
	
	st.name = name;
	
	st.tmp_tst       = pathconvert( TMPDIR + "/" + name + ".tst" , %F);
	st.tmp_dia       = pathconvert( TMPDIR + "/" + name + ".dia" , %F);
	st.tmp_res       = pathconvert( TMPDIR + "/" + name + ".res" , %F);
	st.tmp_err       = pathconvert( TMPDIR + "/" + name + ".err" , %F);
	
endfunction

function st = st_set_type(st,sttype)
	st.type = sttype;
endfunction

function st = st_set_path(st,path)
	
	st.path     = path;
	basepath    = strsubst(path,"/\.tst$/","","r");
	st.path_dia = basepath + ".dia";
	
	st.path_dia_ref = basepath + ".dia.ref";
	
	// Reference file management OS by OS
	if MSDOS then
		altreffile = [ basepath+".win.dia.ref" ];
	elseif MACOSX then
		altreffile = [ basepath+".unix.dia.ref" ; basepath+".macosx.dia.ref" ];
	elseif LINUX then
		altreffile = [ basepath+".unix.dia.ref" ; basepath+".linux.dia.ref" ];
	else
		altreffile = [ basepath+".unix.dia.ref" ];
	end
	
	for i=1:size(altreffile,"*")
		if ~ isempty(fileinfo(altreffile(i))) then
			st.path_dia_ref = altreffile(i);
		end
	end
	
endfunction

function st = st_set_module(st,module)
	st.module = module;
endfunction

function st = st_set_skip(st,skip)
	st.skip = skip;
endfunction

function st = st_set_content(st,content)
	st.content = content;
endfunction

function st = st_set_interactive(st,interactive)
	st.interactive = interactive;
endfunction

function st = st_set_notyetfixed(st,notyetfixed)
	st.notyetfixed = notyetfixed;
endfunction

function st = st_set_reopened(st,reopened)
	st.reopened = reopened;
endfunction

function st = st_set_platform(st,platform)
	st.platform = platform;
endfunction

function st = st_set_jvm_mandatory(st,jvm_mandatory)
	st.jvm_mandatory = jvm_mandatory;
endfunction

function st = st_set_graphic(st,graphic)
	st.graphic = graphic;
endfunction

function st = st_set_language(st,language)
	st.language = language;
endfunction

function st = st_set_try_catch(st,try_catch)
	st.try_catch = try_catch;
endfunction

function st = st_set_error_output(st,error_output)
	st.error_output = error_output;
endfunction

function st = st_set_reference(st,reference)
	st.reference = reference;
endfunction

function st = st_set_status(st,status)
	st.status = status;
endfunction

function st = st_set_cmd(st,cmd)
	st.cmd = cmd;
endfunction

function st = st_set_mode(st,smode)
	st.mode = smode;
endfunction

// show
// -----------------------------------------------------------------------------

function st_show(st)
	
	if st.skip           then st_skip           = "Yes"; else st_skip           = "No"; end
	if st.interactive    then st_interactive    = "Yes"; else st_interactive    = "No"; end
	if st.notyetfixed    then st_notyetfixed    = "Yes"; else st_notyetfixed    = "No"; end
	if st.reopened       then st_reopened       = "Yes"; else st_reopened       = "No"; end
	if st.jvm_mandatory  then st_jvm_mandatory  = "Yes"; else st_jvm_mandatory  = "No"; end
	if st.graphic        then st_graphic        = "Yes"; else st_graphic        = "No"; end
	if st.try_catch      then st_try_catch      = "Yes"; else st_try_catch      = "No"; end
	
	mprintf("Test :\n");
	mprintf("  name           = %s\n"   ,st.name);
	mprintf("  type           = %s\n"   ,st.type);
	mprintf("  module         = %s\n"   ,st.module.name);
	mprintf("\n");
	
	mprintf("Test paths :\n");
	mprintf("  path           = %s\n"   ,st.path);
	mprintf("  path_dia       = %s\n"   ,st.path_dia);
	mprintf("  path_dia_ref   = %s\n"   ,st.path_dia_ref);
	mprintf("  tmp_tst        = %s\n"   ,st.tmp_tst);
	mprintf("  tmp_dia        = %s\n"   ,st.tmp_dia);
	mprintf("  tmp_res        = %s\n"   ,st.tmp_res);
	mprintf("  tmp_err        = %s\n"   ,st.tmp_err);
	mprintf("\n");
	
	mprintf("Test features :\n");
	mprintf("  skip           = %s\n"   ,st_skip);
	mprintf("  interactive    = %s\n"   ,st_interactive);
	mprintf("  notyetfixed    = %s\n"   ,st_notyetfixed);
	mprintf("  reopened       = %s\n"   ,st_reopened);
	mprintf("  platform       = %s\n"   ,st.platform);
	mprintf("  jvm_mandatory  = %s\n"   ,st_interactive);
	mprintf("  graphic        = %s\n"   ,st_graphic);
	mprintf("  mode           = %s\n"   ,st.mode);
	mprintf("  reference      = %s\n"   ,st.reference);
	mprintf("  error_output   = %s\n"   ,st.error_output);
	mprintf("  try_catch      = %s\n"   ,st_try_catch);
	mprintf("\n");
	
	mprintf("Test scilab cmd :\n");
	mprintf("  cmd            = %s\n"   ,st.cmd);
	mprintf("\n");
	
	module_show(test.module);
	status_show(test.status);
	
endfunction

// Analyse
// -----------------------------------------------------------------------------

function st = st_analyse(st)
	
	if typeof(st) <> "T_SINGLETEST" then
		error(msprintf(gettext("%s: Wrong type for input argument #%d: %s expected.\n"),"st_analyse","T_SINGLETEST",1));
	end
	
	if isempty( fileinfo(st.path) ) then
		error(msprintf(gettext("%s: The test ''%s'' doesn''t exist or is not read available.\n"),"st_analyse",st.path));
	end
	
	// Get the test content
	st = st_set_content(st,mgetl(st.path));
	
	// Test status
	// =========================================================================
	
	if ~ isempty( grep(st.content,"<-- NOT FIXED -->") ) then
		st = st_set_notyetfixed(st,%T);
	end
	
	if ~ isempty( grep(st.content,"<-- REOPENED -->") ) then
		st = st_set_reopened(st,%T);
	end
	
	// platform
	// =========================================================================
	
	if ~ isempty( grep(st.content,"<-- WINDOWS ONLY -->") ) then
		st = st_set_platform(st,"windows");
	end
	
	if ~ isempty( grep(st.content,"<-- LINUX ONLY -->") ) then
		st = st_set_platform(st,"linux");
	end
	
	if ~ isempty( grep(st.content,"<-- MACOSX ONLY -->") ) then
		st = st_set_platform(st,"macosx");
	end
	
	if ~ isempty( grep(st.content,"<-- MACOSX ONLY -->") ) then
		st = st_set_platform(st,"macosx");
	end
	
	// Test execution
	// =========================================================================
	
	if ~ isempty( grep(st.content,"<-- INTERACTIVE TEST -->") ) then
		st = st_set_interactive(st,%T);
	end
	
	if ~ isempty( grep(st.content,"<-- TEST WITH GRAPHIC -->") ) then
		st = st_set_graphic(st,%T);
		st = st_set_jvm_mandatory(st,%T);
		st = st_set_mode(st,"NW");
	end
	
	if ~ isempty( grep(st.content,"<-- JVM NOT MANDATORY -->") ) then
		st = st_set_jvm_mandatory(st,%F);
		st = st_set_mode(st,"NWNI");
	end
	
	// Language
	// =========================================================================
	
	if ~ isempty( grep(st.content,"<-- FRENCH IMPOSED -->") ) then
		st = st_set_language(st,"fr_FR");
	end
	
	if ~ isempty( grep(st.content,"<-- ENGLISH IMPOSED -->") ) then
		st = st_set_language(st,"en_US");
	end
	
	// Test building
	// =========================================================================
	
	if ~ isempty( grep(st.content,"<-- NO TRY CATCH -->") ) then
		st = st_set_try_catch(st,%F);
	end
	
	// Test result
	// =========================================================================
	
	if ~ isempty( grep(st.content,"<-- NO CHECK ERROR OUTPUT -->") ) then
		st = st_set_error_output(st,"skip");
	end
	
	if ~ isempty( grep(st.content,"<-- NO CHECK REF -->") ) then
		st = st_set_reference(st,"skip");
	end
	
endfunction


//-----------------------------------------------------------------------------
// Pierre MARECHAL
// Scilab team
// Date : 28 oct. 2007
//
// => Run one test
//
// Example of test variable :
//    name: "bug_793"
//    path: "/home/robert/scilab/modules/time/tests/nonreg_tests/bug_793.tst"
//    type: "nonreg_tests"
//    module: "time"
//
//-----------------------------------------------------------------------------

function st = st_run(st)
	
	//Reset standard globals
	rand("seed",0);
	rand("uniform");
	
	st.status = status_new();
	
	// Case where the test is skipped
	// =========================================================================
	
	// The test is interactive
	
	if st.interactive then
		st.status = status_set_id(st.status,10);
		st.status = status_set_message(st.status,"skipped : interactive test");
		return;
	end
	
	// The bug is not yet fixed
	
	if st.notyetfixed then
		st.status = status_set_id(st.status,10);
		st.status = status_set_message(st.status,"skipped : not yet fixed");
		return;
	end
	
	// The bug is reopenned
	
	if st.reopened then
		st.status = status_set_id(st.status,10);
		st.status = status_set_message(st.status,"skipped : Bug reopened");
		return;
	end
	
	// The test cannot be launched on this platform
	
	if (st.platform=="windows") & (~MSDOS) then
		st.status = status_set_id(st.status,10);
		st.status = status_set_message(st.status,"skipped : Windows only");
		return;
	end
	
	if (st.platform=="unix") & MSDOS then
		st.status = status_set_id(st.status,10);
		st.status = status_set_message(st.status,"skipped : Unix only");
		return;
	end
	
	if (st.platform=="linux") & (~LINUX) then
		st.status = status_set_id(status,10);
		st.status = status_set_message(status,"skipped : Linux only");
		return;
	end
	
	if (st.platform=="macosx") & (~MACOSX) then
		st.status = status_set_id(st.status,10);
		st.status = status_set_message(st.status,"skipped : MacOSX only");
		return;
	end
	
	// The test launches some graphic windows
	
	if st.graphic & (testsuite.wanted_mode == "NWNI") then
		st.status = status_set_id(st.status,10);
		st.status = status_set_message(st.status,"skipped : Test with graphic");
		return;
	end
	
	// Build the test
	// =========================================================================
	
	txt = st.content;
	
	// Do some modification in tst file
	txt = strsubst(txt,"pause,end" ,"bugmes();quit;end");
	txt = strsubst(txt,"pause, end","bugmes();quit;end");
	txt = strsubst(txt,"pause;end" ,"bugmes();quit;end");
	txt = strsubst(txt,"pause; end","bugmes();quit;end");
	txt = strsubst(txt,"-->","@#>"); //to avoid suppression of input --> with prompts
	txt = strsubst(txt,"halt();","");
	
	// Test header
	
	head = [                                                                    ...
		"// <-- HEADER START -->";                                              ...
		"mode(3);" ;                                                            ...
		"lines(28,72);";                                                        ...
		"lines(0);" ;                                                           ...
		"function %onprompt" ;                                                           ...
		"quit;" ;                                                           ...
		"endfunction" ;                                                           ...
		"deff(''[]=bugmes()'',''write(%io(2),''''error on test'''')'');" ;      ...
		"predef(''all'');" ;                                                    ...
		"tmpdirToPrint = msprintf(''TMPDIR1=''''%s''''\n'',TMPDIR);"            ...
	]
	
	if st.try_catch then
		head = [ head ; "try" ];
	end
	
	head = [                                                                    ...
		head ;                                                                  ...
		"diary(''"+st.tmp_dia+"'');";                                          ...
		"write(%io(2),tmpdirToPrint);";                                         ...
		"// <-- HEADER END -->"                                                 ...
	];
	
	// Test footer
	
	tail = [ "// <-- FOOTER START -->" ];
	
	if st.try_catch then
		tail = [ tail;                                                          ...
			"catch";                                                            ...
			"   errmsg = ""<--""+""Error on the test script file""+""-->"";";   ...
			"   printf(""%s\n"",errmsg);";                                      ...
			"   lasterror()";                                                   ...
			"end";                                                              ...
			];
	end
	
	tail = [ tail; "diary(0);" ];
	
	if st.graphic then
		tail = [ tail; "xdel(winsid());sleep(1000);" ];
	end
	
	tail = [ tail; "exit;" ; "// <-- FOOTER END -->" ];
	
	// Assembly
	
	txt = [head;
		txt;
		tail];
	
	// Build the command
	// =========================================================================
	
	// Gestion de l'emplacement de bin/scilab
	// -------------------------------------------------------------------------
	
	if (~MSDOS) & (fileinfo(SCI+"/bin/scilab")==[]) then
		SCI_BIN = strsubst(SCI,'share/scilab','');
	else
		SCI_BIN = SCI;
	end
	
	// Mode Argument (NW, NWNI, GUI)
	// -------------------------------------------------------------------------
	
	if testsuite.wanted_mode == "NW" then
		mode_arg = "-nw";
	
	elseif testsuite.wanted_mode == "NWNI" then
		mode_arg = "-nwni";
	
	else
		if st.mode == "NWNI" then
			mode_arg = "-nwni";
		elseif st.mode == "NW" then
			mode_arg = "-nw";
		else
			mode_arg = "-nw";
		end
	end
	
	// Language Argument (en_US, fr_FR, ... )
	// -------------------------------------------------------------------------
	
	if st.language == "any" then
		language_arg = "";
	else
		language_arg = "-l "+ st.language;
	end
	
	// Assembly
	// -------------------------------------------------------------------------
	
	if MSDOS then
		test_cmd = "( """+SCI_BIN+"\bin\scilex.exe"+""""+" "+mode_arg+" "+language_arg+" -nb -f """+st.tmp_tst+""" > """+st.tmp_res+""" ) 2> """+st.tmp_err+"""";
	else
		test_cmd = "( "+SCI_BIN+"/bin/scilab "+mode_arg+" "+language_arg+" -nb -f "+st.tmp_tst+" > "+st.tmp_res+" ) 2> "+st.tmp_err;
	end
	
	st= st_set_cmd(st,test_cmd);
	
	// Remove the previous tmp files
	// =========================================================================
	
	if ~ isempty(fileinfo(st.tmp_tst)) then
		deletefile(st.tmp_tst);
	end
	
	if ~ isempty(fileinfo(st.tmp_dia)) then
		deletefile(st.tmp_dia);
	end
	
	if ~ isempty(fileinfo(st.tmp_res)) then
		deletefile(st.tmp_res);
	end
	
	if ~ isempty(fileinfo(st.tmp_err)) then
		deletefile(st.tmp_err);
	end
	
	
	// Write the tmp test file
	// =========================================================================
	mputl(txt,st.tmp_tst);
	
	
	// Launch the test exec
	// =========================================================================
	host(st.cmd);
	
	
	
	// First Check : error output
	// =========================================================================
	
	if (st.error_output == "check") & (testsuite.error_output == "check") then
		
		tmp_errfile_info = fileinfo(st.tmp_err);
		
		if ( (tmp_errfile_info <> []) & (tmp_errfile_info(1)<>0) ) then
			st.status = status_set_id(st.status,5);
			st.status = status_set_message(st.status,"failed  : error_output not empty");
			st.status = status_set_details(st.status,sprintf("     Check the following file : \n     - %s",st.tmp_err));
			return;
		end
	end
	
	//  Get the dia file
	dia = mgetl(st.tmp_dia);
	
	// To get TMPDIR value
	tmpdir1_line = grep(dia,"TMPDIR1");
	execstr(dia(tmpdir1_line));
	
	//Check for execution errors
	if st.try_catch & grep(dia,"<--Error on the test script file-->")<>[] then
		details = [ sprintf("     Check the following file : \n     - %s",st.tmp_dia); ..
					sprintf("     Or launch the following command : \n     - exec %s;",st.path) ];
		st.status = status_set_id(st.status,3);
		st.status = status_set_message(st.status,"failed  : premature end of the test script");
		st.status = status_set_details(st.status,details);
		return;
	end
	
	// Remove Header and Footer
	dia = remove_headers(dia);
	
	//Check for execution errors
	dia_tmp                     = dia;
	dia_tmp(grep(dia_tmp,"//")) = [];  // remove commented lines
	
	if st.try_catch & grep(dia_tmp,"!--error")<>[] then
		details = [ sprintf("     Check the following file : \n     - %s",st.tmp_dia); ..
					sprintf("     Or launch the following command : \n     - exec %s;",st.path) ];
		st.status = status_set_id(st.status,1);
		st.status = status_set_message(st.status,"failed  : the string (!--error) has been detected");
		st.status = status_set_details(st.status,details);
		return;
	end
	
	
	if grep(dia_tmp,"error on test")<>[] then
		details = [ sprintf("     Check the following file : \n     - %s",st.tmp_dia); ..
					sprintf("     Or launch the following command : \n     - exec %s;",st.path) ];
		st.status = status_set_id(st.status,2);
		st.status = status_set_message(st.status, "failed  : one or several tests failed");
		st.status = status_set_details(st.status,details);
		return;
	end
	
	
	if tmpdir1_line == [] then
		st.status = status_set_id(st.status,6);
		st.status = status_set_message(st.status, "failed  : the dia file is not correct");
		st.status = status_set_details(st.status,sprintf("     Check the following file : \n     - %s",st.tmp_dia));
		return;
	end
	
	
	
	// Check the reference file only if check_ref (i.e. for the whole 
	// test sequence) is true and this_check_ref (i.e. for the specific current .tst)
	// is true.
	
	if (st.reference=="check") & (testsuite.reference=="check")  then
		if isempty(fileinfo(st.path_dia_ref)) then
			st.status = status_set_id(st.status,5);
			st.status = status_set_message(st.status,"failed  : the ref file doesn''t exist");
			st.status = status_set_details(st.status,sprintf("     Add or create the following file : \n     - %s",st.path_dia_ref));
			return;
		end
	end
	
	// Comparaison ref <--> dia
	
	if ( (st.reference=="check") & (testsuite.reference=="check") ) | (testsuite.reference=="create") then
		
		//  Do some modification in  dia file
		dia(grep(dia,"exec("))                     = [];
		dia(grep(dia,"write(%io(2),tmpdirToPrint"))= [];
		dia(grep(dia,"TMPDIR1"))                   = [];
		dia(grep(dia,"diary(0)"))                  = [];
		
		dia = strsubst(dia,TMPDIR ,"TMPDIR");
		dia = strsubst(dia,TMPDIR1,"TMPDIR");
		
		if MSDOS then
			dia = strsubst(dia,strsubst(TMPDIR ,"\","/"),"TMPDIR");
			dia = strsubst(dia,strsubst(TMPDIR1,"\","/"),"TMPDIR");
			dia = strsubst(dia,strsubst(TMPDIR ,"/","\"),"TMPDIR");
			dia = strsubst(dia,strsubst(TMPDIR1,"/","\"),"TMPDIR");
			dia = strsubst(dia,strsubst(getshortpathname(TMPDIR) ,"\","/"),"TMPDIR");
			dia = strsubst(dia,strsubst(getshortpathname(TMPDIR1),"\","/"),"TMPDIR");
			dia = strsubst(dia,getshortpathname(TMPDIR) ,"TMPDIR");
			dia = strsubst(dia,getshortpathname(TMPDIR1),"TMPDIR");
		end
		
		dia = strsubst(dia,SCI,"SCI");
	
		if MSDOS then
			dia = strsubst(dia,strsubst(SCI ,"\","/"),"SCI");
			dia = strsubst(dia,strsubst(SCI ,"/","\"),"SCI");
			dia = strsubst(dia,strsubst(getshortpathname(SCI) ,"\","/"),"SCI");
			dia = strsubst(dia,getshortpathname(SCI) ,"SCI");
		end
		
		//suppress the prompts
		dia = strsubst(dia,"-->" ,"");
		dia = strsubst(dia,"@#>" ,"-->");
		dia = strsubst(dia,"-1->","");
		
		//standardise  number display
		
		// strsubst(dia," .","0.");
		// strsubst(dia,"-.","-0.")
		// strsubst(dia,"E+","D+");
		// strsubst(dia,"E-","D-");
		
		//not to change the ref files
		dia = strsubst(dia,"bugmes();return","bugmes();quit");
		
		if testsuite.reference=="create" then
			
			// Delete previous .dia.ref file
			if fileinfo(st.path_dia_ref) <> [] then
				deletefile(st.path_dia_ref)
			end
			
			mputl(dia,st.path_dia_ref);
			
			st.status = status_set_id(st.status,20);
			st.status = status_set_message(st.status,"passed : ref created");
			return;
			
		else
			
			// write down the resulting dia file
			mputl(dia,st.path_dia);
			
			//Check for diff with the .ref file
			
			[u,ierr] = mopen(st.path_dia_ref,"r");
			if ierr== 0 then //ref file exists
				
				ref=mgetl(u);
				mclose(u)
				
				// suppress blank (diff -nw)
				
				dia = strsubst(dia,' ','')
				ref = strsubst(ref,' ','')
				
				dia(find(dia=='')) = [];
				ref(find(ref=='')) = [];
				
				dia(find(dia=='')) = [];
				ref(find(ref=='')) = [];
				
				dia( find(part(dia,(1:2))=="//") ) = [];
				ref( find(part(ref,(1:2))=="//") ) = [];
				
				if or(ref<>dia) then
					st.status = status_set_id(st.status,4);
					st.status = status_set_message(st.status,"failed  : dia and ref are not equal");
					st.status = status_set_details(st.status,sprintf("     Compare the following files : \n     - %s\n     - %s",st.path_dia,st.path_dia_ref));
					return;
				end
				
			else
				error(sprintf(gettext("The ref file (%s) doesn''t exist"),st.path_dia_ref));
			end
		end
		
	end
	
	st.status = status_set_id(st.status,0);
	st.status = status_set_message(st.status,"passed");
	
	return;
	
endfunction







// =============================================================================
// Pierre MARECHAL
// Scilab team
// Date : 31 août 2009
//
// module interface
// =============================================================================

// constructor
// -----------------------------------------------------------------------------

function md = module_new()
	
	md = tlist([  "T_MODULE" ..
				  "name" ..
				  "path" ..
				  "items" ] );
	
endfunction

// setters
// -----------------------------------------------------------------------------

function md = module_set_name(md,name)
	md.name  = name;
	if isempty( regexp(stripblanks(name),"/\|/") ) then
		md.items = [ name ];
	else
		md.items = stripblanks( strsubst( strsplit(name,regexp(stripblanks(name),"/\|/")) , "/\|$/","","r" ) );
	end
endfunction

function md = module_set_path(md,path)
	md.path = path;
endfunction

// show
// -----------------------------------------------------------------------------

function module_show(module)
		mprintf("Module :\n");
		mprintf("  name           = %s\n"   ,module.name);
		mprintf("  path           = %s\n"   ,module.path);
		mprintf("  items          = %s\n"   ,module.items);
		mprintf("\n");
endfunction



// =============================================================================
// Pierre MARECHAL
// Scilab team
// Date : 31 août 2009
//
// directory interface
// =============================================================================

// constructor
// -----------------------------------------------------------------------------

function directory = directory_new()
	
	directory = tlist([  "T_DIRECTORY" ..
				  "path"         ..
				  "module"       ..
				  "type"         ..
				  ]);
	
endfunction

// setters
// -----------------------------------------------------------------------------

function directory = directory_set_path(directory,path)
	directory.path = path;
endfunction

function directory = directory_set_module(directory,module)
	directory.module = module;
endfunction

function directory = directory_set_type(directory,dtype)
	directory.type = dtype;
endfunction


// show
// -----------------------------------------------------------------------------

function directory_show(directory)
	mprintf("Directory :\n");
	mprintf("  path           = %s\n"   ,directory.path);
	mprintf("  module         = %s\n"   ,directory.module.name);
	mprintf("  type           = %s\n"   ,directory.type);
	mprintf("\n");
endfunction




// =============================================================================
// Pierre MARECHAL
// Scilab team
// Date : 31 août 2009
//
// testsuite interface
// =============================================================================

// constructor
// -----------------------------------------------------------------------------

function testsuite = testsuite_new()
	
	testsuite = tlist([  "T_TESTSUITE" ..
				 "items"           ..
				 "current_mode"    ..    // NW, NWNI, GUI
				 "wanted_mode"     ..    // NW, NWNI, GUI
				 "reference"       ..    // check, create, skip
				 "error_output"    ..    // check, skip
				 ]);
	
	testsuite.items = list();
	
	// Default values
	
	if and(getscilabmode() <> ["NW";"STD"]) then
		testsuite.current_mode = "NWNI";
	else
		testsuite.current_mode = "NW";
	end
	
	testsuite.wanted_mode  = "";
	testsuite.reference    = "check";
	testsuite.error_output = "check";
	
endfunction

// Add tests
// -----------------------------------------------------------------------------

function testsuite = testsuite_add_tests(testsuite,tests)
	testsuite.items = lstcat(testsuite.items,tests);
endfunction

function testsuite = testsuite_add_one_test(testsuite,test)
	testsuite.items($+1) = test;
endfunction

// Number of tests
// -----------------------------------------------------------------------------

function l = testsuite_length(testsuite)
	l = size(testsuite.items);
endfunction

// Setters
// -----------------------------------------------------------------------------

function testsuite = testsuite_set_WM(testsuite,wanted_mode)
	testsuite.wanted_mode = wanted_mode;
endfunction

function testsuite = testsuite_set_reference(testsuite,reference)
	testsuite.reference = reference;
endfunction

function testsuite = testsuite_set_EO(testsuite,error_output)
	testsuite.error_output = error_output;
endfunction

// List tests
// -----------------------------------------------------------------------------

function testsuite_list(testsuite)
	
	for i=1:size(testsuite.items)
		test = testsuite.items(i);
		printf("   %03d - ",i);
		printf("[%s] %s\n",test.module.name,test.name);
	end
	
endfunction

// Launch tests
// -----------------------------------------------------------------------------

function testsuite_run(testsuite)

	details_failed     = "";
	test_count         = 0;
	test_passed_count  = 0;
	test_failed_count  = 0;
	test_skipped_count = 0;
	
	printf("   TMPDIR = %s\n",TMPDIR);
	printf("\n");
	
	start_date = getdate();
	test_count = length(testsuite.items);
	
	for i=1:test_count
		
		test        = testsuite.items(i);
		test_module = test.module.name;
		test_name   = test.name;
		
		// Improve the display of the module
		if isdir(test_module) then
			if part(test.module.name,1:length(SCI)) == SCI then
				test_module = "SCI" + part(test_module,length(SCI)+1:length(test_module));
			elseif part(test.module.name,1:length(SCIHOME)) == SCIHOME then
				test_module = "SCIHOME" + part(test_module,length(SCIHOME)+1:length(test_module));
			end
		end
		
		printf("   %03d/%03d - ",i,test_count);
		printf("[%s] %s",test_module,test_name);
		for j = length(test_name+test_module):50
			printf(".");
		end
		
		test = st_run(test);
		
		printf("%s \n",test.status.message);
		
		// Recencement des tests
		
		if test.status.id == 0 then
			// passed
			test_passed_count = test_passed_count + 1;
		
		elseif (test.status.id > 0) & (test.status.id < 10) then
			// failed
			test_failed_count = test_failed_count + 1;
			details_failed = [ details_failed ; sprintf("   TEST : [%s] %s",test.module.name,test.name)];
			details_failed = [ details_failed ; sprintf("     %s",test.status.message) ];
			details_failed = [ details_failed ; test.status.details ];
			details_failed = [ details_failed ; "" ];
		
		elseif (test.status.id >= 10) & (test.status.id < 20) then
			// skipped
			test_skipped_count = test_skipped_count + 1;
		end
	end
	
	end_date = getdate();
	
	// Summary
	
	if test_count<>0 then
		test_passed_percent  = test_passed_count  / test_count * 100;
		test_skipped_percent = test_skipped_count / test_count * 100;
		test_failed_percent  = test_failed_count  / test_count * 100;
	else
		test_passed_percent  = 0;
		test_skipped_percent = 0;
		test_failed_percent  = 0;
	end
	
	printf("\n");
	printf("   --------------------------------------------------------------------------\n");
	printf("   Summary\n\n");
	printf("   tests                     %4d - 100 %% \n",test_count);
	printf("   passed                    %4d - %3d %% \n",test_passed_count ,test_passed_percent);
	printf("   failed                    %4d - %3d %% \n",test_failed_count ,test_failed_percent);
	printf("   skipped                   %4d - %3d %% \n",test_skipped_count,test_skipped_percent);
	printf("   length                          %4.2f sec \n" ,etime(end_date,start_date));
	printf("   --------------------------------------------------------------------------\n");
	
	if test_failed_count > 0 then
		printf("   Details\n\n");
		printf("%s\n",details_failed);
		printf("\n");
		printf("   --------------------------------------------------------------------------\n");
	end

endfunction


// =============================================================================
// Pierre MARECHAL
// Scilab team
// Date : 1 septembre 2009
//
// status
// =============================================================================

// constructor
// -----------------------------------------------------------------------------

function status = status_new()
	status = tlist([  "T_STATUS" ..
				"id"             ..
				"message"        ..
				"details"        ..
				]);
				
	status.id      = 0;
	status.message = "";
	status.details = "";
	
endfunction

// setters
// -----------------------------------------------------------------------------

function status = status_set_id(status,id)
	status.id = id;
endfunction

function status = status_set_message(status,smessage)
	status.message = smessage;
endfunction

function status = status_set_details(status,details)
	status.details = details;
endfunction

// show
// -----------------------------------------------------------------------------

function status_show(status)
	mprintf("Status :\n");
	mprintf("  id             = %d\n"   ,status.id);
	mprintf("  message        = %s\n"   ,status.message);
	mprintf("  details        = %s\n"   ,status.details);
	mprintf("\n");
endfunction


