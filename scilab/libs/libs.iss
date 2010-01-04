;
; Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
; Copyright (C) 2008 - DIGITEO - Allan CORNET
;
; This file must be used under the terms of the CeCILL.
; This source file is licensed as described in the file COPYING, which
; you should have received as part of this distribution.  The terms
; are also available at
; http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
;
;
; f2c directory
Source: libs\f2c\f2c.h; DestDir: {app}\libs\f2c; Components: {#COMPN_SCILAB}
;
; MALLOC directory
Source: libs\MALLOC\includes\*.h; DestDir: {app}\libs\MALLOC\includes; Components: {#COMPN_SCILAB}
;
; dynamiclibrary directory
Source: libs\dynamiclibrary\includes\*.h; DestDir: {app}\libs\dynamiclibrary\includes; Components: {#COMPN_SCILAB}
;
; doublylinkedlist directory
Source: libs\doublylinkedlist\includes\*.h; DestDir: {app}\libs\doublylinkedlist\includes; Components: {#COMPN_SCILAB}

; hashtable directory
Source: libs\hashtable\*.h; DestDir: {app}\libs\hashtable\includes; Components: {#COMPN_SCILAB}
;
; intl used by localization
Source: libs\intl\*.h; DestDir: {app}\libs\intl; Components: {#COMPN_SCILAB}
;
