// =============================================================================
// Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Pierre MARECHAL <pierre.marechal@scilab.org>
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
// <-- NO CHECK REF -->

// xmltojar is obsolete: it builds no jar anymore and forwards each help
// directory to tbx_build_help. Fixture: SCI/modules/helptools/examples.

tmpPath = TMPDIR + "/xmltojar";
copyfile("SCI/modules/helptools/examples", tmpPath);

for ex = ["example_1" "example_2" "example_3"]
    my_module_path = pathconvert(tmpPath + "/" + ex, %f, %f);
    for lang = ["fr_FR" "en_US"]
        r = xmltojar(my_module_path + "/help/" + lang, ex + " [" + lang + "]", lang);
        assert_checkequal(r, []);
    end
    assert_checkfalse(isdir(my_module_path + "/jar"));
end

// No argument: Scilab's own help, nothing to build anymore
assert_checkequal(xmltojar(), []);

rmdir(tmpPath, "s");
