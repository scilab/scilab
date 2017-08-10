// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Simon LIPP <simon.lipp@scilab.org>
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

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
