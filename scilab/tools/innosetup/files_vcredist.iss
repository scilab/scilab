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
Source: tools\innosetup\VCREDIST2012\vcredist_x64.exe; DestDir: {tmp}; Flags: deleteafterinstall ignoreversion
#else
Source: tools\innosetup\VCREDIST2012\vcredist_x86.exe; DestDir: {tmp}; Flags: deleteafterinstall ignoreversion
#endif
