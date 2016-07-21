// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2007-2008 - INRIA
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

// =============================================================================
// unit tests ascii
// =============================================================================

// == TEST 1 ===================================================================

if or(ascii("abcdefghijklmnopqrstuvwxyz")<>(97:122)) then pause,end
if or(ascii(97:122)<>"abcdefghijklmnopqrstuvwxyz") then pause,end
if ascii("")<>[] then pause,end
if ascii([])<>"" then pause,end

// == TEST 2 ===================================================================

CODE_ASCII_SCILAB   = [115 99 105 108 97 98];
str_scilab          = "scilab";
if ascii(str_scilab) <> CODE_ASCII_SCILAB   then pause,end
if (ascii(CODE_ASCII_SCILAB) <> str_scilab) then pause,end

// == TEST 3 ===================================================================

if ascii([str_scilab "a";"b" "c"]) <> [115 99 105 108 97 98 98 97 99] then pause,end

// == TEST 4 ===================================================================

fd  = mopen(SCI+"/modules/string/tests/unit_tests/text.txt","r");
txt = mgetl( fd );
mclose( fd );
ASCII_TXT           = ascii(txt);
TXT_FROM_ASCII_CODE = ascii(ASCII_TXT);

if (length(TXT_FROM_ASCII_CODE) <> 27455) then pause,end

// == TEST 5 ===================================================================

if ascii("!") <> 33 then pause,end
if ascii("""")<> 34 then pause,end
if ascii("#") <> 35 then pause,end
if ascii("$") <> 36 then pause,end
if ascii("%") <> 37 then pause,end
if ascii("&") <> 38 then pause,end
if ascii("''")<> 39 then pause,end
if ascii("(") <> 40 then pause,end
if ascii(")") <> 41 then pause,end
if ascii("*") <> 42 then pause,end
if ascii("+") <> 43 then pause,end
if ascii(",") <> 44 then pause,end
if ascii("-") <> 45 then pause,end
if ascii(".") <> 46 then pause,end
if ascii("/") <> 47 then pause,end
if ascii("0") <> 48 then pause,end
if ascii("1") <> 49 then pause,end
if ascii("2") <> 50 then pause,end
if ascii("3") <> 51 then pause,end
if ascii("4") <> 52 then pause,end
if ascii("5") <> 53 then pause,end
if ascii("6") <> 54 then pause,end
if ascii("7") <> 55 then pause,end
if ascii("8") <> 56 then pause,end
if ascii("9") <> 57 then pause,end
if ascii(":") <> 58 then pause,end
if ascii(";") <> 59 then pause,end
if ascii("<") <> 60 then pause,end
if ascii("=") <> 61 then pause,end
if ascii(">") <> 62 then pause,end
if ascii("?") <> 63 then pause,end
if ascii("@") <> 64 then pause,end
if ascii("A") <> 65 then pause,end
if ascii("B") <> 66 then pause,end
if ascii("C") <> 67 then pause,end
if ascii("D") <> 68 then pause,end
if ascii("E") <> 69 then pause,end
if ascii("F") <> 70 then pause,end
if ascii("G") <> 71 then pause,end
if ascii("H") <> 72 then pause,end
if ascii("I") <> 73 then pause,end
if ascii("J") <> 74 then pause,end
if ascii("K") <> 75 then pause,end
if ascii("L") <> 76 then pause,end
if ascii("M") <> 77 then pause,end
if ascii("N") <> 78 then pause,end
if ascii("O") <> 79 then pause,end
if ascii("P") <> 80 then pause,end
if ascii("Q") <> 81 then pause,end
if ascii("R") <> 82 then pause,end
if ascii("S") <> 83 then pause,end
if ascii("T") <> 84 then pause,end
if ascii("U") <> 85 then pause,end
if ascii("V") <> 86 then pause,end
if ascii("W") <> 87 then pause,end
if ascii("X") <> 88 then pause,end
if ascii("Y") <> 89 then pause,end
if ascii("Z") <> 90 then pause,end
if ascii("[") <> 91 then pause,end
if ascii("\") <> 92 then pause,end
if ascii("]") <> 93 then pause,end
if ascii("^") <> 94 then pause,end
if ascii("_") <> 95 then pause,end
if ascii("`") <> 96 then pause,end
if ascii("a") <> 97 then pause,end
if ascii("b") <> 98 then pause,end
if ascii("c") <> 99 then pause,end
if ascii("d") <> 100 then pause,end
if ascii("e") <> 101 then pause,end
if ascii("f") <> 102 then pause,end
if ascii("g") <> 103 then pause,end
if ascii("h") <> 104 then pause,end
if ascii("i") <> 105 then pause,end
if ascii("j") <> 106 then pause,end
if ascii("k") <> 107 then pause,end
if ascii("l") <> 108 then pause,end
if ascii("m") <> 109 then pause,end
if ascii("n") <> 110 then pause,end
if ascii("o") <> 111 then pause,end
if ascii("p") <> 112 then pause,end
if ascii("q") <> 113 then pause,end
if ascii("r") <> 114 then pause,end
if ascii("s") <> 115 then pause,end
if ascii("t") <> 116 then pause,end
if ascii("u") <> 117 then pause,end
if ascii("v") <> 118 then pause,end
if ascii("w") <> 119 then pause,end
if ascii("x") <> 120 then pause,end
if ascii("y") <> 121 then pause,end
if ascii("z") <> 122 then pause,end
if ascii("{") <> 123 then pause,end
if ascii("|") <> 124 then pause,end
if ascii("}") <> 125 then pause,end
if ascii("~") <> 126 then pause,end

if ascii(33) <> "!" then pause,end
if ascii(34) <> """" then pause,end
if ascii(35) <> "#" then pause,end
if ascii(36) <> "$" then pause,end
if ascii(37) <> "%" then pause,end
if ascii(38) <> "&" then pause,end
if ascii(39) <> "''" then pause,end
if ascii(40) <> "(" then pause,end
if ascii(41) <> ")" then pause,end
if ascii(42) <> "*" then pause,end
if ascii(43) <> "+" then pause,end
if ascii(44) <> "," then pause,end
if ascii(45) <> "-" then pause,end
if ascii(46) <> "." then pause,end
if ascii(47) <> "/" then pause,end
if ascii(48) <> "0" then pause,end
if ascii(49) <> "1" then pause,end
if ascii(50) <> "2" then pause,end
if ascii(51) <> "3" then pause,end
if ascii(52) <> "4" then pause,end
if ascii(53) <> "5" then pause,end
if ascii(54) <> "6" then pause,end
if ascii(55) <> "7" then pause,end
if ascii(56) <> "8" then pause,end
if ascii(57) <> "9" then pause,end
if ascii(58) <> ":" then pause,end
if ascii(59) <> ";" then pause,end
if ascii(60) <> "<" then pause,end
if ascii(61) <> "=" then pause,end
if ascii(62) <> ">" then pause,end
if ascii(63) <> "?" then pause,end
if ascii(64) <> "@" then pause,end
if ascii(65) <> "A" then pause,end
if ascii(66) <> "B" then pause,end
if ascii(67) <> "C" then pause,end
if ascii(68) <> "D" then pause,end
if ascii(69) <> "E" then pause,end
if ascii(70) <> "F" then pause,end
if ascii(71) <> "G" then pause,end
if ascii(72) <> "H" then pause,end
if ascii(73) <> "I" then pause,end
if ascii(74) <> "J" then pause,end
if ascii(75) <> "K" then pause,end
if ascii(76) <> "L" then pause,end
if ascii(77) <> "M" then pause,end
if ascii(78) <> "N" then pause,end
if ascii(79) <> "O" then pause,end
if ascii(80) <> "P" then pause,end
if ascii(81) <> "Q" then pause,end
if ascii(82) <> "R" then pause,end
if ascii(83) <> "S" then pause,end
if ascii(84) <> "T" then pause,end
if ascii(85) <> "U" then pause,end
if ascii(86) <> "V" then pause,end
if ascii(87) <> "W" then pause,end
if ascii(88) <> "X" then pause,end
if ascii(89) <> "Y" then pause,end
if ascii(90) <> "Z" then pause,end
if ascii(91) <> "[" then pause,end
if ascii(92) <> "\" then pause,end
if ascii(93) <> "]" then pause,end
if ascii(94) <> "^" then pause,end
if ascii(95) <> "_" then pause,end
if ascii(96) <> "`" then pause,end
if ascii(97) <> "a" then pause,end
if ascii(98) <> "b" then pause,end
if ascii(99) <> "c" then pause,end
if ascii(100) <> "d" then pause,end
if ascii(101) <> "e" then pause,end
if ascii(102) <> "f" then pause,end
if ascii(103) <> "g" then pause,end
if ascii(104) <> "h" then pause,end
if ascii(105) <> "i" then pause,end
if ascii(106) <> "j" then pause,end
if ascii(107) <> "k" then pause,end
if ascii(108) <> "l" then pause,end
if ascii(109) <> "m" then pause,end
if ascii(110) <> "n" then pause,end
if ascii(111) <> "o" then pause,end
if ascii(112) <> "p" then pause,end
if ascii(113) <> "q" then pause,end
if ascii(114) <> "r" then pause,end
if ascii(115) <> "s" then pause,end
if ascii(116) <> "t" then pause,end
if ascii(117) <> "u" then pause,end
if ascii(118) <> "v" then pause,end
if ascii(119) <> "w" then pause,end
if ascii(120) <> "x" then pause,end
if ascii(121) <> "y" then pause,end
if ascii(122) <> "z" then pause,end
if ascii(123) <> "{" then pause,end
if ascii(124) <> "|" then pause,end
if ascii(125) <> "}" then pause,end
if ascii(126) <> "~" then pause,end

// == TEST 6 ===================================================================

NB_CHARACTERS = 400000;
letters_ascii_code = ones(1,NB_CHARACTERS)*78;
letters=ascii(letters_ascii_code);
if (length(letters) <> NB_CHARACTERS) then pause,end

// == TEST 7 ===================================================================

A = floor( 126 * rand(100,100) );

if or( ascii(A) <> ascii(int8(A)) )  then pause, end
if or( ascii(A) <> ascii(int16(A)) ) then pause, end
if or( ascii(A) <> ascii(int32(A)) ) then pause, end

if or( ascii(A) <> ascii(uint8(A)) )  then pause, end
if or( ascii(A) <> ascii(uint16(A)) ) then pause, end
if or( ascii(A) <> ascii(uint32(A)) ) then pause, end

// valid UTF8
assert_checkequal(ascii("é"), [195 169]);
assert_checkequal(ascii([195 169]), "é");
assert_checkequal(ascii("€"), [226 130 172]);
assert_checkequal(ascii([226 130 172]), "€");

// invalid UTF8
assert_checkequal(length(ascii([190 169])), 2);
assert_checkequal(length(ascii([223 130 172])), 3);
