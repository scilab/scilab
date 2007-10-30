//-----------------------------------------------------------------------------
// Pierre MARECHAL
// Scilab team
// Copyright INRIA
// Date : 25/10/2007
//
// Launch unitary tests
//-----------------------------------------------------------------------------

function unit_test_run(varargin)
	
	lhs = argn(1);
	rhs = argn(2);
	
	global test_list;
	global test_count;
	
	test_count         = 0;
	test_passed_count  = 0;
	test_failed_count  = 0;
	test_skipped_count = 0;
	
	select rhs
		case 0
			// No input argument
			// unit_test_run()
			// => Launch each test of each module
			
			module_list = getmodules();
			for k=1:size(module_list,'*')
				unit_test_add_module(module_list(k));
			end
			
			break
		
		case 1
			
			// One input argument
			// unit_test_run(<module_name>)
			// unit_test_run([<module_name_1>,<module_name_2>])
			
			// varargin(1) = [<module_name_1>,<module_name_2>]
			
			module_mat = varargin(1);
			
			[nl,nc] = size(module_mat);
			
			for i=1:nl
				for j=1:nc
					if( with_module(module_mat(i,j)) ) then
						unit_test_add_module(module_mat(i,j));
					else
						error(sprintf(gettext("%s is not an installed module"),module_mat(i,j)));
					end
				end
			end
			
			break
			
		case 2
			
			// Two input arguments
			// unit_test_run(<module_name>,<test_name>)
			// unit_test_run(<module_name>,[<test_name_1>,<test_name_2>] )
			
			// varargin(1) = <module_name> ==> string 1x1
			// varargin(2) = <test_name_1> ==> mat nl x nc
			
			module   = varargin(1);
			test_mat = varargin(2);
			
			if (size(module) <> [1,1]) & (size(module) <> [1,1]) then
				error(gettext("Input argument sizes are not valid"));
			end
			
			[nl,nc] = size(test_mat);
			
			for i=1:nl
				for j=1:nc
					if( fileinfo(SCI+"/modules/"+module+"/unit_tests/"+test_mat(i,j)+".tst") <> [] ) then
						unit_test_add_onetest(module,test_mat(i,j));
					else
						error(sprintf(gettext("The test ""%s"" is not available from the ""%s"" module"),test_mat(i,j),module));
					end
				end
			end
			
			break
			
		else
			error(gettext('Number of parameters incorrect.'));
	end

	// Test launch
	
	printf("\n");
	
	for i=1:test_count
		
		printf("\t %02d/%02d - ",i,test_count);
		printf("[%s] %s",test_list(i,1),test_list(i,2));
		for j = length(test_list(i,2) + test_list(i,1)):50
			printf(".");
		end
		
		[status_id,status_msg] = unit_test_run_onetest(test_list(i,1),test_list(i,2));
		printf("%s\n",status_msg);
		
		if status_id == 0 then
			test_passed_count = test_passed_count + 1;
		elseif status_id > 0 then
			test_failed_count = test_failed_count + 1;
		end
	end
	
	// Summary
	
	test_passed_percent = test_passed_count / test_count * 100;
	test_failed_percent = test_failed_count / test_count * 100;

	printf("\n");
	printf("\t---------------------------------------------------------------------------------------\n");
	printf("\tSummary\n\n");
	printf("\ttests                     %4d - 100 %% \n",test_count);
	printf("\tpassed                    %4d - %3d %% \n",test_passed_count,test_passed_percent);
	printf("\tfailed                    %4d - %3d %% \n",test_failed_count,test_failed_percent);
	printf("\t---------------------------------------------------------------------------------------\n");
	
endfunction

//-----------------------------------------------------------------------------
// Pierre MARECHAL
// Scilab team
// Copyright INRIA
// Date : 28 oct. 2007
//
// => List all test files in the module <module_mat>
// => Add them to the test_mat matrix
//-----------------------------------------------------------------------------

function unit_test_add_module(module_mat)
	
	module_test_dir = SCI+"/modules/"+module_mat+"/unit_tests";
	test_mat        = basename(listfiles(module_test_dir+"/*.tst"));
	
	nl = size(test_mat,"*");
	for i=1:nl
		unit_test_add_onetest(module_mat,test_mat(i));
	end
	
endfunction

//-----------------------------------------------------------------------------
// Pierre MARECHAL
// Scilab team
// Copyright INRIA
// Date : 28 oct. 2007
//
// => Add the test <test> to the test_mat matrix
//-----------------------------------------------------------------------------

function unit_test_add_onetest(module,test)
	
	global test_list;
	global test_count;
	
	test_count = test_count + 1;
	test_list( test_count , 1 ) = module;
	test_list( test_count , 2 ) = test;
	
endfunction

//-----------------------------------------------------------------------------
// Pierre MARECHAL
// Scilab team
// Copyright INRIA
// Date : 28 oct. 2007
//
// => Run one test
//-----------------------------------------------------------------------------

function [status_id,status_msg] = unit_test_run_onetest(module,test)
	
	status_id  = 0 ;
	status_msg = "passed" ;
	
	// Some definitions
	
	tstfile     = pathconvert(SCI+"/modules/"+module+"/unit_tests/"+test+".tst",%f,%f);
	diafile     = pathconvert(SCI+"/modules/"+module+"/unit_tests/"+test+".dia",%f,%f);
	reffile     = pathconvert(SCI+"/modules/"+module+"/unit_tests/"+test+".dia.ref",%f,%f);
	
	tmp_tstfile = pathconvert(TMPDIR+"/"+test+".tst",%f,%f);
	tmp_diafile = pathconvert(TMPDIR+"/"+test+".dia",%f,%f);
	tmp_resfile = pathconvert(TMPDIR+"/"+test+".res",%f,%f);
	tmp_errfile = pathconvert(TMPDIR+"/"+test+".err",%f,%f);
	
	// DEBUG DEBUG DEBUG DEBUG DEBUG DEBUG DEBUG DEBUG DEBUG DEBUG DEBUG DEBUG
	// printf("tstfile = %s\n",tstfile);
	// printf("diafile = %s\n",diafile);
	// printf("tmp_tstfile = %s\n",tmp_tstfile);
	// printf("tmp_diafile = %s\n",tmp_diafile);
	// DEBUG DEBUG DEBUG DEBUG DEBUG DEBUG DEBUG DEBUG DEBUG DEBUG DEBUG DEBUG
	
	//Reset standard globals
	rand('seed',0);
	rand('uniform');

	// Do some modification in  tst file
	
	txt = mgetl(tstfile);
	txt = strsubst(txt,'pause,end','bugmes();quit;end');
	txt = strsubst(txt,'-->','@#>'); //to avoid suppression of input --> with prompts
	txt = strsubst(txt,'halt()','');
	
	// Header : the same for each test
	
	head = [	"// <-- HEADER START -->";
				"mode(3);" ;
				"clear;" ;
				"lines(28,72);";
				"lines(0);" ;
				"deff(''[]=bugmes()'',''write(%io(2),''''error on test'''')'');" ;
				"predef(''all'');" ;
				"tmpdirToPrint = msprintf(''TMPDIR1=''''%s''''\n'',TMPDIR);" ;
				"try";
				"diary(''"+tmp_diafile+"'');";
				"write(%io(2),tmpdirToPrint);";
				"// <-- HEADER END -->"];
				
	tail = [	"// <-- FOOTER START -->";
				"catch";
				"	printf(""Error on the test script file"")";
				"end";
				"diary(0);";
				"exit;";
				"// <-- FOOTER END -->"]
	
	txt = [head;
		txt;
		tail];
	
	// and save it in a temporary file
	mputl(txt,tmp_tstfile);
	
	// Delete previous dia file
	if fileinfo(diafile) <> [] then
		deletefile(diafile)
	end
	
	// Build the command to launch
	if MSDOS then
		unit_test_cmd = """"+SCI+"\bin\scilex.exe"+""""+" -nw -args -nouserstartup -f "+tmp_tstfile+" 1>NUL";
	else
		unit_test_cmd = "( "+SCI+"/bin/scilab -nw -nb -args -nouserstartup -f "+tmp_tstfile+" > "+tmp_resfile+" ) 2> "+tmp_errfile;
	end
	
	// Launch the test exec
	
	//printf("%s\n",unit_test_cmd);
	host(unit_test_cmd);

	// First Check
	tmp_errfile_info = fileinfo(tmp_errfile);
	if ( (tmp_errfile_info <> []) & (tmp_errfile_info(1)<>0) ) then
		status_msg = "failed : error_output not empty"
		status_id  = 5;
		return;
	end
	
	//  Do some modification in  dia file
	dia = mgetl(tmp_diafile);
	
	// To get TMPDIR line
	tmpdir1_line = grep(dia,"TMPDIR1");
	execstr(dia(tmpdir1_line));
	
	// Remove Header and Footer
	
	body_start = grep(dia,"// <-- HEADER END -->");
	
	if body_start<>[] then
		dia(1:body_start(1)) = [];
	end
	
	body_end   = grep(dia,"// <-- FOOTER START -->");
	
	if body_end<>[] then
		[dia_nl,dia_nc] = size(dia);
		dia(body_end(1):dia_nl) = [];
	end
	
	//Check for execution errors
	
	if grep(dia,"!--error")<>[] then
		status_msg = "failed : the string (!--error) has been detected";
		status_id  = 1;
		return;
	end
	
	if grep(dia,"error on test")<>[] then
		status_msg = "failed : one or several unit tests failed";
		status_id  = 2;
		return;
	end
	
	if grep(dia,"Error on the test script file")<>[] then
		status_msg = "failed : premature end of the test script";
		status_id = 3;
		return;
	end
	
	//  Do some modification in  dia file
	dia(grep(dia,"exec("))                     = [];
	dia(grep(dia,"write(%io(2),tmpdirToPrint"))= [];
	dia(grep(dia,"TMPDIR1"))                   = [];
	dia(grep(dia,"diary(0)"))                  = [];
	
	dia = strsubst(dia,TMPDIR,'TMPDIR');
	dia = strsubst(dia,TMPDIR1,'TMPDIR');
	dia = strsubst(dia,TMPDIR1,'TMPDIR');
	dia = strsubst(dia,SCI,'SCI');
	
	//suppress the prompts
	dia = strsubst(dia,'-->','');
	dia = strsubst(dia,'@#>','-->');
	dia = strsubst(dia,'-1->','');
	
	//standardise  number display
	dia = strsubst(strsubst(strsubst(strsubst(dia,' .','0.'),'E+','D+'),'E-','D-'),'-.','-0.');

	//not to change the ref files
	dia=strsubst(dia,'bugmes();return','bugmes();quit');
	
	// write down the resulting dia file
	mputl(dia,diafile)
	
	//Check for diff with the .ref file
	
	[u,ierr] = mopen(reffile,'r');
	if ierr== 0 then //ref file exists
		
		ref=mgetl(u);
		mclose(u)
		
		// suppress blank (diff -nw)
		dia=strsubst(dia,' ','')
		ref=strsubst(ref,' ','')
		
		if or(ref<>dia) then
			if MSDOS then
				status_msg = "failed : dia and ref are not equal";
				status_id = 4;
			else
				status_msg = "failed : dia and ref are not equal";
				status_id = 4;
			end
		end
	else
		error(sprintf(gettext("The ref file (%s) doesn''t exist"),reffile));
	end
	
	return;
	
endfunction
