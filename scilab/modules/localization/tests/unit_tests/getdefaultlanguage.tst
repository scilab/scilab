//<-- CLI SHELL MODE -->
// ============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Sylvestre LEDRU <sylvestre.ledru@inria.fr>
//
//  This file is distributed under the same license as the Scilab package.
// ============================================================================

//===============================
// unit tests getdefaultlanguage
//===============================
warning("off")
// check that the default language is really english
 if getdefaultlanguage() <> "en_US" then pause, end

// Switch to french with small code and check if the default language has not changed
lang="fr"; setlanguage(lang); if getdefaultlanguage() <> "en_US" then pause, end

// Switch to english with small code
lang="en"; setlanguage(lang); if getdefaultlanguage() <> "en_US" then pause, end

// Switch to english with full code
lang="en_US"; setlanguage(lang); if getdefaultlanguage() <> "en_US" then pause, end

