//<-- CLI SHELL MODE -->
// ============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// ============================================================================

// setdefaultlanguage only on Windows
// returns false in others case

if getos() == "Windows" then
    ierr = execstr("setdefaultlanguage()","errcatch");
    if ierr <> 999 then pause,end

    ierr = execstr("setdefaultlanguage(1)","errcatch");
    if ierr <> 999 then pause,end

    warning("off");

    if setdefaultlanguage("toto") <> %f then pause,end

    if setdefaultlanguage("en") <> %t then pause,end
    if getlanguage() <> "en_US" then pause,end

    if setdefaultlanguage("fr") <> %t then pause,end
    if getlanguage() <> "fr_FR" then pause,end

    if setdefaultlanguage("en_US") <> %t then pause,end
    if getlanguage() <> "en_US" then pause,end

    if setdefaultlanguage("fr_FR") <> %t then pause,end
    if getlanguage() <> "fr_FR" then pause,end

    if setdefaultlanguage("ru_RU") <> %t then pause,end
    if getlanguage() <> "ru_RU" then pause,end

    if setdefaultlanguage("zh_CN") <> %t then pause,end
    if getlanguage() <> "zh_CN" then pause,end

    if setdefaultlanguage("zh_TW") <> %t then pause,end
    if getlanguage() <> "zh_TW" then pause,end

    if setdefaultlanguage("C") <> %t then pause,end
    if getlanguage() <> getdefaultlanguage() then pause,end

    if setdefaultlanguage("") <> %t then pause,end

    warning("on");

end
