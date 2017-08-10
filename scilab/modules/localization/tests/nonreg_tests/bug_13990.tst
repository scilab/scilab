// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2015 - Scilab Enterprises - Antoine ELIAS
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
// <-- WINDOWS ONLY -->
//
// <-- CLI SHELL MODE -->
//
// <-- Non-regression test for bug 13990 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=13990
//
// <-- Short Description -->
// Check that localization files are installed 

setlanguage zh_TW;
val = gettext("Monday");       
assert_checkequal(val, "星期一");

setlanguage zh_CN;
val = gettext("Monday");
assert_checkequal(val, "星期一");

setlanguage uk_UA;
val = gettext("Monday");
assert_checkequal(val, "Понеділок");

setlanguage ru_RU;
val = gettext("Monday");
assert_checkequal(val, "Понедельник");

setlanguage pt_BR;
val = gettext("Monday");
assert_checkequal(val, "Segunda-feira");

setlanguage pl_PL;
val = gettext("Monday");
assert_checkequal(val, "poniedziałek");

setlanguage ja_JP;
val = gettext("Monday");
assert_checkequal(val, "月曜日");

setlanguage it_IT;
val = gettext("Monday");
assert_checkequal(val, "Lunedì");

setlanguage fr_FR;
val = gettext("Monday");
assert_checkequal(val, "Lundi");

setlanguage es_ES;
val = gettext("Monday");
assert_checkequal(val, "Lunes");

setlanguage en_US;
val = gettext("Monday");
assert_checkequal(val, "Monday");

setlanguage de_DE;
val = gettext("Monday");
assert_checkequal(val, "Montag");

setlanguage ca_ES;
val = gettext("Monday");
assert_checkequal(val, "Dilluns");
