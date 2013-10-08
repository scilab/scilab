// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2004 - INRIA - Vincent COUVERT
// Copyright (C) ???? - INRIA - Serge STEER
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function count=mtlb_fwrite(fid,a,prec)
    // Emulation function for fwrite() Matlab function

    prectbl=["c"  "char"    "char"
    "?"  "schar"   "signed char"
    "s"  "short"   "short"
    "i"  "int"     "int"
    "l"  "long"    "long"
    "f"  "float"   "float"
    "d"  "double"  "double"
    "uc" "uchar"   "unsigned char"
    "us" "ushort"  "unsigned short"
    "ui" "uint"    "unsigned int"
    "ul" "ulong"   "unsigned long"
    "c"  "char"    "char*1"
    "f"  "float32" "real*4"
    "d"  "float64" "real*8"
    "c"  "int8"    "integer*1"
    "s"  "int16"   "integer*2"
    "i"  "int32"   "integer*4"
    "l"  ""        "integer*8"
    "?"  "intN"    ""
    "?"  "uintN"   ""         ]

    [lhs,rhs]=argn(0)
    if rhs<3 then prec="uchar";end

    [l,k]=find(prec==prectbl)
    if l==[] then
        error(msprintf(gettext("%s: The format ''%s'' is unknown.\n"),"mtlb_fwrite",prec));
    end
    Prec=prectbl(l,1)
    if Prec=="?" then
        error(msprintf(gettext("%s: The format ''%s'' is not yet handled.\n"),"mtlb_fwrite",prec));
    end
    mput(a,Prec,fid)
    count=size(a,"*")

endfunction
