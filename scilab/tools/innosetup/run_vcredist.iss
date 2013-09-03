;
; Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
; Copyright (C) Scilab Enterprises - 2013 - Antoine ELIAS
;
; This file must be used under the terms of the CeCILL.
; This source file is licensed as described in the file COPYING, which
; you should have received as part of this distribution.  The terms
; are also available at
; http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
;
;-------------------------------------------------------------------------------
; Inno Setup Script (5.3 and more) for Scilab (UNICODE version required)
;
;-------------------------------------------------------------------------------

#ifdef SCILAB_X64
    Filename: {tmp}\vcredist_x64.exe; Parameters: "/passive"; Description:{cm:VCREDIST}; Flags: waituntilterminated;
#else
    Filename: {tmp}\vcredist_x86.exe; Parameters: "/passive"; Description:{cm:VCREDIST}; Flags: waituntilterminated;
#endif
