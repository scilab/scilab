// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function r=rgb2name(r,g,b)

    [lhs,rhs]=argn(0)
    if rhs==1 then
        if (type(r)<>1 & size(r,"*")<>3) then
            error(msprintf(gettext("%s: Wrong type for input argument #%d: A vector of 3 integers expected.\n"),"rgb2name",1));
        end
        b=r(3); g=r(2); r=r(1);
    elseif rhs==3 then
        if (type(r)<>1 | type(g)<>1 | type(b)<>1 | size(r,"*")<>1 | size(g,"*")<>1 | size(b,"*")<>1) then
            error(msprintf(gettext("%s: Wrong type for input arguments #%d, #%d and #%d: Scalars expected.\n"),"rgb2name", 1, 2, 3));
        end
    else
        error(msprintf(gettext("%s: Wrong number of input arguments: %d or %d expected.\n"),"rgb2name",1,3));
    end
    if (r>255 | r<0) then
        error(msprintf(gettext("%s: Wrong value for input argument #%d: must be in the interval [%d, %d].\n"),"rgb2name",1,0,255));
    end
    if (g>255 | g<0) then
        error(msprintf(gettext("%s: Wrong value for input argument #%d: must be in the interval [%d, %d].\n"),"rgb2name",2,0,255));
    end
    if (b>255 | b<0) then
        error(msprintf(gettext("%s: Wrong value for input argument #%d: must be in the interval [%d, %d].\n"),"rgb2name",3,0,255));
    end

    names=[
    "scilab blue4"
    "scilabblue4"
    "scilab blue3"
    "scilabblue3"
    "scilab blue2"
    "scilabblue2"
    "scilab green4"
    "scilabgreen4"
    "scilab green3"
    "scilabgreen3"
    "scilab green2"
    "scilabgreen2"
    "scilab cyan4"
    "scilabcyan4"
    "scilab cyan3"
    "scilabcyan3"
    "scilab cyan2"
    "scilabcyan2"
    "scilab red4"
    "scilabred4"
    "scilab red3"
    "scilabred3"
    "scilab red2"
    "scilabred2"
    "scilab magenta4"
    "scilabmagenta4"
    "scilab magenta3"
    "scilabmagenta3"
    "scilab magenta2"
    "scilabmagenta2"
    "scilab brown4"
    "scilabbrown4"
    "scilab brown3"
    "scilabbrown3"
    "scilab brown2"
    "scilabbrown2"
    "scilab pink4"
    "scilabpink4"
    "scilab pink3"
    "scilabpink3"
    "scilab pink2"
    "scilabpink2"
    "scilab pink"
    "scilabpink"
    "snow"
    "ghost white"
    "ghostwhite"
    "white smoke"
    "whitesmoke"
    "gainsboro"
    "floral white"
    "floralwhite"
    "old lace"
    "oldlace"
    "linen"
    "antique white"
    "antiquewhite"
    "papaya whip"
    "papayawhip"
    "blanched almond"
    "blanchedalmond"
    "bisque"
    "peach puff"
    "peachpuff"
    "navajo white"
    "navajowhite"
    "moccasin"
    "cornsilk"
    "ivory"
    "lemon chiffon"
    "lemonchiffon"
    "seashell"
    "honeydew"
    "mint cream"
    "mintcream"
    "azure"
    "alice blue"
    "aliceblue"
    "lavender"
    "lavender blush"
    "lavenderblush"
    "misty rose"
    "mistyrose"
    "white"
    "black"
    "dark slate gray"
    "darkslategray"
    "dark slate grey"
    "darkslategrey"
    "dim gray"
    "dimgray"
    "dim grey"
    "dimgrey"
    "slate gray"
    "slategray"
    "slate grey"
    "slategrey"
    "light slate gray"
    "lightslategray"
    "light slate grey"
    "lightslategrey"
    "gray"
    "grey"
    "light grey"
    "lightgrey"
    "light gray"
    "lightgray"
    "midnight blue"
    "midnightblue"
    "navy"
    "navy blue"
    "navyblue"
    "cornflower blue"
    "cornflowerblue"
    "dark slate blue"
    "darkslateblue"
    "slate blue"
    "slateblue"
    "medium slate blue"
    "mediumslateblue"
    "light slate blue"
    "lightslateblue"
    "medium blue"
    "mediumblue"
    "royal blue"
    "royalblue"
    "blue"
    "dodger blue"
    "dodgerblue"
    "deep sky blue"
    "deepskyblue"
    "sky blue"
    "skyblue"
    "light sky blue"
    "lightskyblue"
    "steel blue"
    "steelblue"
    "light steel blue"
    "lightsteelblue"
    "light blue"
    "lightblue"
    "powder blue"
    "powderblue"
    "pale turquoise"
    "paleturquoise"
    "dark turquoise"
    "darkturquoise"
    "medium turquoise"
    "mediumturquoise"
    "turquoise"
    "cyan"
    "light cyan"
    "lightcyan"
    "cadet blue"
    "cadetblue"
    "medium aquamarine"
    "mediumaquamarine"
    "aquamarine"
    "dark green"
    "darkgreen"
    "dark olive green"
    "darkolivegreen"
    "dark sea green"
    "darkseagreen"
    "sea green"
    "seagreen"
    "medium sea green"
    "mediumseagreen"
    "light sea green"
    "lightseagreen"
    "pale green"
    "palegreen"
    "spring green"
    "springgreen"
    "lawn green"
    "lawngreen"
    "green"
    "chartreuse"
    "medium spring green"
    "mediumspringgreen"
    "green yellow"
    "greenyellow"
    "lime green"
    "limegreen"
    "yellow green"
    "yellowgreen"
    "forest green"
    "forestgreen"
    "olive drab"
    "olivedrab"
    "dark khaki"
    "darkkhaki"
    "khaki"
    "pale goldenrod"
    "palegoldenrod"
    "light goldenrod yellow"
    "lightgoldenrodyellow"
    "light yellow"
    "lightyellow"
    "yellow"
    "gold"
    "light goldenrod"
    "lightgoldenrod"
    "goldenrod"
    "dark goldenrod"
    "darkgoldenrod"
    "rosy brown"
    "rosybrown"
    "indian red"
    "indianred"
    "saddle brown"
    "saddlebrown"
    "sienna"
    "peru"
    "burlywood"
    "beige"
    "wheat"
    "sandy brown"
    "sandybrown"
    "tan"
    "chocolate"
    "firebrick"
    "brown"
    "dark salmon"
    "darksalmon"
    "salmon"
    "light salmon"
    "lightsalmon"
    "orange"
    "dark orange"
    "darkorange"
    "coral"
    "light coral"
    "lightcoral"
    "tomato"
    "orange red"
    "orangered"
    "red"
    "hot pink"
    "hotpink"
    "deep pink"
    "deeppink"
    "pink"
    "light pink"
    "lightpink"
    "pale violet red"
    "palevioletred"
    "maroon"
    "medium violet red"
    "mediumvioletred"
    "violet red"
    "violetred"
    "magenta"
    "violet"
    "plum"
    "orchid"
    "medium orchid"
    "mediumorchid"
    "dark orchid"
    "darkorchid"
    "dark violet"
    "darkviolet"
    "blue violet"
    "blueviolet"
    "purple"
    "medium purple"
    "mediumpurple"
    "thistle"
    "snow1"
    "snow2"
    "snow3"
    "snow4"
    "seashell1"
    "seashell2"
    "seashell3"
    "seashell4"
    "antiquewhite1"
    "antiquewhite2"
    "antiquewhite3"
    "antiquewhite4"
    "bisque1"
    "bisque2"
    "bisque3"
    "bisque4"
    "peachpuff1"
    "peachpuff2"
    "peachpuff3"
    "peachpuff4"
    "navajowhite1"
    "navajowhite2"
    "navajowhite3"
    "navajowhite4"
    "lemonchiffon1"
    "lemonchiffon2"
    "lemonchiffon3"
    "lemonchiffon4"
    "cornsilk1"
    "cornsilk2"
    "cornsilk3"
    "cornsilk4"
    "ivory1"
    "ivory2"
    "ivory3"
    "ivory4"
    "honeydew1"
    "honeydew2"
    "honeydew3"
    "honeydew4"
    "lavenderblush1"
    "lavenderblush2"
    "lavenderblush3"
    "lavenderblush4"
    "mistyrose1"
    "mistyrose2"
    "mistyrose3"
    "mistyrose4"
    "azure1"
    "azure2"
    "azure3"
    "azure4"
    "slateblue1"
    "slateblue2"
    "slateblue3"
    "slateblue4"
    "royalblue1"
    "royalblue2"
    "royalblue3"
    "royalblue4"
    "blue1"
    "blue2"
    "blue3"
    "blue4"
    "dodgerblue1"
    "dodgerblue2"
    "dodgerblue3"
    "dodgerblue4"
    "steelblue1"
    "steelblue2"
    "steelblue3"
    "steelblue4"
    "deepskyblue1"
    "deepskyblue2"
    "deepskyblue3"
    "deepskyblue4"
    "skyblue1"
    "skyblue2"
    "skyblue3"
    "skyblue4"
    "lightskyblue1"
    "lightskyblue2"
    "lightskyblue3"
    "lightskyblue4"
    "slategray1"
    "slategray2"
    "slategray3"
    "slategray4"
    "lightsteelblue1"
    "lightsteelblue2"
    "lightsteelblue3"
    "lightsteelblue4"
    "lightblue1"
    "lightblue2"
    "lightblue3"
    "lightblue4"
    "lightcyan1"
    "lightcyan2"
    "lightcyan3"
    "lightcyan4"
    "paleturquoise1"
    "paleturquoise2"
    "paleturquoise3"
    "paleturquoise4"
    "cadetblue1"
    "cadetblue2"
    "cadetblue3"
    "cadetblue4"
    "turquoise1"
    "turquoise2"
    "turquoise3"
    "turquoise4"
    "cyan1"
    "cyan2"
    "cyan3"
    "cyan4"
    "darkslategray1"
    "darkslategray2"
    "darkslategray3"
    "darkslategray4"
    "aquamarine1"
    "aquamarine2"
    "aquamarine3"
    "aquamarine4"
    "darkseagreen1"
    "darkseagreen2"
    "darkseagreen3"
    "darkseagreen4"
    "seagreen1"
    "seagreen2"
    "seagreen3"
    "seagreen4"
    "palegreen1"
    "palegreen2"
    "palegreen3"
    "palegreen4"
    "springgreen1"
    "springgreen2"
    "springgreen3"
    "springgreen4"
    "green1"
    "green2"
    "green3"
    "green4"
    "chartreuse1"
    "chartreuse2"
    "chartreuse3"
    "chartreuse4"
    "olivedrab1"
    "olivedrab2"
    "olivedrab3"
    "olivedrab4"
    "darkolivegreen1"
    "darkolivegreen2"
    "darkolivegreen3"
    "darkolivegreen4"
    "khaki1"
    "khaki2"
    "khaki3"
    "khaki4"
    "lightgoldenrod1"
    "lightgoldenrod2"
    "lightgoldenrod3"
    "lightgoldenrod4"
    "lightyellow1"
    "lightyellow2"
    "lightyellow3"
    "lightyellow4"
    "yellow1"
    "yellow2"
    "yellow3"
    "yellow4"
    "gold1"
    "gold2"
    "gold3"
    "gold4"
    "goldenrod1"
    "goldenrod2"
    "goldenrod3"
    "goldenrod4"
    "darkgoldenrod1"
    "darkgoldenrod2"
    "darkgoldenrod3"
    "darkgoldenrod4"
    "rosybrown1"
    "rosybrown2"
    "rosybrown3"
    "rosybrown4"
    "indianred1"
    "indianred2"
    "indianred3"
    "indianred4"
    "sienna1"
    "sienna2"
    "sienna3"
    "sienna4"
    "burlywood1"
    "burlywood2"
    "burlywood3"
    "burlywood4"
    "wheat1"
    "wheat2"
    "wheat3"
    "wheat4"
    "tan1"
    "tan2"
    "tan3"
    "tan4"
    "chocolate1"
    "chocolate2"
    "chocolate3"
    "chocolate4"
    "firebrick1"
    "firebrick2"
    "firebrick3"
    "firebrick4"
    "brown1"
    "brown2"
    "brown3"
    "brown4"
    "salmon1"
    "salmon2"
    "salmon3"
    "salmon4"
    "lightsalmon1"
    "lightsalmon2"
    "lightsalmon3"
    "lightsalmon4"
    "orange1"
    "orange2"
    "orange3"
    "orange4"
    "darkorange1"
    "darkorange2"
    "darkorange3"
    "darkorange4"
    "coral1"
    "coral2"
    "coral3"
    "coral4"
    "tomato1"
    "tomato2"
    "tomato3"
    "tomato4"
    "orangered1"
    "orangered2"
    "orangered3"
    "orangered4"
    "red1"
    "red2"
    "red3"
    "red4"
    "deeppink1"
    "deeppink2"
    "deeppink3"
    "deeppink4"
    "hotpink1"
    "hotpink2"
    "hotpink3"
    "hotpink4"
    "pink1"
    "pink2"
    "pink3"
    "pink4"
    "lightpink1"
    "lightpink2"
    "lightpink3"
    "lightpink4"
    "palevioletred1"
    "palevioletred2"
    "palevioletred3"
    "palevioletred4"
    "maroon1"
    "maroon2"
    "maroon3"
    "maroon4"
    "violetred1"
    "violetred2"
    "violetred3"
    "violetred4"
    "magenta1"
    "magenta2"
    "magenta3"
    "magenta4"
    "orchid1"
    "orchid2"
    "orchid3"
    "orchid4"
    "plum1"
    "plum2"
    "plum3"
    "plum4"
    "mediumorchid1"
    "mediumorchid2"
    "mediumorchid3"
    "mediumorchid4"
    "darkorchid1"
    "darkorchid2"
    "darkorchid3"
    "darkorchid4"
    "purple1"
    "purple2"
    "purple3"
    "purple4"
    "mediumpurple1"
    "mediumpurple2"
    "mediumpurple3"
    "mediumpurple4"
    "thistle1"
    "thistle2"
    "thistle3"
    "thistle4"
    "gray0"
    "grey0"
    "gray1"
    "grey1"
    "gray2"
    "grey2"
    "gray3"
    "grey3"
    "gray4"
    "grey4"
    "gray5"
    "grey5"
    "gray6"
    "grey6"
    "gray7"
    "grey7"
    "gray8"
    "grey8"
    "gray9"
    "grey9"
    "gray10"
    "grey10"
    "gray11"
    "grey11"
    "gray12"
    "grey12"
    "gray13"
    "grey13"
    "gray14"
    "grey14"
    "gray15"
    "grey15"
    "gray16"
    "grey16"
    "gray17"
    "grey17"
    "gray18"
    "grey18"
    "gray19"
    "grey19"
    "gray20"
    "grey20"
    "gray21"
    "grey21"
    "gray22"
    "grey22"
    "gray23"
    "grey23"
    "gray24"
    "grey24"
    "gray25"
    "grey25"
    "gray26"
    "grey26"
    "gray27"
    "grey27"
    "gray28"
    "grey28"
    "gray29"
    "grey29"
    "gray30"
    "grey30"
    "gray31"
    "grey31"
    "gray32"
    "grey32"
    "gray33"
    "grey33"
    "gray34"
    "grey34"
    "gray35"
    "grey35"
    "gray36"
    "grey36"
    "gray37"
    "grey37"
    "gray38"
    "grey38"
    "gray39"
    "grey39"
    "gray40"
    "grey40"
    "gray41"
    "grey41"
    "gray42"
    "grey42"
    "gray43"
    "grey43"
    "gray44"
    "grey44"
    "gray45"
    "grey45"
    "gray46"
    "grey46"
    "gray47"
    "grey47"
    "gray48"
    "grey48"
    "gray49"
    "grey49"
    "gray50"
    "grey50"
    "gray51"
    "grey51"
    "gray52"
    "grey52"
    "gray53"
    "grey53"
    "gray54"
    "grey54"
    "gray55"
    "grey55"
    "gray56"
    "grey56"
    "gray57"
    "grey57"
    "gray58"
    "grey58"
    "gray59"
    "grey59"
    "gray60"
    "grey60"
    "gray61"
    "grey61"
    "gray62"
    "grey62"
    "gray63"
    "grey63"
    "gray64"
    "grey64"
    "gray65"
    "grey65"
    "gray66"
    "grey66"
    "gray67"
    "grey67"
    "gray68"
    "grey68"
    "gray69"
    "grey69"
    "gray70"
    "grey70"
    "gray71"
    "grey71"
    "gray72"
    "grey72"
    "gray73"
    "grey73"
    "gray74"
    "grey74"
    "gray75"
    "grey75"
    "gray76"
    "grey76"
    "gray77"
    "grey77"
    "gray78"
    "grey78"
    "gray79"
    "grey79"
    "gray80"
    "grey80"
    "gray81"
    "grey81"
    "gray82"
    "grey82"
    "gray83"
    "grey83"
    "gray84"
    "grey84"
    "gray85"
    "grey85"
    "gray86"
    "grey86"
    "gray87"
    "grey87"
    "gray88"
    "grey88"
    "gray89"
    "grey89"
    "gray90"
    "grey90"
    "gray91"
    "grey91"
    "gray92"
    "grey92"
    "gray93"
    "grey93"
    "gray94"
    "grey94"
    "gray95"
    "grey95"
    "gray96"
    "grey96"
    "gray97"
    "grey97"
    "gray98"
    "grey98"
    "gray99"
    "grey99"
    "gray100"
    "grey100"
    "dark grey"
    "darkgrey"
    "dark gray"
    "darkgray"
    "dark blue"
    "darkblue"
    "dark cyan"
    "darkcyan"
    "dark magenta"
    "darkmagenta"
    "dark red"
    "darkred"
    "light green"
    "lightgreen"
    ];

    rgb=[
    0   0 144
    0   0 144
    0   0 176
    0   0 176
    0   0 208
    0   0 208
    0 144   0
    0 144   0
    0 176   0
    0 176   0
    0 208   0
    0 208   0
    0 144 144
    0 144 144
    0 176 176
    0 176 176
    0 208 208
    0 208 208
    144   0   0
    144   0   0
    176   0   0
    176   0   0
    208   0   0
    208   0   0
    144   0 144
    144   0 144
    176   0 176
    176   0 176
    208   0 208
    208   0 208
    128  48   0
    128  48   0
    160  64   0
    160  64   0
    192  96   0
    192  96   0
    255 128 128
    255 128 128
    255 160 160
    255 160 160
    255 192 192
    255 192 192
    255 224 224
    255 224 224
    255 250 250
    248 248 255
    248 248 255
    245 245 245
    245 245 245
    220 220 220
    255 250 240
    255 250 240
    253 245 230
    253 245 230
    250 240 230
    250 235 215
    250 235 215
    255 239 213
    255 239 213
    255 235 205
    255 235 205
    255 228 196
    255 218 185
    255 218 185
    255 222 173
    255 222 173
    255 228 181
    255 248 220
    255 255 240
    255 250 205
    255 250 205
    255 245 238
    240 255 240
    245 255 250
    245 255 250
    240 255 255
    240 248 255
    240 248 255
    230 230 250
    255 240 245
    255 240 245
    255 228 225
    255 228 225
    255 255 255
    0   0   0
    47  79  79
    47  79  79
    47  79  79
    47  79  79
    105 105 105
    105 105 105
    105 105 105
    105 105 105
    112 128 144
    112 128 144
    112 128 144
    112 128 144
    119 136 153
    119 136 153
    119 136 153
    119 136 153
    190 190 190
    190 190 190
    211 211 211
    211 211 211
    211 211 211
    211 211 211
    25  25 112
    25  25 112
    0   0 128
    0   0 128
    0   0 128
    100 149 237
    100 149 237
    72  61 139
    72  61 139
    106  90 205
    106  90 205
    123 104 238
    123 104 238
    132 112 255
    132 112 255
    0   0 205
    0   0 205
    65 105 225
    65 105 225
    0   0 255
    30 144 255
    30 144 255
    0 191 255
    0 191 255
    135 206 235
    135 206 235
    135 206 250
    135 206 250
    70 130 180
    70 130 180
    176 196 222
    176 196 222
    173 216 230
    173 216 230
    176 224 230
    176 224 230
    175 238 238
    175 238 238
    0 206 209
    0 206 209
    72 209 204
    72 209 204
    64 224 208
    0 255 255
    224 255 255
    224 255 255
    95 158 160
    95 158 160
    102 205 170
    102 205 170
    127 255 212
    0 100   0
    0 100   0
    85 107  47
    85 107  47
    143 188 143
    143 188 143
    46 139  87
    46 139  87
    60 179 113
    60 179 113
    32 178 170
    32 178 170
    152 251 152
    152 251 152
    0 255 127
    0 255 127
    124 252   0
    124 252   0
    0 255   0
    127 255   0
    0 250 154
    0 250 154
    173 255  47
    173 255  47
    50 205  50
    50 205  50
    154 205  50
    154 205  50
    34 139  34
    34 139  34
    107 142  35
    107 142  35
    189 183 107
    189 183 107
    240 230 140
    238 232 170
    238 232 170
    250 250 210
    250 250 210
    255 255 224
    255 255 224
    255 255   0
    255 215   0
    238 221 130
    238 221 130
    218 165  32
    184 134  11
    184 134  11
    188 143 143
    188 143 143
    205  92  92
    205  92  92
    139  69  19
    139  69  19
    160  82  45
    205 133  63
    222 184 135
    245 245 220
    245 222 179
    244 164  96
    244 164  96
    210 180 140
    210 105  30
    178  34  34
    165  42  42
    233 150 122
    233 150 122
    250 128 114
    255 160 122
    255 160 122
    255 165   0
    255 140   0
    255 140   0
    255 127  80
    240 128 128
    240 128 128
    255  99  71
    255  69   0
    255  69   0
    255   0   0
    255 105 180
    255 105 180
    255  20 147
    255  20 147
    255 192 203
    255 182 193
    255 182 193
    219 112 147
    219 112 147
    176  48  96
    199  21 133
    199  21 133
    208  32 144
    208  32 144
    255   0 255
    238 130 238
    221 160 221
    218 112 214
    186  85 211
    186  85 211
    153  50 204
    153  50 204
    148   0 211
    148   0 211
    138  43 226
    138  43 226
    160  32 240
    147 112 219
    147 112 219
    216 191 216
    255 250 250
    238 233 233
    205 201 201
    139 137 137
    255 245 238
    238 229 222
    205 197 191
    139 134 130
    255 239 219
    238 223 204
    205 192 176
    139 131 120
    255 228 196
    238 213 183
    205 183 158
    139 125 107
    255 218 185
    238 203 173
    205 175 149
    139 119 101
    255 222 173
    238 207 161
    205 179 139
    139 121  94
    255 250 205
    238 233 191
    205 201 165
    139 137 112
    255 248 220
    238 232 205
    205 200 177
    139 136 120
    255 255 240
    238 238 224
    205 205 193
    139 139 131
    240 255 240
    224 238 224
    193 205 193
    131 139 131
    255 240 245
    238 224 229
    205 193 197
    139 131 134
    255 228 225
    238 213 210
    205 183 181
    139 125 123
    240 255 255
    224 238 238
    193 205 205
    131 139 139
    131 111 255
    122 103 238
    105  89 205
    71  60 139
    72 118 255
    67 110 238
    58  95 205
    39  64 139
    0   0 255
    0   0 238
    0   0 205
    0   0 139
    30 144 255
    28 134 238
    24 116 205
    16  78 139
    99 184 255
    92 172 238
    79 148 205
    54 100 139
    0 191 255
    0 178 238
    0 154 205
    0 104 139
    135 206 255
    126 192 238
    108 166 205
    74 112 139
    176 226 255
    164 211 238
    141 182 205
    96 123 139
    198 226 255
    185 211 238
    159 182 205
    108 123 139
    202 225 255
    188 210 238
    162 181 205
    110 123 139
    191 239 255
    178 223 238
    154 192 205
    104 131 139
    224 255 255
    209 238 238
    180 205 205
    122 139 139
    187 255 255
    174 238 238
    150 205 205
    102 139 139
    152 245 255
    142 229 238
    122 197 205
    83 134 139
    0 245 255
    0 229 238
    0 197 205
    0 134 139
    0 255 255
    0 238 238
    0 205 205
    0 139 139
    151 255 255
    141 238 238
    121 205 205
    82 139 139
    127 255 212
    118 238 198
    102 205 170
    69 139 116
    193 255 193
    180 238 180
    155 205 155
    105 139 105
    84 255 159
    78 238 148
    67 205 128
    46 139  87
    154 255 154
    144 238 144
    124 205 124
    84 139  84
    0 255 127
    0 238 118
    0 205 102
    0 139  69
    0 255   0
    0 238   0
    0 205   0
    0 139   0
    127 255   0
    118 238   0
    102 205   0
    69 139   0
    192 255  62
    179 238  58
    154 205  50
    105 139  34
    202 255 112
    188 238 104
    162 205  90
    110 139  61
    255 246 143
    238 230 133
    205 198 115
    139 134  78
    255 236 139
    238 220 130
    205 190 112
    139 129  76
    255 255 224
    238 238 209
    205 205 180
    139 139 122
    255 255   0
    238 238   0
    205 205   0
    139 139   0
    255 215   0
    238 201   0
    205 173   0
    139 117   0
    255 193  37
    238 180  34
    205 155  29
    139 105  20
    255 185  15
    238 173  14
    205 149  12
    139 101   8
    255 193 193
    238 180 180
    205 155 155
    139 105 105
    255 106 106
    238  99  99
    205  85  85
    139  58  58
    255 130  71
    238 121  66
    205 104  57
    139  71  38
    255 211 155
    238 197 145
    205 170 125
    139 115  85
    255 231 186
    238 216 174
    205 186 150
    139 126 102
    255 165  79
    238 154  73
    205 133  63
    139  90  43
    255 127  36
    238 118  33
    205 102  29
    139  69  19
    255  48  48
    238  44  44
    205  38  38
    139  26  26
    255  64  64
    238  59  59
    205  51  51
    139  35  35
    255 140 105
    238 130  98
    205 112  84
    139  76  57
    255 160 122
    238 149 114
    205 129  98
    139  87  66
    255 165   0
    238 154   0
    205 133   0
    139  90   0
    255 127   0
    238 118   0
    205 102   0
    139  69   0
    255 114  86
    238 106  80
    205  91  69
    139  62  47
    255  99  71
    238  92  66
    205  79  57
    139  54  38
    255  69   0
    238  64   0
    205  55   0
    139  37   0
    255   0   0
    238   0   0
    205   0   0
    139   0   0
    255  20 147
    238  18 137
    205  16 118
    139  10  80
    255 110 180
    238 106 167
    205  96 144
    139  58  98
    255 181 197
    238 169 184
    205 145 158
    139  99 108
    255 174 185
    238 162 173
    205 140 149
    139  95 101
    255 130 171
    238 121 159
    205 104 137
    139  71  93
    255  52 179
    238  48 167
    205  41 144
    139  28  98
    255  62 150
    238  58 140
    205  50 120
    139  34  82
    255   0 255
    238   0 238
    205   0 205
    139   0 139
    255 131 250
    238 122 233
    205 105 201
    139  71 137
    255 187 255
    238 174 238
    205 150 205
    139 102 139
    224 102 255
    209  95 238
    180  82 205
    122  55 139
    191  62 255
    178  58 238
    154  50 205
    104  34 139
    155  48 255
    145  44 238
    125  38 205
    85  26 139
    171 130 255
    159 121 238
    137 104 205
    93  71 139
    255 225 255
    238 210 238
    205 181 205
    139 123 139
    0   0   0
    0   0   0
    3   3   3
    3   3   3
    5   5   5
    5   5   5
    8   8   8
    8   8   8
    10  10  10
    10  10  10
    13  13  13
    13  13  13
    15  15  15
    15  15  15
    18  18  18
    18  18  18
    20  20  20
    20  20  20
    23  23  23
    23  23  23
    26  26  26
    26  26  26
    28  28  28
    28  28  28
    31  31  31
    31  31  31
    33  33  33
    33  33  33
    36  36  36
    36  36  36
    38  38  38
    38  38  38
    41  41  41
    41  41  41
    43  43  43
    43  43  43
    46  46  46
    46  46  46
    48  48  48
    48  48  48
    51  51  51
    51  51  51
    54  54  54
    54  54  54
    56  56  56
    56  56  56
    59  59  59
    59  59  59
    61  61  61
    61  61  61
    64  64  64
    64  64  64
    66  66  66
    66  66  66
    69  69  69
    69  69  69
    71  71  71
    71  71  71
    74  74  74
    74  74  74
    77  77  77
    77  77  77
    79  79  79
    79  79  79
    82  82  82
    82  82  82
    84  84  84
    84  84  84
    87  87  87
    87  87  87
    89  89  89
    89  89  89
    92  92  92
    92  92  92
    94  94  94
    94  94  94
    97  97  97
    97  97  97
    99  99  99
    99  99  99
    102 102 102
    102 102 102
    105 105 105
    105 105 105
    107 107 107
    107 107 107
    110 110 110
    110 110 110
    112 112 112
    112 112 112
    115 115 115
    115 115 115
    117 117 117
    117 117 117
    120 120 120
    120 120 120
    122 122 122
    122 122 122
    125 125 125
    125 125 125
    127 127 127
    127 127 127
    130 130 130
    130 130 130
    133 133 133
    133 133 133
    135 135 135
    135 135 135
    138 138 138
    138 138 138
    140 140 140
    140 140 140
    143 143 143
    143 143 143
    145 145 145
    145 145 145
    148 148 148
    148 148 148
    150 150 150
    150 150 150
    153 153 153
    153 153 153
    156 156 156
    156 156 156
    158 158 158
    158 158 158
    161 161 161
    161 161 161
    163 163 163
    163 163 163
    166 166 166
    166 166 166
    168 168 168
    168 168 168
    171 171 171
    171 171 171
    173 173 173
    173 173 173
    176 176 176
    176 176 176
    179 179 179
    179 179 179
    181 181 181
    181 181 181
    184 184 184
    184 184 184
    186 186 186
    186 186 186
    189 189 189
    189 189 189
    191 191 191
    191 191 191
    194 194 194
    194 194 194
    196 196 196
    196 196 196
    199 199 199
    199 199 199
    201 201 201
    201 201 201
    204 204 204
    204 204 204
    207 207 207
    207 207 207
    209 209 209
    209 209 209
    212 212 212
    212 212 212
    214 214 214
    214 214 214
    217 217 217
    217 217 217
    219 219 219
    219 219 219
    222 222 222
    222 222 222
    224 224 224
    224 224 224
    227 227 227
    227 227 227
    229 229 229
    229 229 229
    232 232 232
    232 232 232
    235 235 235
    235 235 235
    237 237 237
    237 237 237
    240 240 240
    240 240 240
    242 242 242
    242 242 242
    245 245 245
    245 245 245
    247 247 247
    247 247 247
    250 250 250
    250 250 250
    252 252 252
    252 252 252
    255 255 255
    255 255 255
    169 169 169
    169 169 169
    169 169 169
    169 169 169
    0     0 139
    0     0 139
    0   139 139
    0   139 139
    139   0 139
    139   0 139
    139   0   0
    139   0   0
    144 238 144
    144 238 144
    ];

    k=find(rgb(:,1)==r & rgb(:,2)==g & rgb(:, 3)==b)
    if k==[] then
        r=[];
    else
        r=names(k);
    end

endfunction
