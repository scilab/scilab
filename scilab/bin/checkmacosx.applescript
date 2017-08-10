(*************************************************************************
 *
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) DIGITEO - 2011 - Sylvestre Ledru
 * Copyright (C) DIGITEO - 2011 - Bruno JOFRET
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
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

