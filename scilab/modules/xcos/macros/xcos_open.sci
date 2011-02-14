//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009-2010 - DIGITEO - Antoine ELIAS <antoine.elias@scilab.org>
// Copyright (C) 2010-2010 - DIGITEO - Clément DAVID <clement.david@scilab.org>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
//
//

function xcos_open(blockId, show)
// Open a block from scilab
//
// Calling Sequence
//  xcos_open(blockId);
//  xcos_open(blockId, show);
//
// Parameters
//  blockId: the id of the block
//  show: if the block settings may be opened
//
// Description
//  To communicate with from scilab to Java, we use an id per block stored in 
//  the first block.doc field.
    [lhs, rhs] = argn(0);
    if rhs == 1 then
        show = %t;
    end

    if size(scs_m.objs(blockId).doc) > 0 then
        xcosDiagramOpen(scs_m.objs(blockId).doc(1), show);
    end
endfunction
