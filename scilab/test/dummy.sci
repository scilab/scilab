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
col = [%f;%t;%f;%t;%f;%t;%f;%t;%f;%t]
row = [%f,%t,%f,%t,%f,%t,%f,%t,%f,%t,%f,%t,%f,%t,%f,%t,%f,%t,%f,%t,%f,%t,%f,%t,%f,%t,%f,%t,%f,%t,%f,%t,%f,%t,%f,%t,%f,%t,%f,%t,%f,%t,%f,%t,%f,%t,%f,%t,%f,%t,%f,%t,%f,%t,%f,%t,%f,%t,%f,%t,%f,%t,%f,%t,%f,%t,%f,%t,%f,%t,%f,%t,%f,%t,%f,%t,%f,%t,%f,%t,%f,%t,%f,%t,%f,%t,%f,%t,%f,%t,%f,%t,%f,%t,%f,%t,%f,%t,%f,%t]
row_col = [
	%f,%t,%f,%t,%f,%t,%f,%t,%f,%t,%f,%t,%f,%t,%f,%t,%f,%t,%f,%t;
	%f,%t,%f,%t,%f,%t,%f,%t,%f,%t,%f,%t,%f,%t,%f,%t,%f,%t,%f,%t;
	%f,%t,%f,%t,%f,%t,%f,%t,%f,%t,%f,%t,%f,%t,%f,%t,%f,%t,%f,%t]

col = [0;1.2345678901234567890;2;3;4;5;6;7;8;9;0;1;2;3;4;5;6;7;8;9]
*/

/*
a01 = 0.123456
a02 = a01 * -10
a03 = a02 * 10
a04 = a03 * -10
a05 = a04 * 10
*/



//dblCol = [dbl01;dbl02;dbl03;dbl04;dbl05;dbl06;dbl07;dbl08;dbl09;dbl10;dbl11;dbl12;dbl13;dbl14;dbl15;dbl16;dbl17;dbl18]
//dblRow = [dbl01,dbl02,dbl03,dbl04,dbl05,dbl06,dbl07,dbl08,dbl09,dbl10,dbl11,dbl12,dbl13,dbl14,dbl15,dbl16,dbl17,dbl18]


				


//b01 = dbl01 * %i
//b02 = -dbl02 * %i
//b03	= -dbl03 + dbl04 * %i
//b04 = dbl05 - dbl07 * %i


//dblColI = [b01;b02;b03;b04]
//dblRowI = [b01,b02,b03,b04,b01,b02,b03,b04,b01,b02,b03,b04,b01,b02,b03,b04]
/*
dbl01 =  0.0001234;
dbl02 =  dbl01 * -5;
dbl03 =  dbl02 * -5;
dbl04 =  dbl03 * -5;
dbl05 =  dbl04 * -5;
dbl06 =  dbl05 * -5;
dbl07 =  dbl06 * -5;
dbl08 =  dbl07 * -5;
dbl09 =  dbl08 * -5;
dbl10 =  dbl09 * -5;
dbl11 =  dbl10 * -5;
dbl12 =  dbl11 * -5;
dbl13 =  dbl12 * -5;
dbl14 =  dbl13 * -5;
dbl15 =  dbl14 * -5;
dbl16 =  dbl15 * -5;
dbl17 =  dbl16 * -5;
dbl18 =  dbl17 * -5;

dblMatrix = [	dbl01,dbl02,dbl03,dbl04,dbl05,dbl06,dbl07,dbl08,dbl09,dbl10,dbl11,dbl12;
				dbl13,dbl14,dbl15,dbl16,dbl17,dbl18,dbl01,dbl02,dbl03,dbl04,dbl05,dbl06;
				dbl07,dbl08,dbl09,dbl10,dbl11,dbl12,dbl13,dbl14,dbl15,dbl16,dbl17,dbl18]
				
dblMatrixI = [	dbl02 + dbl01 * %i, dbl03 + dbl04 * %i, dbl06 + dbl05 * %i, dbl07 + dbl08 * %i;
				dbl10 + dbl09 * %i, dbl11 + dbl12 * %i, dbl01 + dbl02 * %i, dbl04 + dbl03 * %i;
				dbl05 + dbl06 * %i, dbl08 + dbl07 * %i, dbl09 + dbl10 * %i, dbl12 + dbl11 * %i]
*/
/*
//dbl01 =  0.0001234;
a = cos([	0, %pi/4, %pi/3, %pi/2, 2*%pi/3, 3*%pi/4, %pi, 0,%pi/4,%pi/3,%pi/2,2*%pi/3,3*%pi/4,%pi;
			0, %pi/4, %pi/3, %pi/2, 2*%pi/3, 3*%pi/4, %pi, 0,%pi/4,%pi/3,%pi/2,2*%pi/3,3*%pi/4,%pi])
*/
/*
a1 = [1 2 3 4 5 6];
b1 = [%i+1 2 3 4 5 6];
a2 = [1 2 3 4 5 6];
b2 = [%i+1 2 3 4 5 6];
c = 10
d = 7 + 8 * %i

a1(1) = c
a2(1) = d
b1(1) = c
b2(1) = d
*/
/*
//cas scalar
a = [1,2];
a(1) = 3;
a(4) = 5;
b(2) = 6;
*/

/*
//cas vector
c = [1,2];
c([1,2]) = 3;

d = [1,2];
d([1;2]) = 3;

e = [1,2];
e([1,2,3]) = 4;

f = [1,2];
f([1,2,3]) = [1 2 3];

g = [1,2];
g([1,2,3]) = [1;2;3];


h([1,2]) = 1;
i([1,2]) = [1,2];

j([1,2]) = [1;2];

a,b,c,d,e,f,g,h,i,j
*/
/*
//matrix
a([1,2],[3,4]) = 6 //creation
a([1,2],[2,3]) = 5 //insertion
a([1,2],[4,5]) = 7 //agrandissement
*/
b([1,2],[3,4]) = [1 2;3 4]; //creation
b([1,2],[2,3]) = [4,3;2,1]; //insertion
b([1,2],[4,5]) = [5 6;7 8]; //agrandissement
b
