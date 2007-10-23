function  generatePoFile(LANGUAGE)
// ======================================
// INRIA 2007
// Allan CORNET
// based on generatePoFile.sh
// ======================================
PATH_GETTEXT_TOOLS = '';
LC = 'LC_MESSAGES';
DEST_FILE_MO = SCI+filesep()+'locale'+filesep()+LANGUAGE+filesep()+LC+filesep()+'scilab.mo';
DEST_FILE_PO = SCI+filesep()+'locale'+filesep()+LANGUAGE+filesep()+LC+filesep()+'scilab.po';
FILENAME_PO = 'messages.po*';
LC = 'LC_MESSAGES';

if MSDOS then
PATH_GETTEXT_TOOLS = SCI + filesep() + 'tools/gettext';
end

// make destination directories 

if (fileinfo(SCI+filesep()+'locale') == []) then
  mkdir(SCI,'locale');
end 

if (fileinfo(SCI+filesep()+'locale'+filesep()+LANGUAGE) == []) then
  mkdir(SCI+filesep()+'locale',LANGUAGE);
end 

if (fileinfo(SCI+filesep()+'locale'+filesep()+LANGUAGE+filesep()+LC) == []) then
  mkdir(SCI+filesep()+'locale'+filesep()+LANGUAGE,LC);
end 

List_files = [];
j = 1;
for MODULE = getmodules()'
  PATH_PO = SCI + filesep() + 'modules' + filesep() + MODULE + filesep() + 'locales' + filesep() + LANGUAGE;
  FULLFILENAMEPO = PATH_PO + filesep() + FILENAME_PO;
  FINDFULLFILENAMEPO = ls(FULLFILENAMEPO);
  if FINDFULLFILENAMEPO <> [] then
    if (fileinfo(FINDFULLFILENAMEPO)<>[]) then
      // '""' path windows with blank
      List_files(j) = '""'+FINDFULLFILENAMEPO+'""';
      j = j + 1;
    end
  end
end

if (List_files <> []) then
  unix(PATH_GETTEXT_TOOLS+filesep()+'msgcat -o '+DEST_FILE_PO+' '+strcat(List_files,' '));
  unix(PATH_GETTEXT_TOOLS+filesep()+'msgfmt --statistics -o '+DEST_FILE_MO+' '+DEST_FILE_PO);
end

endfunction

// ======================================
// Main
LANGUAGES = ['fr_FR','en_US'];
for L = LANGUAGES
  generatePoFile(L);
end
// ======================================
