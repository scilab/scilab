function maketests(job)
	path=pwd()
	cd SCI/tests/automatic_tests
	lines(0) // to disable line control
	if argn(2)<1 then job='tests',end
	if job=='clean' then //remove .dia files
		cleantests()
	else
		force=%f
		tests=testslist()
		for k1=1:size(tests,'*')  // loop on .tst files
			if k1 <> 'banner.tst' then
				scitest(tests(k1),postscript_graphics=%f);
			end
		end
	end
	cd(path)
endfunction


function tests=testslist()
	tests=listfiles('*.tst')
endfunction


function cleantests()
	file_to_clean=strsubst(testslist(),'.tst','.dia')
	if MSDOS then
		unix_s('del '+strcat(file_to_clean,' '))
	else
		unix_s('rm -f '+strcat(file_to_clean,' '))
	end
endfunction
