// Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2026 - Dassault Systèmes S.E. - Antoine ELIAS
//
// For more information, see the COPYING file which you should have received
// along with this program.

// Regenerates codicon.css, the console completion icons, from the Monaco
// editor shipped in SCI/thirdparty (SciNotes uses the same icons and colors).
// Run it by hand from a source tree after a Monaco update, then commit codicon.css:
//     exec("SCI/modules/console/src/web/css/codicon.sce", -1)
// The console does not load Monaco: the icons are copied as SVG masks.

function res = codicon_search(str, pattern, mandatory)
    [s, e, match, res] = regexp(str, pattern, "o");
    if s == [] then
        if mandatory then
            error(msprintf("%s not found in Monaco files.", pattern));
        end
        res = [];
    end
endfunction

function bytes = codicon_base64(str)
    alphabet = ascii("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/");
    values = zeros(1, 256);
    values(alphabet + 1) = 0:63;
    c = ascii(str);
    c(c == ascii("=")) = [];
    v = values(c + 1);
    n = floor(size(v, "*") * 3 / 4);
    v = matrix([v zeros(1, modulo(4 - modulo(size(v, "*"), 4), 4))], 4, -1);
    bytes = [v(1, :) * 4 + floor(v(2, :) / 16)
             modulo(v(2, :), 16) * 16 + floor(v(3, :) / 4)
             modulo(v(3, :), 4) * 64 + v(4, :)];
    bytes = bytes(1:n);
endfunction

// Big-endian integers at the 0-based offsets o of the font
function v = codicon_u16(font, o)
    v = font(o + 1) * 256 + font(o + 2);
endfunction

function v = codicon_s16(font, o)
    v = codicon_u16(font, o);
    v = v - 65536 * (v >= 32768);
endfunction

function v = codicon_u32(font, o)
    v = codicon_u16(font, o) * 65536 + codicon_u16(font, o + 2);
endfunction

// Glyph index of the codepoint c, from the cmap table (Windows Unicode BMP, format 4)
function gid = codicon_glyph(font, cmap, c)
    sub = [];
    for r = cmap + 4 + 8 * (0:codicon_u16(font, cmap + 2) - 1)
        if codicon_u16(font, r) == 3 & codicon_u16(font, r + 2) == 1 then
            sub = cmap + codicon_u32(font, r + 4);
        end
    end
    if sub == [] | codicon_u16(font, sub) <> 4 then
        error("No Unicode cmap (format 4) in the codicon font.");
    end
    segs = codicon_u16(font, sub + 6) / 2;
    ends = sub + 14;
    starts = ends + 2 * segs + 2;
    deltas = starts + 2 * segs;
    ranges = deltas + 2 * segs;
    i = find(codicon_u16(font, ends + 2 * (0:segs - 1)) >= c, 1) - 1;
    start = codicon_u16(font, starts + 2 * i);
    delta = codicon_u16(font, deltas + 2 * i);
    range = codicon_u16(font, ranges + 2 * i);
    if c < start then
        gid = 0;
    elseif range == 0 then
        gid = modulo(c + delta, 65536);
    else
        gid = codicon_u16(font, ranges + 2 * i + range + 2 * (c - start));
        if gid <> 0 then
            gid = modulo(gid + delta, 65536);
        end
    end
    if gid == 0 then
        error(msprintf("No glyph for U+%s in the codicon font.", dec2hex(c)));
    end
endfunction

// Relative coordinates of a simple glyph: short (1 byte), same (or sign) flag bits
function [v, p] = codicon_coords(font, p, flags, short, same)
    v = zeros(flags);
    cur = 0;
    for k = 1:size(flags, "*")
        if bitand(flags(k), short) then
            d = font(p + 1);
            p = p + 1;
            if ~bitand(flags(k), same) then
                d = -d;
            end
        elseif bitand(flags(k), same) then
            d = 0;
        else
            d = codicon_s16(font, p);
            p = p + 2;
        end
        cur = cur + d;
        v(k) = cur;
    end
endfunction

function s = codicon_num(v)
    s = strsubst(msprintf("%.3f", v), "/\.?0+$/", "", "r");
endfunction

function s = codicon_point(x, y)
    s = codicon_num(x) + " " + codicon_num(y);
endfunction

// SVG path of the glyph at the 0-based offset g, (x, y) -> (scale*x, offset - scale*y)
function d = codicon_path(font, g, scale, offset)
    nc = codicon_s16(font, g);
    if nc < 0 then
        error("Composite glyphs are not supported.");
    end
    endPts = codicon_u16(font, g + 10 + 2 * (0:nc - 1));
    np = endPts($) + 1;
    p = g + 12 + 2 * nc + codicon_u16(font, g + 10 + 2 * nc);

    flags = zeros(1, np);
    k = 1;
    while k <= np
        flags(k) = font(p + 1);
        p = p + 1;
        if bitand(flags(k), 8) then
            flags(k + 1:k + font(p + 1)) = flags(k);
            k = k + font(p + 1);
            p = p + 1;
        end
        k = k + 1;
    end
    [x, p] = codicon_coords(font, p, flags, 2, 16);
    [y, p] = codicon_coords(font, p, flags, 4, 32);
    x = scale * x;
    y = offset - scale * y;
    on = bitand(flags, 1) <> 0;

    d = "";
    first = 1;
    for last = endPts + 1
        cx = x(first:last);
        cy = y(first:last);
        con = on(first:last);
        first = last + 1;
        // start on an on-curve point, or on the implied one between the two ends
        j = find(con, 1);
        if j == [] then
            cx = [(cx($) + cx(1)) / 2 cx];
            cy = [(cy($) + cy(1)) / 2 cy];
            con = [%t con];
        else
            cx = [cx(j:$) cx(1:j - 1)];
            cy = [cy(j:$) cy(1:j - 1)];
            con = [con(j:$) con(1:j - 1)];
        end
        d = d + "M" + codicon_point(cx(1), cy(1));
        // back to the start point at the end of the contour
        cx($ + 1) = cx(1);
        cy($ + 1) = cy(1);
        con($ + 1) = %t;
        ctrl = [];
        for k = 2:size(cx, "*")
            if con(k) & ctrl == [] then
                if k < size(cx, "*") then
                    d = d + "L" + codicon_point(cx(k), cy(k));
                end
            elseif con(k) then
                d = d + "Q" + codicon_point(ctrl(1), ctrl(2)) + " " + codicon_point(cx(k), cy(k));
                ctrl = [];
            else
                if ctrl <> [] then
                    // implied on-curve point between two off-curve points
                    d = d + "Q" + codicon_point(ctrl(1), ctrl(2)) + " " + ..
                    codicon_point((ctrl(1) + cx(k)) / 2, (ctrl(2) + cy(k)) / 2);
                end
                ctrl = [cx(k) cy(k)];
            end
        end
        d = d + "Z";
    end
endfunction

// Codicons used by the console, without the "symbol-" prefix
names = ["function" "variable" "field" "keyword" "file" "folder"];

here = get_absolute_file_path("codicon.sce");
// installed Linux and macOS versions have thirdparty/ in SCI/../../
vs = strsplit(SCI, "share/scilab")(1) + "/thirdparty/web/monaco-editor/vs/";

// The codicon font is embedded in editor.main.css
mainCss = strcat(mgetl(vs + "editor/editor.main.css"));
font = codicon_base64(codicon_search(mainCss, "/@font-face\{font-family:codicon;[^}]*base64,([A-Za-z0-9+\/=]+)/", %t));
tags = [];
offsets = [];
for r = 12 + 16 * (0:codicon_u16(font, 4) - 1)
    tags($ + 1) = ascii(font(r + 1:r + 4));
    offsets($ + 1) = codicon_u32(font, r + 8);
end
head = offsets(tags == "head");
loca = offsets(tags == "loca");
glyf = offsets(tags == "glyf");
// font units (y up, baseline at 0) to a 16x16 viewBox (y down)
scale = 16 / codicon_u16(font, head + 18);
offset = scale * codicon_s16(font, offsets(tags == "hhea") + 4);
longLoca = codicon_s16(font, head + 50) == 1;

// The codepoints and colors of the icons are registered by editor.api-*.js
api = findfiles(vs, "editor.api-*.js");
if size(api, "*") <> 1 then
    error(msprintf("editor.api-*.js not found in %s.", vs));
end
api = strcat(mgetl(vs + api));

css = ["/*"
       " * Generated by codicon.sce from the Monaco editor, do not edit."
       " * Completion icons from Codicons (https://github.com/microsoft/vscode-codicons),"
       " * (c) Microsoft Corporation, CC BY 4.0, with the SciNotes (Monaco) symbol colors."
       " * Icons are masks painted with the text color, so that they follow the selection."
       " */"
       ".codicon{display:inline-block;flex-shrink:0;width:16px;height:16px;background-color:currentColor;" + ..
       "-webkit-mask:var(--icon) no-repeat center/16px 16px;mask:var(--icon) no-repeat center/16px 16px}"];

for n = names
    gid = codicon_glyph(font, offsets(tags == "cmap"), strtod(codicon_search(api, "/""symbol-" + n + """,(\d+)\)/", %t)));
    if longLoca then
        g = glyf + codicon_u32(font, loca + 4 * gid);
    else
        g = glyf + 2 * codicon_u16(font, loca + 2 * gid);
    end
    svg = "%3Csvg xmlns=''http://www.w3.org/2000/svg'' viewBox=''0 0 16 16''%3E%3Cpath d=''" + ..
    codicon_path(font, g, scale, offset) + "''/%3E%3C/svg%3E";
    css($ + 1) = ".codicon-symbol-" + n + "{--icon:url(""data:image/svg+xml," + svg + """)}";
    // Kinds without colors of their own use the foreground color
    colors = codicon_search(api, "/""symbolIcon\." + n + "Foreground"",\{dark:""(#\w+)"",light:""(#\w+)""/", %f);
    if colors <> [] then
        css($ + 1) = ".codicon-symbol-" + n + "{color:" + colors(2) + "}";
        css($ + 1) = ".dark .codicon-symbol-" + n + "{color:" + colors(1) + "}";
    end
end

mputl(css, here + "codicon.css");
