// =============================================================================
// Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2026 - Dassault Systèmes S.E. - Antoine ELIAS
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->

// tbx_generate_pofile extracts the translatable messages of the macros AND of
// the classdef files of classes/

tbx = fullfile(TMPDIR, "tbx_pofile");
if isdir(tbx) then
    rmdir(tbx, "s");
end
mkdir(tbx);
mkdir(fullfile(tbx, "macros"));
mkdir(fullfile(tbx, "classes"));
mkdir(fullfile(tbx, "etc"));
mputl("", fullfile(tbx, "etc", "tbx_pofile.start"));

mputl(["function r = tbx_pofile_f()"
       "    r = gettext(""tbx_pofile"", ""Message from a macro"");"
       "endfunction"], fullfile(tbx, "macros", "tbx_pofile_f.sci"));
mputl(["classdef TbxPofileClass"
       "    methods"
       "        function r = hello()"
       "            r = gettext(""tbx_pofile"", ""Message from a class"");"
       "        end"
       "    end"
       "end"], fullfile(tbx, "classes", "TbxPofileClass.sci"));

tbx_generate_pofile(tbx);

pot = mgetl(fullfile(tbx, "locales", "tbx_pofile.pot"));
assert_checktrue(or(pot == "msgid ""Message from a macro"""));
assert_checktrue(or(pot == "msgid ""Message from a class"""));

rmdir(tbx, "s");
