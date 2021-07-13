// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2020 - Samuel GOUGEON
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.
// ============================================================================
//
// <-- NO CHECK REF -->
// <-- TEST WITH GRAPHIC -->
//
// Unitary test or mapsound()
// ==========================

fs = 22050;
t = 0:1/fs:1*(1-%eps);
y = 0.3 + [1.5*sin(2*%pi*800*t) sin(2*%pi*1200*t)];

// All default arguments
// ---------------------
clf reset
mapsound(y)
ax = gca();
assert_checkalmostequal(ax.x_ticks.locations, (0:0.2:1.8)', 2*%eps);
assert_checkequal(ax.y_ticks.locations, (0:500:4000)');
assert_checkequal(ax.children.type, "Fac3d");
assert_checkequal(ax.grid_position, "foreground");

smap = ax.children;
assert_checkequal(smap.type, "Fac3d");
assert_checkequal(smap.data_mapping, "scaled");
assert_checkequal(smap.color_flag, 4);

assert_checkequal(ax.x_label.text, _("Time [s]"));
ref = linspace(0, 1.956989247, 92);
assert_checkalmostequal(smap.data.x(1,1:92), ref);

assert_checkequal(ax.y_label.text, _("Frequency [Hz]"));
ref = linspace(0, 4326.265823, 94);
assert_checkalmostequal(smap.data.y(1,1:92:$), ref);

cb = gcf().children(1);
assert_checkequal(cb.title.text, _("Amplitude"));
assert_checkequal(cb.x_label.text, "95 × 93");
assert_checkalmostequal(cb.y_ticks.locations, (0:0.2:1.4)', 2*%eps);
assert_checkequal(size(gcf().color_map,1), 101);


// Setting Dt
// ----------
Dt = 0.04;
clf
data = mapsound(y, Dt);
[t, f] = (data.times, data.frequencies);
assert_checkequal([t(2) f(2)], [Dt 25]);

clf
data = mapsound(y, -1/20);
[t, f] = (data.times, data.frequencies);
assert_checkequal([t(2) f(2)], [0.1 10]);


// Setting displayed frequencies
// -----------------------------
clf
data = mapsound(y, , 1300);
[t, f] = (data.times, data.frequencies);
assert_checkequal([t(2), f(1), f(2), f($)], [0.04 0 25 1300]);

clf
data = mapsound(y, , [500 1300]);
[t, f] = (data.times, data.frequencies);
assert_checkequal([t(2), f(1), f(2)-f(1), f($)], [0.04 500 25 1300]);

data = mapsound(y, , -1/11);
[t, f] = (data.times, data.frequencies);
assert_checkequal([f(1), f(2), f($)], [0, 31.5, 1984.5]);
assert_checkalmostequal(t(2), 0.031746032);

clf
data = mapsound(y, , [500 -1/11]);
[t, f] = (data.times, data.frequencies);
assert_checkequal([f(1), f(2)-f(1), f($)], [504, 31.5, 1984.5]);
assert_checkalmostequal([t(2) t($)], [0.031746032 1.968253968]);

clf
data = mapsound(y, , [-1/110 -1/11]);
[t, f] = (data.times, data.frequencies);
assert_checkequal([f(1), f(2)-f(1), f($)], [220.5, 31.5, 1984.5]);
assert_checkalmostequal([t(2) t($)], [0.031746032 1.968253968]);


// With Colormap
// -------------
clf
mapsound(y, 0.04, , , parulacolormap)
ref = [1       1       1
       0.99052 0.99324 0.62152
       0.9763  0.9831  0.0538
       0.2081  0.1663  0.5292];
assert_checkequal(gcf().color_map([1 5 11 $],:), ref);

msg = msprintf(_("%s: Argument #%d: A colormap function expected.\n"), "mapsound", 5);
assert_checkerror("mapsound(y, 0.04, , , members)", msg);

msg = msprintf(_("%s: Argument #%d: Function expected.\n"), "mapsound", 5);
assert_checkerror("mapsound(y, 0.04, , , sin)", msg);
