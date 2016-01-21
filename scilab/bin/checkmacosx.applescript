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
 *
 *************************************************************************)

on run argv
    tell application  "System Events" -- Mandatory to use tell application otherwise, osascript complains
        activate
        display dialog item 1 of argv buttons {"Try anyway", "Quit"} default button "Quit"
        if the button returned of the result is "Quit" then
            error number 128
            return false
        else
            return true
        end if
        end tell
end run

