// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) DIGITEO - 2010-2010 - Clément DAVID <clement.david@scilab.org>
//
// This file is distributed under the same license as the Scilab package.

// <-- XCOS TEST -->
// <-- ENGLISH IMPOSED -->
//
// <-- Short Description -->
// White-box test for the xcosPalAddBlock macro.

//
// Init
//
iconPath = SCI + "/modules/xcos/images/palettes/NPN.png";
stylePath = SCI + "/modules/xcos/images/blocks/NPN.svg";
scs_m = SUM_f("define");
blockPath = TMPDIR + "/block.sod";

//
// Test block argument
//

// check call with a block name only
pal = xcosPal();
pal = xcosPalAddBlock(pal, "SUM_f");

if getos() == "Windows" then
    root_url = "file:///";
else
    root_url = "file://";
end

expectedResult = ["SUM_f" msprintf("%s/SUM_f.gif", TMPDIR) msprintf("noLabel=1;image="+ root_url +"%s/SUM_f.svg;", TMPDIR)];
expectedResult = strsubst(expectedResult, "\", "/");
result = [pal.blockNames(1) pal.icons(1) pal.style(1)];
result = strsubst(result, "\", "/");
if or(expectedResult <> result) then pause, end

// check call with a block instance only
pal = xcosPal();
pal = xcosPalAddBlock(pal, scs_m);

expectedResult = ["SUM_f" msprintf("%s/SUM_f.gif", TMPDIR) msprintf("noLabel=1;image="+ root_url +"%s/SUM_f.svg;", TMPDIR)];
expectedResult = strsubst(expectedResult, "\", "/");
result = [pal.blockNames(1) pal.icons(1) pal.style(1)];
result = strsubst(result, "\", "/");
if or(expectedResult <> result) then pause, end

// check call with a stored block instance
pal = xcosPal();
save(blockPath, "scs_m");
pal = xcosPalAddBlock(pal, blockPath);

expectedResult = ["SUM_f" msprintf("%s/SUM_f.gif", TMPDIR) msprintf("noLabel=1;image="+ root_url +"%s/SUM_f.svg;", TMPDIR)];
expectedResult = strsubst(expectedResult, "\", "/");
result = [pal.blockNames(1) pal.icons(1) pal.style(1)];
result = strsubst(result, "\", "/");
if or(expectedResult <> result) then pause, end

// check call with empty icon and style
pal = xcosPal();
pal = xcosPalAddBlock(pal, blockPath, [], []);
expectedResult = ["SUM_f" msprintf("%s/SUM_f.gif", TMPDIR) msprintf("noLabel=1;image="+ root_url +"%s/SUM_f.svg;", TMPDIR)];
expectedResult = strsubst(expectedResult, "\", "/");
result = [pal.blockNames(1) pal.icons(1) pal.style(1)];
result = strsubst(result, "\", "/");
if or(expectedResult <> result) then pause, end

//
// Test pal_block_img argument
//

// check call with a relative icon path
pal = xcosPal();
pal = xcosPalAddBlock(pal, blockPath, iconPath);
current = pwd();
cd(SCI);
pal = xcosPal();
pal = xcosPalAddBlock(pal, blockPath, "modules/xcos/images/palettes/NPN.png");

expectedResult = ["SUM_f" iconPath msprintf("noLabel=1;image="+ root_url +"%s/SUM_f.svg;", TMPDIR)];
expectedResult = strsubst(expectedResult, "\", "/");
result = [pal.blockNames(1) pal.icons(1) pal.style(1)];
result = strsubst(result, "\", "/");
if or(expectedResult <> result) then pause, end

cd(current);

//
// Test style argument
//

// check call with an empty icon and a struct style
myStyle = struct();
myStyle.block = [];
myStyle.image = root_url + iconPath;
pal = xcosPal();
pal = xcosPalAddBlock(pal, blockPath, [], myStyle);

expectedResult = ["SUM_f" msprintf("%s/SUM_f.gif", TMPDIR)  msprintf("block;image="+ root_url +"%s;", iconPath)];
expectedResult = strsubst(expectedResult, "\", "/");
result = [pal.blockNames(1) pal.icons(1) pal.style(1)];
result = strsubst(result, "\", "/");
if or(expectedResult <> result) then pause, end

// check call with an empty icon and a path style
pal = xcosPal();
pal = xcosPalAddBlock(pal, blockPath, [], iconPath);
expectedResult = ["SUM_f" msprintf("%s/SUM_f.gif", TMPDIR)  msprintf("shape=label;image="+ root_url +"%s;", iconPath)];
expectedResult = strsubst(expectedResult, "\", "/");
result = [pal.blockNames(1) pal.icons(1) pal.style(1)];
result = strsubst(result, "\", "/");
if or(expectedResult <> result) then pause, end

// check a full call with only paths (eg for toolbox creation)
pal = xcosPal();
pal = xcosPalAddBlock(pal, blockPath, iconPath, stylePath);

expectedResult = ["SUM_f" iconPath  msprintf("shape=label;image="+ root_url +"%s;", stylePath)];
expectedResult = strsubst(expectedResult, "\", "/");
result = [pal.blockNames(1) pal.icons(1) pal.style(1)];
result = strsubst(result, "\", "/");
if or(expectedResult <> result) then pause, end

