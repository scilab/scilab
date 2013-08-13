// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2012 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

function CreateDir_test()

    TST_LANG = ["Simplified Chinese"  , "世界您好"; ..
    "Cyrillic"            , "азеазея"; ..
    "amharique"           , "ድቅስድቅስግ"; ..
    "Japanese"            , "ハロー・ワールド"; ..
    "Thai"                , "เฮลโลเวิลด์"; ..
    "Arabish"             , "حريات وحقوق"; ..
    "Korean"              , "프로그램"; ..
    "Vietanmien"          , "프로그램"; ..
    "Malien"              , "ഹലോ വേള്‍ഡ"; ..
    "Hebreu"              , "תוכנית"];

    for i = 1:size(TST_LANG,"r")
        DIR_TST = TMPDIR + "/dir_" + TST_LANG(i, 2);
        mkdir(DIR_TST);
        if ~isdir(DIR_TST) then
            error(999, "directory did not create");
        end
        FILENAME_TST = DIR_TST + "/" + "file_" + TST_LANG(i, 2);
        STR_TST = "str_" + TST_LANG(i, 2) + " : " + TST_LANG(i, 1);
        fd = mopen(FILENAME_TST, "wt");
        mfprintf(fd, "%s", STR_TST);
        mclose(fd);
        if ~isfile(FILENAME_TST) then
            error(999, "file  did not create");
        end
    end
endfunction

CreateDir_test();
clear CreateDir_test;
