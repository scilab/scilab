// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2012 - Digiteo - Vincent Couvert
//
// This file is released under the 3-clause BSD license. See COPYING-BSD.

demo_plot3d = scf(100002);

// Display image with default size (centered in figure)
f = gcf();
figSize = f.axes_size;
imageWidth = 181;
imageHeight = 144;
h = uicontrol("Parent", f, ..
"Style", "image", ..
"Position", [(figSize(1)-imageWidth)/2 (figSize(2)-imageHeight)/2 imageWidth imageHeight], ..
"String", SCI + "/modules/demo_tools/images/logo_scilab.png");

demo_viewCode(SCI+ "/modules/gui/demos/uicontrol_image.dem.sce");

// Play with image size
scales = [1:.01:2 2:-0.01:1];
for k=scales
    if ~is_handle_valid(h) then
        break
    end
    scaledImageWidth = int(imageWidth * k);
    scaledImageHeight = int(imageHeight * k);
    scaleXY = [scaledImageWidth/imageWidth scaledImageHeight/imageHeight];
    set(h, "Position", [(figSize(1)-scaledImageWidth)/2 (figSize(2)-scaledImageHeight)/2 scaledImageWidth scaledImageHeight]);
    set(h, "Value", scaleXY);
    sleep(10);
end

// Play with image shear
xShear = [0:.01:1 1:-0.01:0];
yShear = xShear;
for k=1:size(xShear, "*")
    if ~is_handle_valid(h) then
        break
    end
    valueProp = [1 1 xShear(k) yShear(k)];
    set(h, "Value", valueProp);
    sleep(10);
end

// Play with rotation
for k=0:180
    if ~is_handle_valid(h) then
        break
    end
    valueProp = [1 1 0 0 k];
    set(h, "Value", valueProp);
    sleep(10);
end

