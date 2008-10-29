/*
a = 1
b = %i
c = a + b
cc = a - b
d = 4*a
e = 4*b
f = 2*a + 3*b
ff = 2*a - 3*b

l = "re" + "toto"
m = ["je", "tu", "il"; "nous", "vous", "ils"]
n = ["e", "es", "e"; "eons", "ez", "ent"]
o = m + " mang" + n
a = [1 2;2 1]
b = [1 2;1 2]

*/
/* Error cases
j = [1 + "toto",4]
k = [1 ; 2  3]

if "toto" then
disp true
else
disp false
end

*/
/*
a = [1 2;3 4]
b = [2 1;4 3]
c = [2 3;4 5]

if a <> b then
	if c < a then
		d = "true true"
	else
		d = "true false"
	end
else
	if a < c then
		d = "true true"
	else
		d = "true false"
	end
end

a = 1;
b = 2;
*/
/*
a_0_1 = cos(0)
a_1_4 = cos(%pi/4)
a_1_3 = cos(%pi/3)
a_1_2 = cos(%pi/2)
a_2_3 = cos(2*%pi/3)
a_3_4 = cos(3*%pi/4)
a_1_1 = cos(%pi)
a = [a_0_1; a_1_4; a_1_3; a_1_2; a_2_3; a_3_4; a_1_1]
*/
/*
col = [%f;%t;%f;%t;%f;%t;%f;%t;%f;%t]
row = [%f,%t,%f,%t,%f,%t,%f,%t,%f,%t,%f,%t,%f,%t,%f,%t,%f,%t,%f,%t,%f,%t,%f,%t,%f,%t,%f,%t,%f,%t,%f,%t,%f,%t,%f,%t,%f,%t,%f,%t,%f,%t,%f,%t,%f,%t,%f,%t,%f,%t,%f,%t,%f,%t,%f,%t,%f,%t,%f,%t,%f,%t,%f,%t,%f,%t,%f,%t,%f,%t,%f,%t,%f,%t,%f,%t,%f,%t,%f,%t,%f,%t,%f,%t,%f,%t,%f,%t,%f,%t,%f,%t,%f,%t,%f,%t,%f,%t,%f,%t]
row_col = [
	%f,%t,%f,%t,%f,%t,%f,%t,%f,%t,%f,%t,%f,%t,%f,%t,%f,%t,%f,%t;
	%f,%t,%f,%t,%f,%t,%f,%t,%f,%t,%f,%t,%f,%t,%f,%t,%f,%t,%f,%t;
	%f,%t,%f,%t,%f,%t,%f,%t,%f,%t,%f,%t,%f,%t,%f,%t,%f,%t,%f,%t]

col = [0;1.2345678901234567890;2;3;4;5;6;7;8;9;0;1;2;3;4;5;6;7;8;9]
*/

dbl = [0.0000000001,2000.0001,0.0000000003,0.00000000035,1,2,0.0000000001;0.0000000004,0.00005,0.00006,0.00007,1,0.00001,3]