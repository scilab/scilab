//================================
//
// Copyright INRIA 2005
// Scilab team
// Date : 08/05/2006
// 
//================================

function tests=testslist()
tests = ['arma.tst';
	'binary.tst';
	'binary_representation.tst';
	'calpol.tst';
	'clause.tst';
	'cmp.tst';
	'comand.tst';
	'comment.tst';
	'dassldasrt.tst';
	'date.tst';
	'dcd.tst';
	'display.tst';
	'errcatch.tst';
	'feval.tst';
	'fptr.tst';
	'fscanfMat.tst';
	'function.tst';
	'global.tst';
	'grand.tst';
	'grandb.tst';
	'gsort.tst';
	'hypermat.tst'
	'listextraction.tst';
	'listinsertion.tst';
	'lsparse.tst';
	'lstops.tst';
	'lstops.tst';
	'macro.tst';
	'matelm.tst';
	'matimp.tst';
	'matode.tst';
	'matops.tst';
	'matopt.tst';
	'maxi.tst';
	'metanet.tst';
	'mfprintf.tst';
	'mfscanf.tst';
	'msprintf.tst';
	'msscanf.tst';
	'norm.tst';
	'odedc.tst';
	'odedi.tst';
	'polelm.tst';
	'polops.tst';
	'pvm.tst';
	'resume.tst';
	'saveload.tst';
	'scicos.tst';
	'scilab.tst';
	'slicot.tst';
	'sound.tst';
	'sparse.tst';
	'specfun.tst';
	'sprintf.tst';
	'sscanf.tst';
	'strelm.tst';
	'trycatch.tst';
	'parse.tst';
	'bvode.tst';
	'basiclink.tst';
	'links.tst';
	'algebra.tst';
	'intops.tst';
	'matsys.tst';
	'opcode.tst';
	'TCL_SetGet.tst';
	'breakpoints.tst';
	'pause.tst';
	'swap_handles.tst'; // graphics is broken 
	'auto.tst'; // graphics is broken 
	'filesassociation.tst';
	'scicos_filesassociation.tst';];
	
	
	if ~%scicos then
		tests(find(tests=='scicos.tst'))=[];
		tests(find(tests=='scicos_filesassociation.tst'))=[];
	end
	
	if ~MSDOS then
		tests(find(tests=='filesassociation.tst'))=[];
		tests(find(tests=='scicos_filesassociation.tst'))=[];
	end
	
	if ~%tk then
		tests(find(tests=='TCL_SetGet.tst'))=[];
	end
	
	if ~%pvm then
		tests(find(tests=='pvm.tst'))=[];
	end
	
	if MSDOS then
		tests(find(tests=='pvm.tst'))=[];
	end
	
endfunction

function maketests(job)
	lines(0) // to disable line control
	if argn(2)<1 then job='tests',end
	if job=='clean' then //remove .dia files
		cleantests()
	else
		tests=testslist()
		for k1=1:size(tests,'*')  // loop on .tst files
			scitest(tests(k1))
		end
	end
endfunction

function cleantests()
	if MSDOS then
		path=SCI+'\tests\basic_tests'
		file_to_clean=path+strsubst(testslist(),'.tst','.dia')
		unix_s('del '+'""'+strcat(file_to_clean,'"" ""')+'""')
	else
		path=SCI+'/tests/basic_tests'
		file_to_clean=path+strsubst(testslist(),'.tst','.dia')
		unix_s('rm -f '+strcat(file_to_clean,' '))
	end
endfunction
