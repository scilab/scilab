// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Laurent VAYLET
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function scs_m = renameIO(scs_m, idxBlock, newName)

    //  Description:
    //
    //    Modify input or output filename of a block whose index in scs_m is known
    //
    //  Input argument(s):
    //
    //    scs_m - MANDATORY - structure
    //      Diagram structure, such as obtained after a 'load' instruction
    //
    //    idxBlock - MANDATORY - single integer
    //      Index of the block, such as obtained after a 'findIOblocks' instruction
    //
    //    newName - MANDATORY - string
    //      New output filename
    //
    //  Output argument(s):
    //
    //    scs_m - MANDATORY - structure
    //      Updated diagram structure
    //
    //  Usage:
    //
    //    [idxWrite, idxRead] = findIOblocks(scs_m)
    //    scs_m = renameIO(scs_m, idxWrite, newName)
    //
    //  Algorithm: (grep "^\s*//--" renameIO.sci | awk -F "//-- " '{print "//  " $1 $2}')
    //
    //      Set new filename ...
    //      ... and new filename length
    //      VERY IMPORTANT: Do not forget to modify dialog box settings !!
    //
    //  Notes:
    //
    //    It is assumed that the diagram contains only one "Write to File" block
    //
    //    According to modules/scicos_blocks/src/fortran/writef.f and
    //     modules/scicos_blocks/src/fortran/readf.f :
    //      ipar(1) = lfil ............. filename length
    //      ipar(4) = N ................ buffer length
    //      ipar(5:4+lfil) ............. character codes for filename
    //
    //  2008/03/31  Laurent Vaylet    Creation
    //  YYYY/MM/DD  ???               Modification: ???
    //

    iparIO = scs_m.objs(idxBlock).model.ipar;
    lfil = iparIO(1);
    lfmt = iparIO(2);

    //-- Set new filename ...
    iparIO = [iparIO(1:4) ; _str2code(newName) ; iparIO(5+lfil:$)];
    //-- ... and new filename length
    iparIO(1) = length(newName);

    scs_m.objs(idxBlock).model.ipar = iparIO;

    //-- VERY IMPORTANT: Do not forget to modify dialog box settings !!
    // Otherwise, these saved strings are considered defaults values when calling
    //  scicos_simulate->do_eval->set->setvalue (a temporary alias for "getvalue")
    // Item to be modified here is graphics.exprs(2) for "Write to File" blocks or
    // graphics.exprs(3) for "Read from File" blocks
    if scs_m.objs(idxBlock).model.sim == "writef"
        scs_m.objs(idxBlock).graphics.exprs(2) = newName
    else
        scs_m.objs(idxBlock).graphics.exprs(3) = newName
    end
endfunction

