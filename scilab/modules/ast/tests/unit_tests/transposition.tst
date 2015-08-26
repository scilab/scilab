// ============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2014 - Scilab Enterprises - Calixte DENIZET
//
//  This file is distributed under the same license as the Scilab package.
// ============================================================================

// <-- CLI SHELL MODE -->

S = 123;
C = [1 2;3 4];
R = [1 2 3;4 5 6];
SC = 1+2*%i
CC = C + %i * [5 6;7 8];
RC = R + %i * [7 8 9;10 11 12];
SB = %F;
CB = [%T %T;%F %T];
RB = [%T %T %F;%T %F %T];
X = poly(0, "x");
CP = C + C * X + C * X ^ 2;
RP = R + R * X + R * X ^ 2;
CCP = CC + CC * X + CC * X ^ 2;
RCP = RC + RC * X + RC * X ^ 2;

assert_checkequal([], []');
assert_checkequal([], [].');
assert_checkequal(eye(), eye()');
assert_checkequal(eye(), eye().');
assert_checkequal(S, S');
assert_checkequal(S, S.');
assert_checkequal(SB, SB');
assert_checkequal(SB, SB.');
assert_checkequal(SC', 1 - 2 * %i);
assert_checkequal(SC, SC.');

assert_checkequal(C', [1 3;2 4]);
assert_checkequal(C.', [1 3;2 4]);
assert_checkequal(CC', [1 3;2 4] - %i * [5 7;6 8]);
assert_checkequal(CC.', [1 3;2 4] + %i * [5 7;6 8]);
assert_checkequal(R', [1 4;2 5;3 6]);
assert_checkequal(R.', [1 4;2 5;3 6]);
assert_checkequal(RC', [1 4;2 5;3 6] - %i * [7 10;8 11;9 12]);
assert_checkequal(RC.', [1 4;2 5;3 6] + %i * [7 10;8 11;9 12]);

assert_checkequal(string(C)', string([1 3;2 4]));
assert_checkequal(string(C).', string([1 3;2 4]));
assert_checkequal(string(R)', string([1 4;2 5;3 6]));
assert_checkequal(string(R).', string([1 4;2 5;3 6]));

assert_checkequal(sparse(C)', sparse([1 3;2 4]));
assert_checkequal(sparse(C).', sparse([1 3;2 4]));
assert_checkequal(sparse(CC)', sparse([1 3;2 4] - %i * [5 7;6 8]));
assert_checkequal(sparse(CC).', sparse([1 3;2 4] + %i * [5 7;6 8]));
assert_checkequal(sparse(R)', sparse([1 4;2 5;3 6]));
assert_checkequal(sparse(R).', sparse([1 4;2 5;3 6]));
assert_checkequal(sparse(RC)', sparse([1 4;2 5;3 6] - %i * [7 10;8 11;9 12]));
assert_checkequal(sparse(RC).', sparse([1 4;2 5;3 6] + %i * [7 10;8 11;9 12]));

assert_checkequal(CB', [%T %F;%T %T]);
assert_checkequal(CB.', [%T %F;%T %T]);
assert_checkequal(RB', [%T %T;%T %F;%F %T]);
assert_checkequal(RB.', [%T %T;%T %F;%F %T]);
assert_checkequal(sparse(CB)', sparse([%T %F;%T %T]));
assert_checkequal(sparse(CB).', sparse([%T %F;%T %T]));
assert_checkequal(sparse(RB)', sparse([%T %T;%T %F;%F %T]));
assert_checkequal(sparse(RB).', sparse([%T %T;%T %F;%F %T]));

assert_checkequal(int8(S)', int8(S));
assert_checkequal(int8(C)', int8([1 3;2 4]));
assert_checkequal(int8(C).', int8([1 3;2 4]));
assert_checkequal(int8(R)', int8([1 4;2 5;3 6]));
assert_checkequal(int8(R).', int8([1 4;2 5;3 6]));
assert_checkequal(int16(S)', int16(S));
assert_checkequal(int16(C)', int16([1 3;2 4]));
assert_checkequal(int16(C).', int16([1 3;2 4]));
assert_checkequal(int16(R)', int16([1 4;2 5;3 6]));
assert_checkequal(int16(R).', int16([1 4;2 5;3 6]));
assert_checkequal(int32(S)', int32(S));
assert_checkequal(int32(C)', int32([1 3;2 4]));
assert_checkequal(int32(C).', int32([1 3;2 4]));
assert_checkequal(int32(R)', int32([1 4;2 5;3 6]));
assert_checkequal(int32(R).', int32([1 4;2 5;3 6]));
assert_checkequal(int64(S)', int64(S));
assert_checkequal(int64(C)', int64([1 3;2 4]));
assert_checkequal(int64(C).', int64([1 3;2 4]));
assert_checkequal(int64(R)', int64([1 4;2 5;3 6]));
assert_checkequal(int64(R).', int64([1 4;2 5;3 6]));
assert_checkequal(uint8(S)', uint8(S));
assert_checkequal(uint8(C)', uint8([1 3;2 4]));
assert_checkequal(uint8(C).', uint8([1 3;2 4]));
assert_checkequal(uint8(R)', uint8([1 4;2 5;3 6]));
assert_checkequal(uint8(R).', uint8([1 4;2 5;3 6]));
assert_checkequal(uint16(S)', uint16(S));
assert_checkequal(uint16(C)', uint16([1 3;2 4]));
assert_checkequal(uint16(C).', uint16([1 3;2 4]));
assert_checkequal(uint16(R)', uint16([1 4;2 5;3 6]));
assert_checkequal(uint16(R).', uint16([1 4;2 5;3 6]));
assert_checkequal(uint32(S)', uint32(S));
assert_checkequal(uint32(C)', uint32([1 3;2 4]));
assert_checkequal(uint32(C).', uint32([1 3;2 4]));
assert_checkequal(uint32(R)', uint32([1 4;2 5;3 6]));
assert_checkequal(uint32(R).', uint32([1 4;2 5;3 6]));
assert_checkequal(uint64(S)', uint64(S));
assert_checkequal(uint64(C)', uint64([1 3;2 4]));
assert_checkequal(uint64(C).', uint64([1 3;2 4]));
assert_checkequal(uint64(R)', uint64([1 4;2 5;3 6]));
assert_checkequal(uint64(R).', uint64([1 4;2 5;3 6]));

assert_checkequal(X', X);
assert_checkequal(X.', X);
assert_checkequal(CP', C' * (1 + X + X ^ 2));
assert_checkequal(CP.', C.' * (1 + X + X ^ 2));
assert_checkequal(RP', R' * (1 + X + X ^ 2));
assert_checkequal(RP.', R.' * (1 + X + X ^ 2));
assert_checkequal(CCP', CC' * (1 + X + X ^ 2));
assert_checkequal(CCP.', CC.' * (1 + X + X ^ 2));
assert_checkequal(RCP', RC' * (1 + X + X ^ 2));
assert_checkequal(RCP.', RC.' * (1 + X + X ^ 2));

assert_checkequal((1:5)', [1;2;3;4;5]);
assert_checkequal((1:2:10)', [1;3;5;7;9]);
