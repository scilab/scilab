// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Simon LIPP <simon.lipp@scilab.org>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

// Run the path/subdir/builder script, changing its working directory
// to path/subdir
function devtools_run_builder(path, subdir, builder)
    oldpath = pwd();

    chdir(path);

    if(isdir(subdir)) then
        chdir(subdir);
        exec(builder, -1);
    end

    chdir(oldpath);
endfunction
