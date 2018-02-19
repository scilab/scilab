// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2017 - ESI - Antoine ELIAS
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- Non-regression test for bug 14901 -->
// <-- TEST WITH GRAPHIC -->
// <-- WINDOWS ONLY -->
// <-- NO CHECK REF -->
//
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=14901
//
// <-- Short Description -->


txt = [...
"import java.io.IOException;"
"import java.lang.InterruptedException;"
""
"public class Test_Exec_Scilab {"
"    public static void main(String[] args) throws IOException, InterruptedException {"
"    String[] cmd = {""" + SCI + "/bin/scilab.bat"", ""-nw"", ""-e"", ""a=string(1:10);mputl(a, \\\"""" + args[0] + ""\\\"");"", ""-quit""};"
"    Process p = Runtime.getRuntime().exec(cmd, null, null);"
"    p.waitFor();"
"    }"
"}"];

f = fullfile(TMPDIR, "/scilab.out");

j = jcompile("Test_Exec_Scilab", txt);
j.main(f);

v = mgetl(f);
assert_checkequal(v, string(1:10)');

