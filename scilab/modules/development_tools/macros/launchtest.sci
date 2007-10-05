//
// Sylvestre Ledru <sylvestre.ledru@inria.fr>
// INRIA - Scilab 2006 
// 
// Purpose : this file provides functions to test scilab for each modules
// Under Linux/Unix it is called by the command "make check"
// Which will call the target check-local in the file Makefile.incl.am

function launchtest(myPath,myTests)
	chdir(myPath)
	maketests(myTests,'clean')
	maketests(myTests,'test')
endfunction


function maketests(myTests,job)
	lines(0) // to disable line control
	if argn(2)<1 then job='tests',end
	if job=='clean' then //remove .dia files
		cleantests(myTests)
	else
		for k1=1:size(myTests,'*')  // loop on .tst files
			scitest(myTests(k1))
		end
	end
endfunction

function cleantests(myTests)
	if MSDOS then
		path=SCI+'\tests\basic_tests'
		file_to_clean=path+strsubst(myTests,'.tst','.dia')
		unix_s('del '+'""'+strcat(file_to_clean,'"" ""')+'""')
	else
		path='./'
		file_to_clean=path+strsubst(myTests,'.tst','.dia')
		unix_s('rm -f '+strcat(file_to_clean,' '))
	end
endfunction
