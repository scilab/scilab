// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2007 - INRIA - Allan CORNET
// Copyright (C) 2011 - DIGITEO - Allan CORNET
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function  generatePoFile(LANGUAGE)
  PATH_GETTEXT_TOOLS = "/usr/bin/";
  LC = "LC_MESSAGES";
  DEST_FILE_MO = SCI + filesep() + "locale" + filesep() + LANGUAGE + filesep() + LC + filesep() + "scilab.mo";
  DEST_FILE_PO = strsubst(DEST_FILE_MO, "scilab.mo", "scilab.po");
  LC = "LC_MESSAGES";

  if getos() == 'Windows' then
    PATH_GETTEXT_TOOLS = SCI + filesep() + "tools/gettext";
  end

  // make destination directories

  if ~isdir(SCI + filesep() + "locale") then
    mkdir(SCI, "locale");
  end

  if ~isdir(SCI + filesep() + "locale" + filesep() + LANGUAGE) then
    mkdir(SCI + filesep() + "locale", LANGUAGE);
  end

  if ~isdir(SCI + filesep() + "locale" + filesep() + LANGUAGE + filesep() + LC) then
    mkdir(SCI + filesep() + "locale" + filesep() + LANGUAGE, LC);
  end

  List_files = [];
  j = 1;

  for MODULE = getmodules()'
    PATH_PO = SCI + filesep() + "modules" + filesep() + MODULE + filesep() + "locales" + filesep();
    if LANGUAGE ==  "en_US" then
      FULLFILENAMEPO = PATH_PO + filesep() + MODULE + ".pot";
    else
      FULLFILENAMEPO = PATH_PO + filesep() + LANGUAGE + ".po";
    end
    FINDFULLFILENAMEPO = ls(FULLFILENAMEPO);

    if FINDFULLFILENAMEPO <> [] then
      if isfile(FINDFULLFILENAMEPO) then
        ext = fileext(FINDFULLFILENAMEPO);
        if ( ( ext == ".pot" ) | (ext == ".po") ) then
          // '""' path windows with blank
          List_files(j) = '""' + FINDFULLFILENAMEPO + '""';
          j = j + 1;
        end
      end
    end
  end

  for MODULE = getmodules()'
    PATH_PO = SCI + filesep() + "modules" + filesep() + MODULE + filesep() + "locales_macros" + filesep();
    if LANGUAGE ==  "en_US" then
      FULLFILENAMEPO = PATH_PO + filesep() + MODULE + ".pot";
    else
      FULLFILENAMEPO = PATH_PO + filesep() + LANGUAGE + ".po";
    end
    FINDFULLFILENAMEPO = ls(FULLFILENAMEPO);

    if FINDFULLFILENAMEPO <> [] then
      if isfile(FINDFULLFILENAMEPO) then
        ext = fileext(FINDFULLFILENAMEPO);
        if ( ( ext == ".pot" ) | (ext == ".po") ) then
          // '""' path windows with blank
          List_files(j) = '""' + FINDFULLFILENAMEPO + '""';
          j = j + 1;
        end
      end
    end
  end

  if (List_files <> []) then
    unix(PATH_GETTEXT_TOOLS + filesep() + "msgcat -o " + DEST_FILE_PO + " " + strcat(List_files, " "));
    unix(PATH_GETTEXT_TOOLS + filesep() + "msgfmt --statistics -o " + DEST_FILE_MO + " " + DEST_FILE_PO);
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
  mprintf("        -- Building for ""%s"" --\n", L);
  generatePoFile(L);
end
// ======================================
