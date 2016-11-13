// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Pierre MARECHAL <pierre.marechal@scilab.org>
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

// Internal function
// Operating system detection + archi

function [OSNAME,ARCH,LINUX,MACOSX,SOLARIS,BSD] = atomsGetPlatform()

    // Operating system detection

    if getos() <> "Windows" then
        OSNAME   = unix_g("uname");
        MACOSX   = (strcmp(OSNAME,"darwin", "i") == 0);
        LINUX    = (strcmp(OSNAME,"linux", "i")  == 0);
        SOLARIS  = (strcmp(OSNAME,"sunos", "i")  == 0);
        BSD      = (regexp(OSNAME ,"/BSD$/") <> []);
    else
        MACOSX  = %F;
        LINUX   = %F;
        SOLARIS = %F;
        BSD     = %F;
    end

    if getos() == "Windows" then
        OSNAME = "windows";
    elseif LINUX then
        OSNAME = "linux";
    elseif MACOSX then
        OSNAME = "macosx";
    elseif SOLARIS then
        OSNAME = "solaris";
    elseif BSD then
        OSNAME = "bsd";
    end

    // Architecture

    [dynamic_info,static_info] = getdebuginfo();
    arch_info  = static_info(grep(static_info,"/^Compiler Architecture:/","r"))

    if ~isempty(arch_info) & (regexp(arch_info,"/\sX64$/","o") <> []) then
        ARCH = "64";
    else
        ARCH = "32";
    end

endfunction
