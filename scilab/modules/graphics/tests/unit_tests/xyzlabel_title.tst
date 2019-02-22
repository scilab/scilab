// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2019 - Samuel GOUGEON
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.
// =============================================================================

// <-- TEST WITH GRAPHIC -->
// <-- ENGLISH IMPOSED -->
// <-- NO CHECK REF -->

clf reset
plotframe([0 0 1 1]);
ax = gca();
t = "Title à";
funs = list(xlabel, ylabel, zlabel, title);
axes = ["x_label" "y_label" "z_label" "title"];
for i = 1:4
    fun = funs(i);
    label = ax(axes(i));
    fun(t, "visible","off");
    assert_checkequal(label.visible, "off");
    fun(t, "visible","on");
    assert_checkequal(label.visible, "on");

    // Fonts
    fonts = ["courrier" "symbol" "times" "times italic" "times bold" ..
             "times bold italic" "helvetica" "helvetica italic" ..
             "helvetica bold" "helvetica bold italic"];
    for prop = ["fontname" "font_style"]
        for j = 1:10
            fun(t, prop, fonts(j));
            assert_checkequal(label.font_style, j-1);
        end
        for j = 1:10
            fun(t, prop, j-1);
            assert_checkequal(label.font_style, j-1);
        end
    end

    // Sizes
    values =  [0.5 1 2 2.7 6];
    for p = ["font_size" "fontsize"]
        for v = values
            fun(t, p, v)
            assert_checkequal(label.font_size, v);
            if v~=int(v)
                assert_checkequal(label.fractional_font, "on");
            end
        end
    end

    // Colors
    props = ["font_foreground" "color"
             "foreground"  "edgecolor"
             "background" "backgroundcolor" ];
    values = list("r", "green", "grey70", -1, -2, 12, "15", "#AA285D", [255,150,50]/255);
    h = hex2dec
    cols = [ color("red") color("green") color("grey70"), -1, -2, 12, 15 ..
             color(h("AA"),h("28"),h("5D")), color(255,150,50) ];
    for p = props
        for k = 1:3
            for m = 1:size(values)
                fun(t, p(k), values(m));
                assert_checkequal(label(props(k,1)), cols(m));
            end
        end
    end

    // tag
    // ---
    t = string(rand(1,1)*100);
    fun("tag",t);
    assert_checkequal(label("tag"), t);

    // box
    // ---
    val = ["off" "on" "off"];
    for v = val
        fun("box", v);
        assert_checkequal(label("fill_mode"), v);
    end

    // position
    xy = (0.3:0.1:0.6)'*[1 1]
    for p = xy'
        fun("position", p)
        assert_checkequal(label.position, p')
    end

    // label's rotation
    angles = [-90 -20 0 155 450];
    for p = ["rotation" "font_angle"]
        for a = angles
            fun(p,a)
            aref = 360 - modulo(a,360);
            assert_checkalmostequal(label.font_angle, aref)
        end
    end
end
