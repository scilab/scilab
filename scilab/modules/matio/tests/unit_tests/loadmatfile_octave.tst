// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2019 - Samuel GOUGEON
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// Tests for the loadmatfile("-octave") function
// =============================================================================
// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->

true = %T;
false = %F;
e = %e;
pi = %pi;
i = %i;
NaN = %nan;
Na = %nan;
Inf = %inf;
path = pathconvert("SCI/modules/matio/tests/unit_tests/octave");

function r = set_single_precision(r)
    // Copy of the loadmatfile.sci internal dependency
    if r==[] then
        return
    end
    s = size(r)
    if ~isreal(r) then
        r2 = [real(r(:)) imag(r(:))]
    else
        r2 = r
    end
    biggestFloat = 2^127*(2-2^-23); // (normal full accuracy)
    k = find(r2 > biggestFloat);  r2(k) = %inf
    k = find(r2 < -biggestFloat); r2(k) = -%inf
    tiniestFloat = 2^-126;          // (normal full accuracy)
    k = find(abs(r2) < tiniestFloat); r2(k) = 0;
    k = find(r2 <> 0 & ~isinf(r2) & ~isnan(r2))
    if k==[] then
        return
    end
    L = log10(abs(r2(k)))
    p = round(10.^(8-ceil(L)))
    r2(k) = round(r2(k) .* p) ./ p
    if ~isreal(r) then
        r2 = complex(r2(:,1), r2(:,2));
    end
    r = matrix(r2, s)
endfunction

/*
 NAMING:
 Initials of variables names:
 ---------------------------
  - "b" = booleans
  - "d" = decimal real numbers (single precision)
  - "D" = decimal real numbers (double precision)
  - "c" = complex numbers (single precision)
  - "C" = complex numbers (double precision)
  - "spd" = sparse arrays of decimal real numbers
  - "spc" = sparse arrays of complex numbers
  - "spb" = sparse arrays of booleans
  - "int"|"uint" = encoded integers
  - "t" = text arrays
  - "cel" = cells arrays
  - "st"  = structures arrays

 Last character of variable names:
 --------------------------------
  - "e" = Empty
  - "s" = Scalars
  - "r" = Row vectors
  - "c" = Column vectors
  - "m" = Matrices
  - "h" = Hyperarrays
*/
// ========
// BOOLEANS
// ========
/*
OCTAVE generating code:
----------------------
% Booleans (dense)
bs = true;
bs0 = false;
br = [true false false true false];
bc = [false true false true true]';
bm = [false true true ; true false true];
bh = cat(3, bm, [bm(2,:) ; bm(1,3:-1:1)]);

% Booleans (sparse)
spbe = sparse(false); spbe(1) = [];
spbs = sparse(bs);
spbs0 = sparse(bs0);
spbr = sparse(br);
spbc = sparse(bc);
spbm = sparse(bm);
spbmFalse = sparse(zeros(2,3)==1);
spbh = sparse(bh);

save octave_booleans.txt bs bs0 br bc bm bh spbe spbs spbs0 spbr spbc spbm spbmFalse spbh
*/
// Scilab refs
// -----------
bs = true;
bs0 = false;
br = [true false false true false];
bc = [false true false true true]';
bm = [false true true ; true false true];
bh = cat(3, bm, [bm(2,:) ; bm(1,3:-1:1)]);
// Booleans (sparse)
spbe = spzeros(0,1)==1;
spbs = sparse(bs);
spbs0 = sparse(bs0);
spbr = sparse(br);
spbc = sparse(bc);
spbm = sparse(bm);
spbmFalse = sparse(zeros(2,3)==1);
spbh = sparse([bh(:,:,1), bh(:,:,2)]);   // /!\

// Tests
// -----
names = ["bs" "bs0" "br" "bc" "bm" "bh" "spbe" "spbs" "spbs0" "spbr" "spbc" "spbm" "spbmFalse" "spbh"];
r = loadmatfile(path + "octave_booleans.txt","-toStruct");
for n = names
    assert_checkequal(r(n), evstr(n));
end


// ========
// INTEGERS
// ========
/*
OCTAVE generating code:
----------------------
% Common
ie = [];
is = 87;
ir = [8 43 111];
ic = ir';
im = [7 35 109 ; 14 61 113];
ih = cat(3, im, im(:,end:-1:1));

funs = {@int8 @uint8 @int16 @uint16 @int32 @uint32 @int64 @uint64};
labels = {"8" "8u" "16" "16u" "32" "32u" "64" "64u"};
names = ["ie" ; "is" ; "ir" ; "ic" ; "im" ; "ih"];
for k = 1:8
    fun = funs{k};
    for m = 1:6
        cmdtxt = strcat(names(m,:),labels{k},"=fun(",names(m,:),");");
        disp(cmdtxt);
        eval(cmdtxt);
    end
end

save octave_integers.txt ie8 is8 ir8 ic8 im8 ih8 ie8u is8u ir8u ic8u im8u ih8u ie16 is16 ir16 ic16 im16 ih16 ie16u is16u ir16u ic16u im16u ih16u ie32 is32 ir32 ic32 im32 ih32 ie32u is32u ir32u ic32u im32u ih32u ie64 is64 ir64 ic64 im64 ih64 ie64u is64u ir64u ic64u im64u ih64u
*/
// Scilab refs and test
// --------------------
ie = [];
is = 87;
ir = [8 43 111];
ic = ir';
im = [7 35 109 ; 14 61 113];
ih = cat(3, im, im(:, $:-1:1));

funs = list(int8, uint8, int16, uint16, int32, uint32, int64, uint64);
labels = ["8" "8u" "16" "16u" "32" "32u" "64" "64u"];
names = ["ie" ; "is" ; "ir" ; "ic" ; "im" ; "ih"];

r = loadmatfile(path + "octave_integers.txt", "-toStruct");
for k = 1:8
    fun = funs(k);
    for m = 1:6
        vname = names(m) + labels(k);
        execstr("ref = fun(" + names(m) + ");");
        assert_checkequal(r(vname), ref);
    end
end

// ===============
// DECIMAL NUMBERS
// ===============
/*
OCTAVE generating code:
----------------------
% Decimal numbers (double dense)
Na = NA(1);
De = [];
Ds = e;
Ds2 = Na;
Dr = [-Inf -e*[1e100 1 1e-200] 0 NaN e*[1e-100 Na 1e200] Inf];
Dc = Dr';
Dm = [Dr(1,1:5); Dr(1,10:-1:6)];
Dh = reshape(Dm, [1 5 2]);

% Decimal numbers (single dense)
de = single(De);
ds = single(Ds);
dr = single(Dr);
dc = single(Dc);
dm = single(Dm);
dh = single(Dh);

% Decimal numbers (double sparse)
spDe = sparse(De);
spDs = sparse(Ds);
spDr = sparse(Dr);
spDc = sparse(Dc);
spDm = sparse(Dm);
spDmZeros = sparse(zeros(2,3));
spDh = sparse(Dh);

% Decimal numbers (single sparse): Do not exist

save octave_reals.txt De Ds Ds2 Dr Dc Dm Dh de ds dr dc dm dh spDe spDs spDr spDc spDm spDmZeros spDh
*/
// Scilab refs
// -----------
De = [];
Ds = e;
Ds2 = Na;
Dr = [-Inf -e*[1e100 1 1e-200] 0 NaN e*[1e-100 Na 1e200] Inf];
Dc = Dr';
Dm = [Dr(1,1:5); Dr(1,10:-1:6)];
Dh = matrix(Dm, [1 5 2]);

// Decimal numbers (single dense)
de = set_single_precision(De);
ds = set_single_precision(Ds);
dr = set_single_precision(Dr);
dc = set_single_precision(Dc);
dm = set_single_precision(Dm);
dh = set_single_precision(Dh);

// Decimal numbers (double sparse)
spDe = sparse([],[]);   // /!\
spDs = sparse(Ds);
spDr = sparse(Dr);
spDc = sparse(Dc);
spDm = sparse(Dm);
spDmZeros = sparse(zeros(2,3));
spDh = sparse([Dh(:,:,1) Dh(:,:,2)]);

// Tests
// -----
names = ["De" "Ds" "Ds2" "Dr" "Dc" "Dm" "Dh" "de" "ds" "dr" "dc" "dm" "dh" "spDe" "spDs" "spDr" "spDc" "spDm" "spDmZeros" "spDh"]; //
r = loadmatfile(path + "octave_reals.txt","-toStruct");
for n = names
    if part(n,1)=="d"
        assert_checkalmostequal(r(n), evstr(n),1e-7);
    elseif part(n,1:2)=="sp"
        assert_checkalmostequal(r(n), evstr(n),%eps);
    else
        assert_checkequal(r(n), evstr(n));
    end
end

// ===============
// COMPLEX NUMBERS
// ===============
/*
OCTAVE generating code:
----------------------
% Complex numbers (double dense)
Cs = pi + e*i;
Cr = complex(Dr(1, 1:5), Dr(1, 10:-1:6));
Cc = Cr.';
Cm = [[Cr 1-i]; complex(-1:4, [-Inf -1e300 -1 1e-300 1 NaN])];
Ch = reshape(Cm, [2 3 2]);

% Complex numbers (single dense)
cs = single(Cs);
cr = single(Cr);
cc = single(Cc);
cm = single(Cm);
ch = single(Ch);

% Complex numbers (double sparse)
spCs = sparse(Cs);
spCr = sparse(Cr);
spCc = sparse(Cc);
spCm = sparse(Cm);
spCh = sparse(Ch);

save octave_complexes.txt Cs Cr Cc Cm Ch cs cr cc cm ch spCs spCr spCc spCm spCh
*/
// Scilab refs
// -----------
// Complex numbers (double dense)
Cs = pi + e*i;
Cr = complex(Dr(1, 1:5), Dr(1, 10:-1:6));
Cc = Cr.';
Cm = [[Cr 1-i]; complex(-1:4, [-Inf -1e300 -1 1e-300 1 NaN])];
Ch = matrix(Cm, [2 3 2]);   // /!\

// Complex numbers (single dense)
cs = set_single_precision(Cs);
cr = set_single_precision(Cr);
cc = set_single_precision(Cc);
cm = set_single_precision(Cm);
ch = set_single_precision(Ch);

// Complex numbers (double sparse)
spCs = sparse(Cs);
spCr = sparse(Cr);
spCc = sparse(Cc);
spCm = sparse(Cm);
spCh = sparse([Ch(:,:,1) Ch(:,:,2)]);

// Tests
// -----
names = ["Cs" "Cr" "Cc" "Cm" "Ch" "cs" "cr" "cc" "cm" "ch" "spCs" "spCr" "spCc" "spCm" "spCh"]; //
r = loadmatfile(path + "octave_complexes.txt", "-toStruct");
for n = names
    if part(n,1)=="c"
        assert_checkalmostequal(r(n), evstr(n),1e-7);
    elseif part(n,1:2)=="sp"
        assert_checkalmostequal(r(n), evstr(n),%eps);
    else
        assert_checkequal(r(n), evstr(n));
    end
end

// ====
// TEXT
// ====
/*
OCTAVE generating code
----------------------
% Text
te = "";
ts = "Bonjour";
ts2 = " Bonjour ";
ts3 = strcat("Bonjour", char(10), char(9), {"=> ABCD "});
tc = ["Hello" ; "Allo" ; "Hola"];
tr = {"" "  " " ABC " "ABC " "AB C"};
tm = {"Hello" "Hi" "Good evening" ; "Bonjour" "Salut" "Bonsoir"};
th = cat(3, tm, {"ABCD" "ABC" "AB" ; "abcd" "abc" "ab"});

save octave_strings.txt te ts ts2 ts3 tc tr tm th
*/
// Scilab refs
// -----------
te = "";
ts = "Bonjour";
ts2 = " Bonjour ";
ts3 = strcat(["Bonjour", char(10), char(9), "=> ABCD "]);           // /!\
tc = justify(["Hello" ; "Allo" ; "Hola"], "l");                     // /!\
tr = ["" "  " " ABC " "ABC " "AB C"];                               // /!\
tm = ["Hello" "Hi" "Good evening" ; "Bonjour" "Salut" "Bonsoir"];   // /!\
th = cat(3, tm, ["ABCD" "ABC" "AB" ; "abcd" "abc" "ab"]);            // /!\
// Tests
// -----
names = ["te" "ts" "ts2" "ts3" "tc" "tr" "tm" "th"];
r = loadmatfile(path + "octave_strings.txt", "-toStruct");
for n = names
    assert_checkequal(r(n), evstr(n));
end

// ===========
// CELLS ARRAY
// ===========
/*
OCTAVE generating code
----------------------
% Cells
cele = {};
cels = {e};
cels2 = {[]};
celr = {[], Inf, [1 3 ; 4 2]<3};
celr2 = celr; celr2(5) = "ABCD";
celc = celr';
celm = {[], Inf, [1 3 ; 4 2]<3 ; {} {e} celr ; "abcd", int8(-47), sparse([2 0 3])};
celh = cat(3, celm, celm(end:-1:1, end:-1:1));

save octave_cells.txt cele cels cels2 celr celr2 celc celm celh
*/
// Scilab refs
// -----------
cele = {};
cels = {e};
cels2 = {[]};
celr = {[], Inf, [1 3 ; 4 2]<3};
celr2 = celr; celr2{5} = "ABCD";            //   /!\
celc = celr';
celm = {[], Inf, [1 3 ; 4 2]<3 ; {} {e} celr ; "abcd", int8(-47), sparse([2 0 3])};
celh = cat(3, celm, celm($:-1:1, $:-1:1));  //   /!\
// Tests
// -----
names = ["cele" "cels" "cels2" "celr" "celr2" "celc" "celm" "celh"];
r = loadmatfile(path + "octave_cells.txt", "-toStruct");
for n = names
    assert_checkequal(r(n), evstr(n));
end

// ==========
// STRUCTURES
// ==========
/*
OCTAVE generating code
----------------------
% Structures
ste = struct();
ste2 = struct("r",1,"b",false); ste2(1) = [];
sts = struct("r",[pi e i], "b",false, "t",{{"abc" "ABC"}});
stm = struct("r",{i -1 ; e -i}, "b", {false, [true false]; true, [false false]'});
stm2 = stm; stm2(2,2).n = stm % Nested struct

save octave_struct.txt ste ste2 sts stm stm2
*/
// Scilab refs
// -----------
ste = struct();
ste2 = struct();
sts = struct("r",[pi e i], "b",false, "t",["abc" "ABC"]);
stm = struct("r",{i -1 ; e -i}, "b", {false, [true false]; true, [false false]'});
stm2 = stm; stm2(2,2).n = stm   // Nested struct

// Tests
// -----
names = ["ste" "ste2" "sts" "stm" "stm2"];
r = loadmatfile(path + "octave_struct.txt", "-toStruct");
for n = names
    assert_checkequal(r(n), evstr(n));
end
