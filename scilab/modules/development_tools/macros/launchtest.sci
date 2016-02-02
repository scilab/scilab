// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2006-2008 - INRIA - Sylvestre LEDRU <sylvestre.ledru@inria.fr>
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

// Purpose : this file provides functions to test scilab for each modules
// Under Linux/Unix it is called by the command "make check"
// Which will call the target check-local in the file Makefile.incl.am

function launchtest(myPath,myTests)
    chdir(myPath)
    maketests(myTests,"clean")
    maketests(myTests,"test")
endfunction

function maketests(myTests,job)
    lines(0) // to disable line control
    if argn(2)<1 then job="tests",end
    if job=="clean" then //remove .dia files
        cleantests(myTests)
    else
        for k1=1:size(myTests,"*")  // loop on .tst files
            scitest(myTests(k1))
        end
    end
endfunction

function cleantests(myTests)
    if getos() == "Windows" then
        path=SCI+"\tests\basic_tests"
        file_to_clean=path+strsubst(myTests,".tst",".dia")
        unix_s("del "+""""+strcat(file_to_clean,""" """)+"""")
    else
        path="./"
        file_to_clean=path+strsubst(myTests,".tst",".dia")
        unix_s("rm -f "+strcat(file_to_clean," "))
    end
endfunction
