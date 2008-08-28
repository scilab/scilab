// ============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Sylvestre LEDRU <sylvestre.ledru@inria.fr>
//
//  This file is distributed under the same license as the Scilab package.
// ============================================================================

//===============================
// unit tests setlanguage
//===============================

// Void call
if execstr('setlanguage()','errcatch')==0 then pause,end

// Double call
lang="fr_FR"; lang2="en_US"; if execstr('setlanguage(lang, lang2)','errcatch')==0 then pause,end

// Empty call
lang=getdefaultlanguage(); if setlanguage(lang) <> %T then pause,end

// Switch to english
lang="en_US"; if setlanguage(lang) <> %T then pause, end
getlanguage()

// Switch to english with small code
lang="en"; if setlanguage(lang) <> %T then pause, end
getlanguage()

// Switch to french with small code
lang="fr"; if setlanguage(lang) <> %T then pause, end
getlanguage()

// Switch to russian with full code
lang="ru_RU"; if setlanguage(lang) <> %T then pause, end
getlanguage()

// Switch to a wrong code
lang="f"; if setlanguage(lang) <> %F then pause, end
getlanguage()

// Switch to a wrong code
lang="ff"; if setlanguage(lang) <> %F then pause, end

// Switch to a wrong code
lang="fff"; if setlanguage(lang) <> %F then pause, end

// Switch to a wrong code
lang="ffff"; if setlanguage(lang) <> %F then pause, end
