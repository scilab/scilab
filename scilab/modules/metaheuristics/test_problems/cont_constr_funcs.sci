// G1  13  The global minima: x* = (1,1,,1,3,3,3,1), f(x*) = -15.
// G2  n   Best known: at n = 20,  f(x*) = 0.803619
// G3  n   The global minima: x* =  (1/n0.5, , 1/n0.5), f(x*) = 1
// G4  5   The global minima: x* =  (78,33,29.995,45,36.7758), f(x*) = -30665.539
// G5  4   Best known: x* =  (679.9453,1026,0.118876,-0.3962336), f(x*) = 5126.4981
// G6  2   The global minima: x* =  (14.095,0.84296), f(x*) = -6961.81388
// G7  10  The global minima: x*=(2.171996, 2.363683, 8.773926, 5.095984, 0.9906548, 1.430574,1.321644, 9.828726, 8.280092, 8.375927), f(x*) = 24.3062091
// G8  2   The global minima: x? = (1.2279713, 4.2453733) , f (x?) = 0.095825
// G9  7   The global minima: x? = (2.330499, 1.951372,?0.4775414, 4.365726,?0.6244870, 1.038131,1.594227), f (x?) = 680.6300573
// G10 8   The global minima: x? = (579.3167, 1359.943, 5110.071, 182.0174, 295.5985, 217.9799, 286.4162,395.5979), f (x?) = 7049.3307
// G11 2   The global minima: x* =  ±(1/2^0.5, 1/2), f(x*) = 0.75.
// G12 3   The global minima: x* =  (5, 5, 5), f(x*) = 1
// G13 5   The global minima: x? = (?1.717143, 1.595709, 1.827247, ?0.7636413, ?0.763645) , f (x?) = 0.0539498
// weldedbeam     4 ??
// pressurevessel 4 ??
// tensiomcompr   3 ??
// Electron on a sphere
// Largest small polygon
// Valve Opening constrained problem

////////////////
// G1 problem //
////////////////

function y = G1_x_init()
y = ones(13,1);
endfunction

function Res = min_bd_G1()
Res = zeros(13,1);
endfunction

function Res = max_bd_G1()
Res = ones(13,1);
Res($-3:$-1) = 100;
endfunction

function y = G1_obj(x)
// Matlab Code by A. Hedar (Nov. 23, 2005).
x1 = x(1:4); x2 = x(5:13);
y = 5*sum(x1)-5*sum(x1.*x1)-sum(x2);
endfunction

function y = G1_ineq(x)
// Matlab Code by A. Hedar (Nov. 23, 2005).
// Constraints
y(1) = 2*x(1)+2*x(2)+x(10)+x(11)-10;
y(2) = 2*x(1)+2*x(3)+x(10)+x(12)-10;
y(3) = 2*x(2)+2*x(3)+x(11)+x(12)-10;
y(4) = -8*x(1)+x(10);
y(5) = -8*x(2)+x(11);
y(6) = -8*x(3)+x(12);
y(7) = -2*x(4)-x(5)+x(10);
y(8) = -2*x(6)-x(7)+x(11);
y(9) = -2*x(8)-x(9)+x(12);
// Variable lower bounds
y(10) = -x(1);
y(11) = -x(2);
y(12) = -x(3);
y(13) = -x(4);
y(14) = -x(5);
y(15) = -x(6);
y(16) = -x(7);
y(17) = -x(8);
y(18) = -x(9);
y(19) = -x(10);
y(20) = -x(11);
y(21) = -x(12);
y(22) = -x(13);
// Variable upper bounds
y(23) = x(1)-1;
y(24) = x(2)-1;
y(25) = x(3)-1;
y(26) = x(4)-1;
y(27) = x(5)-1;
y(28) = x(6)-1;
y(29) = x(7)-1;
y(30) = x(8)-1;
y(31) = x(9)-1;
y(32) = x(10)-100;
y(33) = x(11)-100;
y(34) = x(12)-100;
y(35) = x(13)-1;
endfunction

function y = G1_eq(x)
y = 0;
endfunction

////////////////
// G2 problem //
////////////////

function y = G2_x_init(n)
if ~isdef('n','local') then n = 20; end
y = ones(n,1);
endfunction

function Res = min_bd_G2(n)
if ~isdef('n','local') then n = 20; end
Res = zeros(n,1);
endfunction

function Res = max_bd_G2(n)
if ~isdef('n','local') then n = 20; end
Res = 10*ones(n,1);
endfunction

function y = G2_obj(x)
// Matlab Code by A. Hedar (Nov. 23, 2005).
K = 1:length(x);
sum_jx = sum((K').*(x).^2);
y = -abs((sum(cos(x).^4)-2*prod(cos(x).^2))/sqrt(sum_jx));
endfunction

function y = G2_ineq(x)
// Matlab Code by A. Hedar (Nov. 23, 2005).
// Constraints
n = length(x);
y(1) = -prod(x)+0.75;
y(2) = sum(x)-7.5*n;
// Variable lower bounds
for j=1:n; y(j+2) = -x(j); end
// Variable upper bounds
for j=1:n; y(j+n+2) = x(j)-10; end
endfunction

function y = G2_eq(x)
y = 0;
endfunction

////////////////
// G3 problem //
////////////////

function y = G3_x_init(n)
if ~isdef('n','local') then n = 4; end
y = 0.5*ones(n,1);
endfunction

function Res = min_bd_G3(n)
if ~isdef('n','local') then n = 4; end
Res = zeros(n,1);
endfunction

function Res = max_bd_G3(n)
if ~isdef('n','local') then n = 4; end
Res = ones(n,1);
endfunction

function y = G3_obj(x)
// Matlab Code by A. Hedar (Nov. 23, 2005).
n = length(x);
y = -sqrt(n)^n*prod(x);
endfunction

function y = G3_ineq(x)
n = length(x);
// Variable lower bounds
for j=1:n; y(j) = -x(j); end
// Variable upper bounds
for j=1:n; y(j+n) = x(j)-1; end
endfunction

function y = G3_eq(x)
// Matlab Code by A. Hedar (Nov. 23, 2005).
// Constraints
y(1) = sum(x.^2)-1;
endfunction

////////////////
// G4 problem //
////////////////

function y = G4_x_init()
y = [80 40 40 40 40]';
endfunction

function Res = min_bd_G4()
Res = [78 33 27 27 27]';
endfunction

function Res = max_bd_G4()
Res = [102,45,45,45,45]';
endfunction

function y = G4_obj(x)
// Matlab Code by A. Hedar (Nov. 23, 2005).
y = 5.3578547*x(3)^2+0.8356891*x(1)*x(5)+37.293239*x(1)-40792.141;
endfunction

function y = G4_ineq(x)
// Matlab Code by A. Hedar (Nov. 23, 2005).
// Constraints
u = 85.334407+0.0056858*x(2)*x(5)+0.0006262*x(1)*x(4)-0.0022053*x(3)*x(5);
y(1) = -u;
y(2) = u-92;
v = 80.51249+0.0071317*x(2)*x(5)+0.0029955*x(1)*x(2)+0.0021813*x(3)^2;
y(3) = -v+90;
y(4) = v-110;
w = 9.300961+0.0047026*x(3)*x(5)+0.0012547*x(1)*x(3)+0.0019085*x(3)*x(4);
y(5) = -w+20;
y(6) = w-25;
// Variable lower bounds
y(7) = -x(1)+78;
y(8) = -x(2)+33;
y(9) = -x(3)+27;
y(10) = -x(4)+27;
y(11) = -x(5)+27;
// Variable upper bounds
y(12) = x(1)-102;
y(13) = x(2)-45;
y(14) = x(3)-45;
y(15) = x(4)-45;
y(16) = x(5)-45;
endfunction

function y = G4_eq(x)
y = 0;
endfunction

////////////////
// G5 problem //
////////////////

function y = G5_x_init()
y = [180 140 0 0]';
endfunction

function Res = min_bd_G5()
Res = [0 0 -0.55 -0.55]';
endfunction

function Res = max_bd_G5()
Res = [1200 1200 0.55 0.55]';
endfunction

function y = G5_obj(x)
// Matlab Code by A. Hedar (Nov. 23, 2005).
y = 3*x(1)+1e-6*x(1)^3+2*x(2)+2e-6/3*x(2)^3;
endfunction

function y = G5_constr(x)
// Matlab Code by A. Hedar (Nov. 23, 2005).
// Constraints
y(1) = x(3)-x(4)-0.55;
y(2) = x(4)-x(3)-0.55;
// Variable lower bounds
for j=1:2; y(j+2) = -x(j); end
for j=1:2; y(j+4) = -x(j+2)-0.55; end
// Variable upper bounds
for j=1:2; y(j+6) = x(j)-1200; end
for j=1:2; y(j+8) = x(j+2)-0.55; end
endfunction

function y = G5_eq(x)
y(1) = 1000*(sin(-x(3)-0.25)+sin(-x(4)-0.25))+894.8-x(1);
y(2) = 1000*(sin(x(3)-0.25)+sin(x(3)-x(4)-0.25))+894.8-x(2);
y(3) = 1000*(sin(x(4)-0.25)+sin(x(4)-x(3)-0.25))+1294.8;
endfunction

////////////////
// G6 problem //
////////////////

function y = G6_x_init()
y = [80 40]';
endfunction

function Res = min_bd_G6()
Res = [13 0]';
endfunction

function Res = max_bd_G6()
Res = [100 100]';
endfunction

function y = G6_obj(x)
// Matlab Code by A. Hedar (Nov. 23, 2005).
y = (x(1)-10)^3+(x(2)-20)^3;
endfunction

function y = G6_ineq(x)
// Matlab Code by A. Hedar (Nov. 23, 2005).
// Constraints
y(1) = (x(1)-5)^2+(x(2)-5)^2+100;
y(2) = (x(1)-5)^2+(x(2)-5)^2-82.81;
// Variable lower bounds
y(3) = -x(1)+13;
y(4) = -x(2);
// Variable upper bounds
y(5) = x(1)-100; 
y(6) = x(2)-100;
endfunction

function y = G6_eq(x)
y = 0;
endfunction

////////////////
// G7 problem //
////////////////

function y = G7_x_init()
y = zeros(10,1);
endfunction

function Res = min_bd_G7()
Res = -10*ones(10,1);
endfunction

function Res = max_bd_G7()
Res = 10*ones(10,1);
endfunction

function y = G7_obj(x)
// Matlab Code by A. Hedar (Nov. 23, 2005).
y = x(1)^2+x(2)^2+x(1)*x(2)-14*x(1)-16*x(2)+(x(3)-10)^2+...
    4*(x(4)-5)^2+(x(5)-3)^2+2*(x(6)-1)^2+5*x(7)^2+...
    7*(x(8)-11)^2+2*(x(9)-10)^2+(x(10)-7)^2+45;
endfunction
function y = G7_ineq(x)
// Matlab Code by A. Hedar (Nov. 23, 2005).
// Constraints
y(1) = 4*x(1)+5*x(2)-3*x(7)+9*x(8)-105;
y(2) = 10*x(1)-8*x(2)-17*x(7)+2*x(8);    
y(3) = -8*x(1)+2*x(2)+5*x(9)-2*x(10)-12; 
y(4) = 3*(x(1)-2)^2+4*(x(2)-3)^2+2*x(3)^2-7*x(4)-120;      
y(5) = 5*x(1)^2+8*x(2)+(x(3)-6)^2-2*x(4)-40;
y(6) = 0.5*(x(1)-8)^2+2*(x(2)-4)^2+3*x(5)^2-x(6)-30; 
y(7) = x(1)^2+2*(x(2)-2)^2-2*x(1)*x(2)+14*x(5)-6*x(6);       
y(8) = -3*x(1)+6*x(2)+12*(x(9)-8)^2-7*x(10);    
// Variable lower bounds
for j=1:10; y(j+8) = -x(j)-10; end
// Variable upper bounds
for j=1:10; y(j+18) = x(j)-10; end
endfunction

function y = G7_eq(x)
y = 0;
endfunction

////////////////
// G8 problem //
////////////////

function y = G8_x_init()
y = 5*ones(2,1);
endfunction

function Res = min_bd_G8()
Res = zeros(2,1);
endfunction

function Res = max_bd_G8()
Res = 10*ones(2,1);
endfunction

function y = G8_obj(x)
// Matlab Code by A. Hedar (Nov. 23, 2005).
y = -(sin(2*%pi*x(1))^3*sin(2*%pi*x(2)))/(x(1)^3*(x(1)+x(2)));
endfunction

function y = G8_constr(x)
// Matlab Code by A. Hedar (Nov. 23, 2005).
// Constraints
y(1) = x(1)^2-x(2)+1;
y(2) = 1-x(1)+(x(2)-4)^2;
// Variable lower bounds
y(3) = -x(1);
y(4) = -x(2);
// Variable upper bounds
y(5) = x(1)-10;
y(6) = x(2)-10;
endfunction

function y = G8_eq(x)
y = 0;
endfunction

////////////////
// G9 problem //
////////////////

function y = G9_x_init()
y = zeros(7,1);
endfunction

function Res = min_bd_G9()
Res = -10*ones(7,1);
endfunction

function Res = max_bd_G9()
Res = 10*ones(7,1);
endfunction

function y = G9_obj(x)
// Matlab Code by A. Hedar (Nov. 23, 2005).
y = (x(1)-10)^2+5*(x(2)-12)^2+x(3)^4+3*(x(4)-11)^2+...
    10*x(5)^6+7*x(6)^2+x(7)^4-4*x(6)*x(7)-10*x(6)-8*x(7);
endfunction

function y = G9_ineq(x)
// Matlab Code by A. Hedar (Nov. 23, 2005).
// Constraints
y(1) = 2*x(1)^2+3*x(2)^4+x(3)+4*x(4)^2+5*x(5)-127;
y(2) = 7*x(1)+3*x(2)+10*x(3)^2+x(4)-x(5)-282;
y(3) = 23*x(1)+x(2)^2+6*x(6)^2-8*x(7)-196;
y(4) = 4*vx(1)^2+x(2)^2-3*x(1)*x(2)+2*x(3)^2+5*x(6)-11*x(7);
// Variable lower bounds
for j=1:7; y(j+4) = -x(j)-10; end
// Variable upper bounds
for j=1:7; y(j+11) = x(j)-10; end
endfunction

function y = G9_eq(x)
y = 0;
endfunction

/////////////////
// G10 problem //
/////////////////

function y = G10_x_init()
y = [200 4000 4000 100 100 100 100 100]';
endfunction

function Res = min_bd_G10()
Res = 10*[10 100 100 1 1 1 1 1]';
endfunction

function Res = max_bd_G10()
Res = 1000*[10 10 10 1 1 1 1 1]';
endfunction

function y = G10_obj(x)
// Matlab Code by A. Hedar (Nov. 23, 2005).
y = x(1)+x(2)+x(3);
endfunction

function y = G10_ineq(x)
// Matlab Code by A. Hedar (Nov. 23, 2005).
// Constraints
y(1) = -1+0.0025*(x(4)+x(6));
y(2) = -1+0.0025*(-x(4)+x(5)+x(7));
y(3) = -1+0.01*(-x(5)+x(8));
y(4) = 100*x(1)-x(1)*x(6)+833.33252*x(4)-83333.333;
y(5) = x(2)*x(4)-x(2)*x(7)-1250*x(4)+1250*x(5);
y(6) = x(3)*x(5)-x(3)*x(8)-2500*x(5)+1250000;
// Variable upper bounds
y(7) = x(1)-10000;
y(8) = x(2)-10000;
y(9) = x(3)-10000;
y(10) = x(4)-1000;
y(11) = x(5)-1000;
y(12) = x(6)-1000;
y(13) = x(7)-1000;
y(14) = x(8)-1000;
// Variable lower bounds
y(15) = -x(1)+100;
y(16) = -x(2)+1000;
y(17) = -x(3)+1000;
y(18) = -x(4)+10;
y(19) = -x(5)+10;
y(20) = -x(6)+10;
y(21) = -x(7)+10;
y(22) = -x(8)+10;
endfunction

function y = G10_eq(x)
y = 0;
endfunction

/////////////////
// G11 problem //
/////////////////

function y = G11_x_init()
y = [0 0]';
endfunction

function Res = min_bd_G11()
Res = [-1 -1]';
endfunction

function Res = max_bd_G11()
Res = [1 1]';
endfunction

function y = G11_obj(x)
// Matlab Code by A. Hedar (Nov. 23, 2005).
y = x(1)^2+(x(2)-1)^2;
endfunction

function y = G11_ineq(x)
// Variable lower bounds
y(2) = -x(1)-1;
y(3) = -x(2)-1;
// Variable upper bounds
y(4) = x(1)-1;
y(5) = x(2)-1; 
endfunction

function y = G11_eq(x)
// Matlab Code by A. Hedar (Nov. 23, 2005).
//Constraints
y(1) = x(2)-x(1)^2;
endfunction

/////////////////
// G12 problem //
/////////////////

function y = G12_x_init()
y = [1 1 1]';
endfunction

function Res = min_bd_G12()
Res = [0 0 0]';
endfunction

function Res = max_bd_G12()
Res = [10 10 10]';
endfunction

function y = G12_obj(x)
// Matlab Code by A. Hedar (Nov. 23, 2005).
y = (1-0.01*((x(1)-5)^2+(x(2)-5)^2+(x(3)-5)^2));
endfunction

function y = G12_ineq(x)
// Matlab Code by A. Hedar (Nov. 23, 2005).
// Constraints
Index = 1;
for p=1:9
  for q=1:9
    for r=1:9
      y(Index) = (x(1)-p)^2+(x(2)-q)^2+(x(3)-r)^2-0.0625;
      Index = Index + 1;
    end
  end
end
endfunction

function y = G12_eqq_constr(x)
y = 0;
endfunction

/////////
// G13 //
/////////

function y = G13_x_init()
y = [0 0 0 0 0]';
endfunction

function Res = min_bd_G13()
Res = - [2.3 2.3 3.2 3.2 3.2]';
endfunction

function Res = max_bd_G13()
Res = [2.3 2.3 3.2 3.2 3.2]';
endfunction

function y = G13_obj(x)
// Matlab Code by A. Hedar (Nov. 23, 2005).
y = exp(prod(x));
endfunction 

function y = G13_ineq(x)
// Variable lower bounds
for j=1:2; y(j) = -x(j)-2.3; end
for j=3:5; y(j) = -x(j)-3.2; end
// Variable upper bounds
for j=1:2; y(j+5) = x(j)-2.3; end
for j=3:5; y(j+7) = x(j)-3.2; end
endfunction

function y = G13_eq(x)
// Matlab Code by A. Hedar (Nov. 23, 2005).
// Constraints
y(1) = sum(x.^2)-10;
y(2) = x(2)*x(3)-5*x(4)*x(5);
y(3) = x(1)^3+x(2)^3+1;
endfunction

/////////////////////////
// Welded beam problem //
/////////////////////////

function y = weldedbeam_x_init()
y = zeros(4,1);
endfunction

function Res = min_bd_weldedbeam()
Res = [0.125 0.1 0.1 0.1]';
endfunction

function Res = max_bd_weldedbeam()
Res = [10 10 10 10]';
endfunction

function y = weldedbeam_obj(x)
// Matlab Code by A. Hedar (Nov. 23, 2005).
y = 1.10471*x(1)^2*x(2)+0.04811*x(3)*x(4)*(14.0+x(2));
endfunction

function y = weldedbeam_ineq(x)
// Matlab Code by A. Hedar (Nov. 23, 2005).
// Parameters
P = 6000; L = 14; E = 30e+6; G = 12e+6;
t_max = 13600; s_max = 30000; d_max = 0.25;
M = P*(L+x(2)/2); R = sqrt(0.25*(x(2)^2+(x(1)+x(3))^2));
J = 2/sqrt(2)*x(1)*x(2)*(x(2)^2/12+0.25*(x(1)+x(3))^2);
P_c = (4.013*E/(6*L^2))*x(3)*x(4)^3*(1-0.25*x(3)*sqrt(E/G)/L);
t1 = P/(sqrt(2)*x(1)*x(2)); t2 = M*R/J;
t = sqrt(t1^2+t1*t2*x(2)/R+t2^2);
s = 6*P*L/(x(4)*x(3)^2);
d = 4*P*L^3/(E*x(4)*x(3)^3);
// Constraints
y(1) = t-t_max;
y(2) = s-s_max;
y(3) = x(1)-x(2);
y(4) = d-d_max;
y(5) = P-P_c;
// Variable lower bounds
y(6) = -x(1)+0.125;
y(7) = -x(2)+0.1;
y(8) = -x(3)+0.1;
y(9) = -x(4)+0.1;
// Variable upper bounds
y(10) = x(1)-10;
y(11) = x(2)-10;
y(12) = x(3)-10;
y(13) = x(4)-10;
endfunction

function y = weldedbeam_eq(x)
y = 0;
endfunction

/////////////////////////////
// Pressure vessel problem //
/////////////////////////////

function y = pressurevessel_x_init()
y = zeros(4,1);
endfunction

function Res = min_bd_pressurevessel()
Res = -1e6 * ones(4,1);
endfunction

function Res = max_bd_pressurevessel()
Res = 1e6 * ones(4,1);
endfunction

function y = pressurevessel_obj(x)
// Matlab Code by A. Hedar (Nov. 23, 2005).
y = 0.6224*x(1)*x(3)*x(4)+1.7781*x(2)*x(3)^2+3.1661*x(1)^2*x(4)+19.84*x(1)^2*x(3);
endfunction

function y = pressurevessel_ineq(x)
// Matlab Code by A. Hedar (Nov. 23, 2005).
// Constraints
y(1) = -x(1)+0.0193*x(3);
y(2) = -x(2)+0.00954*x(3);
y(3) = -%pi*x(3)^2*x(4)-(4/3)*%pi*x(3)^3+1296000;
y(4) = x(4)-240;
endfunction

function y = pressurevessel_eq(x)
y = 0;
endfunction

////////////////////////////////////////
// Tension compression spring problem //
////////////////////////////////////////

function y = tensioncompr_x_init()
y = zeros(3,1);
endfunction

function Res = min_bd_tensioncompr()
Res = [0 0 0]';
endfunction

function Res = max_bd_tensioncompr()
Res = [100 100 100]';
endfunction

function y = tensioncompr_obj(x)
// Matlab Code by A. Hedar (Nov. 23, 2005).
y = x(1)^2*x(2)*(x(3)+2);
endfunction

function y = tensioncompr_ineq(x)
// Matlab Code by A. Hedar (Nov. 23, 2005).
// Constraints
y(1) = 1-(x(2)^3*x(3))/(71.785*x(1)^4);
y(2) = (4*x(2)^2-x(1)*x(2))/(12.566*x(1)^3*(x(2)-x(1)))+1/(5.108*x(1)^2)-1;
y(3) = 1-140.45*x(1)/(x(3)*x(2)^2);
y(4) = (x(1)+x(2))/1.5-1;
y(5) = -x(1);
y(6) = -x(2);
y(7) = -x(3);
y(8) = x(1)-100;
y(9) = x(2)-100;
y(10) = x(3)-100;
endfunction

function y = tensioncompr_eq(x)
y = 0;
endfunction

///////////////////////////
// Electron on a sphere  //
// Nb var : user defined //
// x_i between 0 and 1   //
// each x_i is of size 3 //
///////////////////////////

function y = elec_on_sph_x_init(n)
if ~isdef('n','local') then n = 10; end
y = zeros(n,1);
endfunction
function Res = min_bd_elec_on_sph(n)
if ~isdef('n','local') then n = 10; end
Res = -2 * ones(n,1);
endfunction
function Res = max_bd_elec_on_sph(n)
if ~isdef('n','local') then n = 10; end
Res = 2 * ones(n,1);
endfunction

function [value] = elec_on_sph_obj(x)
nb_var = size(x,1);
value  = 0.0;
for i=1:nb_var-1
  for j=i+1:nb_var
    value = value + sqrt((x(i,1)-x(j,1))^2 + (x(i,2) - x(j,2))^2 + (x(i,3) - x(j,3))^2);
  end
end
value = - value;
endfunction

function [diff_bound] = elec_on_sph_ineq(x)
diff_bound = 0;
endfunction

function [diff_bound] = elec_on_sph_eq(x)
// xi^2 + yi^2 + zi^2 = 1 (nb_var equality constraints)
nb_var = size(x,1);
diff_bound = zeros(nb_var,1);
Index = 1;
for i=1:nb_var
  diff_bound(Index) = x(i,1)^2 + x(i,2)^2 + x(i,3)^2 - 1;
  Index = Index + 1;
end
endfunction

///////////////////////////////
// Largest smal polygon      //
// Nb var : user defined     //
// r_i between 0 and 1       //
// theta_i between 0 and %pi //
///////////////////////////////

function y = larg_sm_poly_x_init(n)
if ~isdef('n','local') then n = 10; end
y = zeros(2*n,1);
endfunction

function Res = min_bd_larg_sm_poly(n)
if ~isdef('n','local') then n = 10; end
Res = -10 * ones(2*n,1);
endfunction

function Res = max_bd_larg_sm_poly(n)
if ~isdef('n','local') then n = 10; end
Res = 10 * ones(2*n,1);
endfunction

function [value] = larg_sm_poly_obj(r, theta)
// value = 0.5 * sum from i=1 to nv r_{i+1} * r_{i} * sin(theta_{i+1} - theta_{i})
value = 0.0;
for i=1:size(r,1)-1
  value = value + r(i+1)*r(i)*sin(theta(i+1) - theta(i));
end
value = value * 0.5;
endfunction

function [diff_bound] = larg_sm_poly_ineq(r, theta)
nb_var = size(r,1);
// r_i^2 + r_j^2 - 2*r_i * r_j * cos(theta_i - theta_j) <=1 (nb_var*(nb_var-1) constraints)
// theta_i <= theta_i+1 (nb_var - 1 constraints)
// theta_i >= 0         (nb_var constraints)
// theta_i <= %pi       (nb_var constraints)
// r_i >= 0             (nb_var constraints)
// r_i <= 1             (nb_var constraints)
diff_bound = zeros(nb_var*(nb_var-1) + nb_var-1 + 2*nb_var + 2*nb_var,1);
Index = 1;
for i=1:nb_var
  for j=1:nb_var;
    if (i==j) then continue; end
    diff_bound(Index) = r(i)^2 + r(j)^2 - 2*r(i)*r(j)*cos(theta(i) - theta(j)) - 1;
    Index = Index + 1;
  end
end
for i=1:nb_var-1
  diff_bound(Index) = theta(i) - theta(i+1);
  Index = Index + 1;
end
for i=1:nb_var
  diff_bound(Index) = -theta(i);
  Index = Index + 1;
end
for i=1:nb_var
  diff_bound(Index) = theta(i) - %pi;
  Index = Index + 1;
end
for i=1:nb_var
  diff_bound(Index) = -r(i);
  Index = Index + 1;
end
for i=1:nb_var
  diff_bound(Index) = r(i) - 1;
  Index = Index + 1;
end
endfunction

function [diff_bound] = larg_sm_poly_eq(r, theta)
diff_bound = 0;
endfunction

///////////////////////////////////////
// Valve Opening constrained problem //
// Nb var : user defined             //
// r_i : around 1                    //
///////////////////////////////////////

function y = vv_open_x_init(n)
if ~isdef('n','local') then n = 10; end
y = zeros(n,1);
endfunction

function Res = min_bd_vv_open(n)
if ~isdef('n','local') then n = 10; end
Res = -10 * ones(n,1);
endfunction

function Res = max_bd_vv_open(n)
if ~isdef('n','local') then n = 10; end
Res = 10 * ones(n,1);
endfunction

function [value] = vv_open_obj(r)
value = mean(r);
value = value * %pi * rv;
endfunction

function [diff_bound] = vv_open_ineq(r)
// 2*r_{i-1}*r_{i+1}*cos(theta) <= r_{i}*(r_{i-1} - r_{i+1})
// -alpha <= (r_{i+1} - r_{i})/theta <= alpha
nb_var = length(r);
alpha = 1.5;
theta = 2*%pi/(5*nb_var+1);
rmin  = 1;
rmax  = 2;
Index = 1;
for i=2:nb_var-1
  diff_bound(Index) = 2*r(i-1)*r(i+1)*cos(theta) - r(i)*(r(i-1)+r(i+1));
  Index = Index + 1;
end
diffbound(Index) =  2*rmin*r(1)*cos(theta) - rmin*(rmin+r(1));
Index = Index + 1;
diffbound(Index) =  2*rmin*r(2)*cos(theta) - r(1)*(rmin+r(2));
Index = Index + 1;
diffbound(Index) =  2*r(nb_var-1)*rmax*cos(theta) - r(nb_var)*(r(nb_var-1)+rmax);
Index = Index + 1;
diffbound(Index) =  2*r(nb_var)*rmax*cos(theta) - rmax*(r(nb_var)+rmax);
Index = Index + 1;
for i=1:nb_var-1
  diff_bound(Index) = (r(i+1) - r(i)) / theta - alpha;
  Index = Index + 1;
end
for i=1:nb_var-1
  diff_bound(Index) = - (r(i+1) - r(i)) / theta + alpha;
  Index = Index + 1;
end
endfunction

function [diff_bound] = vv_open_ineq(r)
diff_bound = 0;
endfunction

//
// Himmelblau problem 1
// Nonfeasible starting point X = [2 2]';
// Solution: X*=[0.823 0.911];
//

function y = himmelblau_1_x_init()
y = zeros(2,1);
endfunction

function Res = min_bd_himmelblau_1()
Res = -10 * ones(2,1);
endfunction

function Res = max_bd_himmelblau_1()
Res = 10 * ones(2,1);
endfunction

function y = himmelblau_1_obj(x)
y = (x(1) - 2)^2 + (x(2) - 1)^2;
endfunction

function diff_bound = himmelblau_1_ineq(x)
diff_bound(1) = x(1) - 2*x(2) + 1;
diff_bound(2) = x(1)^2/4 + x(2)^2 - 1;
endfunction

function diff_bound = himmelblau_1_eq(x)
diff_bound = 0;
endfunction

//
// Himmelblau problem 3
// Nonfeasible starting point: X=[90 10]';
// Solution X*=[75 65]';
//

function y = himmelblau_3_x_init()
y = [90;10];
endfunction

function Res = min_bd_himmelblau_3()
Res = -1e6 * ones(2,1);
endfunction

function Res = max_bd_himmelblau_3()
Res = 1e6 * ones(2,1);
endfunction

function y = himmelblau_3_obj(x)
y = 75.196 - 3.8112*x(1) + 0.12694*x(1)^2 - 2.0576D-3*x(1)^3 + 1.0345D-5*x(1)^4 ...
    - 6.8306*x(2) + 0.030234*x(1)*x(2) - 1.28134D-3*x(2)*x(1)^2 + 3.5256D-5*x(2)*x(1)^3 - 2.266D-7*x(2)*x(1)^4 ...
    + 0.25645*x(2)^2 - 3.4604D-3*x(2)^3 + 1.35141D-5*x(2)^4 - 28.106/(x(2)+1) - 5.2375D-6*x(1)^2*x(2)^2 - 6.3D-8*x(1)^3*x(2)^2 ...
    + 7D-10*x(1)^3*x(2)^3 + 3.4054D-4*x(1)*x(2)^2 - 1.6638D-6*x(1)*x(2)^3 - 2.8673*exp(0.0005*x(1)*x(2));
endfunction

function diff_bound = himmelblau_3_ineq(x)
// 0 <= x1 <= 75
// 0 <= x1 <= 65
// x1*x2 - 700  >= 0
// x2 - 5*(x1/25)^2 >= 0
// (x2 - 50)^2 - 5*(x1 - 55) >= 0
diff_bound(1) = -x(1);
diff_bound(2) = x(1) - 75;
diff_bound(3) = -x(2);
diff_bound(4) = x(2) - 65;
diff_bound(5) = -x(1)*x(2) - 700;
diff_bound(6) = - x(2) + 5*(x(1)/25)^2;
diff_bound(7) = - (x(2) - 50)^2 + 5*(x(1) - 55);
endfunction

function diff_bound = himmelblau_3_eq(x)
diff_bound = 0;
endfunction

//
// Himmelblau problem 4
// Non feasible starting point xi = 0.1
// Solution X* = [0.035 0.1142 0.8306 0.0012 0.4887 0.0005 0.0209 0.0157 0.0289 0.0751]';
//

function y = himmelblau_4_x_init()
y = 0.1*ones(10,1);
endfunction

function Res = min_bd_himmelblau_4()
Res = -1e6 * ones(10,1);
endfunction

function Res = max_bd_himmelblau_4()
Res = 1e6 * ones(10,1);
endfunction

function y = himmelblau_4_obj(x)
C = - [6.089 17.164 34.054 5.914 24.721 14.986 24.1 10.708 26.662 22.179]';
y = sum(x.*(C + log(abs(x/sum(x)))));
endfunction

function diff_bound = himmelblau_4_ineq(x)
// h1(x) = x1 + 2x2 + 2x3 + x6 + x10 - 2 = 0
// h2(x) = x4 + 2x5 + x6 + x7 - 1 = 0
// h3(x) = x3 + x7 + x8 + 2x9 + x10 - 1 = 0
// -xi <= 0
diff_bound(1) = x(1) + 2*x(2) + 2*x(3) + x(6) + x(10) - 2;
diff_bound(2) = x(4) + 2*x(5) + x(6) + x(7) - 1;
diff_bound(3) = x(3) + x(7) + x(8) + 2*x(9) + x(10) - 1;
for i=1:10
  diff_bound(3+i) = -x(i);
end
endfunction

function diff_bound = himmelblau_4_eq(x)
diff_bound = 0;
endfunction

//
// Himmelblau problem 4a
// Non feasible starting point xi = -2.3
// Solution X* = [0.035 0.1142 0.8306 0.0012 0.4887 0.0005 0.0209 0.0157 0.0289 0.0751]';
//

function y = himmelblau_4a_x_init()
y = -2.3*ones(10,1);
endfunction

function Res = min_bd_himmelblau_4a()
Res = -1e6 * ones(10,1);
endfunction

function Res = max_bd_himmelblau_4a()
Res = 1e6 * ones(10,1);
endfunction

function y = himmelblau_4a_obj(x)
C = - [6.089 17.164 34.054 5.914 24.721 14.986 24.1 10.708 26.662 22.179]';
y = sum(exp(x).*(C + x - log(sum(exp(x)))));
endfunction

function diff_bound = himmelblau_4a_ineq(x)
diff_bound(1) = exp(x(1)) + 2*exp(x(2)) + 2*exp(x(3)) + 2*exp(x(4)) + exp(x(6)) + exp(x(10)) - 2;
diff_bound(2) = exp(x(4)) + 2*exp(x(5)) + exp(x(6)) + exp(x(7)) - 1;
diff_bound(3) = exp(x(3)) + exp(x(7)) + exp(x(8)) + 2*exp(x(9)) + exp(x(10)) - 1;
endfunction

function diff_bound = himmelblau_4a_eq(x)
diff_bound = 0;
endfunction

//
// Himmelblau problem 5
// Nonfeasible starting point: X = [2 2 2] or [10 10 10];
//

function y = himmelblau_5_x_init()
y = 10*ones(3,1);
endfunction

function Res = min_bd_himmelblau_5()
Res = -1e6 * ones(3,1);
endfunction

function Res = max_bd_himmelblau_5()
Res = 1e6* ones(3,1);
endfunction

function y = himmelblau_5_obj(x)
y = 1000 - x(1)^2 - 2*x(2)^2 - x(3)^2 - x(1)*x(2) - x(1)*x(3);
endfunction

function diff_bound = himmelblau_5_ineq(x)
// h1(x) = x(1)^2 + x(2)^2 + x(3)^2 - 25 = 0
// h2(x) = 8*x(1) + 14*x(2) + 7*x(3) - 56 = 0
diff_bound(1) = x(1)^2 + x(2)^2 + x(3)^2 - 25;
diff_bound(2) = 8*x(1) + 14*x(2) + 7*x(3) - 56;
endfunction

function diff_bound = himmelblau_5_eq(x)
diff_bound = 0;
endfunction

//
// Himmelblau problem 8
// Feasible starting point X = -3 -1 -3 -1]'
// Solution X* = [1 1 1 1]'
//

function y = himmelblau_8_x_init()
y = [-3;-1;-3;-1];
endfunction

function Res = min_bd_himmelblau_8()
Res = -1e6 * ones(4,1);
endfunction

function Res = max_bd_himmelblau_8()
Res = 1e6 * ones(4,1);
endfunction

function y = himmelblau_8_obj(x)
y = 100*(x(2) - x(1)^2)^2 + (1 - x(1))^2 + 90*(x(4) - x(3)^2)^2 + ...
    (1 - x(3))^2 + 10.1*((x(2)-1)^2 + (x(4) - 1)^2) + 19.8*(x(2)-1)*(x(4)-1);
endfunction

function diff_bound = himmelblau_8_ineq(x)
// -10 <= xi <= 10
diff_bound(1) = - x(1) - 10;
diff_bound(2) = - x(2) - 10;
diff_bound(3) = - x(3) - 10;
diff_bound(4) = - x(4) - 10;

diff_bound(5) = x(1) - 10;
diff_bound(6) = x(2) - 10;
diff_bound(7) = x(3) - 10;
diff_bound(8) = x(4) - 10;
endfunction

function diff_bound = himmelblau_8_eq(x)
diff_bound = 0;
endfunction

//
// Himmelblau problem 9
// Starting point : X = [2 4 0.04 2]'
// Solution X*=[12.277 4.632 0.313 2.029]'
//

function y = himmelblau_9_x_init()
y = [2 4 0.04 2]';
endfunction

function Res = min_bd_himmelblau_9()
Res = -1e6 * ones(4,1);
endfunction

function Res = max_bd_himmelblau_9()
Res = 1e6 * ones(4,1);
endfunction

function y = himmelblau_9_obj(x)
C = [0.1 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18]';
y_obs = [0.00189 0.1038 0.268 0.506 0.577 0.604 0.725 0.898 0.947 0.845 0.702 0.528 0.385 0.257 0.159 0.0869 0.0453 0.01509 0.00189]';

_beta = x(3) + (1 - x(3))*x(4);

for i=1:length(C)
  y_cal(i) = x(3)*_beta^x(2)*sqrt(x(2)/6.2832)*(C(i)/7.658)^(x(2) - 1)*exp(x(2) - _beta*C(i)*x(2)/7.658) / (1 + 1/(12*x(2))) ...
           + (1-x(3))*(_beta/x(4))^x(1)*sqrt(x(1)/6.2832)*(C(i)/7.658)^(x(1)-1)*exp(x(1)-_beta*C(i)*x(1)/(7.658*x(4))) / (1 + 1/(12*x(1)));
end
y = sum((y_cal - y_obs).^2);
endfunction

function diff_bound = himmelblau_9_ineq(x)
// x3 + (1-x3)*x4 >= 0
// x4 > 0.1
// x3 <= 1
diff_bound(1) = - x(3) - (1-x(3))*x(4);
diff_bound(2) = - x(4) + 0.1;
diff_bound(3) = x(3) - 1;
endfunction

function diff_bound = himmelblau_9_eq(x)
diff_bound = 0;
endfunction

//
// Himmelblau problem 10
// Feasible starting point X = [0 0 0 0 1]'
// Solution X* = [0.3 0.3335 0.4 0.4285 0.224]'
//

function y = himmelblau_10_x_init()
y = [0 0 0 0 1]';
endfunction

function Res = min_bd_himmelblau_10()
Res = -1e6 * ones(5,1);
endfunction

function Res = max_bd_himmelblau_10()
Res = 1e6 * ones(5,1);
endfunction

function y = himmelblau_10_obj(x)
E = -[15 27 36 18 12];
C = [ 30 -20 -10  32 -10 ; ...
     -20  39 -6  -31  32 ; ...
     -10 -6   10 -6  -10 ; ...
      32 -31 -6   39 -20 ; ...
     -10  32 -10 -20  30];
D = [ 4 8 10 6 2];
y = 0;
for j=1:5
  Aux = 0;
  for i=1:5
    Aux = Aux + C(i,j)*x(i)*x(j);
  end
  y = y + E(j)*x(j) + D(j)*x(j)^3 + Aux;
end
endfunction

function diff_bound = himmelblau_10_ineq()
A = [-16  2  0  1  0   ; ...
      0  -2  0  .4 2   ; ...
     -3.5 0  2  0  0   ; ...
      0  -2  0 -4 -1   ; ...
      0  -9 -2  1 -2.8 ; ...
      2   0 -4  0  0   ; ...
     -1  -1 -1 -1 -1   ; ...
     -1  -2 -3 -2 -1   ; ...
      1   2  3  4  5   ; ...
      1   1  1  1  1];
B = [-40 -2 -0.25 -4 -4 -1 -40 -60 5 1];

diff_bound = zeros(20,1);

for i=1:10
  diff_bound(i) = 0;
  for j=1:5
    diff_bound(i) = diff_bound(i) - A(i,j)*x(j);
  end
  diff_bound(i) = diff_bound(i) + B(i);
end
for i=1:5
  diff_bound(i+10) = - x(i);
end
endfunction

function diff_bound = himmelblau_10_eq(x)
diff_bound = 0;
endfunction

//
// Himmelblau problem 11
// Feasible starting point X = [78.62 33.44 31.07 44.18 35.22]'
// Solution X* = [78 33 29.995 45 36.776]
// Non feasible starting point X = [78 33 27 27 27]'
// 

function y = himmelblau_11_x_init()
y = [78.62 33.44 31.07 44.18 35.22]';
endfunction

function Res = min_bd_himmelblau_11()
Res = -1e6 * ones(5,1);
endfunction

function Res = max_bd_himmelblau_11()
Res = 1e6 * ones(5,1);
endfunction

function y = himmelblau_11_obj(x)
y = 5.3578547*x(3)^2 + 0.8356891*x(1)*x(5) + 37.293239*x(1) - 40792.141;
endfunction

function diff_bound = himmelblau_11_ineq(x)
diff_bound(1) = 85.334407 + 0.0056858*x(2)*x(5) + 0.0006262*x(1)*x(4) - 0.0022053*x(3)*x(5) - 92;
diff_bound(2) = - 85.334407 - 0.0056858*x(2)*x(5) - 0.0006262*x(1)*x(4) + 0.0022053*x(3)*x(5);
diff_bound(3) = 80.51249 + 0.0071317*x(2)*x(5) + 0.0029955*x(1)*x(2) + 0.0021813*x(3)^2 - 110;
diff_bound(4) = 20 - 80.51249 - 0.0071317*x(2)*x(5) - 0.0029955*x(1)*x(2) - 0.0021813*x(3)^2;
diff_bound(5) = 9.300961 + 0.0047026*x(3)*x(5) + 0.0012547*x(1)*x(3) + 0.0019085*x(3)*x(4) - 25;
diff_bound(6) = 20 - 9.300961 - 0.0047026*x(3)*x(5) - 0.0012547*x(1)*x(3) - 0.0019085*x(3)*x(4);
diff_bound(7) = x(1) - 102;
diff_bound(8) = 78 - x(1);
diff_bound(9) = x(2) - 45;
diff_bound(10) = 33 - x(2);
diff_bound(11) = x(3) - 45;
diff_bound(12) = 27 - x(3);
diff_bound(13) = x(4) - 45;
diff_bound(14) = 27 - x(4);
diff_bound(15) = x(5) - 45;
diff_bound(16) = 27 - x(5);
endfunction

function diff_bound = himmelblau_11_eq(x)
diff_bound = 0;
endfunction

//
// Himmelblau problem 12
// Feasible starting point X=[900 80 115 262 27]'
// Solution X* = [705.060 68.6 102.9 282.341 35.627]'
//

function y = himmelblau_12_x_init()
y = [900 80 115 262 27]';
endfunction

function Res = min_bd_himmelblau_12()
Res = -1e6 * ones(5,1);
endfunction

function Res = max_bd_himmelblau_12()
Res = 1e6 * ones(5,1);
endfunction

function y_out = himmelblau_12_obj(x)
y1 = x(2) + x(3) + 41.6;
c1 = 0.024*x(4) - 4.62;
y2 = 12.5 / c1 + 12;
c2 = 0.0003535*x(1)^2 + 0.5311*x(1) + 0.08705*y2*x(1);
c3 = 0.052*x(1) + 78 + 0.002377*y2*x(1);
y3 = c2/c3;
y4 = 19*y3;
c4 = 0.04782*(x(1)-y3) + 0.1956*(x(1) - y3)^2 / x(2) + 0.6376*y4 + 1.594*y3;
c5 = 100*x(2);
c6 = x(1) - y3 - y4;
c7 = 0.95 - c4 / c5;
y5 = c6*c7;
y6 = x(1) - y5 - y4 - y3;
c8 = (y5 + y4)*0.995;
y7 = c8 / y1;
y8 = c8 / 3798;
c9 = y7 - 0.0663*y7/y8 - 0.3153;
y9 = 96.82 / c9 + 0.31*y1;
y10 = 1.29*y5 + 1.258*y4 + 2.29*y3 + 1.71*y6;
y11 = 1.71*x(1) - 0.452*y4 + 0.58*y3;
c10 = 12.3 / 752.3;
c11 = 1.75*y2*0.995*x(1);
c12 = 0.995*y10 + 1998; 
y12 = c10*x(1) + c11 / c12;
y13 = c12 - 1.75*y2;
y14 = 3723 + 64.4*x(2) + 58.4*x(3) + 146.312 / (y9 + x(5));
c13 = 0.995*y10 + 60.8*x(2) + 48*x(4) - 0.1121*y14 - 5095;
y15 = y13 / c13;
y16 = 148000 - 331000*y15 + 40*y13 - 61*y15*y13;
c14 = 2324*y10 -28740000*y2;
y17 = 14130000 - 1328*y10 - 531*y11 + c14 / c12;
c15 = y13 / y15 - y13/0.52;
c16 = 1.104 - 0.72*y15;
c17 = y9 + x(5);

y_out = 0.0000005843*y17 - 0.000117*y14 - 0.1365 - 0.00002358*y13 - 0.000001502*y16 - 0.0321*y12 - 0.004324*y5 - 0.0001*c15/c16 - 37.48*y2 / c12;
endfunction

function diff_bound = himmelblau_12_ineq(x)
y1 = x(2) + x(3) + 41.6;
c1 = 0.024*x(4) - 4.62;
y2 = 12.5 / c1 + 12;
c2 = 0.0003535*x(1)^2 + 0.5311*x(1) + 0.08705*y2*x(1);
c3 = 0.052*x(1) + 78 + 0.002377*y2*x(1);
y3 = c2/c3;
y4 = 19*y3;
c4 = 0.04782*(x(1)-y3) + 0.1956*(x(1) - y3)^2 / x(2) + 0.6376*y4 + 1.594*y3;
c5 = 100*x(2);
c6 = x(1) - y3 - y4;
c7 = 0.95 - c4 / c5;
y5 = c6*c7;
y6 = x(1) - y5 - y4 - y3;
c8 = (y5 + y4)*0.995;
y7 = c8 / y1;
y8 = c8 / 3798;
c9 = y7 - 0.0663*y7/y8 - 0.3153;
y9 = 96.82 / c9 + 0.31*y1;
y10 = 1.29*y5 + 1.258*y4 + 2.29*y3 + 1.71*y6;
y11 = 1.71*x(1) - 0.452*y4 + 0.58*y3;
c10 = 12.3 / 752.3;
c11 = 1.75*y2*0.995*x(1);
c12 = 0.995*y10 + 1998; 
y12 = c10*x(1) + c11 / c12;
y13 = c12 - 1.75*y2;
y14 = 3723 + 64.4*x(2) + 58.4*x(3) + 146.312 / (y9 + x(5));
c13 = 0.995*y10 + 60.8*x(2) + 48*x(4) - 0.1121*y14 - 5095;
y15 = y13 / c13;
y16 = 148000 - 331000*y15 + 40*y13 - 61*y15*y13;
c14 = 2324*y10 -28740000*y2;
y17 = 14130000 - 1328*y10 - 531*y11 + c14 / c12;
c15 = y13 / y15 - y13/0.52;
c16 = 1.104 - 0.72*y15;
c17 = y9 + x(5);

diff_bound(1) = -y4 + 0.28/0.72*y5;
diff_bound(2) = -1.5*x(2) + x(3);
diff_bound(3) = -21 + 3496*y2 / c12;
diff_bound(4) = - 62.212 / c17 + 110.6 + y1;

diff_bound(5)  = y1 - 405.23;
diff_bound(6)  = y2 - 1053.6667;
diff_bound(7)  = y3 - 35.03;
diff_bound(8)  = y4 - 665.585;
diff_bound(9)  = y5 - 584.463;
diff_bound(10) = y6 - 265.916;
diff_bound(11) = y7 - 7.046;
diff_bound(12) = y8 - 0.222;
diff_bound(13) = y9 - 273.366;
diff_bound(14) = y10 - 1286.105;
diff_bound(15) = y11 - 1444.046;
diff_bound(16) = y12 - 537.141;
diff_bound(17) = y13 - 3247.039;
diff_bound(18) = y14 - 26844.086;
diff_bound(19) = y15 - 0.386;
diff_bound(20) = y16 - 140;
diff_bound(21) = y17 - 12146108;
diff_bound(22) = x(1) - 906.3855;
diff_bound(23) = x(2) - 288.88;
diff_bound(24) = x(3) - 134.75;
diff_bound(25) = x(4) - 287.0966;
diff_bound(26) = x(5) - 84.1988;
diff_bound(27) = 213.1    - y1;
diff_bound(28) = 17.505   - y2;
diff_bound(29) = 11.275   - y3;
diff_bound(30) = 214.228  - y4;
diff_bound(31) = 7.458    - y5;
diff_bound(32) = 0.961    - y6;
diff_bound(33) = 1.612    - y7;
diff_bound(34) = 0.146    - y8;
diff_bound(35) = 107.99   - y9;
diff_bound(36) = 922.693  - y10;
diff_bound(37) = 926.832  - y11;
diff_bound(38) = 18.766   - y12;
diff_bound(39) = 1072.163 - y13;
diff_bound(40) = 8961.448 - y14;
diff_bound(41) = 0.063    - y15;
diff_bound(42) = 71084.33 - y16;
diff_bound(43) = 2802713  - y17;
diff_bound(44) = 704.4148 - x(1);
diff_bound(45) = 68.6     - x(2);
diff_bound(46) = 0        - x(3);
diff_bound(47) = 193      - x(4);
diff_bound(48) = 25       - x(5);
endfunction

function diff_bound = himmelblau_12_eq(x)
diff_bound = 0;
endfunction

//
// Himmelblau problem 13
// Starting point: X = [2.52 2 37.5 9.25 6.8]'
// Solution X* = [4.538 2.4 60 9.3 7]'
//

function y = himmelblau_13_x_init()
y = [2.52 2 37.5 9.25 6.8]';
endfunction

function Res = min_bd_himmelblau_13()
Res = -1e6 * ones(5,1);
endfunction

function Res = max_bd_himmelblau_13()
Res = 1e6 * ones(5,1);
endfunction

function y = himmelblau_13_obj(x)
K = [-145421.402 2931.1506 -40.427932 5106.192 15711.36 -161622.577 4176.15328 2.8260078 9200.476 13160.295 -21686.9194 123.56928 ...
     -21.1188894 706.934 2898.573 28298.388 60.81096 31.242116 329.574 -2882.082 74095.3845 -306.262544 16.243649 -3094.252 -5566.2628 ...
     -26.237 99 -0.42 1300 2100 925548.252 -61968.8432 23.3088196 -27097.648 -50843.766];
     
x6 = (K(1) + K(2)*x(2) + K(3)*x(3) + K(4)*x(4) + K(5)*x(5))*x(1);
y1 = K(6) + K(1)*x(2) + K(8)*x(3) + K(9)*x(4) + K(10)*x(5);
y2 = K(11) + K(12)*x(2) + K(13)*x(3) + K(14)*x(4) + K(15)*x(5);
y3 = K(16) + K(17)*x(2) + K(18)*x(3) + K(19)*x(4) + K(20)*x(5);
y4 = K(21) + K(22)*x(2) + K(23)*x(3) + K(24)*x(4) + K(25)*x(5);
x7 = (y1 + y2 + y3)*x(1);
x8 = (K(26) + K(27)*x(2) + K(28)*x(3) + K(29)*x(4) + K(30)*x(5))*x(1) + x6 + x7;

y = (50*y1 + 9.583*y2 + 20*y3 + 15*y4 - 852960 - 38100*(x(2) + 0.01*x(3)) + K(3,1) + K(32)*x(2) + K(33)*x(3) + K(34)*x(4) + K(35)*x(5))*x(1) - 24345 + 15*x(6);
// A verifier. A la fin, on ne doit pas avoir x(6)
endfunction

function diff_bound = himmelblau_13_ineq(x)
K = [-145421.402 2931.1506 -40.427932 5106.192 15711.36 -161622.577 4176.15328 2.8260078 9200.476 13160.295 -21686.9194 123.56928 ...
     -21.1188894 706.934 2898.573 28298.388 60.81096 31.242116 329.574 -2882.082 74095.3845 -306.262544 16.243649 -3094.252 -5566.2628 ...
     -26.237 99 -0.42 1300 2100 925548.252 -61968.8432 23.3088196 -27097.648 -50843.766];
     
x6 = (K(1) + K(2)*x(2) + K(3)*x(3) + K(4)*x(4) + K(5)*x(5))*x(1);
y1 = K(6) + K(1)*x(2) + K(8)*x(3) + K(9)*x(4) + K(10)*x(5);
y2 = K(11) + K(12)*x(2) + K(13)*x(3) + K(14)*x(4) + K(15)*x(5);
y3 = K(16) + K(17)*x(2) + K(18)*x(3) + K(19)*x(4) + K(20)*x(5);
y4 = K(21) + K(22)*x(2) + K(23)*x(3) + K(24)*x(4) + K(25)*x(5);
x7 = (y1 + y2 + y3)*x(1);
x8 = (K(26) + K(27)*x(2) + K(28)*x(3) + K(29)*x(4) + K(30)*x(5))*x(1) + x6 + x7;

diff_bound(1)  = x(1) - 5;
diff_bound(2)  = x(2) - 2.4;
diff_bound(3)  = x(3) - 60;
diff_bound(4)  = x(4) - 9.3;
diff_bound(5)  = x(5) - 7;
diff_bound(6)  = x6 - 294000;
diff_bound(7)  = x7 - 294000;
diff_bound(8)  = x8 - 277200;
diff_bound(9)  = -x(1);
diff_bound(10) = 1.2 - x(2);
diff_bound(11) = 20 - x(3);
diff_bound(12) = 9 - x(4);
diff_bound(13) = 6.5 - x(5);
diff_bound(14) = - x6;
diff_bound(15) = - x7;
diff_bound(16) = - x8;
endfunction

function diff_bound = himmelblau_13_eq(x)
diff_bound = 0;
endfunction

//
// Himmelblau problem 14
// Feasible Starting point X = [8000 3000 14000 2000 300 10]'
// Non feasible starting point X = [8000 3000 10000 2000 200 10]'
// Solution X* = [11884 3288 20000 4000 114.18 - 155.03]'
//

function y = himmelblau_14_x_init()
y = [8000 3000 14000 2000 300 10]';
endfunction

function Res = min_bd_himmelblau_14()
Res = -1e6 * ones(6,1);
endfunction

function Res = max_bd_himmelblau_14()
Res = 1e6 * ones(6,1);
endfunction

function y = himmelblau_14_obj(x)
X = 2.7*x + 1300;
y = sum(X(1:4)) + sum(100*X(5:6));
endfunction

function diff_bound = himmelblau_14_ineq(x)
T1     = (0.0285*x(1) + 300)/(1 + 0.0001425*x(1));
t1     = 500 - T1;
alpha2 = - 0.0001665*x(2);
T2     = (200 - 350*exp(-alpha2))/(1 - 1.5*exp(-alpha2));
t2     = 300 + (200 - T2)*exp(alpha2);
alpha3 = 0.085*9.36*1e-5*x(3);
T3     = (t1 + (29.75 - t1)*exp(-alpha3)) / (1 - 0.915*exp(-alpha3));
t3     = 350 + (t1 - T3)*exp(alpha3);
alpha4 = 0.00025*x(4);
T4     = (t2 + (70 - t2)*exp(-alpha4)) / (1 - 0.8*exp(-alpha4));
t4     = 350 + (t2 - T4)*exp(alpha4);
Tj2    = 0.8*T3 + 0.2*T4;
alpha5 = 0.000375*x(5);
T5     = 80 + (Tj2 - 80)*exp(-alpha5);
Tj1    = 0.7*T1 + 0.3*T2;
alpha6 = 0.0003*x(6);
T6     = 80 + (Tj1 - 80)*exp(-alpha6);
diff_bound(1) = -t4 + 300;
diff_bound(2) = -280 + T5;
diff_bound(3) = -250 + T6;
endfunction

function diff_bound = himmelblau_14_eq(x)
diff_bound = 0;
endfunction

//
// Himmelblau problem 15
// Starting point X = [390 1000 419.5 340.5 198.175 0.5]'
// First solution X* = [107.81 196.32 373.83 420 21.31 0.153]'
// Second solution X* = [201.78 100 383.07 420 -10.907 0.07314]'
//

function y = himmelblau_15_x_init()
y = [390 1000 419.5 340.5 198.175 0.5]';
endfunction

function Res = min_bd_himmelblau_15()
Res = -1e6 * ones(6,1);
endfunction

function Res = max_bd_himmelblau_15()
Res = 1e6 * ones(6,1);
endfunction

function y = himmelblau_15_obj(x)
if (x(1) < 300 & x(1)>= 0) then
  f_x1 = 30*x(1);
else
  f_x1 = 31*x(1);
end
if (x(2) < 100 & x(2) >= 0) then
  f_x2 = 28*x(2);
elseif (x(2) < 200 & x(2) >= 100) then
  f_x2 = 29*x(2);
else
  f_x2 = 30*x(2);
end
y = f_x1 + f_x2;
endfunction

function diff_bound = himmelblau_15_ineq(x)
diff_bound(1) = x(1) - 300 + x(3)*x(4) / 131.078 * cos(1.48577 - x(6)) - 0.90798*x(3)^2 / 131.078 * cos(1.47588);
diff_bound(2) = x(2) + x(3)*x(4) / 131.078 * cos(1.48477 + x(6)) - 0.90798*x(4)^2 / 131.078 * cos(1.47588);
diff_bound(3) = x(5) + x(3)*x(4) / 131.078 * sin(1.48477 + x(6)) - 0.90798*x(4)^2 / 131.078 * sin(1.47588);
diff_bound(4) = 200  - x(3)*x(4) / 131.078 * sin(1.48477 - x(6)) + 0.90798*x(3)^2 / 131.078 * sin(1.47588);

diff_bound(5) = x(1) - 400;
diff_bound(6) = x(2) - 1000;
diff_bound(7) = x(3) - 420;
diff_bound(8) = x(4) - 420;
diff_bound(9) = x(5) - 1000;
diff_bound(10) = x(6) - 0.5236;
diff_bound(11) = - x(1);
diff_bound(12) = - x(2);
diff_bound(13) = 340 - x(3);
diff_bound(14) = 340 - x(4);
diff_bound(15) = -1000 - x(5);
diff_bound(16) = - x(6);
endfunction

function diff_bound = himmelblau_15_eq(x)
diff_bound = 0;
endfunction

//
// Himmelblau fonction 16
// Starting point Xi = 1;
// Solution = X* = [0.9971 - 0.0758 0.5530 0.8331 0.9981 -0.0623 0.5642 0.8256 0.0000024]'
//

function y = himmelblau_16_x_init()
y = ones(9,1);
endfunction

function Res = min_bd_himmelblau_16()
Res = -1e6 * ones(9,1);
endfunction

function Res = max_bd_himmelblau_16()
Res = 1e6 * ones(9,1);
endfunction

function y = himmelblau_16_obj(x)
y = 0.5*(x(1)*x(4) - x(2)*x(3) + x(3)*x(9) - x(5)*x(9) + x(5)*x(8) - (6)*x(7));
endfunction

function diff_bound = himmelblau_16_ineq(x)
diff_bound(1) = -1 + x(3)^2 + x(4)^2;
diff_bound(2) = -1 + x(9)^2;
diff_bound(3) = -1 + x(5)^2 + x(6)^2;
diff_bound(4) = -1 + x(1)^2 + (x(2) - x(9))^2;
diff_bound(5) = -1 + (x(1) - x(5))^2 + (x(2) - x(9))^2;
diff_bound(6) = -1 + (x(1) - x(7))^2 + (x(2) - x(8))^2;
diff_bound(7) = -1 + (x(3) - x(5))^2 + (x(4) - x(6))^2;
diff_bound(8) = -1 + (x(3) - x(7))^2 + (x(4) - x(8))^2;
diff_bound(9) = -1 + x(7)^2 + (x(8) - x(9))^2;
diff_bound(10) = -x(1)*x(4) + x(2)*x(3);
diff_bound(11) = -x(3)*x(9);
diff_bound(12) =  x(5)*x(9);
diff_bound(13) = -x(5)*x(8) + x(6)*x(7);
diff_bound(14) = -x(9);
endfunction

function diff_bound = himmelblau_16_eq(x)
diff_bound = 0;
endfunction

//
// Himmelblau problem 24
//

function y = himmelblau_24_x_init()
y = ones(2,1);
endfunction

function Res = min_bd_himmelblau_24()
Res = -10 * ones(2,1);
endfunction

function Res = max_bd_himmelblau_24()
Res = 10 * ones(2,1);
endfunction

function y = himmelblau_24_obj(x)
y = (x(1) - 2)^2 + (x(2) - 1)^2;
endfunction

function diff_bound = himmelblau_24_ineq(x)
diff_bound(1) = x(1)^2 - x(2);
diff_bound(2) = x(1) + x(2) - 2;
endfunction

function diff_bound = himmelblau_24_eq(x)
diff_bound = 0;
endfunction

//
// Problem from K. Svanberg
// K. Svanberg, "A class of globally convergent optimization methods based on conservative convex separable approximations"
// SIAM J. OPTIM., vol. 12, n. 2, pp 555-573
// Problem 2
//

function y = mma_pb_1_x_init(n)
if ~isdef('n','local') then n = 9; end
y = 0.5*ones(n,1);
endfunction
function y = min_bd_mma_pb_1(n)
if ~isdef('n','local') then n = 9; end
y = -ones(n,1);
endfunction
function y = max_bd_mma_pb_1(n)
if ~isdef('n','local') then n = 9; end
y = ones(n,1);
endfunction

function y = mma_pb_1_obj(x)
n = length(x);
Res = zeros(size(x,1),size(x,2));
j=1:n;
for i=1:n
  alpha = (i+j-2) / (2*n-2);
  aux_coeff = (2 + sin(4*%pi*alpha)) ./ ((1 + abs(i-j))*log(n));
  Res(i) = Res(i) + sum(aux_coeff .* x');
end
y = sum(x.*Res);
endfunction

function y = mma_pb_1_df_obj(x)
n = length(x);
Res = zeros(size(x,1),size(x,2));
j=1:n;
for i=1:n
  alpha = (i+j-2) / (2*n-2);
  aux_coeff = (2 + sin(4*%pi*alpha)) ./ ((1 + abs(i-j))*log(n));
  Res(i) = Res(i) + sum(aux_coeff .* x');
end
y = 2*Res;
endfunction

function y = mma_pb_1_ineq(x)
n = length(x);
Res1 = zeros(size(x,1),size(x,2));
Res2 = zeros(size(x,1),size(x,2));
j=1:n;
for i=1:n
  alpha = (i+j-2) / (2*n-2);
  aux_coeff = (1 + 2*alpha) ./ ((1 + abs(i - j))*log(n));
  Res1(i) = Res1(i) + sum(aux_coeff .* x');
    
  aux_coeff = (3 - 2*alpha) ./ ((1 + abs(i - j))*log(n));
  Res2(i) = Res2(i) + sum(aux_coeff .* x');
end
y(1,1) = n/2 - sum(x .* Res1);
y(2,1) = n/2 - sum(x .* Res2);
endfunction

function y = mma_pb_1_df_ineq(x)
n = length(x);
Res1 = zeros(size(x,1),size(x,2));
Res2 = zeros(size(x,1),size(x,2));
j=1:n;
for i=1:n
  alpha = (i+j-2) / (2*n-2);
  aux_coeff = (1 + 2*alpha) ./ ((1 + abs(i - j))*log(n));
  Res1(i) = Res1(i) + sum(aux_coeff .* x');
  
  aux_coeff = (3 - 2*alpha) ./ ((1 + abs(i - j))*log(n));
  Res2(i) = Res2(i) + sum(aux_coeff .* x');
end
y(:,1) = -2 * Res1;
y(:,2) = -2 * Res2;
endfunction

function y = mma_pb_1_eq(x)
y = 0;
endfunction

function y = mma_pb_1_df_eq(x)
y = 0;
endfunction

//
// Problem from K. Svanberg
// K. Svanberg, "A class of globally convergent optimization methods based on conservative convex separable approximations"
// SIAM J. OPTIM., vol. 12, n. 2, pp 555-573
// Problem 2
//

function y = mma_pb_2_x_init(n)
if ~isdef('n','local') then n = 9; end
y = 0.25*ones(n,1);
endfunction
function y = min_bd_mma_pb_2(n)
if ~isdef('n','local') then n = 9; end
y = -ones(n,1);
endfunction
function y = max_bd_mma_pb_2(n)
if ~isdef('n','local') then n = 9; end
y = ones(n,1);
endfunction

function y = mma_pb_2_obj(x)
n = length(x);
Res = zeros(size(x,1),size(x,2));
j=1:n;
for i=1:n
  alpha = (i+j-2) / (2*n-2);
  aux_coeff = (2 + sin(4*%pi*alpha)) ./ ((1 + abs(i-j))*log(n));
  Res(i) = Res(i) + sum(aux_coeff .* x');
end
y = - sum(x .* Res)
endfunction

function y = mma_pb_2_df_obj(x)
n = length(x);
Res = zeros(size(x,1),size(x,2));
j=1:n;
for i=1:n
  alpha = (i+j-2) / (2*n-2);
  aux_coeff = (2 + sin(4*%pi*alpha)) ./ ((1 + abs(i-j))*log(n));
  Res(i) = Res(i) + sum(aux_coeff .* x');
end
y = - 2 * Res;
endfunction

function y = mma_pb_2_ineq(x)
n = length(x);
Res1 = zeros(size(x,1),size(x,2));
Res2 = zeros(size(x,1),size(x,2));
j=1:n;
for i=1:n
  alpha = (i+j-2) / (2*n-2);
  aux_coeff = (1 + 2*alpha) ./ ((1 + abs(i - j))*log(n));
  Res1(i) = Res1(i) + sum(aux_coeff .* x');

  aux_coeff = (3 - 2*alpha) ./ ((1 + abs(i - j))*log(n));
  Res2(i) = Res2(i) + sum(aux_coeff .* x');
end
y(1,1) = sum(x .* Res1) - n/2;
y(2,1) = sum(x .* Res2) - n/2;
endfunction

function y = mma_pb_2_df_ineq(x)
n = length(x);
Res1 = zeros(size(x,1),size(x,2));
Res2 = zeros(size(x,1),size(x,2));
j=1:n;
for i=1:n
  alpha = (i+j-2) / (2*n-2);
  aux_coeff = (1 + 2*alpha) ./ ((1 + abs(i - j))*log(n));
  Res1(i) = Res1(i) + sum(aux_coeff .* x');

  aux_coeff = (3 - 2*alpha) ./ ((1 + abs(i - j))*log(n));
  Res2(i) = Res2(i) + sum(aux_coeff .* x');
end
y(:,1) = 2 * Res1;
y(:,2) = 2 * Res2;
endfunction

function y = mma_pb_2_eq(x)
y = 0;
endfunction

function y = mma_pb_2_df_eq(x)
y = 0;
endfunction

//
// Problem ENPC 1
// This problem has been developped at ENPC during academic year 2006/2007.
// Author: Schuster Moldenhauer Traki Thou
//

function y = enpc_1_init()
y = 2.5*ones(n,1);
endfunction
function y = min_bd_enpc_1()
y = [-2;-2];
endfunction
function y = max_bd_enpc_1()
y = [8;8];
endfunction

function y = enpc_1_obj(x)
y = -(x(1) - 3)^4 - (x(2) - 3)^4;
endfunction
function y = enpc_1_df_obj(x)
y(1,1) = -4*(x(1) - 3)^3;
y(2,1) = -4*(x(2) - 3)^3;
endfunction

function y = enpc_1_ineq(x)
y(1,1) = -x(1);
y(2,1) = -x(2);
y(3,1) = x(1) - 5;
y(4,1) = x(2) - 6;
endfunction
function y = enpc_1_df_ineq(x)
y(1,1) = -1;
y(1,2) = 0;
y(2,1) = 0;
y(2,2) = -1;
y(3,1) = 1;
y(3,2) = 0;
y(4,1) = 0;
y(4,2) = 1;
endfunction
