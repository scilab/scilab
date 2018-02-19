// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Sylvestre LEDRU <sylvestre.ledru@inria.fr>
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
//<-- CLI SHELL MODE -->
//<-- NO CHECK REF -->
//
//===============================
// unit tests gettext()
//===============================

// Void call
if execstr('gettext()','errcatch')==0 then pause,end

// one input argument
str1 = "plop";
assert_checktrue(execstr('gettext(str1)','errcatch')==0);

// three inputs arguments
str1 = "plop"; str2 = "plip"; str3 = "plup";
assert_checkfalse(execstr('gettext(str1, str2, str3)','errcatch')==0);

// Try to translate from en_US to en_US to a string which doesn't exist
lang = "en_US"; setlanguage(lang);
msg = "Localization does not exist";
assert_checkequal(gettext(msg), msg);

// Try to translate from en_US to fr_FR a string which doesn't exist
lang = "fr_FR"; setlanguage(lang);
msg = "Localization does not exist";
assert_checkequal(gettext(msg), msg);

msg = "Startup execution:";

lang = "fr_FR"; setlanguage(lang);
assert_checkequal(gettext(msg), "Initialisation :");
lang = "fr"; setlanguage(lang);
assert_checkequal(gettext(msg), "Initialisation :");
lang = "en"; setlanguage(lang);
assert_checkequal(gettext(msg), msg);
lang = "en_US"; setlanguage(lang);
assert_checkequal(gettext(msg), msg);

addlocalizationdomain("tbx", "SCI/modules/localization/tests/unit_tests/locale");
setlanguage("fr_FR");
assert_checkequal(gettext("%s: No more memory.\n"), "%s : Plus de mémoire disponible.\n");
assert_checkequal(gettext("tbx", "%s: No more memory.\n"), "%s : Overwrite Scilab translation.\n");
assert_checkequal(gettext("%s: something not in Scilab.\n"), "%s: something not in Scilab.\n");
assert_checkequal(gettext("tbx", "%s: something not in Scilab.\n"), "%s : it is true, that is not in Scilab.\n");

// ======================================================
// With a domain (formerly dgettext() < 5.5)
// ======================================================

// Try to translate from en_US to en_US to domain/string which doesn't exist
lang="en_US"; setlanguage(lang); domain="fake_domain"; msg="Localization does not exist";
if gettext(domain,msg) <> msg then pause, end

// Try to translate from en_US to fr_FR to a domain/string which doesn't exist
lang="fr_FR"; setlanguage(lang); domain="fake_domain"; msg="Localization does not exist";
if gettext(domain,msg) <> msg then pause, end


// Check if it is working with a good domain & good msgid
lang="fr_FR"; setlanguage(lang); domain="scilab"; msg="Startup execution:";
if gettext(domain,msg) <> "Initialisation :" then pause, end

// Check if it is working with a bad domain & good msgid
lang="fr_FR"; setlanguage(lang); domain="fake_domain"; msg="Startup execution:";
if gettext(domain,msg) == "Initialisation :" then pause, end

// Check if it is working with a good domain & good msgid and alias
lang="fr"; setlanguage(lang); domain="scilab"; msg="Startup execution:";
if gettext(domain,msg) <> "Initialisation :" then pause, end

// Check if it is working with a bad domain & good msgid and alias
lang="fr"; setlanguage(lang); domain="fake_domain"; msg="Startup execution:";
if gettext(domain,msg) == "Initialisation :" then pause, end


// Check if it is working with a good domain & good msgid
lang="en_US"; setlanguage(lang); domain="scilab"; msg="Startup execution:";
if gettext(domain,msg) <> msg then pause, end

// Check if it is working with a bad domain & good msgid
lang="en_US"; setlanguage(lang); domain="fake_domain"; msg="Startup execution:";
if gettext(domain,msg) <> msg then pause, end

// Check if it is working with a good domain & good msgid and alias
lang="en"; setlanguage(lang); domain="scilab"; msg="Startup execution:";
if gettext(domain,msg) <> msg then pause, end

// Check if it is working with a bad domain & good msgid and alias
lang="en"; setlanguage(lang); domain="fake_domain"; msg="Startup execution:";
if gettext(domain,msg) <> msg then pause, end

// @TODO :
// Add test to use other domain which are working (ie not fake_domain)
