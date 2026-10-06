// =============================================================================
// Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2026 - UTC - Stéphane MOTTELET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->
// <-- NO CHECK REF -->

// Unit tests of org.scilab.modules.renderer.utils.EntityPicker

jimport org.scilab.modules.renderer.utils.EntityPicker;
jimport org.scilab.modules.gui.datatip.DatatipMove;

// 3D polyline: the middle segment P1->P2 is vertical
// P0=(0,0,0)  P1=(1,0,0)  P2=(1,0,1)  P3=(2,0,1)
clf();
param3d([0 1 1 2], [0 0 0 0], [0 0 1 1]);
e = gce();
a = gca();
a.rotation_angles = [70 30];

// getSegment(uid, index): first, middle and last segments (index is 0-based)
P = [0 0 0; 1 0 0; 1 0 1; 2 0 1];
for i = 0:2
    seg = EntityPicker.getSegment(e.UID, i);
    assert_checkequal([seg.x0 seg.y0 seg.z0], P(i + 1, :));
    assert_checkequal([seg.x1 seg.y1 seg.z1], P(i + 2, :));
end

// Moving a datatip with the keyboard uses getSegment: starting from P1,
// the datatip must move up along the vertical segment P1->P2
d = datatipCreate(e, 2);
assert_checkequal(d.data, [1 0 0]);
DatatipMove.moveRight(d.UID);
assert_checkequal(d.data(1:2), [1 0]);
assert_checktrue(d.data(3) > 0 & d.data(3) < 1);
