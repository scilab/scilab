//<-- CLI SHELL MODE -->
// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Sylvestre LEDRU <sylvestre.ledru@inria.fr>
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

//===============================
// unit tests gettext
//===============================

// Void call
if execstr('gettext()','errcatch')==0 then pause,end

// three inputs arguments
str1="plop"; str2="plip"; str3="plup"; if execstr('gettext(str1, str2, str3)','errcatch')==0 then pause,end

// Try to translate from en_US to en_US to a string which doesn't exist
lang="en_US"; setlanguage(lang); msg="Localization does not exist"; if gettext(msg) <> msg then pause, end

// Try to translate from en_US to fr_FR to a string which doesn't exist
lang="fr_FR"; setlanguage(lang); msg="Localization does not exist"; if gettext(msg) <> msg then pause, end

lang="fr_FR"; setlanguage(lang); msg="Startup execution:"; if gettext(msg) <> "Initialisation :" then pause, end

lang="fr"; setlanguage(lang); msg="Startup execution:"; if gettext(msg) <> "Initialisation :" then pause, end

lang="en"; setlanguage(lang); msg="Startup execution:"; if gettext(msg) <> msg then pause, end

lang="en_US"; setlanguage(lang); msg="Startup execution:"; if gettext(msg) <> msg then pause, end

addlocalizationdomain("tbx", "SCI/modules/localization/tests/unit_tests/locale");
setlanguage("fr_FR");
assert_checkequal(gettext("%s: No more memory.\n"), "%s : Plus de mémoire disponible.\n");
assert_checkequal(gettext("tbx", "%s: No more memory.\n"), "%s : Overwrite Scilab translation.\n");
assert_checkequal(gettext("%s: something not in Scilab.\n"), "%s: something not in Scilab.\n");
assert_checkequal(gettext("tbx", "%s: something not in Scilab.\n"), "%s : it is true, that is not in Scilab.\n");

