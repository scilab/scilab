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

function scs_m = setW2Fformat(scs_m, idxBlock, formatStr)

    //  Description:
    //
    //    Set output format of a "Write to File" block whose index in scs_m is known
    //
    //  Input argument(s):
    //
    //    scs_m - MANDATORY - structure
    //      Diagram structure, typically obtained after a 'load' instruction
    //
    //    idxBlock - MANDATORY - single integer
    //      Index of the block, typically obtained after a 'findW2Fblocks' instruction
    //
    //    formatStr - MANDATORY - single string
    //      Format used in output of the "Write to File" block
    //
    //  Output argument(s):
    //
    //    scs_m - MANDATORY - structure
    //      Updated diagram structure
    //
    //  Usage:
    //
    //    idxBlock = findW2Fblocks(scs_m); setW2Foutput(scs_m, idxBlock, formatStr)
    //
    //  Algorithm: (grep "^\s*//--" getW2Fformat.sci | awk -F "//-- " '{print "//  " $1 $2}')
    //
    //    -
    //
    //  Notes:
    //
    //    It is assumed that the diagram contains only one "Write to File" block
    //
    //    According to modules/scicos_blocks/src/fortran/writef.f:
    //      ipar(1) = lfil ............. filename length
    //      ipar(2) = lfmt ............. format length (0) if binary file
    //      ipar(3) .................... unused
    //      ipar(4) = N ................ buffer length
    //      ipar(5:4+lfil) ............. character codes for filename
    //      ipar(5+lfil:4+lfil+lfmt) ... character codes for format if any
    //
    //    Tip: convert from ASCII to human-readable => _code2str(ipar)
    //
    //  2008/03/31  Laurent Vaylet    Creation
    //  YYYY/MM/DD  ???               Modification: ???
    //

    ipar = scs_m.objs(idxBlock).model.ipar
    lfil = ipar(1);
    lfmt = ipar(2);

    //-- Set new format...
    ipar = [ipar(1:5+lfil-1) ; _str2code(formatStr)];
    //-- ... and new format length
    ipar(2) = length(formatStr);

    scs_m.objs(idxBlock).model.ipar = ipar;

    //-- VERY IMPORTANT: Do not forget to modify dialog box settings !!
    // Otherwise, these saved strings are considered defaults values when calling
    //  scicos_simulate->do_eval->set->setvalue (a temporary alias for "getvalue")
    // Item to be modified here is graphics.exprs(3) for "Write to File" blocks
    scs_m.objs(idxBlock).graphics.exprs(3) = formatStr
endfunction

