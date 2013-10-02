// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Sylvestre Ledru
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

c = jcompile("Test", ["public class Test {";
"private double field;";
"public Test() {";
"}";
"public double getField() {";
"return field;";
"}";
"public void setField(double a) {";
"field = a;";
"}";
"}";]);
a = c.new();
a.field=2;
