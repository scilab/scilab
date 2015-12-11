(*************************************************************************
 *
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) DIGITEO - 2011 - Sylvestre Ledru
 * Copyright (C) DIGITEO - 2011 - Bruno JOFRET
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

 * For now, this script displays a message if the version of mac os x
 * is wrong.
 *
 *************************************************************************)

on run argv
    tell application  "System Events" -- Mandatory to use tell application otherwis, osascript complains
        activate
        display dialog  "This version of Scilab will probably fail on this system (" & item 1 of argv & "): Scilab requires " & item 2 of argv & "." buttons {"Try anyway", "Quit"} default button "Try anyway"
        if the button returned of the result is "Quit" then
            error number 128
            return false
        else
            return true
        end if
        end tell
end run

