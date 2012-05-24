// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Sylvestre Ledru
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

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

BResultLatex = "${\begin{pmatrix}50.153416&28.06498&40.948255\cr 43.685876&12.800585&87.841258\cr 26.931248&77.831286&11.383597\cr 63.257449&21.190304&19.983377\cr 40.51954&11.213547&56.186607\cr 91.847078&68.56896&58.961773\cr 4.3733433&15.312167&68.539797\cr 48.185089&69.708506&89.062247\cr 26.39556&84.155184&50.422128\cr 41.481037&40.620248&34.936154\cr \end{pmatrix}}$";
if prettyprint(B) <> BResultLatex then pause, end

BResultTex = "${\pmatrix{50.153416&28.06498&40.948255\cr 43.685876&12.800585&87.841258\cr 26.931248&77.831286&11.383597\cr 63.257449&21.190304&19.983377\cr 40.51954&11.213547&56.186607\cr 91.847078&68.56896&58.961773\cr 4.3733433&15.312167&68.539797\cr 48.185089&69.708506&89.062247\cr 26.39556&84.155184&50.422128\cr 41.481037&40.620248&34.936154\cr }}$";
if prettyprint(B,"tex") <> BResultTex then pause, end

BResultMathML="<mfenced open=""("" close="")""><mtable><mtr><mtd><mn>50.153416</mn></mtd><mtd><mn>28.06498</mn></mtd><mtd><mn>40.948255</mn></mtd></mtr><mtr><mtd><mn>43.685876</mn></mtd><mtd><mn>12.800585</mn></mtd><mtd><mn>87.841258</mn></mtd></mtr><mtr><mtd><mn>26.931248</mn></mtd><mtd><mn>77.831286</mn></mtd><mtd><mn>11.383597</mn></mtd></mtr><mtr><mtd><mn>63.257449</mn></mtd><mtd><mn>21.190304</mn></mtd><mtd><mn>19.983377</mn></mtd></mtr><mtr><mtd><mn>40.51954</mn></mtd><mtd><mn>11.213547</mn></mtd><mtd><mn>56.186607</mn></mtd></mtr><mtr><mtd><mn>91.847078</mn></mtd><mtd><mn>68.56896</mn></mtd><mtd><mn>58.961773</mn></mtd></mtr><mtr><mtd><mn>4.3733433</mn></mtd><mtd><mn>15.312167</mn></mtd><mtd><mn>68.539797</mn></mtd></mtr><mtr><mtd><mn>48.185089</mn></mtd><mtd><mn>69.708506</mn></mtd><mtd><mn>89.062247</mn></mtd></mtr><mtr><mtd><mn>26.39556</mn></mtd><mtd><mn>84.155184</mn></mtd><mtd><mn>50.422128</mn></mtd></mtr><mtr><mtd><mn>41.481037</mn></mtd><mtd><mn>40.620248</mn></mtd><mtd><mn>34.936154</mn></mtd></mtr></mtable></mfenced>";
if prettyprint(B,"mathml") <> BResultMathML then pause, end


C = [ %t %t; %f %t ];

CResultLatex="${\begin{pmatrix}T&T\cr F&T\cr \end{pmatrix}}$";
if prettyprint(C) <> CResultLatex then pause, end

CResultTex="${\pmatrix{T&T\cr F&T\cr }}$";
if prettyprint(C,"tex") <> CResultTex then pause, end

CResultMathML="<mfenced open=""("" close="")""><mtable><mtr><mtd><mi>T</mi></mtd><mtd><mi>T</mi></mtd></mtr><mtr><mtd><mi>F</mi></mtd><mtd><mi>T</mi></mtd></mtr></mtable></mfenced>";
if prettyprint(C,"mathml") <> CResultMathML then pause, end

s=poly(0,"s");p=1+s+2*s^2;
D=poly(B,"s");

DResultLatex="$1.553\times10^{+47}-1.738\times10^{+47}s+8.994\times10^{+46}s^{2}-2.885\times10^{+46}s^{3}+6.474\times10^{+45}s^{4}-1.086\times10^{+45}s^{5}+1.422\times10^{+44}s^{6}-1.493\times10^{+43}s^{7}+1.284\times10^{+42}s^{8}-9.182\times10^{+40}s^{9}+5.527\times10^{+39}s^{10}-2.824\times10^{+38}s^{11}+1.233\times10^{+37}s^{12}-4.626\times10^{+35}s^{13}+1.496\times10^{+34}s^{14}-4.182\times10^{+32}s^{15}+1.011\times10^{+31}s^{16}-2.116\times10^{+29}s^{17}+3.826\times10^{+27}s^{18}-5.965\times10^{+25}s^{19}+7.990\times10^{+23}s^{20}-9.144\times10^{+21}s^{21}+8.876\times10^{+19}s^{22}-7.234\times10^{+17}s^{23}+4.885\times10^{+15}s^{24}-2.683\times10^{+13}s^{25}+1.167\times10^{+11}s^{26}-3.868\times10^{+08}s^{27}+916980.98s^{28}-1384.5606s^{29}+s^{30}$";
if prettyprint(D,"latex","[") <> DResultLatex then pause, end

DResultTex="$1.553\times10^{+47}-1.738\times10^{+47}s+8.994\times10^{+46}s^{2}-2.885\times10^{+46}s^{3}+6.474\times10^{+45}s^{4}-1.086\times10^{+45}s^{5}+1.422\times10^{+44}s^{6}-1.493\times10^{+43}s^{7}+1.284\times10^{+42}s^{8}-9.182\times10^{+40}s^{9}+5.527\times10^{+39}s^{10}-2.824\times10^{+38}s^{11}+1.233\times10^{+37}s^{12}-4.626\times10^{+35}s^{13}+1.496\times10^{+34}s^{14}-4.182\times10^{+32}s^{15}+1.011\times10^{+31}s^{16}-2.116\times10^{+29}s^{17}+3.826\times10^{+27}s^{18}-5.965\times10^{+25}s^{19}+7.990\times10^{+23}s^{20}-9.144\times10^{+21}s^{21}+8.876\times10^{+19}s^{22}-7.234\times10^{+17}s^{23}+4.885\times10^{+15}s^{24}-2.683\times10^{+13}s^{25}+1.167\times10^{+11}s^{26}-3.868\times10^{+08}s^{27}+916980.98s^{28}-1384.5606s^{29}+s^{30}$";
if prettyprint(D,"tex","[") <> DResultLatex then pause, end

DResultMathML="<mn>1.553</mn><mo>&#x00D7;</mo><msup><mn>10</mn><mrow><mo>+</mo><mn>47</mn></mrow></msup><mo>-</mo><mn>1.738</mn><mo>&#x00D7;</mo><msup><mn>10</mn><mrow><mo>+</mo><mn>47</mn></mrow></msup><mi>s</mi><mo>+</mo><mn>8.994</mn><mo>&#x00D7;</mo><msup><mn>10</mn><mrow><mo>+</mo><mn>46</mn></mrow></msup><msup><mi>s</mi><mn>2</mn></msup><mo>-</mo><mn>2.885</mn><mo>&#x00D7;</mo><msup><mn>10</mn><mrow><mo>+</mo><mn>46</mn></mrow></msup><msup><mi>s</mi><mn>3</mn></msup><mo>+</mo><mn>6.474</mn><mo>&#x00D7;</mo><msup><mn>10</mn><mrow><mo>+</mo><mn>45</mn></mrow></msup><msup><mi>s</mi><mn>4</mn></msup><mo>-</mo><mn>1.086</mn><mo>&#x00D7;</mo><msup><mn>10</mn><mrow><mo>+</mo><mn>45</mn></mrow></msup><msup><mi>s</mi><mn>5</mn></msup><mo>+</mo><mn>1.422</mn><mo>&#x00D7;</mo><msup><mn>10</mn><mrow><mo>+</mo><mn>44</mn></mrow></msup><msup><mi>s</mi><mn>6</mn></msup><mo>-</mo><mn>1.493</mn><mo>&#x00D7;</mo><msup><mn>10</mn><mrow><mo>+</mo><mn>43</mn></mrow></msup><msup><mi>s</mi><mn>7</mn></msup><mo>+</mo><mn>1.284</mn><mo>&#x00D7;</mo><msup><mn>10</mn><mrow><mo>+</mo><mn>42</mn></mrow></msup><msup><mi>s</mi><mn>8</mn></msup><mo>-</mo><mn>9.182</mn><mo>&#x00D7;</mo><msup><mn>10</mn><mrow><mo>+</mo><mn>40</mn></mrow></msup><msup><mi>s</mi><mn>9</mn></msup><mo>+</mo><mn>5.527</mn><mo>&#x00D7;</mo><msup><mn>10</mn><mrow><mo>+</mo><mn>39</mn></mrow></msup><msup><mi>s</mi><mn>10</mn></msup><mo>-</mo><mn>2.824</mn><mo>&#x00D7;</mo><msup><mn>10</mn><mrow><mo>+</mo><mn>38</mn></mrow></msup><msup><mi>s</mi><mn>11</mn></msup><mo>+</mo><mn>1.233</mn><mo>&#x00D7;</mo><msup><mn>10</mn><mrow><mo>+</mo><mn>37</mn></mrow></msup><msup><mi>s</mi><mn>12</mn></msup><mo>-</mo><mn>4.626</mn><mo>&#x00D7;</mo><msup><mn>10</mn><mrow><mo>+</mo><mn>35</mn></mrow></msup><msup><mi>s</mi><mn>13</mn></msup><mo>+</mo><mn>1.496</mn><mo>&#x00D7;</mo><msup><mn>10</mn><mrow><mo>+</mo><mn>34</mn></mrow></msup><msup><mi>s</mi><mn>14</mn></msup><mo>-</mo><mn>4.182</mn><mo>&#x00D7;</mo><msup><mn>10</mn><mrow><mo>+</mo><mn>32</mn></mrow></msup><msup><mi>s</mi><mn>15</mn></msup><mo>+</mo><mn>1.011</mn><mo>&#x00D7;</mo><msup><mn>10</mn><mrow><mo>+</mo><mn>31</mn></mrow></msup><msup><mi>s</mi><mn>16</mn></msup><mo>-</mo><mn>2.116</mn><mo>&#x00D7;</mo><msup><mn>10</mn><mrow><mo>+</mo><mn>29</mn></mrow></msup><msup><mi>s</mi><mn>17</mn></msup><mo>+</mo><mn>3.826</mn><mo>&#x00D7;</mo><msup><mn>10</mn><mrow><mo>+</mo><mn>27</mn></mrow></msup><msup><mi>s</mi><mn>18</mn></msup><mo>-</mo><mn>5.965</mn><mo>&#x00D7;</mo><msup><mn>10</mn><mrow><mo>+</mo><mn>25</mn></mrow></msup><msup><mi>s</mi><mn>19</mn></msup><mo>+</mo><mn>7.990</mn><mo>&#x00D7;</mo><msup><mn>10</mn><mrow><mo>+</mo><mn>23</mn></mrow></msup><msup><mi>s</mi><mn>20</mn></msup><mo>-</mo><mn>9.144</mn><mo>&#x00D7;</mo><msup><mn>10</mn><mrow><mo>+</mo><mn>21</mn></mrow></msup><msup><mi>s</mi><mn>21</mn></msup><mo>+</mo><mn>8.876</mn><mo>&#x00D7;</mo><msup><mn>10</mn><mrow><mo>+</mo><mn>19</mn></mrow></msup><msup><mi>s</mi><mn>22</mn></msup><mo>-</mo><mn>7.234</mn><mo>&#x00D7;</mo><msup><mn>10</mn><mrow><mo>+</mo><mn>17</mn></mrow></msup><msup><mi>s</mi><mn>23</mn></msup><mo>+</mo><mn>4.885</mn><mo>&#x00D7;</mo><msup><mn>10</mn><mrow><mo>+</mo><mn>15</mn></mrow></msup><msup><mi>s</mi><mn>24</mn></msup><mo>-</mo><mn>2.683</mn><mo>&#x00D7;</mo><msup><mn>10</mn><mrow><mo>+</mo><mn>13</mn></mrow></msup><msup><mi>s</mi><mn>25</mn></msup><mo>+</mo><mn>1.167</mn><mo>&#x00D7;</mo><msup><mn>10</mn><mrow><mo>+</mo><mn>11</mn></mrow></msup><msup><mi>s</mi><mn>26</mn></msup><mo>-</mo><mn>3.868</mn><mo>&#x00D7;</mo><msup><mn>10</mn><mrow><mo>+</mo><mn>08</mn></mrow></msup><msup><mi>s</mi><mn>27</mn></msup><mo>+</mo><mn>916980.98</mn><msup><mi>s</mi><mn>28</mn></msup><mo>-</mo><mn>1384.5606</mn><msup><mi>s</mi><mn>29</mn></msup><mo>+</mo><msup><mi>s</mi><mn>30</mn></msup>";
if prettyprint(D,"mathml","[") <> DResultMathML then pause, end

if prettyprint(%inf) <> "${\infty}$" then pause, end
if prettyprint([%inf 0  1 -%inf]) <> "${\begin{pmatrix}{\infty}&0&1&{-\infty}\cr \end{pmatrix}}$" then pause, end
if prettyprint([%inf 0; 1 -%inf]) <> "${\begin{pmatrix}{\infty}&0\cr"+...
      " 1&{-\infty}\cr \end{pmatrix}}$" then pause, end
if prettyprint(%inf,"mathml") <> "<infinity/>" then pause, end