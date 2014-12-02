// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2014 - Scilab Enterprises - Calixte DENIZET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- Non-regression test for bug 13676 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/13676
//
// <-- Short Description -->
// An invisible figure could not be exported.

f = gcf();
f.visible = "off";
plot2d();

path = TMPDIR + "/bug_13676.png";
xs2png(f, path);

jimport javax.imageio.ImageIO;
jimport java.net.URL;

url = URL.new("file:////" + path);
bi = ImageIO.read(url);
w = bi.getWidth(jvoid);
h = bi.getHeight(jvoid);
data = uint32(bi.getRGB(0, 0, w, h, [], 0, w));
jremove ImageIO URL url bi

// Check if the image is not fully white
assert_checkfalse(and(data == (uint32(2^32 - 1))))
