// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2019 - Samuel GOUGEON
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->

TMP = strsubst(fileparts(TMPDIR), "\", "/");

// LINEAR SYSTEMS
// ==============
grand("setsd",0);
A = grand(3,3,"uin",-999,999)/100;
B = grand(3,2,"uin",-999,999)/100;
C = grand(2,3,"uin",-999,999)/100;

// Continuous time:
// ---------------
s = syslin("c", A, B, C);

// LaTeX
ref = "${\begin{pmatrix}{\left\{\begin{array}{rcl}\dot{X}(t)&=&{\begin{pmatrix}2.02&7.14&-8.59\cr -9.99&-9.64&-1.27\cr 5.59&7.25&7.44\cr \end{pmatrix}} X(t)+{\begin{pmatrix}4.46&-0.62\cr 8.42&-3.3\cr -2.67&9.04\cr \end{pmatrix}}U(t)\cr Y(t) &=& {\begin{pmatrix}-7.72&6.86&-6.75\cr -4.58&-3.59&-8.06\cr \end{pmatrix}} X(t) \end{array}\right.}\cr \end{pmatrix}}$";
assert_checkequal(prettyprint(s), ref);

// TeX
ref = "${\pmatrix{{\left\{\eqalign{\dot{X}(t) &= {\pmatrix{2.02&7.14&-8.59\cr -9.99&-9.64&-1.27\cr 5.59&7.25&7.44\cr }} X(t)+{\pmatrix{4.46&-0.62\cr 8.42&-3.3\cr -2.67&9.04\cr }}U(t)\cr Y(t) &= {\pmatrix{-7.72&6.86&-6.75\cr -4.58&-3.59&-8.06\cr }} X(t) }\right.}\cr }}$";
assert_checkequal(prettyprint(s, "tex"), ref);

// MathML
ref = [""
"<mfenced open=""("" close="")"">"
"<mtable>"
"<mtr>"
"<mtd><mfenced close="""" open=""{"">"
"<mtable columnalign=""right center left"">"
"<mtr><mtd><mrow><mover><mi>X</mi><mo>.</mo></mover><mo>(</mo><mi>t</mi><mo>)</mo></mrow></mtd><mtd><mo>=</mo></mtd><mtd>"
"<mfenced open=""("" close="")"">"
"<mtable>"
"<mtr>"
"<mtd><mn>2.02</mn></mtd>"
"<mtd><mn>7.14</mn></mtd>"
"<mtd><mo>-</mo><mn>8.59</mn></mtd>"
"</mtr>"
"<mtr>"
"<mtd><mo>-</mo><mn>9.99</mn></mtd>"
"<mtd><mo>-</mo><mn>9.64</mn></mtd>"
"<mtd><mo>-</mo><mn>1.27</mn></mtd>"
"</mtr>"
"<mtr>"
"<mtd><mn>5.59</mn></mtd>"
"<mtd><mn>7.25</mn></mtd>"
"<mtd><mn>7.44</mn></mtd>"
"</mtr>"
"</mtable>"
"</mfenced>"
"<mrow><mi>X</mi><mo>(</mo><mi>t</mi><mo>)</mo></mrow><mo>+</mo>"
"<mfenced open=""("" close="")"">"
"<mtable>"
"<mtr>"
"<mtd><mn>4.46</mn></mtd>"
"<mtd><mo>-</mo><mn>0.62</mn></mtd>"
"</mtr>"
"<mtr>"
"<mtd><mn>8.42</mn></mtd>"
"<mtd><mo>-</mo><mn>3.3</mn></mtd>"
"</mtr>"
"<mtr>"
"<mtd><mo>-</mo><mn>2.67</mn></mtd>"
"<mtd><mn>9.04</mn></mtd>"
"</mtr>"
"</mtable>"
"</mfenced>"
"<mrow><mi>U</mi><mo>(</mo><mi>t</mi><mo>)</mo></mrow></mtd></mtr>"
"<mtr><mtd><mrow><mi>Y</mi><mo>(</mo><mi>t</mi><mo>)</mo></mrow></mtd><mtd><mo>=</mo></mtd><mtd>"
"<mfenced open=""("" close="")"">"
"<mtable>"
"<mtr>"
"<mtd><mo>-</mo><mn>7.72</mn></mtd>"
"<mtd><mn>6.86</mn></mtd>"
"<mtd><mo>-</mo><mn>6.75</mn></mtd>"
"</mtr>"
"<mtr>"
"<mtd><mo>-</mo><mn>4.58</mn></mtd>"
"<mtd><mo>-</mo><mn>3.59</mn></mtd>"
"<mtd><mo>-</mo><mn>8.06</mn></mtd>"
"</mtr>"
"</mtable>"
"</mfenced>"
"<mrow><mi>X</mi><mo>(</mo><mi>t</mi><mo>)</mo></mrow></mtd></mtr>"
"</mtable>"
"</mfenced>"
"</mtd>"
"</mtr>"
"</mtable>"
"</mfenced>"
""];
ref = strcat(ref, ascii(10));
assert_checkequal(prettyprint(s, "mathml"), ref);

// Discrete states (and improper syslin):
// -------------------------------------
s = syslin("d", A, B, C, [1+%s, 1-%s; -%s, %s]);
// LaTeX
ref = "${\begin{pmatrix}{\left\{\begin{array}{rcl}\stackrel{+}{X}(t)&=&{\begin{pmatrix}2.02&7.14&-8.59\cr -9.99&-9.64&-1.27\cr 5.59&7.25&7.44\cr \end{pmatrix}} X(t)+{\begin{pmatrix}4.46&-0.62\cr 8.42&-3.3\cr -2.67&9.04\cr \end{pmatrix}}U(t)\cr Y(t) &=& {\begin{pmatrix}-7.72&6.86&-6.75\cr -4.58&-3.59&-8.06\cr \end{pmatrix}} X(t)  + {\begin{pmatrix}1+s &1-s \cr -s &s \cr \end{pmatrix}} U(t)\end{array}\right.}\cr \end{pmatrix}}$";
assert_checkequal(prettyprint(s, "latex"), ref);

// TeX
ref = "${\pmatrix{{\left\{\eqalign{{\buildrel + \over X(t)} &= {\pmatrix{2.02&7.14&-8.59\cr -9.99&-9.64&-1.27\cr 5.59&7.25&7.44\cr }} X(t)+{\pmatrix{4.46&-0.62\cr 8.42&-3.3\cr -2.67&9.04\cr }}U(t)\cr Y(t) &= {\pmatrix{-7.72&6.86&-6.75\cr -4.58&-3.59&-8.06\cr }} X(t)  + {\pmatrix{1+s &1-s \cr -s &s \cr }} U(t)}\right.}\cr }}$";
assert_checkequal(prettyprint(s, "tex"), ref);

// MathML
ref = [""
"<mfenced open=""("" close="")"">"
"<mtable>"
"<mtr>"
"<mtd><mfenced close="""" open=""{"">"
"<mtable columnalign=""right center left"">"
"<mtr><mtd><mrow><mover><mi>X</mi><mo>+</mo></mover><mo>(</mo><mi>t</mi><mo>)</mo></mrow></mtd><mtd><mo>=</mo></mtd><mtd>"
"<mfenced open=""("" close="")"">"
"<mtable>"
"<mtr>"
"<mtd><mn>2.02</mn></mtd>"
"<mtd><mn>7.14</mn></mtd>"
"<mtd><mo>-</mo><mn>8.59</mn></mtd>"
"</mtr>"
"<mtr>"
"<mtd><mo>-</mo><mn>9.99</mn></mtd>"
"<mtd><mo>-</mo><mn>9.64</mn></mtd>"
"<mtd><mo>-</mo><mn>1.27</mn></mtd>"
"</mtr>"
"<mtr>"
"<mtd><mn>5.59</mn></mtd>"
"<mtd><mn>7.25</mn></mtd>"
"<mtd><mn>7.44</mn></mtd>"
"</mtr>"
"</mtable>"
"</mfenced>"
"<mrow><mi>X</mi><mo>(</mo><mi>t</mi><mo>)</mo></mrow><mo>+</mo>"
"<mfenced open=""("" close="")"">"
"<mtable>"
"<mtr>"
"<mtd><mn>4.46</mn></mtd>"
"<mtd><mo>-</mo><mn>0.62</mn></mtd>"
"</mtr>"
"<mtr>"
"<mtd><mn>8.42</mn></mtd>"
"<mtd><mo>-</mo><mn>3.3</mn></mtd>"
"</mtr>"
"<mtr>"
"<mtd><mo>-</mo><mn>2.67</mn></mtd>"
"<mtd><mn>9.04</mn></mtd>"
"</mtr>"
"</mtable>"
"</mfenced>"
"<mrow><mi>U</mi><mo>(</mo><mi>t</mi><mo>)</mo></mrow></mtd></mtr>"
"<mtr><mtd><mrow><mi>Y</mi><mo>(</mo><mi>t</mi><mo>)</mo></mrow></mtd><mtd><mo>=</mo></mtd><mtd>"
"<mfenced open=""("" close="")"">"
"<mtable>"
"<mtr>"
"<mtd><mo>-</mo><mn>7.72</mn></mtd>"
"<mtd><mn>6.86</mn></mtd>"
"<mtd><mo>-</mo><mn>6.75</mn></mtd>"
"</mtr>"
"<mtr>"
"<mtd><mo>-</mo><mn>4.58</mn></mtd>"
"<mtd><mo>-</mo><mn>3.59</mn></mtd>"
"<mtd><mo>-</mo><mn>8.06</mn></mtd>"
"</mtr>"
"</mtable>"
"</mfenced>"
"<mrow><mi>X</mi><mo>(</mo><mi>t</mi><mo>)</mo></mrow><mo>+</mo>"
"<mfenced open=""("" close="")"">"
"<mtable>"
"<mtr>"
"<mtd><mn>1</mn><mo>+</mo><mi>s</mi></mtd>"
"<mtd><mn>1</mn><mo>-</mo><mi>s</mi></mtd>"
"</mtr>"
"<mtr>"
"<mtd><mo>-</mo><mi>s</mi></mtd>"
"<mtd><mi>s</mi></mtd>"
"</mtr>"
"</mtable>"
"</mfenced>"
"<mrow><mi>U</mi><mo>(</mo><mi>t</mi><mo>)</mo></mrow></mtd></mtr>"
"</mtable>"
"</mfenced>"
"</mtd>"
"</mtr>"
"</mtable>"
"</mfenced>"
""];
ref = strcat(ref, ascii(10));
assert_checkequal(prettyprint(s, "mathml"), ref);
