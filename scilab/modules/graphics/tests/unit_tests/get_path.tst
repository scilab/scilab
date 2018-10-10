// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2014 - Scilab Enterprises - Bruno JOFRET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

f = figure("tag", "mainFigure", "visible", "off");
    f1 = uicontrol(f, "style", "frame", "tag", "f1");
        f11 = uicontrol(f1, "style", "frame", "tag", "f11");
        f12 = uicontrol(f1, "style", "frame", "tag", "f12");
        f13 = uicontrol(f1, "style", "frame", "tag", "f13");
        f14 = uicontrol(f1, "style", "frame", "tag", "leaf");
            f141 = uicontrol(f14, "style", "frame", "tag", "leaf1");
            f142 = uicontrol(f14, "style", "frame", "tag", "leaf2");
            f143 = uicontrol(f14, "style", "frame", "tag", "f143");
    f2 = uicontrol(f, "style", "frame", "tag", "f2");
        f21 = uicontrol(f2, "style", "frame", "tag", "f21");
        f22 = uicontrol(f2, "style", "frame", "tag", "f22");
        f23 = uicontrol(f2, "style", "frame", "tag", "f23");
        f24 = uicontrol(f2, "style", "frame", "tag", "leaf");
            f241 = uicontrol(f24, "style", "frame", "tag", "leaf1");
            f242 = uicontrol(f24, "style", "frame", "tag", "leaf2");
            f243 = uicontrol(f24, "style", "frame", "tag", "f243");

res = get("mainFigure");
assert_checkequal(f.UID, res.UID);

res = get("mainFigure/f1");
assert_checkequal(f1.UID, res.UID);

res = get("mainFigure/f2");
assert_checkequal(f2.UID, res.UID);

res = get("mainFigure/f1/f12");
assert_checkequal(f12.UID, res.UID);

res = get("mainFigure/f2/f22");
assert_checkequal(f22.UID, res.UID);

res = get("f1");
assert_checkequal(f1.UID, res.UID);
res = get("f13");
assert_checkequal(f13.UID, res.UID);

res = get("f5");
assert_checkequal([], res);

res = get("*/f2");
assert_checkequal(f2.UID, res.UID);

res = get("*/*/leaf");
assert_checktrue(or(res.UID == [f14.UID, f24.UID]));

res = get("*/leaf");
assert_checktrue(or(res.UID == [f14.UID, f24.UID]));

res = get("*/*/toto");
assert_checkequal([], res);

res = get("*/toto");
assert_checkequal([], res);

res = get("mainFigure/*/f23");
assert_checkequal(f23.UID, res.UID);

res = get("mainFigure/*/toto");
assert_checkequal([], res);

res = get("mainFigure/*/leaf/f143");
assert_checkequal(f143.UID, res.UID);

res = get("mainFigure/*/leaf/leaf1");
assert_checktrue(or(res.UID == [f141.UID, f241.UID]));

res = get("mainFigure/*/leaf/leaf2");
assert_checktrue(or(res.UID == [f142.UID, f242.UID]));
