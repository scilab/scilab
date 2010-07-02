// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2007 - INRIA - Allan CORNET
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function  generatePoFile(LANGUAGE)
PATH_GETTEXT_TOOLS = '/usr/bin/';
LC = 'LC_MESSAGES';
DEST_FILE_MO = SCI+filesep()+'locale'+filesep()+LANGUAGE+filesep()+LC+filesep()+'scilab.mo';
DEST_FILE_PO = SCI+filesep()+'locale'+filesep()+LANGUAGE+filesep()+LC+filesep()+'scilab.po';
LC = 'LC_MESSAGES';

if getos() == 'Windows' then
PATH_GETTEXT_TOOLS = SCI + filesep() + 'tools/gettext';
end

// make destination directories 

if ~isdir(SCI+filesep()+'locale') then
  mkdir(SCI,'locale');
end 

if ~isdir(SCI+filesep()+'locale'+filesep()+LANGUAGE) then
  mkdir(SCI+filesep()+'locale',LANGUAGE);
end 

if ~isdir(SCI+filesep()+'locale'+filesep()+LANGUAGE+filesep()+LC) then
  mkdir(SCI+filesep()+'locale'+filesep()+LANGUAGE,LC);
end 

List_files = [];
j = 1;
for MODULE = getmodules()'
  PATH_PO = SCI + filesep() + 'modules' + filesep() + MODULE + filesep() + 'locales' + filesep() + LANGUAGE;
  FULLFILENAMEPO = PATH_PO + filesep() + MODULE + ".po*";
  FINDFULLFILENAMEPO = ls(FULLFILENAMEPO);
  
  // we take only .pot or .po
  if ( size(FINDFULLFILENAMEPO,'*')>1 ) then
  
    FULLFILENAMEPO = PATH_PO + filesep() + MODULE + ".pot";
    FINDFULLFILENAMEPO = ls(FULLFILENAMEPO);
    
    if (FINDFULLFILENAMEPO == []) then
    
      FULLFILENAMEPO = PATH_PO + filesep() + MODULE + ".po";
      FINDFULLFILENAMEPO = ls(FULLFILENAMEPO);
      
    end
    
  end
  
  if FINDFULLFILENAMEPO <> [] then
    if isfile(FINDFULLFILENAMEPO) then
      ext = fileext(FINDFULLFILENAMEPO);
      if ( ( ext == '.pot' ) | (ext == '.po') ) then
        // '""' path windows with blank
        List_files(j) = '""'+FINDFULLFILENAMEPO+'""';
        j = j + 1;
      end
    end
  end
end

if (List_files <> []) then
  unix(PATH_GETTEXT_TOOLS+filesep()+'msgcat -o '+DEST_FILE_PO+' '+strcat(List_files,' '));
  unix(PATH_GETTEXT_TOOLS+filesep()+'msgfmt --statistics -o '+DEST_FILE_MO+' '+DEST_FILE_PO);
end

endfunction
// ======================================
function languages = findLanguagesToBuild()

languages = [];

langs = ls('SCI/modules/core/locales/');
// check that we have a .pot or .po

for i = 1:size(langs,'*')
  f1 = findfiles('SCI/modules/core/locales/'+langs(i),'*.pot');
  f2 = findfiles('SCI/modules/core/locales/'+langs(i),'*.po');

  if (f1 <> []) | (f2 <> []) then 
    languages = [languages,langs(i)];
  end
end  
endfunction
// ======================================
// Main
LANGUAGES = findLanguagesToBuild();
mprintf("-- Build localization (.mo from .po) --\n");

for L = LANGUAGES
  mprintf("        -- Building for ""%s"" --\n",L);
  generatePoFile(L);
end
// ======================================
