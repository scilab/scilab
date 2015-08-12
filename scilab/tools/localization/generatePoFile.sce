// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2007 - INRIA - Allan CORNET
// Copyright (C) 2011-2012 - DIGITEO - Allan CORNET
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function  generatePoFile(LANGUAGE)
    if getos() <> "Windows" then
        error(999, "Used only under Windows");
    end

    if ~isdef("%c_a_c") then
        exec("SCI/modules/overloading/macros/%c_a_c.sci");
    end

    LC = "LC_MESSAGES";
    DEST_PATH_MO = SCI + filesep() + "locale" + filesep() + LANGUAGE + filesep() + LC + filesep();
    DEST_FILE_MO = "scilab.mo";
    DEST_FILE_PO = "scilab.po";
    LC = "LC_MESSAGES";
    PATH_GETTEXT_TOOLS = SCI + filesep() + "tools/gettext";
    PATH_GETTEXT_TOOLS = getrelativefilename(DEST_PATH_MO, PATH_GETTEXT_TOOLS);

    // make destination directories
    if ~isdir(SCI + filesep() + "locale") then
        createdir(SCI + filesep() + "locale");
    end

    if ~isdir(SCI + filesep() + "locale" + filesep() + LANGUAGE) then
        createdir(SCI + filesep() + "locale" + filesep() + LANGUAGE);
    end

    if ~isdir(SCI + filesep() + "locale" + filesep() + LANGUAGE + filesep() + LC) then
        createdir(SCI + filesep() + "locale" + filesep() + LANGUAGE + filesep() + LC);
    end

    Modules = getmodules();
    PATH_PO = SCI + filesep() + "modules" + filesep() + Modules + filesep() + "locales" + filesep();
    if LANGUAGE ==  "en_US" then
        FILENAME_PO = Modules + ".pot";
    else
        FILENAME_PO = LANGUAGE + ".po";
    end

    List_files = [];
    for k = 1:size(PATH_PO, "*")
        if LANGUAGE ==  "en_US" then
            if findfiles(PATH_PO(k), FILENAME_PO(k)) <> [] then
                List_files = [List_files ; getrelativefilename(DEST_PATH_MO, fullpath(PATH_PO(k) +  FILENAME_PO(k)))];
            end
        else
            if findfiles(PATH_PO(k), FILENAME_PO) <> [] then
                List_files = [List_files ; getrelativefilename(DEST_PATH_MO, fullpath(PATH_PO(k) +  FILENAME_PO))];
            end
        end
    end

    if (List_files <> []) then
        if newest([DEST_PATH_MO + DEST_FILE_MO; DEST_PATH_MO + List_files]) <> 1 then
            curPath = pwd();
            cd(DEST_PATH_MO)
            cmdline_msgcat = PATH_GETTEXT_TOOLS + filesep() + "msgcat --use-first -o " + DEST_FILE_PO + " " + strcat("""" + List_files + """", " ");
            unix(cmdline_msgcat);
            cmline_msgfmt = PATH_GETTEXT_TOOLS + filesep() + "msgfmt --statistics -o " + DEST_FILE_MO + " " + DEST_FILE_PO;
            unix(cmline_msgfmt);
            cd(curPath)
        end
    end

endfunction
// ======================================
function languages = findLanguagesToBuild()
    languages = [];

    f1 = findfiles("SCI/modules/core/locales/", "*.pot");
    f2 = findfiles("SCI/modules/core/locales/", "*.po");

    if f1 <> [] then
        languages = ["en_US"];
    end

    if f2 <> [] then
        f2 = strsubst(f2, ".po", "");
        languages = [languages, f2'];
    end
endfunction
// ======================================
// Main
LANGUAGES = findLanguagesToBuild();
mprintf("-- Build localization (.mo from .po) --\n");

for L = LANGUAGES
    mprintf("    -- Building for ""%s"" --\n", L);
    generatePoFile(L);
end
// ======================================
