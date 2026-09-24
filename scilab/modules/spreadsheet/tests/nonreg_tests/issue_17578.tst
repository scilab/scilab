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
// <-- Non-regression test for issue 17578 -->
//
// <-- GitLab URL -->
// https://gitlab.com/scilab/scilab/-/issues/17578
//
// <-- Short Description -->
// xlsxInfo reported no sheet on files whose relationships reference drawing
// parts absent from the archive.

f = fullfile(SCI, "modules", "spreadsheet", "tests", "nonreg_tests", "issue_17578.xlsx");

assert_checkequal(isXlsx(f), %t);

info = xlsxInfo(f);
assert_checkequal(info.total_sheets, 2);
assert_checkequal(info.sheet_names, ["trade_out", "inpro_out"]);

// The streaming fallback reads core.xml but does not expose its properties.
// This assertion will fail the day xlnt loads the file natively again, which
// is when the fallback can go.
assert_checkequal(info.creator, "");

first = xlsxInfo(f, 1);
assert_checkequal(first.sheet_name, "trade_out");
assert_checkequal(first.cell_count, 18578);

second = xlsxInfo(f, "inpro_out");
assert_checkequal(second.sheet_index, 2);

data = xlsxRead(f);
assert_checkequal(size(data), [77 306]);

// F8 and F77 of trade_out: the sheet starts at column B, so its column F
// is column 5 of the returned matrix.
assert_checkalmostequal(data(8, 5), 52.8905384327492, 1e-12);
assert_checkalmostequal(data(77, 5), 49.4576878711019, 1e-12);

other = xlsxRead(f, "sheet", "inpro_out");
assert_checkequal(size(other), [40 306]);

// Writing into such a file is refused: xlnt cannot load it, and starting from
// a blank workbook would silently drop the two sheets it still holds.
target = fullfile(TMPDIR, "issue_17578_write.xlsx");
copyfile(f, target);
assert_checktrue(execstr("xlsxWrite([1 2 3], target)", "errcatch") <> 0);
assert_checkequal(xlsxInfo(target).sheet_names, ["trade_out", "inpro_out"]);
