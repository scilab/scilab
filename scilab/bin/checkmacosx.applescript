(*************************************************************************
 *
 * This code comes from the Openoffice project and has been adapted for
 * Scilab.
 * For now, this script checks the minimal version of Mac OS X
 *
 * Copyright (C) 2011 - DIGITEO - Sylvestre LEDRU
 *
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 *
 * Copyright 2008 by Sun Microsystems, Inc.
 *
 * OpenOffice.org - a multi-platform office productivity suite
 *
 * $RCSfile: main.applescript,v $
 * $Revision: 1.5 $
 *
 * This file is part of OpenOffice.org.
 *
 * OpenOffice.org is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License version 3
 * only, as published by the Free Software Foundation.
 *
 * OpenOffice.org is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License version 3 for more details
 * (a copy is included in the LICENSE file that accompanied this code).
 *
 * You should have received a copy of the GNU Lesser General Public License
 * version 3 along with OpenOffice.org.  If not, see
 * <http://www.openoffice.org/license.html>
 * for a copy of the LGPLv3 License.
 *
 *************************************************************************)

on run()
    -- Check for the required version of Mac OS X
    if (not atLeastOSXVersion(10, 6, 4)) then
                tell application  "System Events" -- Mandatory to use tell application otherwis, osascript complains
                activate
        display dialog  "This version of Scilab will probably fail on this system: requires MacOSX 10.6.5 (Snow Leopard) or newer system" buttons {"Try anyway", "Quit"} default button "Try anyway"
        if the button returned of the result is "Quit" then
            error number 128 
            return false
        else
            return true
        end if
                end tell
    end if
end run

-- Test for a minimum version of Mac OS X
on atLeastOSXVersion(verMajor, verMinor, verStep)
    -- The StandardAdditions's 'system attribute' used to be the Finder's 'computer' command.
    tell application "Finder" to set sysv to (system attribute "sysv")
    
    -- Generate sysv-compatible number from given version
    set reqVer to ((verMajor div 10) * 4096 + (verMajor mod 10) * 256 + verMinor * 16 + verStep)
    
    -- DEBUGGING:
    -- display dialog ("RV:" & reqVer & " < " & sysv as string)
    
    -- set major to ((sysv div 4096) * 10 + (sysv mod 4096 div 256))
    -- set minor to (sysv mod 256 div 16)
    -- set step to (sysv mod 16)
    --display dialog ("Your Mac OS X version: " & major & "." & minor & "." & step)
    
    if (reqVer > sysv) then
        return false
    else
        return true
    end if
end atLeastOSXVersion


