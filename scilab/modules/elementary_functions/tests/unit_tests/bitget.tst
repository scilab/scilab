// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Pierre MARECHAL <pierre.marechal@inria.fr>
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

// Test 1
// =============================================================================

// Test 1.1

A = floor(2^52 * rand(30,30));

for i=1:size(A,"*");
	B = strcat(string(bitget(A(i),[1:52])));
	C = strrev(dec2bin(A(i),52));
	if B<>C then pause, end
end

// Test 1.2

A = uint8(floor( 2^8 * rand(30,30)));

for i=1:size(A,"*");
	B = strcat(string(bitget(A(i),[1:8])));
	C = strrev(dec2bin(A(i),8));
	if B<>C then pause, end
end

// Test 1.3

A = uint16(floor( 2^16 * rand(30,30)));

for i=1:size(A,"*");
	B = strcat(string(bitget(A(i),[1:16])));
	C = strrev(dec2bin(A(i),16));
	if B<>C then pause, end
end

// Test 1.4

A = uint32(floor( 2^32 * rand(30,30)));

for i=1:size(A,"*");
	B = strcat(string(bitget(A(i),[1:32])));
	C = strrev(dec2bin(A(i),32));
	if B<>C then pause, end
end

// Test 2
// =============================================================================

// Test 2.1

A = bin2dec(["0001" "0010";"0011" "0100"]);
B =         [    1      0 ;    1      0 ];
C =         [   0      1  ;   1      0  ];

if or( bitget(A,1) <> B ) then pause, end
if or( bitget(A,2) <> C ) then pause, end

// Test 2.2

A = uint8(bin2dec(["0001" "0010";"0011" "0100"]));
B =               [    1      0 ;    1      0 ];
C =               [   0      1  ;   1      0  ];

if or( bitget(A,1) <> B ) then pause, end
if or( bitget(A,2) <> C ) then pause, end

// Test 2.3

A = uint16(bin2dec(["0001" "0010";"0011" "0100"]));
B =                [    1      0 ;    1      0 ];
C =                [   0      1  ;   1      0  ];

if or( bitget(A,1) <> B ) then pause, end
if or( bitget(A,2) <> C ) then pause, end

// Test 2.4

A = uint32(bin2dec(["0001" "0010";"0011" "0100"]));
B =                [    1      0 ;    1      0 ];
C =                [   0      1  ;   1      0  ];

if or( bitget(A,1) <> B ) then pause, end
if or( bitget(A,2) <> C ) then pause, end


// Test 3
// =============================================================================

if execstr("bitget( 123 , 52);","errcatch") <> 0 then pause,end
if execstr("bitget( 123 , 53);","errcatch") == 0 then pause,end
if execstr("bitget( 123 , 0);" ,"errcatch") == 0 then pause,end

if execstr("bitget( uint8(123)  , 8);","errcatch")  <> 0 then pause,end
if execstr("bitget( uint8(123)  , 9);","errcatch")  == 0 then pause,end

if execstr("bitget( uint16(123) , 16);","errcatch") <> 0 then pause,end
if execstr("bitget( uint16(123) , 17);","errcatch") == 0 then pause,end

if execstr("bitget( uint32(123) , 32);","errcatch") <> 0 then pause,end
if execstr("bitget( uint32(123) , 33);","errcatch") == 0 then pause,end
