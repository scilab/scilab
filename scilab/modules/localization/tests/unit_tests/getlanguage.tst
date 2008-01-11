//===============================
// unit tests getlanguage
// INRIA 2008
// @author Sylvestre LEDRU
//===============================

// Switch to french with small code
lang="fr"; setlanguage(lang); if getlanguage() <> "fr_FR" then pause, end

// Switch to english with small code
lang="en"; setlanguage(lang); if getlanguage() <> "en_US" then pause, end

// Switch to english with full code
lang="en_US"; setlanguage(lang); if getlanguage() <> "en_US" then pause, end

// Switch to french with full code
lang="fr_FR"; setlanguage(lang); if getlanguage() <> "fr_FR" then pause, end

