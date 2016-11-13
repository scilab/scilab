;
; Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
; Copyright (C) INRIA - Allan CORNET
; Copyright (C) DIGITEO - 2010 - Allan CORNET
;
; Copyright (C) 2012 - 2016 - Scilab Enterprises
;
; This file is hereby licensed under the terms of the GNU GPL v2.0,
; pursuant to article 5.3.4 of the CeCILL v.2.1.
; This file was originally licensed under the terms of the CeCILL v2.1,
; and continues to be available under such terms.
; For more information, see the COPYING file which you should have received
; along with this program.
;
;--------------------------------------------------------------------------------------------------------------
; Inno Setup Script (5.3 and more) for Scilab (UNICODE version required)
;
;--------------------------------------------------------------------------------------------------------------
; core module
;--------------------------------------------------------------------------------------------------------------
;
#define CORE "core"
;
Source: bin\{#CORE}_f.dll; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
Source: bin\{#CORE}_f.lib; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
Source: bin\{#CORE}.dll; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
Source: bin\{#CORE}.lib; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
Source: bin\{#CORE}_gw.dll; DestDir: {app}\bin; Components: {#COMPN_SCILAB}

;
Source: modules\{#CORE}\jar\org.scilab.modules.{#CORE}.jar;DestDir: {app}\modules\{#CORE}\jar; Components: {#COMPN_SCILAB} and {#COMPN_JVM_MODULE}
;
Source: modules\{#CORE}\license.txt; DestDir: {app}\modules\{#CORE}; Components: {#COMPN_SCILAB}
;
Source: modules\{#CORE}\sci_gateway\{#CORE}_gateway.xml; DestDir: {app}\modules\{#CORE}\sci_gateway; Components: {#COMPN_SCILAB}
;
Source: modules\{#CORE}\etc\{#CORE}.quit; DestDir: {app}\modules\{#CORE}\etc; Components: {#COMPN_SCILAB}
Source: modules\{#CORE}\etc\{#CORE}.start; DestDir: {app}\modules\{#CORE}\etc; Components: {#COMPN_SCILAB}
;
Source: modules\{#CORE}\includes\backtrace_print.h; DestDir: {app}\modules\{#CORE}\includes; Components: {#COMPN_SCILAB}
Source: modules\{#CORE}\includes\BOOL.h; DestDir: {app}\modules\{#CORE}\includes; Components: {#COMPN_SCILAB}
Source: modules\{#CORE}\includes\core_math.h; DestDir: {app}\modules\{#CORE}\includes; Components: {#COMPN_SCILAB}
Source: modules\{#CORE}\includes\existfunction.h; DestDir: {app}\modules\{#CORE}\includes; Components: {#COMPN_SCILAB}
Source: modules\{#CORE}\includes\exit_status.hxx; DestDir: {app}\modules\{#CORE}\includes; Components: {#COMPN_SCILAB}
Source: modules\{#CORE}\includes\freeArrayOfString.h; DestDir: {app}\modules\{#CORE}\includes; Components: {#COMPN_SCILAB}
Source: modules\{#CORE}\includes\getcommandlineargs.h; DestDir: {app}\modules\{#CORE}\includes; Components: {#COMPN_SCILAB}
Source: modules\{#CORE}\includes\getmemory.h; DestDir: {app}\modules\{#CORE}\includes; Components: {#COMPN_SCILAB}
Source: modules\{#CORE}\includes\getos.h; DestDir: {app}\modules\{#CORE}\includes; Components: {#COMPN_SCILAB}
Source: modules\{#CORE}\includes\getversion.h; DestDir: {app}\modules\{#CORE}\includes; Components: {#COMPN_SCILAB}
Source: modules\{#CORE}\includes\hashtable.h; DestDir: {app}\modules\{#CORE}\includes; Components: {#COMPN_SCILAB}
Source: modules\{#CORE}\includes\hashtable_utility.h; DestDir: {app}\modules\{#CORE}\includes; Components: {#COMPN_SCILAB}
Source: modules\{#CORE}\includes\machine.h; DestDir: {app}\modules\{#CORE}\includes; Components: {#COMPN_SCILAB}
Source: modules\{#CORE}\includes\PATH_MAX.h; DestDir: {app}\modules\{#CORE}\includes; Components: {#COMPN_SCILAB}
Source: modules\{#CORE}\includes\scilabDefaults.h; DestDir: {app}\modules\{#CORE}\includes; Components: {#COMPN_SCILAB}
Source: modules\{#CORE}\includes\sci_malloc.h; DestDir: {app}\modules\{#CORE}\includes; Components: {#COMPN_SCILAB}
Source: modules\{#CORE}\includes\scisparse.h; DestDir: {app}\modules\{#CORE}\includes; Components: {#COMPN_SCILAB}
Source: modules\{#CORE}\includes\sci_types.h; DestDir: {app}\modules\{#CORE}\includes; Components: {#COMPN_SCILAB}
Source: modules\{#CORE}\includes\storeCommand.h; DestDir: {app}\modules\{#CORE}\includes; Components: {#COMPN_SCILAB}
Source: modules\{#CORE}\includes\timer.hxx; DestDir: {app}\modules\{#CORE}\includes; Components: {#COMPN_SCILAB}
Source: modules\{#CORE}\includes\version.h; DestDir: {app}\modules\{#CORE}\includes; Components: {#COMPN_SCILAB}
Source: modules\{#CORE}\includes\win_mem_alloc.h; DestDir: {app}\modules\{#CORE}\includes; Components: {#COMPN_SCILAB}

;
Source: modules\{#CORE}\macros\buildmacros.sce; DestDir: {app}\modules\{#CORE}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#CORE}\macros\buildmacros.bat; DestDir: {app}\modules\{#CORE}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#CORE}\macros\cleanmacros.bat; DestDir: {app}\modules\{#CORE}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#CORE}\macros\lib; DestDir: {app}\modules\{#CORE}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#CORE}\macros\*.sci; DestDir: {app}\modules\{#CORE}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#CORE}\macros\*.bin; DestDir: {app}\modules\{#CORE}\macros; Components: {#COMPN_SCILAB}
;
Source: modules\{#CORE}\xml\*.dtd; DestDir: {app}\modules\{#CORE}\xml; Components: {#COMPN_SCILAB}
;
Source: modules\{#CORE}\demos\*.*; DestDir: {app}\modules\{#CORE}\demos; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
;
Source: modules\{#CORE}\tests\*.*; DestDir: {app}\modules\{#CORE}\tests; Flags: recursesubdirs; Components: {#COMPN_SCILAB} and {#COMPN_TESTS}
;
;--------------------------------------------------------------------------------------------------------------
