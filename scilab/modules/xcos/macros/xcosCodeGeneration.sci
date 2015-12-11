//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009-2009 - DIGITEO - Bruno JOFRET
// Copyright (C) 2012 - Scilab Enterprises - Clement DAVID
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
//
//

function blk = xcosCodeGeneration(blk)
    // define scicos variables
    %scicos_prob = %f
    %scicos_debug_gr = %f
    needcompile = 4;

    ierr = execstr("[ok, XX] = do_compile_superblock42(blk, [], [], %f); ", "errcatch");
    if ierr <> 0 then
        [msg, err] = lasterror();
        disp(msg);

        // push blk error
        blk = [];
        blk = resume(blk)
    end

    if ~ok then
        // push blk error
        blk = [];
        blk = resume(blk)
    end

    blk = XX;
    // push the results (and interface function) on the upper scope
    execstr("[blk, " + XX.gui + "]=resume(blk, "+ XX.gui +")");
endfunction

