// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Sylvestre Ledru
// Copyright (C) 2019 - Samuel GOUGEON
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->

// syslin objects: tests in prettyprint_syslin.tst

TMP = strsubst(fileparts(TMPDIR), "\", "/");

// TEXTS
// =====
text = ["André''s got 50% of 1430 $, & the ""remainder"" 1 month later."
        "x=A\B is such that A*x=B, with A in {a<b, 1-a, ~a, ^a}. _#"];
// LaTeX
ref = "${\begin{pmatrix}\mathsf{\text{André''s got 50\% of 1430 \$, \& the ""remainder"" 1 month later.}}\cr \mathsf{\text{x=A\backslash\!B is such that A*x=B, with A in \{a\!<\!b, 1-a, \sim\!a, \^\;\,a\}. _#}}\cr \end{pmatrix}}$";
assert_checkequal(prettyprint(text), ref);

// TeX
ref = "${\pmatrix{\mathsf{\text{André''s got 50\% of 1430 \$, \& the ""remainder"" 1 month later.}}\cr \mathsf{\text{x=A\backslash\!B is such that A*x=B, with A in \{a\!<\!b, 1-a, \sim\!a, \^\;\,a\}. _#}}\cr }}$";
assert_checkequal(prettyprint(text, "tex"), ref);

// MathML
ref = [""
"<mfenced open=""("" close="")"">"
"<mtable>"
"<mtr>"
"<mtd><mi>André''s got 50% of 1430 $, & the ""remainder"" 1 month later.</mi></mtd>"
"</mtr>"
"<mtr>"
"<mtd><mi>x=A\B is such that A*x=B, with A in {a&lt;b, 1-a, ~a, ^a}. _#</mi></mtd>"
"</mtr>"
"</mtable>"
"</mfenced>"
"" ];
ref = strcat(ref, ascii(10));
assert_checkequal(prettyprint(text, "mathml"), ref);

// HTML > 4
ref = "£<table valign=""middle"" cellspacing=""0"" cellpadding=""3"" style=""padding: 5px 10px; background: url(file:///"+TMP+"openParen.png) left top, url(file:///"+TMP+"closeParen.png) right top; background-size: 5px 100%; background-repeat:no-repeat; display:inline-table;"">£<tr align=""center"">£<td>André''s got 50% of 1430 $, &amp; the ""remainder"" 1 month later.</td>£</tr>£<tr align=""center"">£<td>x=A\B is such that A*x=B, with A in {a&lt;b, 1-a, ~a, ^a}. _#</td>£</tr>£</table>£";
ref = strsubst(ref, "£", ascii(10));
assert_checkequal(prettyprint(text, "html"), ref);


// DECIMAL NUMBERS
// ===============
assert_checkequal(prettyprint(%inf), "${\infty}$");
assert_checkequal(prettyprint(%inf,"mathml"), "<mi>∞</mi>");
assert_checkequal(prettyprint(%inf,"html"), "∞");

ref = "${\begin{pmatrix}{\infty}&0&1&{-\infty}\cr \end{pmatrix}}$";
assert_checkequal(prettyprint([%inf 0  1 -%inf]), ref);
ref = "${\begin{pmatrix}{\infty}&0\cr 1&{-\infty}\cr \end{pmatrix}}$"
assert_checkequal(prettyprint([%inf 0; 1 -%inf]), ref);

B = [ ..
    50.153416    28.06498     40.948255 ; ..
    43.685876    12.800585    87.841258 ; ..
    26.931248    77.831286    11.383597 ; ..
    63.257449    21.190304    19.983377 ; ..
    40.51954     11.213547    56.186607 ; ..
    91.847078    68.56896     58.961773 ; ..
    4.3733433    15.312167    68.539797 ; ..
    48.185089    69.708506    89.062247 ; ..
    26.39556     84.155184    50.422128 ; ..
    41.481037    40.620248    34.936154 ];

// LaTeX
BResultLatex = "${\begin{pmatrix}50.153416&28.06498&40.948255\cr 43.685876&12.800585&87.841258\cr 26.931248&77.831286&11.383597\cr 63.257449&21.190304&19.983377\cr 40.51954&11.213547&56.186607\cr 91.847078&68.56896&58.961773\cr 4.3733433&15.312167&68.539797\cr 48.185089&69.708506&89.062247\cr 26.39556&84.155184&50.422128\cr 41.481037&40.620248&34.936154\cr \end{pmatrix}}$";
assert_checkequal(prettyprint(B), BResultLatex);

// TeX
BResultTex = "${\pmatrix{50.153416&28.06498&40.948255\cr 43.685876&12.800585&87.841258\cr 26.931248&77.831286&11.383597\cr 63.257449&21.190304&19.983377\cr 40.51954&11.213547&56.186607\cr 91.847078&68.56896&58.961773\cr 4.3733433&15.312167&68.539797\cr 48.185089&69.708506&89.062247\cr 26.39556&84.155184&50.422128\cr 41.481037&40.620248&34.936154\cr }}$";
assert_checkequal(prettyprint(B,"tex"), BResultTex);

// MathML
ref = [""
"<mfenced open=""("" close="")"">"
"<mtable>"
"<mtr>"
"<mtd><mn>50.153416</mn></mtd>"
"<mtd><mn>28.06498</mn></mtd>"
"<mtd><mn>40.948255</mn></mtd>"
"</mtr>"
"<mtr>"
"<mtd><mn>43.685876</mn></mtd>"
"<mtd><mn>12.800585</mn></mtd>"
"<mtd><mn>87.841258</mn></mtd>"
"</mtr>"
"<mtr>"
"<mtd><mn>26.931248</mn></mtd>"
"<mtd><mn>77.831286</mn></mtd>"
"<mtd><mn>11.383597</mn></mtd>"
"</mtr>"
"<mtr>"
"<mtd><mn>63.257449</mn></mtd>"
"<mtd><mn>21.190304</mn></mtd>"
"<mtd><mn>19.983377</mn></mtd>"
"</mtr>"
"<mtr>"
"<mtd><mn>40.51954</mn></mtd>"
"<mtd><mn>11.213547</mn></mtd>"
"<mtd><mn>56.186607</mn></mtd>"
"</mtr>"
"<mtr>"
"<mtd><mn>91.847078</mn></mtd>"
"<mtd><mn>68.56896</mn></mtd>"
"<mtd><mn>58.961773</mn></mtd>"
"</mtr>"
"<mtr>"
"<mtd><mn>4.3733433</mn></mtd>"
"<mtd><mn>15.312167</mn></mtd>"
"<mtd><mn>68.539797</mn></mtd>"
"</mtr>"
"<mtr>"
"<mtd><mn>48.185089</mn></mtd>"
"<mtd><mn>69.708506</mn></mtd>"
"<mtd><mn>89.062247</mn></mtd>"
"</mtr>"
"<mtr>"
"<mtd><mn>26.39556</mn></mtd>"
"<mtd><mn>84.155184</mn></mtd>"
"<mtd><mn>50.422128</mn></mtd>"
"</mtr>"
"<mtr>"
"<mtd><mn>41.481037</mn></mtd>"
"<mtd><mn>40.620248</mn></mtd>"
"<mtd><mn>34.936154</mn></mtd>"
"</mtr>"
"</mtable>"
"</mfenced>"
"" ];
ref = strcat(ref, ascii(10));
assert_checkequal(prettyprint(B,"mathml"), ref);


// COMPLEX NUMBERS
// ===============
c = complex(0,%nan);
assert_checkequal(prettyprint(c), "${\mathrm{NaN}}i$");
assert_checkequal(prettyprint(c,"mathml"), "<mi>NaN</mi><mi>i</mi>");
assert_checkequal(prettyprint(c,"html"), "NaNi");

//
c = [complex(1,%nan), complex(-1.25d-12, 2)
     complex(%inf,6.1e167), complex(1,-%inf)];
ref = "${\begin{pmatrix}1+{\mathrm{NaN}}i&-1.250\!\times\!10^{-12}+2i\cr {\infty}+6.10\!\times\!10^{167}i&1{-\infty}i\cr \end{pmatrix}}$";
assert_checkequal(prettyprint(c), ref);
ref = "${\pmatrix{1+{\mathrm{NaN}}i&-1.250\!\times\!10^{-12}+2i\cr {\infty}+6.10\!\times\!10^{167}i&1{-\infty}i\cr }}$";
assert_checkequal(prettyprint(c,"tex"), ref);

// HTML
ref = "£<table valign=""middle"" cellspacing=""0"" cellpadding=""3"" style=""padding: 5px 10px; background: url(file:///"+TMP+"openParen.png) left top, url(file:///"+TMP+"closeParen.png) right top; background-size: 5px 100%; background-repeat:no-repeat; display:inline-table;"">£<tr align=""center"">£<td>1 + NaNi</td>£<td>-1.250&#x00D7;10<sup>-12</sup> + 2i</td>£</tr>£<tr align=""center"">£<td>∞ + 6.10&#x00D7;10<sup>167</sup>i</td>£<td>1- ∞i</td>£</tr>£</table>£";
ref = strsubst(ref, "£", ascii(10));
assert_checkequal(prettyprint(c,"html"), ref);


// INTEGERS
// ========
i = int16([
  26595  1212  4257 -4466  9784
  -4226  3404  5743    3  10032
  30471     6 14918  267     30 ]);
// LaTeX
ref = "${\begin{pmatrix}26595&1212&4257&-4466&9784\cr -4226&3404&5743&3&10032\cr 30471&6&14918&267&30\cr \end{pmatrix}}$";
assert_checkequal(prettyprint(i), ref);
// TeX
ref = "${\pmatrix{26595&1212&4257&-4466&9784\cr -4226&3404&5743&3&10032\cr 30471&6&14918&267&30\cr }}$";
assert_checkequal(prettyprint(i, "tex"), ref);
// MathML
ref  = [""
"<mfenced open=""("" close="")"">"
"<mtable>"
"<mtr>"
"<mtd><mn>26595</mn></mtd>"
"<mtd><mn>1212</mn></mtd>"
"<mtd><mn>4257</mn></mtd>"
"<mtd><mo>-</mo><mn>4466</mn></mtd>"
"<mtd><mn>9784</mn></mtd>"
"</mtr>"
"<mtr>"
"<mtd><mo>-</mo><mn>4226</mn></mtd>"
"<mtd><mn>3404</mn></mtd>"
"<mtd><mn>5743</mn></mtd>"
"<mtd><mn>3</mn></mtd>"
"<mtd><mn>10032</mn></mtd>"
"</mtr>"
"<mtr>"
"<mtd><mn>30471</mn></mtd>"
"<mtd><mn>6</mn></mtd>"
"<mtd><mn>14918</mn></mtd>"
"<mtd><mn>267</mn></mtd>"
"<mtd><mn>30</mn></mtd>"
"</mtr>"
"</mtable>"
"</mfenced>"
""];
ref = strcat(ref, ascii(10));
assert_checkequal(prettyprint(i, "mathml"), ref);

// HTML
ref = "£<table valign=""middle"" cellspacing=""0"" cellpadding=""3"" style=""border-left:solid 1px; border-right:solid 1px; display:inline-table;"">£<tr align=""center"">£<td>26595</td>£<td>1212</td>£<td>4257</td>£<td>-4466</td>£<td>9784</td>£</tr>£<tr align=""center"">£<td>-4226</td>£<td>3404</td>£<td>5743</td>£<td>3</td>£<td>10032</td>£</tr>£<tr align=""center"">£<td>30471</td>£<td>6</td>£<td>14918</td>£<td>267</td>£<td>30</td>£</tr>£</table>£";
ref = strsubst(ref, "£", ascii(10));
assert_checkequal(prettyprint(i, "html", "|"), ref);


// BOOLEANS
// ========
C = [ %t %t; %f %t ];
// LaTeX
CResultLatex="${\begin{pmatrix}T&T\cr F&T\cr \end{pmatrix}}$";
assert_checkequal(prettyprint(C), CResultLatex);
// TeX
CResultTex="${\pmatrix{T&T\cr F&T\cr }}$";
assert_checkequal(prettyprint(C,"tex"), CResultTex);
// MathML
ref = [""
"<mfenced open=""("" close="")"">"
"<mtable>"
"<mtr>"
"<mtd><mi>T</mi></mtd>"
"<mtd><mi>T</mi></mtd>"
"</mtr>"
"<mtr>"
"<mtd><mi>F</mi></mtd>"
"<mtd><mi>T</mi></mtd>"
"</mtr>"
"</mtable>"
"</mfenced>"
""];
ref = strcat(ref, ascii(10));
assert_checkequal(prettyprint(C,"mathml"), ref);

// HTML
ref = "£<table valign=""middle"" cellspacing=""0"" cellpadding=""3"" style=""border-left:solid 1px; border-right:solid 1px; display:inline-table;"">£<tr align=""center"">£<td>T</td>£<td>T</td>£</tr>£<tr align=""center"">£<td>F</td>£<td>T</td>£</tr>£</table>£";
ref = strsubst(ref, "£", ascii(10));
assert_checkequal(prettyprint(C,"html","|"), ref);


// POLYNOMIALS
// ===========
assert_checkequal(prettyprint(0*%z), "$0z$");
assert_checkequal(prettyprint(0*%z, "tex"), "$0z$");
assert_checkequal(prettyprint(0*%z, "mathml"), "<mn>0</mn><mi>z</mi>");
assert_checkequal(prettyprint(0*%z, "html"), "0z");

p = 1+%s+2*%s^2;
assert_checkequal(prettyprint(p), "$1+s +2s^{2} $");
assert_checkequal(prettyprint(p, "html"), "1 + s + 2s<sup>2</sup> ");

D = poly(B,"s");
DResultLatex="$1.553\!\times\!10^{47}-1.738\!\times\!10^{47}s +8.994\!\times\!10^{46}s^{2} -2.885\!\times\!10^{46}s^{3} +6.474\!\times\!10^{45}s^{4} -1.086\!\times\!10^{45}s^{5} +1.422\!\times\!10^{44}s^{6} -1.493\!\times\!10^{43}s^{7} +1.284\!\times\!10^{42}s^{8} -9.182\!\times\!10^{40}s^{9} +5.527\!\times\!10^{39}s^{10} -2.824\!\times\!10^{38}s^{11} +1.233\!\times\!10^{37}s^{12} -4.626\!\times\!10^{35}s^{13} +1.496\!\times\!10^{34}s^{14} -4.182\!\times\!10^{32}s^{15} +1.011\!\times\!10^{31}s^{16} -2.116\!\times\!10^{29}s^{17} +3.826\!\times\!10^{27}s^{18} -5.965\!\times\!10^{25}s^{19} +7.990\!\times\!10^{23}s^{20} -9.144\!\times\!10^{21}s^{21} +8.876\!\times\!10^{19}s^{22} -7.234\!\times\!10^{17}s^{23} +4.885\!\times\!10^{15}s^{24} -2.683\!\times\!10^{13}s^{25} +1.167\!\times\!10^{11}s^{26} -3.868\!\times\!10^{08}s^{27} +916980.98s^{28} -1384.5606s^{29} +s^{30} $";
assert_checkequal(prettyprint(D,"latex","["), DResultLatex);

DResultTex="$1.553\!\times\!10^{47}-1.738\!\times\!10^{47}s +8.994\!\times\!10^{46}s^{2} -2.885\!\times\!10^{46}s^{3} +6.474\!\times\!10^{45}s^{4} -1.086\!\times\!10^{45}s^{5} +1.422\!\times\!10^{44}s^{6} -1.493\!\times\!10^{43}s^{7} +1.284\!\times\!10^{42}s^{8} -9.182\!\times\!10^{40}s^{9} +5.527\!\times\!10^{39}s^{10} -2.824\!\times\!10^{38}s^{11} +1.233\!\times\!10^{37}s^{12} -4.626\!\times\!10^{35}s^{13} +1.496\!\times\!10^{34}s^{14} -4.182\!\times\!10^{32}s^{15} +1.011\!\times\!10^{31}s^{16} -2.116\!\times\!10^{29}s^{17} +3.826\!\times\!10^{27}s^{18} -5.965\!\times\!10^{25}s^{19} +7.990\!\times\!10^{23}s^{20} -9.144\!\times\!10^{21}s^{21} +8.876\!\times\!10^{19}s^{22} -7.234\!\times\!10^{17}s^{23} +4.885\!\times\!10^{15}s^{24} -2.683\!\times\!10^{13}s^{25} +1.167\!\times\!10^{11}s^{26} -3.868\!\times\!10^{08}s^{27} +916980.98s^{28} -1384.5606s^{29} +s^{30} $";
assert_checkequal(prettyprint(D,"tex","["), DResultLatex)

DResultMathML="<mn>1.553</mn><mo>&#x00D7;</mo><msup><mn>10</mn><mrow><mo>+</mo><mn>47</mn></mrow></msup><mo>-</mo><mn>1.738</mn><mo>&#x00D7;</mo><msup><mn>10</mn><mrow><mo>+</mo><mn>47</mn></mrow></msup><mi>s</mi><mo>+</mo><mn>8.994</mn><mo>&#x00D7;</mo><msup><mn>10</mn><mrow><mo>+</mo><mn>46</mn></mrow></msup><msup><mi>s</mi><mn>2</mn></msup> <mo>-</mo><mn>2.885</mn><mo>&#x00D7;</mo><msup><mn>10</mn><mrow><mo>+</mo><mn>46</mn></mrow></msup><msup><mi>s</mi><mn>3</mn></msup> <mo>+</mo><mn>6.474</mn><mo>&#x00D7;</mo><msup><mn>10</mn><mrow><mo>+</mo><mn>45</mn></mrow></msup><msup><mi>s</mi><mn>4</mn></msup> <mo>-</mo><mn>1.086</mn><mo>&#x00D7;</mo><msup><mn>10</mn><mrow><mo>+</mo><mn>45</mn></mrow></msup><msup><mi>s</mi><mn>5</mn></msup> <mo>+</mo><mn>1.422</mn><mo>&#x00D7;</mo><msup><mn>10</mn><mrow><mo>+</mo><mn>44</mn></mrow></msup><msup><mi>s</mi><mn>6</mn></msup> <mo>-</mo><mn>1.493</mn><mo>&#x00D7;</mo><msup><mn>10</mn><mrow><mo>+</mo><mn>43</mn></mrow></msup><msup><mi>s</mi><mn>7</mn></msup> <mo>+</mo><mn>1.284</mn><mo>&#x00D7;</mo><msup><mn>10</mn><mrow><mo>+</mo><mn>42</mn></mrow></msup><msup><mi>s</mi><mn>8</mn></msup> <mo>-</mo><mn>9.182</mn><mo>&#x00D7;</mo><msup><mn>10</mn><mrow><mo>+</mo><mn>40</mn></mrow></msup><msup><mi>s</mi><mn>9</mn></msup> <mo>+</mo><mn>5.527</mn><mo>&#x00D7;</mo><msup><mn>10</mn><mrow><mo>+</mo><mn>39</mn></mrow></msup><msup><mi>s</mi><mn>10</mn></msup> <mo>-</mo><mn>2.824</mn><mo>&#x00D7;</mo><msup><mn>10</mn><mrow><mo>+</mo><mn>38</mn></mrow></msup><msup><mi>s</mi><mn>11</mn></msup> <mo>+</mo><mn>1.233</mn><mo>&#x00D7;</mo><msup><mn>10</mn><mrow><mo>+</mo><mn>37</mn></mrow></msup><msup><mi>s</mi><mn>12</mn></msup> <mo>-</mo><mn>4.626</mn><mo>&#x00D7;</mo><msup><mn>10</mn><mrow><mo>+</mo><mn>35</mn></mrow></msup><msup><mi>s</mi><mn>13</mn></msup> <mo>+</mo><mn>1.496</mn><mo>&#x00D7;</mo><msup><mn>10</mn><mrow><mo>+</mo><mn>34</mn></mrow></msup><msup><mi>s</mi><mn>14</mn></msup> <mo>-</mo><mn>4.182</mn><mo>&#x00D7;</mo><msup><mn>10</mn><mrow><mo>+</mo><mn>32</mn></mrow></msup><msup><mi>s</mi><mn>15</mn></msup> <mo>+</mo><mn>1.011</mn><mo>&#x00D7;</mo><msup><mn>10</mn><mrow><mo>+</mo><mn>31</mn></mrow></msup><msup><mi>s</mi><mn>16</mn></msup> <mo>-</mo><mn>2.116</mn><mo>&#x00D7;</mo><msup><mn>10</mn><mrow><mo>+</mo><mn>29</mn></mrow></msup><msup><mi>s</mi><mn>17</mn></msup> <mo>+</mo><mn>3.826</mn><mo>&#x00D7;</mo><msup><mn>10</mn><mrow><mo>+</mo><mn>27</mn></mrow></msup><msup><mi>s</mi><mn>18</mn></msup> <mo>-</mo><mn>5.965</mn><mo>&#x00D7;</mo><msup><mn>10</mn><mrow><mo>+</mo><mn>25</mn></mrow></msup><msup><mi>s</mi><mn>19</mn></msup> <mo>+</mo><mn>7.990</mn><mo>&#x00D7;</mo><msup><mn>10</mn><mrow><mo>+</mo><mn>23</mn></mrow></msup><msup><mi>s</mi><mn>20</mn></msup> <mo>-</mo><mn>9.144</mn><mo>&#x00D7;</mo><msup><mn>10</mn><mrow><mo>+</mo><mn>21</mn></mrow></msup><msup><mi>s</mi><mn>21</mn></msup> <mo>+</mo><mn>8.876</mn><mo>&#x00D7;</mo><msup><mn>10</mn><mrow><mo>+</mo><mn>19</mn></mrow></msup><msup><mi>s</mi><mn>22</mn></msup> <mo>-</mo><mn>7.234</mn><mo>&#x00D7;</mo><msup><mn>10</mn><mrow><mo>+</mo><mn>17</mn></mrow></msup><msup><mi>s</mi><mn>23</mn></msup> <mo>+</mo><mn>4.885</mn><mo>&#x00D7;</mo><msup><mn>10</mn><mrow><mo>+</mo><mn>15</mn></mrow></msup><msup><mi>s</mi><mn>24</mn></msup> <mo>-</mo><mn>2.683</mn><mo>&#x00D7;</mo><msup><mn>10</mn><mrow><mo>+</mo><mn>13</mn></mrow></msup><msup><mi>s</mi><mn>25</mn></msup> <mo>+</mo><mn>1.167</mn><mo>&#x00D7;</mo><msup><mn>10</mn><mrow><mo>+</mo><mn>11</mn></mrow></msup><msup><mi>s</mi><mn>26</mn></msup> <mo>-</mo><mn>3.868</mn><mo>&#x00D7;</mo><msup><mn>10</mn><mrow><mo>+</mo><mn>08</mn></mrow></msup><msup><mi>s</mi><mn>27</mn></msup> <mo>+</mo><mn>916980.98</mn><msup><mi>s</mi><mn>28</mn></msup> <mo>-</mo><mn>1384.5606</mn><msup><mi>s</mi><mn>29</mn></msup> <mo>+</mo><msup><mi>s</mi><mn>30</mn></msup> ";
assert_checkequal(prettyprint(D,"mathml","["), DResultMathML);

// With complex coefficients
// .........................
// 1)
p = (%i-%z).^[1 3;2 4];

// LaTeX
ref = "${\begin{pmatrix}i-z &-i+3z +3iz^{2} -z^{3} \cr -1-2iz +z^{2} &1+4iz -6z^{2} -4iz^{3} +z^{4} \cr \end{pmatrix}}$";
assert_checkequal(prettyprint(p), ref);

// TeX
ref = "${\pmatrix{i-z &-i+3z +3iz^{2} -z^{3} \cr -1-2iz +z^{2} &1+4iz -6z^{2} -4iz^{3} +z^{4} \cr }}$";
assert_checkequal(prettyprint(p, "tex"), ref);

// MathML
ref = [""
"<mfenced open=""("" close="")"">"
"<mtable>"
"<mtr>"
"<mtd><mi>i</mi><mo>-</mo><mi>z</mi></mtd>"
"<mtd><mo>-</mo><mi>i</mi><mo>+</mo><mn>3</mn><mi>z</mi><mo>+</mo><mn>3</mn><mi>i</mi><msup><mi>z</mi><mn>2</mn></msup> <mo>-</mo><msup><mi>z</mi><mn>3</mn></msup> </mtd>"
"</mtr>"
"<mtr>"
"<mtd><mo>-</mo><mn>1</mn><mo>-</mo><mn>2</mn><mi>i</mi><mi>z</mi><mo>+</mo><msup><mi>z</mi><mn>2</mn></msup> </mtd>"
"<mtd><mn>1</mn><mo>+</mo><mn>4</mn><mi>i</mi><mi>z</mi><mo>-</mo><mn>6</mn><msup><mi>z</mi><mn>2</mn></msup> <mo>-</mo><mn>4</mn><mi>i</mi><msup><mi>z</mi><mn>3</mn></msup> <mo>+</mo><msup><mi>z</mi><mn>4</mn></msup> </mtd>"
"</mtr>"
"</mtable>"
"</mfenced>"
""];
ref = strcat(ref, ascii(10));
assert_checkequal(prettyprint(p, "mathml"), ref);

// HTML > 4
ref = [""
"<table valign=""middle"" cellspacing=""0"" cellpadding=""3"" style=""border-left:solid 1px; border-right:solid 1px; display:inline-table;"">"
"<tr align=""center"">"
"<td>i - z</td>"
"<td> - i + 3z + 3iz<sup>2</sup>  - z<sup>3</sup> </td>"
"</tr>"
"<tr align=""center"">"
"<td>-1-2iz + z<sup>2</sup> </td>"
"<td>1 + 4iz-6z<sup>2</sup> -4iz<sup>3</sup>  + z<sup>4</sup> </td>"
"</tr>"
"</table>"
""];
ref = strcat(ref, ascii(10));
assert_checkequal(prettyprint(p,"html","|"), ref);

// 2) ---------
p = poly(complex([1 0 -2],[%nan 0 -1]),"x","coeff");
// LaTeX
assert_checkequal(prettyprint(p), "$1+{\mathrm{NaN}}i-(2+i)x^{2} $");
// TeX
assert_checkequal(prettyprint(p,"tex"), "$1+{\mathrm{NaN}}i-(2+i)x^{2} $");
// MathML
ref = "<mn>1</mn><mo>+</mo><mi>NaN</mi><mi>i</mi><mo>-</mo><mfenced separator=""""><mrow><mn>2</mn><mo>+</mo><mi>i</mi></mrow></mfenced><msup><mi>x</mi><mn>2</mn></msup> ";
assert_checkequal(prettyprint(p,"mathml"), ref);
// HTML
assert_checkequal(prettyprint(p,"html"), "1 + NaNi - (2 + i)x<sup>2</sup> ");


// CELLS
// =====
c = {["a bc";"defg"], %t ; (1-%s)^3, %pi};
// LaTeX
ref = "${\begin{pmatrix}{\begin{pmatrix}\mathsf{\text{a bc}}\cr \mathsf{\text{defg}}\cr \end{pmatrix}}&T\cr 1-3s +3s^{2} -s^{3} &3.1415927\cr \end{pmatrix}}$";
assert_checkequal(prettyprint(c), ref);

// TeX
ref = "${\pmatrix{{\pmatrix{\mathsf{\text{a bc}}\cr \mathsf{\text{defg}}\cr }}&T\cr 1-3s +3s^{2} -s^{3} &3.1415927\cr }}$";
assert_checkequal(prettyprint(c, "tex"), ref);

// MathML
ref = [""
"<mfenced open=""{"" close=""}"">"
"<mtable>"
"<mtr>"
"<mtd>"
"<mfenced open=""{"" close=""}"">"
"<mtable>"
"<mtr>"
"<mtd><mi>a bc</mi></mtd>"
"</mtr>"
"<mtr>"
"<mtd><mi>defg</mi></mtd>"
"</mtr>"
"</mtable>"
"</mfenced>"
"</mtd>"
"<mtd><mi>T</mi></mtd>"
"</mtr>"
"<mtr>"
"<mtd><mn>1</mn><mo>-</mo><mn>3</mn><mi>s</mi><mo>+</mo><mn>3</mn><msup><mi>s</mi><mn>2</mn></msup> <mo>-</mo><msup><mi>s</mi><mn>3</mn></msup> </mtd>"
"<mtd><mn>3.1415927</mn></mtd>"
"</mtr>"
"</mtable>"
"</mfenced>"
""];
ref = strcat(ref, ascii(10));
assert_checkequal(prettyprint(c, "mathml", "{"), ref);

// HTML
ref = "£<table valign=""middle"" cellspacing=""0"" cellpadding=""3"" style=""border-left:solid 1px; border-right:solid 1px; display:inline-table;"">£<tr align=""center"">£<td>£<table valign=""middle"" cellspacing=""0"" cellpadding=""3"" style=""border-left:solid 1px; border-right:solid 1px; display:inline-table;"">£<tr align=""center"">£<td>a bc</td>£</tr>£<tr align=""center"">£<td>defg</td>£</tr>£</table>£</td>£<td>T</td>£</tr>£<tr align=""center"">£<td>1-3s + 3s<sup>2</sup>  - s<sup>3</sup> </td>£<td>3.1415927</td>£</tr>£</table>£";
ref = strsubst(ref, "£", ascii(10));
assert_checkequal(prettyprint(c, "html", "|"), ref);

// HTML again
ref = "£<table valign=""middle"" cellspacing=""0"" cellpadding=""3"" style=""padding: 5px 10px; background: url(file:///"+TMP+"openParen.png) left top, url(file:///"+TMP+"closeParen.png) right top; background-size: 5px 100%; background-repeat:no-repeat; display:inline-table;"">£<tr align=""center"">£<td>£<table valign=""middle"" cellspacing=""0"" cellpadding=""3"" style=""padding: 5px 10px; background: url(file:///"+TMP+"openParen.png) left top, url(file:///"+TMP+"closeParen.png) right top; background-size: 5px 100%; background-repeat:no-repeat; display:inline-table;"">£<tr align=""center"">£<td>a bc</td>£</tr>£<tr align=""center"">£<td>defg</td>£</tr>£</table>£</td>£<td>T</td>£</tr>£<tr align=""center"">£<td>1-3s + 3s<sup>2</sup>  - s<sup>3</sup> </td>£<td>3.1415927</td>£</tr>£</table>£";
ref = strsubst(ref, "£", ascii(10));
assert_checkequal(prettyprint(c, "html"), ref);
