//================================
//
// Copyright INRIA 2005
// Scilab team
// Date : December 8th 2005
// 
//================================

function tests=testslist()
tests = ['algebra.tst';
	'arma.tst';
	'auto.tst';
	'basiclink.tst';
	'binary.tst';
	'breakpoints.tst';
	'bvode.tst';
	'call.tst';
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
	'intops.tst';
	'links.tst';
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
	'matsys.tst';
	'maxi.tst';
	'metanet.tst';
	'mfprintf.tst';
	'mfscanf.tst';
	'msprintf.tst';
	'msscanf.tst';
	'norm.tst';
	'odedc.tst';
	'odedi.tst';
	'opcode.tst';
	'pause.tst';
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
	'TCL_SetGet.tst';
	'filesassocation.tst'];
	
	if ~MSDOS then
		tests(find(tests=='filesassocation.tst'))=[];
	end
	
	if ~with_tk() then
		tests(find(tests=='links.tst'))=[];
	end
	
	if ~with_pvm() then
		tests(find(tests=='pvm.tst'))=[];
	end
	
	if MSDOS then
		comp_target = getenv('COMPILER','NO');
		if comp_target == 'gcc ' then 
			tests(find(tests=='links.tst'))=[]
		end
		tests(find(tests=='pvm.tst'))=[]
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
		path=SCI+'\tests\'
		file_to_clean=path+strsubst(testslist(),'.tst','.dia')
		unix_s('del '+'""'+strcat(file_to_clean,'"" ""')+'""')
	else
		path=SCI+'/tests/'
		file_to_clean=path+strsubst(testslist(),'.tst','.dia')
		unix_s('rm -f '+strcat(file_to_clean,' '))
	end
endfunction
