//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009-2009 - DIGITEO - Bruno JOFRET
// Copyright (C) 2009-2010 - DIGITEO - Clément DAVID
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

function blk = xcosBlockInterface(interfaceAlias, job, blk, context)

    // push some old scicos variable in environment.
    needcompile = 0;
    alreadyran = %f;

    // define context
    %scicos_context = struct();
    [%scicos_context, ierr] = script2var(context, %scicos_context)

    //for backward compatibility for scifunc
    if ierr==0 then
        %mm = getfield(1,%scicos_context)
        for %mi=%mm(3:$)
            ierr = execstr(%mi+"=%scicos_context(%mi)","errcatch")
            if ierr<>0 then
                break; //** in case of error exit
            end
        end
    end
    //end of for backward compatibility for scifunc

    ierr = execstr("[new_blk, y, typ] = interfaceAlias(job, blk, [])", "errcatch");
    if ierr <> 0 then
        [msg, err] = lasterror();
        disp(msg);
        return blk;
    end

    // Check if the block has been updated or not.
    // If the data has changed then we don't need to recompile (indicated by
    // no file creation).
    updated = and([needcompile == 0, and(new_blk == blk)]) <> %t;
    if updated then
        blk = new_blk;
    else
        blk = [];
    end
endfunction

