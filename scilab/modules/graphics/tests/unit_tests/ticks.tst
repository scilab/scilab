// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011 - DIGITEO - Bruno JOFRET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->

// test ticks

a = gca();

assert_checkequal(typeof(a.x_ticks), "ticks")
assert_checkequal(typeof(a.y_ticks), "ticks")
assert_checkequal(typeof(a.z_ticks), "ticks")

assert_checkequal(a.auto_ticks, ["on" "on" "on"])
assert_checkequal(a.view, "2d")

assert_checkalmostequal(a.x_ticks.locations, [0:0.1:1]')
assert_checkequal(a.x_ticks.labels, ["0.0", string(0.1:0.1:0.9), "1.0"]')

assert_checkalmostequal(a.y_ticks.locations, [0:0.1:1]')
assert_checkequal(a.y_ticks.labels, ["0.0", string(0.1:0.1:0.9), "1.0"]')

assert_checkequal(a.z_ticks.locations, [])
assert_checkequal(a.z_ticks.labels, [])

newTicks = tlist(["ticks" "locations" "labels"], [0:0.01:1]', string([0:0.01:1])');

// Change x_ticks
// auto_ticks wil move to ["off" "on" "on"]
a.x_ticks = newTicks;
assert_checkalmostequal(a.x_ticks.locations, [0:0.01:1]')
assert_checkequal(a.x_ticks.labels, string([0:0.01:1])')
assert_checkequal(a.auto_ticks, ["off" "on" "on"])

// Try to change z_ticks
// View is "2d" => no changes made !!!
// except auto_ticks wil move to ["off" "on" "off"]
a.z_ticks = newTicks;
assert_checkequal(a.z_ticks.locations, [])
assert_checkequal(a.z_ticks.labels, [])
assert_checkequal(a.auto_ticks, ["off" "on" "off"])

// Reset auto_ticks and move to 3d view
a.auto_ticks = ["on" "on" "on"];
a.view = "3d";
assert_checkequal(a.auto_ticks, ["on" "on" "on"])
assert_checkequal(a.view, "3d")
// z_ticks now have default values
assert_checkalmostequal(a.z_ticks.locations, [-1:0.2:1]')
assert_checkequal(a.z_ticks.labels, ["-1.0", string(-0.8:0.2:-0.2),"0.0", string(0.2:0.2:0.8)  "1.0"]')

// Try to change z_ticks
// auto_ticks wil move to ["on" "on" "off"]
a.z_ticks = newTicks;
assert_checkequal(a.auto_ticks, ["on" "on" "off"])
assert_checkalmostequal(a.z_ticks.locations, [0:0.01:1]')
assert_checkequal(a.z_ticks.labels, [string(0:0.01:1)]')

clf();
plot(1d3:2d3, 1d3:2d3);
a = gca();
x_str_1k = a.x_ticks.labels(1);
y_str_1k = a.y_ticks.labels(1);
x_str_2k = a.x_ticks.labels($);
y_str_2k = a.y_ticks.labels($);
assert_checkequal(ascii(x_str_1k), ascii("1 000"));
assert_checkequal(ascii(y_str_1k), ascii("1 000"));
assert_checkequal(ascii(x_str_2k), ascii("2 000"));
assert_checkequal(ascii(y_str_2k), ascii("2 000"));