// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================


// <-- TEST WITH GRAPHIC -->

xlfont('reset')
initialfonts = xlfont();
if size(initialfonts,'*') <> 11 then pause,end

availablefonts = xlfont('AVAILABLE_FONTS');
if size(availablefonts,'*') < 11 then pause,end

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
if ~and(initialfonts == afterresetfonts) then pause,end

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
  if ierr <> 0 then pause, end
  if ~isdef('r') then pause, end
  clear r;
end

xlfont('reset')
