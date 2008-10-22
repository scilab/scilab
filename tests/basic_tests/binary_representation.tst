//===================================================
// Author : Pierre MARECHAL
// Scilab team
// Copyright INRIA 
// Date : 26 jun 2006
//===================================================

// test for dec2bin function
//==========================

if dec2bin(0) <> '0' then pause,end
if dec2bin(1) <> '1' then pause,end
if dec2bin(3) <> '11' then pause,end
if dec2bin(7) <> '111' then pause,end
if dec2bin(15) <> '1111' then pause,end
if dec2bin(255) <> '11111111' then pause,end
if dec2bin(26556445876512) <> '110000010011100100111100001011110000100100000' then pause,end
if dec2bin((2^32)-1) <> '11111111111111111111111111111111' then pause,end
if dec2bin(2^32) <> '100000000000000000000000000000000' then pause,end

if dec2bin(0,4) <> '0000' then pause,end
if dec2bin(1,4) <> '0001' then pause,end
if dec2bin(3,4) <> '0011' then pause,end
if dec2bin(7,4) <> '0111' then pause,end
if dec2bin(15,4) <> '1111' then pause,end

if dec2bin(0,8) <> '00000000' then pause,end
if dec2bin(1,8) <> '00000001' then pause,end
if dec2bin(3,8) <> '00000011' then pause,end
if dec2bin(7,8) <> '00000111' then pause,end
if dec2bin(15,8) <> '00001111' then pause,end

if dec2bin(0,16) <> '0000000000000000' then pause,end
if dec2bin(1,16) <> '0000000000000001' then pause,end
if dec2bin(3,16) <> '0000000000000011' then pause,end
if dec2bin(7,16) <> '0000000000000111' then pause,end
if dec2bin(15,16) <> '0000000000001111' then pause,end

matToTest = ['00001';'00010';'00011';'00100';'00101';'00110';'00111';'01000';'01001';'01010';'01011';'01100';'01101';'01110';'01111';'10000'];
if or( dec2bin([ 1 5 9 13 ; 2 6 10 14 ; 3 7 11 15 ; 4 8 12 16]) <> matToTest ) then pause,end
if or( dec2bin([ 1 5 9 13 ; 2 6 10 14 ; 3 7 11 15 ; 4 8 12 16]) <> matToTest ) then pause,end

// test for bin2dec function
//==========================

if bin2dec('0000 0000 0000 0000 0000 0000 0000 0000 0000 0000') <> 0 then pause,end
if bin2dec('0000 0000 0000 0000 0000 0000 0000 0000 0000 0001') <> 1 then pause,end
if bin2dec('0000 0000 0000 0000 0000 0000 0000 0000 0000 0010') <> 2 then pause,end
if bin2dec('0000 0000 0000 0000 0000 0000 0000 0000 0000 0011') <> 3 then pause,end
if bin2dec('0000 0000 0000 0000 0000 0000 0000 0000 0000 0100') <> 4 then pause,end
if bin2dec('0000 0000 0000 0000 0000 0000 0000 0000 0000 1111') <> 15 then pause,end

if bin2dec('0000 0000 0000 1111 1111 1111 1111 1111 1111 1111') <> (2^28 - 1) then pause,end
if bin2dec('0000 0000 1111 1111 1111 1111 1111 1111 1111 1111') <> (2^32 - 1) then pause,end
if bin2dec('0000 1111 1111 1111 1111 1111 1111 1111 1111 1111') <> (2^36 - 1) then pause,end
if bin2dec('1111 1111 1111 1111 1111 1111 1111 1111 1111 1111') <> (2^40 - 1) then pause,end

if bin2dec('11                             11 111  1 11 11      1111 1111 1111 1  111 111  1 1111 111  1') <> (2^40 - 1) then pause,end

if or(bin2dec(matToTest) <> [ 1:16 ]') then pause,end
