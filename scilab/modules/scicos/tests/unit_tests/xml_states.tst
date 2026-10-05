// =============================================================================
// Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2026 - Dassault Systèmes S.E. - Vincent COUVERT
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->
//
// read_xml_initial_states / write_xml_states, called by the C code modelicac
// generates for the Modelica initialization GUI. Fixtures: xml_states/init.xml,
// xml_states/fixed.xml, and xml_states/demos/*.xml, init files of the Modelica
// demos as saved by Xcos.

ilib_verbose(0);
src = SCI + "/modules/scicos/tests/unit_tests/xml_states/";
wd = TMPDIR + "/xml_states/";
mkdir(wd);
copyfile(src + "xml_states.c", wd);

// Same link setup as buildnewblock for Modelica blocks
loadScicos();
ldflags = "";
if getos() == "Windows" then
    if ~exists("dynamic_linkwindowslib") then
        load("SCI/modules/dynamic_link/macros/windows/lib");
    end
    ldflags = strcat(" """ + dlwGetLibrariesPath() + dlwGetXcosLibraries() + """");
end
cd(wd);
ilib_for_link(["xml_states_read" "xml_states_write"], "xml_states.c", [], "c", "", "loader.sce", "xml_states", ldflags);
exec(wd + "loader.sce", -1);

function [v, ret] = xread(file, ids, v0)
    n = size(ids, "*");
    [v, ret] = call("xml_states_read", file, 1, "c", strcat(ids, ";"), 2, "c", n, 3, "i", v0, 4, "d", ..
    "out", [n, 1], 4, "d", [1, 1], 5, "i");
endfunction

function ret = xwrite(file, ids, v)
    n = size(ids, "*");
    ret = call("xml_states_write", file, 1, "c", strcat(ids, ";"), 2, "c", n, 3, "i", v, 4, "d", ..
    "out", [1, 1], 5, "i");
endfunction

function set_initial(doc, path, value)
    s = xmlXPath(doc, path);
    assert_checkequal(size(s, "*"), 1);
    e = s(1);
    e.attributes.value = value;
endfunction

// Document content, regardless of comments, indentation, line endings and
// encoding declaration (without one, xmlDump escapes non-ASCII characters)
function t = content(doc)
    c = xmlXPath(doc, "//comment()");
    for k = size(c, "*"):-1:1
        xmlRemove(c(k));
    end
    t = strcat(xmlDump(doc.root), "");
    t = strsubst(t, "/>\s+</", "><", "r");
    [_, _, refs] = regexp(t, "/&#x[0-9A-F]+;/");
    for r = unique(refs)'
        c = xmlReadStr("<c>" + r + "</c>");
        t = strsubst(t, r, c.root.content);
        xmlDelete(c);
    end
endfunction

function checkwritten(f, expected)
    doc = xmlRead(f);
    assert_checkequal(content(doc), content(expected));
    xmlDelete(doc);
endfunction

// The layout is kept, except the XML declaration
function checklines(original, written)
    o = mgetl(original);
    o(grep(o, "<?xml")) = [];
    assert_checkequal(size(mgetl(written), "*"), size(o, "*"));
endfunction

NA = -999; // left untouched when an id is not resolved

// ----------------------------------------------------------------------------
// Read
// ----------------------------------------------------------------------------
fixture = src + "init.xml";
crlf = wd + "init_crlf.xml";
fd = mopen(crlf, "wb");
mputstr(strcat(mgetl(fixture) + ascii([13 10])), fd);
mclose(fd);

ids = ["top"; "R1.R"; "R1.i"; "R1.n"; "R1.ref"; "R1.chain"; "R1.empty"; "R1.expr"; ..
"R1.sub.x"; "dup"; "C1.v"; "C1.w"; "missing"; ""];
expected = [0.9; 1.5; -2.5e-3; 42; 1.5; 1.5; NA; NA; 7; 2; 3.25; 8; NA; NA];
for f = [fixture crlf]
    [v, ret] = xread(f, ids, NA * ones(ids));
    assert_checkequal(ret, 0);
    assert_checkequal(v, expected);
end

for k = 1:size(ids, "*")
    [v, ret] = xread(fixture, ids(k), NA);
    assert_checkequal(ret, 0);
    assert_checkequal(v, expected(k));
end

[v, ret] = xread(fixture, ["R1.R"; "R1.R"], [NA; NA]);
assert_checkequal(ret, 0);
assert_checkequal(v, [1.5; 1.5]);

[v, ret] = xread(wd + "nofile.xml", "R1.R", NA);
assert_checkequal(ret, -1);
assert_checkequal(v, NA);

// Only empty ids: the file is not opened
[v, ret] = xread(wd + "nofile.xml", ["", ""], [NA; NA]);
assert_checkequal(ret, 0);
assert_checkequal(v, [NA; NA]);

noelements = wd + "noelements.xml";
mputl("<model><name>x</name></model>", noelements);
[v, ret] = xread(noelements, "x", NA);
assert_checkequal(ret, 0);
assert_checkequal(v, NA);

// ----------------------------------------------------------------------------
// Write
// ----------------------------------------------------------------------------
wids = ["top"; "R1.R"; ""; "R1.sub.x"; "dup"; "missing"; "C1.v"; "C1.w"; "C1.w"];
wval = [0.5; %pi; 99; -1e-10; 0.5; 11; 1/3; 1; 2];
t = "/model/elements/struct[1]/subnodes/";
for f = [fixture crlf]
    w = wd + "written.xml";
    copyfile(f, w);
    assert_checkequal(xwrite(w, wids, wval), 0);

    doc = xmlRead(fixture);
    set_initial(doc, "/model/elements/terminal[id=""top""]/initial_value", "0.5");
    set_initial(doc, t + "terminal[id=""R1.R""]/initial_value", "3.14159");
    set_initial(doc, t + "struct/subnodes/terminal[id=""R1.sub.x""]/initial_value", "-1e-10");
    set_initial(doc, t + "terminal[id=""dup""]/initial_value", "0.5");
    set_initial(doc, "//terminal[id=""C1.v""]/initial_value", "0.333333");
    set_initial(doc, "//terminal[id=""C1.w""]/initial_value", "2");
    checkwritten(w, doc);
    checklines(f, w);
    xmlDelete(doc);

    [v, ret] = xread(w, ["R1.R"; "R1.chain"; "dup"; "C1.v"], NA * ones(4, 1));
    assert_checkequal(ret, 0);
    assert_checkalmostequal(v, [3.14159; 3.14159; 0.5; 0.333333]);
end

assert_checkequal(xwrite(noelements, "x", 1), 0);
checkwritten(noelements, xmlReadStr("<model><name>x</name></model>"));

assert_checkequal(xwrite(wd + "nofile.xml", "R1.R", 1), -1);
assert_checkfalse(isfile(wd + "nofile.xml"));

// Only empty ids: nothing is written
assert_checkequal(xwrite(wd + "nofile.xml", ["", ""], [1; 2]), 0);
assert_checkfalse(isfile(wd + "nofile.xml"));

// ----------------------------------------------------------------------------
// Entities, incomplete terminals, long ids, reference loops, malformed file
// ----------------------------------------------------------------------------
fixed = src + "fixed.xml";
longid = "long." + strcat(emptystr(1, 150) + "x");
ids = ["a<b"; "ref"; "noinit"; "after"; longid; "reflong"; "loop.a"; "loop.b"; "self"];
[v, ret] = xread(fixed, ids, NA * ones(ids));
assert_checkequal(ret, 0);
assert_checkequal(v, [1.5; 1.5; NA; 5; 4; 4; NA; NA; NA]);

w = wd + "fixed.xml";
copyfile(fixed, w);
assert_checkequal(xwrite(w, ["a<b"; "noinit"; "after"; longid], [2; 6; 7; 8]), 0);
doc = xmlRead(fixed);
set_initial(doc, "//terminal[id=""a<b""]/initial_value", "2");
set_initial(doc, "//terminal[id=""after""]/initial_value", "7");
set_initial(doc, "//terminal[id=""" + longid + """]/initial_value", "8");
checkwritten(w, doc);
checklines(fixed, w);
xmlDelete(doc);
doc = xmlRead(w);
assert_checkequal(size(xmlXPath(doc, "//comment()"), "*"), 1);
xmlDelete(doc);

[v, ret] = xread(w, ["ref"; "reflong"], [NA; NA]);
assert_checkequal(ret, 0);
assert_checkequal(v, [2; 8]);

malformed = wd + "malformed.xml";
mputl("<model><elements>", malformed);
[v, ret] = xread(malformed, "x", NA);
assert_checkequal(ret, -1);
assert_checkequal(v, NA);
assert_checkequal(xwrite(malformed, "x", 1), -1);
assert_checkequal(mgetl(malformed), "<model><elements>");

// ----------------------------------------------------------------------------
// Demos: every id is read, then written; the result must still load in Xcos
// and in xml2modelica
// ----------------------------------------------------------------------------
schema = xmlSchema(SCI + "/modules/xcos/etc/Modelica.xsd");
demos = findfiles(src + "demos", "*.xml");
assert_checkfalse(demos == []);
for d = demos'
    f = src + "demos/" + d;
    doc = xmlRead(f);
    s = xmlXPath(doc, "//terminal");
    n = size(s, "*");
    ids = emptystr(n, 1);
    expected = NA * ones(n, 1);
    for k = 1:n
        e = xmlXPath(s(k), "id");
        ids(k) = e(1).content;
        e = xmlXPath(s(k), "initial_value");
        value = e(1).attributes.value;
        if value <> "" then
            [x, rest] = strtod(value);
            assert_checkequal(rest, "");
            expected(k) = x;
        end
    end
    assert_checkequal(size(unique(ids), "*"), n);

    [v, ret] = xread(f, ids, NA * ones(n, 1));
    assert_checkequal(ret, 0);
    assert_checkequal(v, expected);

    w = wd + d;
    copyfile(f, w);
    wval = (1:n)' / 4;
    assert_checkequal(xwrite(w, ids, wval), 0);
    for k = 1:n
        set_initial(doc, "//terminal[id=""" + ids(k) + """]/initial_value", msprintf("%g", wval(k)));
    end
    checkwritten(w, doc);
    checklines(f, w);
    xmlDelete(doc);

    doc = xmlRead(w);
    assert_checkequal(xmlValidate(doc, schema), []);
    xmlDelete(doc);

    if with_modelica_compiler() then
        exe = getmodelicacpath() + "XML2Modelica";
        assert_checkequal(host("""" + exe + """ """ + w + """ -init -o """ + wd + "flat.mo"""), 0);
    end

    [v, ret] = xread(w, ids, NA * ones(n, 1));
    assert_checkequal(ret, 0);
    assert_checkequal(v, wval);
end
xmlDelete(schema);

ulink();
