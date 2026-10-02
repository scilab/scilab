// =============================================================================
// Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->
// <-- NO CHECK REF -->


xlfont('reset')
initialfonts = xlfont();
assert_checkequal(size(initialfonts,'*'), 11);

availablefonts = xlfont('AVAILABLE_FONTS');
// A minimal Linux may only have a few fonts, but Java logical fonts are always there
assert_checktrue(or(availablefonts == "SansSerif"));


for i=1:11:1
  xlfont(availablefonts(i),i);
end

for i=1:11:1
  xlfont(availablefonts(i),i,%t,%f);
end

for i=1:11:1
  xlfont(availablefonts(i),i,%t);
end

xlfont('reset');

afterresetfonts = xlfont();
assert_checkequal(initialfonts, afterresetfonts);

cd(TMPDIR);

exec(SCI+"/modules/localization/tests/unit_tests/CreateDir.sce", -1);

tab_ref = [
"世界您好",
"азеазея",
"ハロー・ワールド",
"حريات وحقوق",
"תוכנית"];

fontpath = SCI + "/modules/graphics/tests/unit_tests";
fontname = "DejaVuSansMono.ttf";
fontfull = fontpath + filesep() + fontname;

for i = 1 : size(tab_ref,'*')
  pathtemp = TMPDIR + filesep() + 'dir_' + tab_ref(i);
  fonttemp = pathtemp + filesep() + fontname;
  copyfile(fontfull, fonttemp);
  ierr = execstr('r = xlfont(fonttemp);','errcatch');
  assert_checkequal(ierr, 0);
  assert_checktrue(isdef('r'));
  clear r;
end

xlfont('reset')

// SansSerif is a Java logical font, available whatever the fonts installed
xlfont("SansSerif", size(xlfont(), "*"), %f, %f);
assert_checkequal(xlfont()($), "SansSerif");

xlfont("SansSerif", size(xlfont(), "*"), %f, %t);
assert_checkequal(xlfont()($), "SansSerif Italic");

xlfont("SansSerif", size(xlfont(), "*"), %t, %f);
assert_checkequal(xlfont()($), "SansSerif Bold");

xlfont("SansSerif", size(xlfont(), "*"), %t, %t);
assert_checkequal(xlfont()($), "SansSerif Bold Italic");

xlfont('reset')
