// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) DIGITEO - 2011  - Allan CORNET
// Copyright (C) Scilab Enterprises - 2015  - Antoine ELIAS
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.
//=============================================================================
function dlwCompile(files, make_command, makename)

    dlwConfigureEnv();

    nf = size(files,"*");

    for i=1:nf
        if ( ilib_verbose() <> 0 ) then
            mprintf(_("   Compilation of ") + string(files(i)) +"\n");
        end
    end

    // then the shared library
    if ( ilib_verbose() <> 0 ) then
        mprintf(_("   Building shared library (be patient)\n"));
    end

    if dlwForceRebuild() then
        target_build = "clean all";
    else
        target_build = "all";
    end

    cmd = make_command + makename + " " + target_build + " 2>&0"
    scibuildfile = writeBatchFile(cmd);
    [msg, stat] = unix_g(scibuildfile);
    deletefile(scibuildfile);

    if stat <> 0 then
        // more feedback when compilation fails
        cmd = make_command + makename + " " + target_build + " 1>&2"
        scibuildfile = writeBatchFile(cmd);
        [msg, stat, stderr] = unix_g(scibuildfile);
        deletefile(scibuildfile);
        disp(stderr);
        error(msprintf(gettext("%s: Error while executing %s.\n"), "ilib_compile", makename));
    else
        if ilib_verbose() > 1 then
            disp(msg);
        end
    end


endfunction
//=============================================================================
function filename = writeBatchFile(cmd)

    //update DEBUG_SCILAB_DYNAMIC_LINK to match with Scilab compilation mode
    val = getenv("DEBUG_SCILAB_DYNAMIC_LINK","");
    debugVal = "NO";
    if val <> "YES" & val <> "NO" & isDebug() then
        debugVal = "YES";
    end

    if win64() then
        if dlwIsExpress() then
            arch = "x86_amd64";
        else
            arch = "x64";
        end
    else
        arch = "x86";
    end

    path = dlwGetVisualStudioPath();

    scibuild = [ ...
    "@call """ + path + "\VC\vcvarsall.bat"" " + arch;
    "set DEBUG_SCILAB_DYNAMIC_LINK=" + debugVal;
    cmd
    ];

    filename = TMPDIR + "/scibuild.bat";
    mputl(scibuild, filename);
    //filename = "call " + filename;
endfunction
