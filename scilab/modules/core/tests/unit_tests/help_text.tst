// =============================================================================
// Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2026 - Dassault Systèmes S.E. - Antoine ELIAS
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->

// Text rendering of help pages (modules/core/etc/help_text.xsl), without and
// with "scilab:" links. It relies on fake help pages shipped with the test
// (help_text/help/en_US), not on real pages whose content may change.

LF = ascii(10);
sheet = fullfile(SCI, "modules", "core", "etc", "help_text.xsl");

// fake toolbox: the fake pages + their inline help index
tbx = fullfile(TMPDIR, "help_text_tbx");
copyfile(fullfile(SCI, "modules", "core", "tests", "unit_tests", "help_text"), tbx);
mkdir(fullfile(tbx, "inline"));
mkdir(fullfile(tbx, "inline", "en_US"));
mputl(["{"
       "  ""help_text_fake_page"": ""TBX/help/en_US/help_text_fake_page.xml"","
       "  ""help_text_fake_nolink"": ""TBX/help/en_US/help_text_fake_nolink.xml"""
       "}"], fullfile(tbx, "inline", "en_US", "links.json"));
loadToolboxInlineHelp(tbx);

// plain text ------------------------------------------------------------------
plain = %_help("help_text_fake_page", sheet);
assert_checkequal(size(plain), [1 1]);
assert_checkequal(strindex(plain, "<a href"), []);

lines = strsplit(plain, LF);
assert_checkequal(lines(1), "help_text_fake_page");
assert_checkequal(lines(2), "    fake page testing the text rendering of help pages");
assert_checktrue(or(lines == "    Syntax:"));
assert_checktrue(or(lines == "        y = help_text_fake_page(x, opt)"));
assert_checktrue(or(lines == "    Arguments:"));
assert_checktrue(or(lines == "          x: a real matrix."));
assert_checktrue(or(lines == "        opt: a string."));
assert_checktrue(or(lines == "    See also:"));
// the page identifiers (to be typed after "help") are listed, not the labels
assert_checktrue(or(lines == "        plot2d, axes_properties, a_first_quite_long_page_identifier,"));
assert_checktrue(or(lines == "Type ""doc help_text_fake_page"" to open the help_text_fake_page reference page"));
assert_checktrue(and(length(lines) <= 80));

// a returned text has no link by default, whatever the Scilab mode
assert_checkequal(%_help("help_text_fake_page", sheet, %f), plain);

// with links ------------------------------------------------------------------
linked = %_help("help_text_fake_page", sheet, %t);

// a link targets the page identifier and shows the label: they may differ,
// and a label may hold spaces
assert_checktrue(strindex(linked, "<a href=""scilab:help plot2d"">plot2d</a>,") <> []);
assert_checktrue(strindex(linked, "<a href=""scilab:help axes_properties"">axes properties</a>,") <> []);
assert_checktrue(strindex(linked, "<a href=""scilab:help last_page"">last_page</a>" + LF) <> []);
assert_checktrue(strindex(linked, "Type <a href=""scilab:doc help_text_fake_page"">doc help_text_fake_page</a> to open the") <> []);

// a link is never torn apart by line wrapping
starts = strindex(linked, "<a href=""scilab:");
ends = strindex(linked, "</a>");
assert_checkequal(size(starts), size(ends));
for i = 1:size(starts, "*")
    assert_checkequal(strindex(part(linked, starts(i):ends(i)), LF), []);
end

// wrapping only counts the visible text
visible = strsubst(linked, "/<a href=""[^""]*"">|<\/a>/", "", "r");
visibleLines = strsplit(visible, LF);
assert_checkequal(strindex(visible, "<"), []);
assert_checktrue(and(length(visibleLines) <= 80));
assert_checktrue(or(visibleLines == "        plot2d, axes properties, a first quite long page label,"));

// an entry without linkend stays plain text
nolink = %_help("help_text_fake_nolink", sheet, %t);
assert_checktrue(strindex(nolink, "        <a href=""scilab:help plot2d"">plot2d</a>, not a link" + LF) <> []);
assert_checktrue(strindex(%_help("help_text_fake_nolink", sheet), "        plot2d, not a link" + LF) <> []);

// display ---------------------------------------------------------------------
// The text is displayed by chunks of at most 2048 bytes, cut on line
// boundaries for the console to get whole links. The fake page is built for
// a link to lie across the first 2048 bytes boundary (ASCII only page:
// characters are bytes).
assert_checktrue(length(linked) > 2048);
assert_checktrue(or(starts <= 2048 & ends + 3 > 2048));

// the displayed text is the returned one
diaryFile = fullfile(TMPDIR, "help_text.diary");
id = diary(diaryFile, "new");
%_help("help_text_fake_page", sheet, %t);
diary(id, "close");
displayed = strcat(mgetl(diaryFile), LF);
assert_checktrue(strindex(displayed, linked) <> []);

// errors ----------------------------------------------------------------------
msg = msprintf(_("%s: Wrong type for input argument #%d: A boolean expected.\n"), "help", 3);
assert_checkerror("%_help(""help_text_fake_page"", sheet, 1)", msg);
msg = msprintf(_("%s: Unknown help page: %s.\n"), "help", "help_text_unknown_page");
assert_checkerror("%_help(""help_text_unknown_page"")", msg);
