//================================
//
// Copyright INRIA 2005
// Scilab team
// Date : December 8th 2005
//
//================================

function %helps=initial_help_chapters(language)

    global LANGUAGE;

    select LANGUAGE

    case "eng" then
        %helps = ["/home/pmarecha/scilab_trunk/modules/time/help/eng"     "Time [eng]";
        "/home/pmarecha/scilab_trunk/modules/metanet/help/eng"    "Metanet [eng]";
        "/home/pmarecha/scilab_trunk/modules/graphics/help/eng"   "Graphics [eng]";
        "/home/pmarecha/scilab_trunk/modules/Javasci/help/eng"    "Javasci [eng]";
        "/home/pmarecha/scilab_trunk/modules/maple/help/eng"      "Maple [eng]";
        "/home/pmarecha/scilab_trunk/modules/tclsci/help/eng"     "Tclsci [eng]";
        "/home/pmarecha/scilab_trunk/modules/helptools/help/eng" "Help-tools [eng]";
        "/home/pmarecha/scilab_trunk/modules/sound/help/eng"      "Sound [eng]"];


    case "fr" then
        %helps = ["/home/pmarecha/scilab_trunk/modules/time/help/fr"     "Time [fr]";
        "/home/pmarecha/scilab_trunk/modules/metanet/help/fr"          "Metanet [fr]";
        "/home/pmarecha/scilab_trunk/modules/graphics/help/fr"         "Graphics [fr]";
        "/home/pmarecha/scilab_trunk/modules/Javasci/help/fr"          "Javasci [fr]";
        "/home/pmarecha/scilab_trunk/modules/maple/help/fr"            "Maple [fr]";
        "/home/pmarecha/scilab_trunk/modules/tclsci/help/fr"           "Tclsci [fr]";
        "/home/pmarecha/scilab_trunk/modules/helptools/help/fr"       "Help-tools [fr]";
        "/home/pmarecha/scilab_trunk/modules/sound/help/fr"            "Sound [fr]"];

    end

endfunction
