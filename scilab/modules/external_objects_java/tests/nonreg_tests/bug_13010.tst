// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

compiledClass = jcompile("Test", ["public class Test {";
"public enum foo { ONE, TWO, THREE; }";
"public static double bar(foo x) { return (double) x.ordinal(); }}"]);

classObj = compiledClass.class;
className = classObj.getName(jvoid);
assert_checkequal(className, "Test");
