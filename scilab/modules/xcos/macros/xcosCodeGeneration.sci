//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009-2009 - DIGITEO - Bruno JOFRET
// Copyright (C) 2012 - Scilab Enterprises - Clement DAVID
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.
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

