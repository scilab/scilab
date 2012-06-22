// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008-2008 - INRIA - Jean-Baptiste Silvy
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->

// <-- Non-regression test for bug 3061 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=3061
//
// <-- Short Description -->
// The title position is incorrect when the graphic window is splitted

subplot(211); title('foo');
// the title used to be draw below the axes

a = gca();
// check that the label is in the window
rect = stringbox(a.title);

// convert it to pixel coordinates
for i = 1:4
  [pixPos(i,1), pixPos(i,2)] = xchange(rect(1,i),rect(2,i), "f2i");
end

// get canvas size
fig = gcf();
axesSize = fig.axes_size;

// check that the label is drawn on top of the canvas and visible
for i = 1:4
  if (pixPos(i,2) < 0 | pixPos(i,2) > axesSize(2) * 0.1) then pause; end
end

// second test
// create two plots
clf();subplot(212),a1=gca();a1.box='on';a1.title.text='foo';
subplot(211),a2=gca();a2.box='on';a2.title.text='foo';

// check that title of a2 is on top of the canvas
subplot(211);
rect = stringbox(a2.title);

// convert it to pixel coordinates
for i = 1:4
  [pixPos(i,1), pixPos(i,2)] = xchange(rect(1,i),rect(2,i), "f2i");
end

// check that the label is drawn on top of the canvas and visible
for i = 1:4
  if (pixPos(i,2) < 0 | pixPos(i,2) > axesSize(2) * 0.1) then pause; end
end


// check that title of a1 is in the middle of the canvas
subplot(212);
rect = stringbox(a1.title);

// convert it to pixel coordinates
for i = 1:4
  [pixPos(i,1), pixPos(i,2)] = xchange(rect(1,i),rect(2,i), "f2i");
end

// check that the label is drawn on top of the canvas and visible
for i = 1:4
  if (pixPos(i,2) < axesSize(2) * 0.4 | pixPos(i,2) > axesSize(2) * 0.6) then pause; end
end

