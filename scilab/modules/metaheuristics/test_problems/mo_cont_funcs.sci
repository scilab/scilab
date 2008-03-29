// Pb name        - constrained ?   - num of objectives
// binh_1         - not constrained - 2D
// fonseca_1      - not constrained - 2D
// fonseca_2      - not constrained - 2D
// kursawe_1      - not constrained - 2D
// laumanns_1     - not constrained - 2D
// lis_1          - not constrained - 2D
// murata_1       - not constrained - 2D
// poloni_1       - not constrained - 2D
// quagliar_1     - not constrained - 2D
// rendon_1       - not constrained - 2D
// rendon_2       - not constrained - 2D
// schaffer_1     - not constrained - 2D
// schaffer_2     - not constrained - 2D
// binh_2         - not constrained - 3D
// viennet_1      - not constrained - 3D
// viennet_2      - not constrained - 3D
// viennet_3      - not constrained - 3D
// belegundu_1    - constrained     - 2D
// binh_3         - constrained     - 2D
// jimenez_1      - constrained     - 2D
// kita_1         - constrained     - 2D
// osyczka_1      - constrained     - 2D
// osyczka_2      - constrained     - 2D
// srinivas_1     - constrained     - 2D
// tanaka_1       - constrained     - 2D
// binh_4         - constrained     - 3D
// viennet_4      - constrained     - 3D
// coello_1       - constrained     - 2D
// coello_2       - not constrained - 4D
// coello_3       - constrained     - 2D
// deb_1          - not constrained - 2D
// deb_2          - not constrained - 2D
// deb_3          - not constrained - 2D
// deb_4          - not constrained - 2D
// deb_5          - not constrained - 2D
// deb_6          - not constrained - 2D
// deb_7          - not constrained - 2D
// deb_8          - not constrained - 2D
// veldmop_3      - not constrained - 2D
// veldmop_4      - not constrained - 2D
// veldmop_5      - not constrained - 3D
// veldmop_6      - not constrained - 2D
// veldmop_7      - not constrained - 3D
// veldmopc_1     - constrained     - 2D
// veldmopc_2     - constrained     - 2D
// hanne_1        - constrained     - 2D
// hanne_2        - constrained     - 2D
// hanne_3        - constrained     - 2D
// hanne_4        - constrained     - 2D
// hanne_5        - constrained     - 2D
// meca_1         - not constrained - 2D
// meca_2         - constrained     - 2D
// trigo_1        - not constrained - 2D
// trigo_2        - not constrained - 2D
// trigo_3        - not constrained - 2D
// trigo_4        - not constrained - 2D
// trigo_5        - not constrained - 2D
// trigo_5_bis    - not constrained - 2D
// trigo_6        - not constrained - 2D
// trigo_6_bis    - not constrained - 2D
// trigo_7        - not constrained - 2D
// trigo_8        - not constrained - 2D
// trigo_9        - not constrained - 2D

//
// La fonction BINH I
//

function Res = min_bd_binh_1()
Res = [-5;-5]
endfunction

function Res = max_bd_binh_1()
Res = [10;10]
endfunction

function f = binh_1(x)
f(1,1) = x(1)^2 + x(2)^2;
f(1,2) = (x(1) - 5.0)^2 + (x(2) - 5.0)^2;
endfunction

//
// La fonction FONSECA I
//

function Res = min_bd_fonseca_1()
Res = [-10;-10]
endfunction

function Res = max_bd_fonseca_1()
Res = [10;10]
endfunction

function f = fonseca_1(x)
f(1,1) = (1.0 - exp(-(x(1)-1.0)^2 - (x(2)+1.0)^2));
f(1,2) = (1.0 - exp(-(x(1)+1.0)^2 - (x(2)-1.0)^2));
endfunction

//
// La fonction FONSECA II
//

function Res = min_bd_fonseca_2(n)
if ~isdef('n','local') then n = 2; end;
Res = -4*ones(n,1);
endfunction

function Res = max_bd_fonseca_2(n)
if ~isdef('n','local') then n = 2; end;
Res = 4*ones(n,1);
endfunction

function f = fonseca_2(x)
SigmF1 = 0;
SigmF2 = 0;
  
for(i=1:length(x)) 
  SigmF1 = SigmF1 + (x(i) - 1.0/sqrt(i))^2;
  SigmF2 = SigmF2 + (x(i) + 1.0/sqrt(i))^2;
end
f(1,1) = 1.0 - exp(-SigmF1);
f(1,2) = 1.0 - exp(-SigmF2);
endfunction

//
// Fonction KURSAWE
//

function Res = min_bd_kursawe_1(n)
if ~isdef('n','local') then n = 2; end;
Res = -10*ones(n,1);
endfunction

function Res = max_bd_kursawe_1(n)
if ~isdef('n','local') then n = 2; end;
Res = 10*ones(n,1);
endfunction

function f = kursawe_1(x)
SigmF1 = 0;
SigmF2 = 0;

for(i=1:length(x)-1)
  SigmF1 = SigmF1 - 10 * exp(-0.2 * sqrt((x(i)^2 + x(i+1)^2)));
  SigmF2 = SigmF2 + abs(x(i))^0.8 + 5*sin(x(i))^3;
end

SigmF2 = SigmF2 + abs(x($))^0.8 + 5*sin(x($))^3;
      
f(1,1) = SigmF1;
f(1,2) = SigmF2;
endfunction


//
// La fonction LAUMANNS
//

function Res = min_bd_laumanns_1()
Res = [-50;-50];
endfunction

function Res = max_bd_laumanns_1()
Res = [50;50];
endfunction

function f = laumanns_1(x)
f(1,1) = x(1)^2 + x(2)^2;
f(1,2) = (x(1) + 2)^2 + x(2)^2;
endfunction

//
// La fonction LIS
//

function Res = min_bd_lis_1()
Res = [-5;-5];
endfunction

function Res = max_bd_lis_1()
Res = [10;10];
endfunction

function f = lis_1(x)
f(1,1) = (x(1)^2 + x(2)^2)^1/8
f(1,2) = ((x(1) - 0.5)^2 + (x(2) - 0.5)^2)^1/4;
endfunction

//
// La fonction MURATA
//

function Res = min_bd_murata_1()
Res = [1;1];
endfunction

function Res = max_bd_murata_1()
Res = [4;2];
endfunction

function f = murata_1(x)
f(1,1) = 2 * sqrt(x(1));
f(1,2) = (x(1) * (1 - x(2)) + 5);
endfunction

//
// La fonction POLONI
//

function Res = min_bd_poloni_1()
Res = [-%pi;-%pi];
endfunction

function Res = max_bd_poloni_1()
Res = [%pi;%pi];
endfunction

function f = poloni_1(x)
A1 = 0.5 * sin(1) - 2 * cos(1) +     sin(2) - 1.5 * cos(2);
A2 = 1.5 * sin(1) -     cos(1) + 2 * sin(2) - 0.5 * cos(2);
B1 = 0.5 * sin(x(1)) - 2 * cos(x(1)) +     sin(x(2)) - 1.5 * cos(x(2));
B2 = 1.5 * sin(x(1)) -     cos(x(1)) + 2 * sin(x(2)) - 0.5 * cos(x(2));

f(1,1) = -(1.0 + (A1 - B1)^2 + (A2 - B2)^2);
f(1,2) = -((x(1) + 3)^2 + (x(2) + 1)^2);
endfunction

//
// La fonction QUAGLIARELLA
//

function Res = min_bd_quagliar_1(n)
if ~isdef('n','local') then n = 2; end;
Res = -5.12 * ones(n,1);
endfunction

function Res = max_bd_quagliar_1(n)
if ~isdef('n','local') then n = 2; end;
Res = 5.12*ones(n,1);
endfunction

function f = quagliar_1(x)
SigmF1 = 0;
SigmF2 = 0;
for(i=1:length(x))
  SigmF1 = SigmF1 + x(i)^2 - 10 * cos(2*%pi*x(i)) + 10.0;
  SigmF2 = SigmF2 + (x(i)-1.5)^2 - 10 * cos(2*%pi*(x(i) - 1.5)) + 10.0;
end

f(1,1) = sqrt(SigmF1/length(x));
f(1,2) = sqrt(SigmF2/length(x));
endfunction

//
// La fonction RENDON I
//

function Res = min_bd_rendon_1()
Res = -3 * ones(2,1);
endfunction

function Res = max_bd_rendon_1()
Res = 3*ones(2,1);
endfunction

function f = rendon_1(x)
f(1,1) = (1.0 / (x(1)^2 + x(2)^2 + 1.0));
f(1,2) = (x(1)^2 + 3*x(2)^2 + 1.0);
endfunction

//
// La fonction RENDON II
//

function Res = min_bd_rendon_2()
Res = -3 * ones(2,1);
endfunction

function Res = max_bd_rendon_2()
Res = 3*ones(2,1);
endfunction

function f = rendon_2(x)
f(1,1) = (x(1) + x(2) + 1.0);
f(1,2) = (x(1)^2 + 2*x(2) - 1.0);
endfunction

//
// La fonction SCHAFFER I
//

function Res = min_bd_schaffer_1()
Res = -5;
endfunction

function Res = max_bd_schaffer_1()
Res = 10;
endfunction

function f = schaffer_1(x)
f(1,1) = x(1)^2;
f(1,2) = (x(1) - 2.0)^2;
endfunction

//
// La fonction SCHAFFER II
//

function Res = min_bd_schaffer_2()
Res = -5;
endfunction

function Res = max_bd_schaffer_2()
Res = 10;
endfunction

function f = schaffer_2(x)
if (x(1)<=1) then
  x_aux = - x(1);
elseif (x(1)>1)&(x(1)<=3) then
  x_aux = -2+x(1);
elseif (x(1)>3)&(x(1)<=4) then
  x_aux = 4 - x(1);
elseif (x(1)>4) then
  x_aux = -4 + x(1);
end  

f(1,1) = x_aux;
f(1,2) =(x(1) - 5)^2;
endfunction

//
// La fonction BINH II
//

function Res = min_bd_binh_2()
Res = 10^(-6)*ones(2,1);
endfunction

function Res = max_bd_binh_2()
Res = 10^6*ones(2,1);
endfunction

function f = binh_2(x)
f(1,1) = x(1) - 10^6;
f(1,2) = x(2) - 2*10^(-6);
f(1,3) = x(1) * x(2) - 2.0;
endfunction

//
// La fonction VIENNET I
//

function Res = min_bd_viennet_1()
Res = -2*ones(2,1);
endfunction

function Res = max_bd_viennet_1()
Res = 2*ones(2,1);
endfunction

function f = viennet_1(x)
f(1,1) = x(1)^2 + (x(2) - 1)^2;
f(1,2) = x(1)^2 + (x(2) + 1)^2 + 1;
f(1,3) = (x(1) - 1)^2 + x(2)^2 + 2;
endfunction

//
// La fonction VIENNET II
//

function Res = min_bd_viennet_2()
Res = -4*ones(2,1);
endfunction

function Res = max_bd_viennet_2()
Res = 4*ones(2,1);
endfunction

function f = viennet_2(x)
f(1,1) = ((x(1) - 2.0)^2)/2.0 + ((x(2) + 1.0)^2)/13.0 + 3.0;
f(1,2) = ((x(1) + x(2) - 3.0)^2)/36.0 + ((-x(1) + x(2) + 2.0)^2)/8.0 - 17.0;
f(1,3) = ((x(1) + 2*x(2) - 1.0)^2)/175.0 + ((2*x(2) - x(1))^2)/17.0 - 13.0;
endfunction

//
// La fonction VIENNET III
//

function Res = min_bd_viennet_3()
Res = -3*ones(2,1);
endfunction

function Res = max_bd_viennet_3()
Res = 3*ones(2,1);
endfunction

function f = viennet_3(x)
f(1,1) = 0.5*(x(1)^2 + x(2)^2) + sin(x(1)^2 + x(2)^2);
f(1,2) = ((3*x(1) - 2*x(2) + 4.0)^2)/8.0 + ((x(1) - x(2) + 1.0)^2)/27.0 + 15;
f(1,3) = 1.0/(x(1)^2 + x(2)^2 + 1.0) - 1.1*exp(-x(1)^2 - x(2)^2);
endfunction

//
// La fonction BELEGUNDU
//

function Res = min_bd_belegundu_1()
Res = 0*ones(2,1);
endfunction

function Res = max_bd_belegundu_1()
Res = [5;3];
endfunction

function f = belegundu_1(x)
f(1,1) = -2*x(1) + x(2);
f(1,2) = 2*x(1) + x(2);
endfunction

function g = constr_belegundu_1(x)
g(1) = -x(1) + x(2) - 1.0;
g(2) = x(1) + x(2) - 7.0;
endfunction

//
// La fonction BINH III
//

function Res = min_bd_binh_3()
Res = 0*ones(2,1);
endfunction

function Res = max_bd_binh_3()
Res = [5;3];
endfunction

function f = binh_3(x)
f(1,1) = 4*(x(1)^2 + x(2)^2);
f(1,2) = (x(1) - 5)^2 + (x(2) - 5)^2;
endfunction

function g = constr_binh_3(x)
g(1) =  (x(1) - 5.0)^2 + x(2)^2 - 25.0;
g(2) = -(x(1) - 8.0)^2 - (x(2) + 3.0)^2 + 7.7;
endfunction

//
// La fonction JIMENEZ
//

function Res = min_bd_jimenez_1()
Res = 0*ones(2,1);
endfunction

function Res = max_bd_jimenez_1()
Res = 100*ones(2,1);
endfunction

function f = jimenez_1(x)
f(1,1) = 5*x(1) + 3*x(2);
f(1,2) = 2*x(1) + 8*x(2);
endfunction

function g = constr_jimenez_1(x)
g(1) =   x(1) + 4*x(2) - 100.0;
g(2) = 3*x(1) + 2*x(2) - 150.0;
g(3) = 200.0 - 5*x(1) - 3*x(2);
g(4) = 75.0  - 2*x(1) - 8*x(2);
endfunction

//
// La fonction de KITA
//

function Res = min_bd_kita_1()
Res = -10*ones(2,1);
endfunction

function Res = max_bd_kita_1()
Res = 10*ones(2,1);
endfunction

function f = kita_1(x)
f(1,1) = -x(1)^2 + x(2);
f(1,2) = 0.5*x(1) + x(2) + 1.0;
endfunction

function g = constr_kita_1(x)
g(1) = 1.0/6.0 * x(1) + x(2) - 13.0/2.0;
g(2) = 0.5     * x(1) + x(2) - 15.0/2.0;
g(3) =   5.0   * x(1) + x(2) - 30.0;
endfunction

//
// La fonction OSYCZKA I
//

function Res = min_bd_osyczka_1()
Res = [2;5]
endfunction

function Res = max_bd_osyczka_1()
Res = [7;10];
endfunction

function f = osyczka_1(x)
f(1,1) = x(1) + x(2)^2;
f(1,2) = x(1)^2 + x(2);
endfunction

function g = constr_osyczka_1(x)
g(1) = -(12.0 - x(1) - x(2));
g(2) = -(x(1)^2 + 10*x(1) - x(2)^2 + 16*x(2) - 80.0);
endfunction

//
// La fonction OSYCZKA II
//

function Res = min_bd_osyczka_2()
Res = [0;0;1;0;1;0]
endfunction

function Res = max_bd_osyczka_2()
Res = [10;10;5;6;5;10];
endfunction

function f = osyczka_2(x)
f(1,1) = (-(25*((x(1) - 2.0)^2) + ((x(2) - 2.0)^2) + ((x(3) - 1.0)^2) + ((x(4) - 4.0)^2) + ((x(5) - 1.0)^2)));
f(1,2) = x(1)^2 + x(2)^2 + x(3)^2 + x(4)^2 + x(5)^2 + x(6)^2;
endfunction

function g = constr_osyczka_2(x)
g(1) = -(x(1) + x(2) - 2.0);
g(2) = -(6.0 - x(1) - x(2));
g(3) = -(2.0 - x(2) + x(1));
g(4) = -(2.0 - x(1) + 3*x(2));
g(5) = -(4.0 - ((x(3) - 3.0)^2) - x(4));
g(6) = -(((x(5) - 3.0)^2) + x(6) - 4.0);
endfunction

//
// La fonction SRINIVAS
//

function Res = min_bd_srinivas_1()
Res = [-20;-20]
endfunction

function Res = max_bd_srinivas_1()
Res = [20;20];
endfunction

function f = srinivas_1(x)
f(1,1) = (((x(1) - 2.0)^2) + ((x(2) - 1.0)^2) + 2.0);
f(1,2) = (9*x(1) - ((x(2) - 1.0)^2.0));
endfunction

function g = constr_srinivas_1(x)
g(1) = x(1)^2 + x(2)^2 - 225.0;
g(2) = x(1) - 3*x(2) + 10.0;
endfunction

//
// La fonction TANAKA
//

function Res = min_bd_tanaka_1()
Res = 0*ones(2,1);
endfunction

function Res = max_bd_tanaka_1()
Res = %pi*ones(2,1);
endfunction

function f = tanaka_1(x)
f(1,1) = x(1);
f(1,2) = x(2);
endfunction

function g = constr_tanaka_1(x)
g(1) = -x(1)^2 - x(2)^2 + 1.0 + 0.1*cos(16*atan(x(1)/x(2)));
g(2) = ((x(1) - 0.5)^2) + ((x(2) - 0.5)^2) - 0.5;
endfunction

//
// La fonction BINH IV
//

function Res = min_bd_binh_4()
Res = -10*ones(2,1);
endfunction

function Res = max_bd_binh_4()
Res = 10*ones(2,1);
endfunction

function f = binh_4(x)
f(1,1) = 1.5 - x(1)*(1.0 - x(2));
f(1,2) = 2.25 - x(1)*(1.0 - x(2)^2);
f(1,3) = 2.625 - x(1)*(1.0 -x(2)^3);
endfunction

function g = constr_binh_4(x)
g(1) = -(x(1)^2) - (x(2) - 0.5)^2 + 9.0;
g(2) = (x(1) - 1.0)^2 + (x(2) - 0.5)^2 - 6.25;
endfunction

//
// La fonction VIENNET IV
//

function Res = min_bd_viennet_4()
Res = -4*ones(2,1);
endfunction

function Res = max_bd_viennet_4()
Res = 4*ones(2,1);
endfunction

function f = viennet_4(x)
f(1,1) = ((x(1) - 2.0)^2)/2.0 + ((x(2) + 1.0)^2)/13.0 + 3.0;
f(1,2) = ((x(1) + x(2) - 3.0)^2)/175.0 + ((2*x(2) - x(1))^2)/17.0 - 13.0;
f(1,3) = ((3*x(1) - 2*x(2) + 4.0)^2)/8.0 + ((x(1) - x(2) + 1.0)^2)/27.0 + 15.0;
endfunction

function g = constr_viennet_4(x)
g(1) = 4*x(1) - 4.0 + x(2);
g(2) = -x(1) - 1.0;
g(3) = x(1) - 2.0 - x(2);
endfunction

//
// La fonction COELLO I 
//

function Res = min_bd_coello_1()
Res = [10;10;0.9;0.9];
endfunction

function Res = max_bd_coello_1()
Res = [80;50;5;5];
endfunction

function f = coello_1(x)
f(1,1) = (2 *x(2)*x(4) - x(3)*(x(1) - 2*x(4)));
f(1,2) = (60000.0/(x(3)*(x(1)-2*x(4)^3)+2*x(2)*x(4)*(4*(x(4)^2)+3*x(1)*(x(1)-2*x(4)))));
endfunction

function g = constr_coello_1(x)
g(1) = -(16.0- 180000.0*x(1)/(x(3)*(x(1)-2*x(4)^3)+2*x(2)*x(4)*(4*(x(4)^2)+3*x(1)*(x(1) - 2*x(4)))) - 15000.0*x(2)/((x(1)-2*x(4))*(x(3)^3)+2*x(4)*(x(2)^3)));
endfunction

//
// La fonction COELLO II
//

function Res = min_bd_coello_2()
Res = [6.3969;0.6931;3.9120];
endfunction

function Res = max_bd_coello_2()
Res = [7.0901;2.8904;4.6052];
endfunction

function f = coello_2(x)
f(1,1) = (7.49   - 0.44*x(1) + 1.16*x(2) - 0.61*x(3));
f(1,2) = (4.13   - 0.92*x(1) + 0.16*x(2) - 0.43*x(3));
f(1,3) = (-21.90 + 1.94*x(1) + 0.30*x(2) + 1.04*x(3));
f(1,4) = (11.331 -      x(1) -      x(2) -      x(3));
endfunction

//
// La fonction COELLO III
//

function Res = min_bd_coello_3()
Res = %eps*ones(4,1);
endfunction

function Res = max_bd_coello_3()
Res = 400*ones(4,1);
endfunction

function f = coello_3(x)
l = x(1); Do = x(2); da = x(3); db = x(4);

F   = 10000.0;
E   = 210000.0;
a   = 80.0;
Ia  = 0.049*((da^4)+(Do^4));
Ib  = 0.049*((db^4)+(Do^4));
dra = -0.001;
drb = -0.001;
Ca  = 35400*(abs(dra)^1.0/9.0)*(da^10.0/9.0);
Cb  = 35400*(abs(drb)^1.0/9.0)*(db^10.0/9.0);

f(1,1) = (%pi/4.0*(a*((da^2)-(Do^2))+l*((db^2)-(Do^2))));
f(1,2) = ((F*(a^3)/(3*E*Ia))*(1.0+l*Ia/(a*Ib))+(F/Ca)*(((1+a/l)^2)+Ca*(a^2)/(Cb*(l^2))));
endfunction

function g = constr_coello_3(x)
l = x(1); Do = x(2); da = x(3); db = x(4);

lg  = 200.0;
lk  = 150.0;
da1 = 80.0;
da2 = 95.0;
db1 = 75.0;
db2 = 90.0;
dom = 25.0;
p1  = 1.25;
p2  = 1.05;
Da  = 0.0054;
Db  = -Da;
D   = 0.01;
a   = 80.0;

g(1)  = l - lg;
g(2)  = lk - l;
g(3)  = da1 - da;
g(4)  = da - da2;
g(5)  = db1 - db;
g(6)  = db - db2;
g(7)  = dom - Do;
g(8)  = p1*Do - db;
g(9)  = p2*Do - da;
g(10) = abs(Da+(Da-Db)*a/l)-D;
endfunction

/////////////////////////////////////////////////////////////////////////
// Deb test functions are extracted from                               //
// Modelling the regularity in an estimation of distribution algorithm //
// for continuous multiobjective optimization with variable linkages   //
// Q. Zhang, A. Zhou, Y. Jin                                           //
// Department of Computer Science, University of Essex                 //
// Technical report CSM-459, july 2006                                 //
/////////////////////////////////////////////////////////////////////////

/////////////////////////
// Deb 1 test function //
// Convex Pareto set   //
/////////////////////////

function Res = min_bd_deb_1(n)
if ~isdef('n','local') then n = 10; end;
Res = zeros(n,1);
endfunction

function Res = max_bd_deb_1(n)
if ~isdef('n','local') then n = 10; end;
Res = ones(n,1);
endfunction

function f = get_opti_deb_1(x)
f1_x1 = x(1);
g_x2  = 1;
h     = 1 - sqrt(f1_x1 / g_x2);

f(1,1) = f1_x1;
f(1,2) = g_x2 * h;
endfunction

function f = deb_1(x)
f1_x1 = x(1);
g_x2  = 1 + 9 * sum((x(2:$)-x(1)).^2) / (length(x) - 1);
h     = 1 - sqrt(f1_x1 / g_x2);

f(1,1) = f1_x1;
f(1,2) = g_x2 * h;
endfunction

////////////////////////////
// Deb 2 test function    //
// Non-convex Pareto set  //
////////////////////////////

function Res = min_bd_deb_2(n)
if ~isdef('n','local') then n = 10; end;
Res = zeros(n,1);
endfunction

function Res = max_bd_deb_2(n)
if ~isdef('n','local') then n = 10; end;
Res = ones(n,1);
endfunction

function f = get_opti_deb_2(x)
f1_x1 = x(1);
g_x2  = 1;
h     = 1 - (f1_x1 / g_x2)^2;

f(1,1) = f1_x1;
f(1,2) = g_x2 * h;
endfunction

function f = deb_2(x)
f1_x1 = x(1);
g_x2  = 1 + 9 * sum((x(2:$) - x(1)).^2) / (length(x) - 1);
h     = 1 - (f1_x1 / g_x2)^2;

f(1,1) = f1_x1;
f(1,2) = g_x2 * h;
endfunction

/////////////////////////////////
// Deb 3 test function         //
// Non-contiguous Pareto set   //
/////////////////////////////////

function Res = min_bd_deb_3(n)
if ~isdef('n','local') then n = 10; end;
Res = zeros(n,1);
endfunction

function Res = max_bd_deb_3(n)
if ~isdef('n','local') then n = 10; end;
Res = ones(n,1);
endfunction

function f = get_opti_deb_3(x)
f1_x1 = x(1);
g_x2  = 1;
h     = 1 - sqrt(f1_x1 / g_x2) - (f1_x1 / g_x2)*sin(10*%pi*f1_x1);

f(1,1) = f1_x1;
f(1,2) = g_x2 * h;
endfunction

function f = deb_3(x)
f1_x1 = 1-exp(-4*x(1))*sin(6*%pi*x(1))^6;
g_x2  = 1 + 9 * (sum((x(2:$)-x(1)).^2) / 9)^0.25;
h     = 1 - (f1_x1 / g_x2)^2;

f(1,1) = f1_x1;
f(1,2) = g_x2 * h;
endfunction

/////////////////////////////////
// Deb 4 test function         //
// Convex Pareto set           //
// non linear linkage variable //
/////////////////////////////////

function Res = min_bd_deb_4(n)
if ~isdef('n','local') then n = 10; end;
Res = zeros(n,1);
endfunction

function Res = max_bd_deb_4(n)
if ~isdef('n','local') then n = 10; end;
Res = ones(n,1);
endfunction

function f = get_opti_deb_4(x)
f1_x1 = x(1);
g_x2  = 1;
h     = 1 - sqrt(f1_x1 / g_x2);

f(1,1) = f1_x1;
f(1,2) = g_x2 * h;
endfunction

function f = deb_4(x)
f1_x1 = x(1);
g_x2  = 1 + 9 * sum((x(2:$).^2-x(1)).^2) / (length(x) - 1);
h     = 1 - sqrt(f1_x1 / g_x2);

f(1,1) = f1_x1;
f(1,2) = g_x2 * h;
endfunction

/////////////////////////////////
// Deb 5 test function         //
// Non-convex Pareto set       //
// non linear linkage variable //
/////////////////////////////////

function Res = min_bd_deb_5(n)
if ~isdef('n','local') then n = 10; end;
Res = zeros(n,1);
endfunction

function Res = max_bd_deb_5(n)
if ~isdef('n','local') then n = 10; end;
Res = ones(n,1);
endfunction

function f = get_opti_deb_5(x)
f1_x1 = x(1);
g_x2  = 1;
h     = 1 - (f1_x1 / g_x2)^2;

f(1,1) = f1_x1;
f(1,2) = g_x2 * h;
endfunction

function f = deb_5(x)
f1_x1 = x(1);
g_x2  = 1 + 9 * sum((x(2:$).^2 - x(1)).^2) / (length(x) - 1);
h     = 1 - (f1_x1 / g_x2)^2;

f(1,1) = f1_x1;
f(1,2) = g_x2 * h;
endfunction

/////////////////////////////////
// Deb 6 test function         //
// Non-contiguous Pareto set   //
// non linear linkage variable //
/////////////////////////////////

function Res = min_bd_deb_6(n)
if ~isdef('n','local') then n = 10; end;
Res = zeros(n,1);
endfunction

function Res = max_bd_deb_6(n)
if ~isdef('n','local') then n = 10; end;
Res = ones(n,1);
endfunction

function f = get_opti_deb_6(x)
f1_x1 = x(1);
g_x2  = 1;
h     = 1 - sqrt(f1_x1 / g_x2) - (f1_x1 / g_x2)*sin(10*%pi*f1_x1);

f(1,1) = f1_x1;
f(1,2) = g_x2 * h;
endfunction

function f = deb_6(x)
f1_x1 = 1-exp(-4*x(1))*sin(6*%pi*x(1))^6;
g_x2  = 1 + 9 * (sum((x(2:$).^2-x(1)).^2) / 9)^0.25;
h     = 1 - (f1_x1 / g_x2)^2;

f(1,1) = f1_x1;
f(1,2) = g_x2 * h;
endfunction

/////////////////////////////////
// Deb 7 test function         //
// Non convex Pareto set       //
// non linear linkage variable //
/////////////////////////////////

function Res = min_bd_deb_7(n)
if ~isdef('n','local') then n = 10; end;
Res = zeros(n,1);
endfunction

function Res = max_bd_deb_7(n)
if ~isdef('n','local') then n = 10; end;
Res = 10*ones(n,1);
Res(1) = 1;
endfunction

function f = get_opti_deb_7(x)
f1_x1 = x(1);
g_x2  = 1;
h     = 1 - sqrt(f1_x1 / g_x2);

f(1,1) = f1_x1;
f(1,2) = g_x2 * h;
endfunction

function f = deb_7(x)
f1_x1 = x(1);
g_x2  = 1 + 10*(length(x) - 1) + sum((x(2:$).^2-x(1)).^2 - 10 * cos(2*%pi*(x(2:$).^2-x(1))));
h     = 1 - sqrt(f1_x1 / g_x2);

f(1,1) = f1_x1;
f(1,2) = g_x2 * h;
endfunction

/////////////////////////////////
// Deb 8 test function         //
// Non convex Pareto set       //
// non linear linkage variable //
/////////////////////////////////

function Res = min_bd_deb_8(n)
if ~isdef('n','local') then n = 10; end;
Res = zeros(n,1);
endfunction

function Res = max_bd_deb_8(n)
if ~isdef('n','local') then n = 10; end;
Res = ones(n,1);
endfunction

function f = get_opti_deb_8(x)
f1_x1 = x(1);
g_x2  = 1;
h     = 1 - sqrt(f1_x1 / g_x2);

f(1,1) = f1_x1;
f(1,2) = g_x2 * h;
endfunction

function f = deb_8(x)
f1_x1 = x(1);
I = (2:length(x))';
g_x2  = 1/4000*sum((x(2:$).^2-x(1)).^2) - prod(cos((x(2:$).^2-x(1))./sqrt(I-1))) + 2;
h     = 1 - sqrt(f1_x1 / g_x2);

f(1,1) = f1_x1;
f(1,2) = g_x2 * h;
endfunction

//
// La fonction de test VeldMOP3. Elle prend 2 variables en entree.
//

function Res = min_bd_veldmop_3()
Res = -%pi*ones(2,1);
endfunction

function Res = max_bd_veldmop_3()
Res = %pi*ones(2,1);
endfunction

function f = veldmop_3(x)
A1 = 0.5*sin(1) - 2*cos(1) +   sin(2) - 1.5*cos(2);
A2 = 1.5*sin(1) -   cos(1) + 2*sin(2) - 0.5*cos(2);
B1 = 0.5*sin(x(1)) - 2*cos(x(1)) +   sin(x(2)) - 1.5*cos(x(2));
B2 = 1.5*sin(x(1)) -   cos(x(1)) - 2*sin(x(2)) - 0.5*cos(x(2));

f(1,1) = (-(1+(A1 - B1)^2 + (A2 - B2)^2));
f(1,2) = (-(((x(1) + 3)^2) + ((x(2) + 1)^2)));
endfunction

//
// La fonction teste VeldMOP4. Elle prend 3 variables en entree.
//

function Res = min_bd_veldmop_4()
Res = -5*ones(3,1);
endfunction

function Res = max_bd_veldmop_4()
Res = 5*ones(3,1);
endfunction

function f = veldmop_4(x)
for(i=1:2)
  SigmF1 = -10*exp(-0.2*sqrt(x(i)^2+x(i+1)^2));
end

for(i=1:2)
  SigmF2 = abs(x(i)) + 5*sin(x(i)^3);
end

f(1,1) = SigmF1;
f(1,2) = SigmF2;
endfunction

//
// La fonction de teste VeldMOP5. Elle prend 2 variables en entree.
//

function Res = min_bd_veldmop_5()
Res = -30*ones(2,1);
endfunction

function Res = max_bd_veldmop_5()
Res = 30*ones(2,1);
endfunction

function f = veldmop_5(x)
f(1,1) = (0.5*x(1)^2+x(2)^2)+sin(x(1)^2 + x(2)^2);
f(1,2) = ((3*x(1)-2*x(2)+4)^2/8.0+(x(1)-x(2)+1)^2/27.0+15.0);
f(1,3) = (1.0/(x(1)^2 + x(2)^2+1)-1.1*exp(-x(1)^2 - x(2)^2));
endfunction

//
// La fonction de teste VeldMOP6. Elle prend 2 variables en entree.
//

function Res = min_bd_veldmop_6()
Res = 0*ones(2,1);
endfunction

function Res = max_bd_veldmop_6()
Res = 1*ones(2,1);
endfunction

function f = veldmop_6(x)
f(1,1) = x(1);
f(1,2) = ((1+10*x(2))*(1-(x(1)/(1+10*x(2))^2)-x(1)/(1+10*x(2))*sin(2*%pi*4*x(1))));
endfunction

//
// La fonction de teste VeldMOP7. Elle prend 2 variables en entree.
//

function Res = min_bd_veldmop_7()
Res = -400*ones(2,1);
endfunction

function Res = max_bd_veldmop_7()
Res = 400*ones(2,1);
endfunction

function f = veldmop_7(x)
f(1,1) = (((x(1)-2)^2)/2.0+((x(2)+1)^2)/13.0+3);
f(1,2) = (((x(1)+x(2)-3)^2)/36.0+((-x(1)+x(2)+2)^2)/8.0-17);
f(1,3) = (((x(1)+2*x(2)-1)^2)/175.0+((2*x(2)-x(1))^2)/17.0-13);
endfunction

//
// La fonction de teste VeldMOPC1. Elle prend 2 variables en entree.
//

function Res = min_bd_veldmopc_1()
Res = [0;0];
endfunction

function Res = max_bd_veldmopc_1()
Res = [5;3];
endfunction

function f = veldmopc_1(x)
f(1,1) = (4*x(1)^2+4*x(2)^2);
f(1,2) = (((x(1)-5)^2)+((x(2)-5)^2));
endfunction

function g = constr_veldmopc_1(x)
g(1) = ((x(1)-5)^2)+x(2)^2-25;
g(2) = -((x(1)-8)^2)-((x(2)+3)^2)+7.7;
endfunction

//
// La fonction de teste VeldMOPC2. Elle prend 6 variables en entree.
//

function Res = min_bd_veldmopc_2()
Res = [0;0;0;1;1;0];
endfunction

function Res = max_bd_veldmopc_2()
Res = [10;10;10;5;5;6];
endfunction

function f = veldmopc_2(x)
f(1,1) = ((((x(1)-2)^2)+((x(2)-2)^2)+((x(3)-1)^2)+((x(4)-4)^2)+((x(5)-1)^2)));
f(1,2) = ((x(1)^2)+(x(2)^2)+(x(3)^2)+(x(4)^2)+(x(5)^2)+(x(6)^2));
endfunction

function g = constr_veldmopc_2(x)
g(1) = -(x(1)+x(2)-2);
g(2) = -(6 - x(1)-x(2));
g(3) = -(2 - x(2)+x(1));
g(4) = -(2 - x(1)+3*x(2));
g(5) = -(4-((x(3)-3)^2)-x(4));
g(6) = -(((x(5)-3)^2)+x(6)-4);
endfunction

//
// La fonction test HANNE1
//

function Res = min_bd_hanne_1()
Res = 0*ones(2,1);
endfunction

function Res = max_bd_hanne_1()
Res = 10*ones(2,1);
endfunction

function f = hanne_1(x)
f(1,1) = x(1);
f(1,2) = x(2);
endfunction

function g = constr_hanne_1(x)
g(1) = -x(1)-x(2)+5;
endfunction

//
// La fonction test HANNE2
//

function Res = min_bd_hanne_2()
Res = 0*ones(2,1);
endfunction

function Res = max_bd_hanne_2()
Res = 10*ones(2,1);
endfunction

function f = hanne_2(x)
f(1,1) = x(1)^2;
f(1,2) = x(2)^2;
endfunction

function g = constr_hanne_2(x)
g(1) = -x(1)-x(2)+5;
endfunction

//
// La fonction test HANNE3
//

function Res = min_bd_hanne_3()
Res = 0*ones(2,1);
endfunction

function Res = max_bd_hanne_3()
Res = 10*ones(2,1);
endfunction

function f = hanne_3(x)
f(1,1) = sqrt(x(1));
f(1,2) = sqrt(x(2));
endfunction

function g = constr_hanne_3(x)
g(1) = -x(1)-x(2)+5;
endfunction

//
// La fonction test HANNE4
//

function Res = min_bd_hanne_4()
Res = 0*ones(2,1);
endfunction

function Res = max_bd_hanne_4()
Res = 10*ones(2,1);
endfunction

function f = hanne_4(x)
f(1,1) = x(1);
f(1,2) = x(2);
endfunction

function g = constr_hanne_4(x)
g(1) = -(x(2) - 5 + 0.5*x(1)*sin(4*x(1)));
endfunction

//
// La fonction test HANNE5
//

function Res = min_bd_hanne_5()
Res = 0*ones(2,1);
endfunction

function Res = max_bd_hanne_5()
Res = 10*ones(2,1);
endfunction

function f = hanne_5(x)
f(1,1) = (floor(x(1))+0.5+(x(1)-floor(x(1)))*sin(2*%pi*(x(2)-floor(x(2)))));
f(1,2) = (floor(x(2))+0.5+(x(1)-floor(x(1)))*cos(2*%pi*(x(2)-floor(x(2)))));
endfunction

function g = constr_hanne_5(x)
g(1) = -x(1)-x(2)+5;
endfunction

//
// Fonctions test mecaniques. Barre avec force au milieu (F).
// Chaque fonction test represente une section differente
// La barre fait 1 m, la force fait 1000 N et le module d'Young
// fait 200 000 MPa
//

//
// Fonction Test MECA1 -> section carre pleine
//

function Res = min_bd_meca_1()
Res = 0.03;
endfunction

function Res = max_bd_meca_1()
Res = 0.1;
endfunction

function f = meca_1(x)
// E = 200000 MPa
// F = 1000 N 
// l = 1 m 

f(1,1) = x(1)^2; // Section;
f(1,2) = (1000.0*1.0/(192.0*200000.0*(x(1)^4)/12.0)*1E-3); // Deformation
endfunction

//
// Fonction Test MECA2 -> section carre creuse
//

function Res = min_bd_meca_2()
Res = 0.03*ones(2,1);
endfunction

function Res = max_bd_meca_2()
Res = 0.1*ones(2,1);
endfunction

function f = meca_2(x)
// E = 200000 MPa
// F = 100 N 
// l = 1 m 

f(1,1) = ((x(1)^2) - (x(2)^2)); // section
f(1,2) = (1000.0*1.0/(192.0*20000.0*(max((x(1)^4)-(x(2)^4),%eps))/12.0)*1E-3);
endfunction

function g = constr_meca_2(x)
g(1) = -x(1)+x(2)+0.04;
endfunction

//
// Probleme test TRIGO1
//

function Res = min_bd_trigo_1()
Res = [0;0];
endfunction

function Res = max_bd_trigo_1()
Res = [%pi/2;1];
endfunction

function f = trigo_1(x)
f(1,1) = (1 - cos(x(1)) + x(2));
f(1,2) = (1 - sin(x(1)) + x(2));
endfunction

//
// Probleme test TRIGO2
//

function Res = min_bd_trigo_2()
Res = [0;0];
endfunction

function Res = max_bd_trigo_2()
Res = [%pi/2;1];
endfunction

function f = trigo_2(x)
if ((x(2)<=0.25)&(x(2)>=0)) then
  f(1,1) = (1 - cos(x(1)) + x(2));
  f(1,2) = (1 - sin(x(1)) + x(2));
elseif ((x(2)<=0.75)&(x(2)>=0.25)) then
  f(1,1) = (1 - cos(x(1)) + 0.25*x(2)+3/16.0);
  f(1,2) = (1 - sin(x(1)) + 0.25*x(2)+3/16.0);
elseif ((x(2)<=1)&(x(2)>=0.75)) then
  f(1,1) = (1 - cos(x(1)) + 2.5*x(2)-1.5);
  f(1,2) = (1 - sin(x(1)) + 2.5*x(2)-1.5);
else
  f(1,1) = 1.0;
  f(1,2) = 1.0;
end
endfunction

//
// Probleme test TRIGO3
//

function Res = min_bd_trigo_3()
Res = [0;0];
endfunction

function Res = max_bd_trigo_3()
Res = [%pi/2;1];
endfunction

function f = trigo_3(x)
if ((x(1)<=(%pi/8.0))&(x(1)>=0)) then
  f(1,1) = (1 - cos(1.5*x(1)) + x(2));
  f(1,2) = (1 - sin(1.5*x(1)) + x(2));
elseif ((x(1)<=(3*%pi/8.0))&(x(1)>=(%pi/8.0))) then
  f(1,1) = (1 - cos(0.5*x(1)+%pi/8) + x(2));
  f(1,2) = (1 - sin(0.5*x(1)+%pi/8) + x(2));
elseif ((x(1)<=%pi/2.0)&(x(1)>=(3*%pi/8.0))) then
  f(1,1) = (1 - cos(1.5*x(1)-%pi/4.0) + x(2));
  f(1,2) = (1 - sin(1.5*x(1)-%pi/4.0) + x(2));
else
  f(1,1) = %pi/2;
  f(1,2) = %pi/2;
end
endfunction

//
// Probleme test TRIGO4
//

function Res = min_bd_trigo_4()
Res = [0;0];
endfunction

function Res = max_bd_trigo_4()
Res = [%pi/2;1];
endfunction

function f = trigo_4(x)
f(1,1) = (sin(x(1)) + x(2));
f(1,2) = (cos(x(1)) + x(2));
endfunction

//
// Probleme test TRIGO4bis
//

function Res = min_bd_trigo_4_bis()
Res = [-%pi/6;0];
endfunction

function Res = max_bd_trigo_4_bis()
Res = [4*%pi/6;1];
endfunction

function f = trigo_4_bis(x)
f(1,1) = (sin(x(1)) + 0.5 + x(2));
f(1,2) = (cos(x(1)) + 0.5 + x(2));
endfunction

//
// Probleme test TRIGO5
//

function Res = min_bd_trigo_5()
Res = [0;0];
endfunction

function Res = max_bd_trigo_5()
Res = [%pi/2;1];
endfunction

function f = trigo_5(x)
if ((x(2)<=0.25)&(x(2)>=0)) then
  f(1,1) = (sin(x(1)) + x(2));
  f(1,2) = (cos(x(1)) + x(2));
elseif ((x(2)<=0.75)&(x(2)>=0.25)) then
  f(1,1) = (sin(x(1)) + 0.25*x(2)+3/16.0);
  f(1,2) = (cos(x(1)) + 0.25*x(2)+3/16.0);
elseif ((x(2)<=1)&(x(2)>=0.75)) then
  f(1,1) = (sin(x(1)) + 2.5*x(2)-1.5);
  f(1,2) = (cos(x(1)) + 2.5*x(2)-1.5);
else 
  f(1,1) = (1.0);
  f(1,2) = (1.0);
end
endfunction

//
// Probleme test TRIGO5bis
//

function Res = min_bd_trigo_5_bis()
Res = [-%pi/6;0];
endfunction

function Res = max_bd_trigo_5_bis()
Res = [4*%pi/6;1];
endfunction

function f = trigo_5_bis(x)
if ((x(2)<=0.25)&(x(2)>=0)) then
  f(1,1) = (sin(x(1)) + 0.5 + x(2));
  f(1,2) = (cos(x(1)) + 0.5 + x(2));
elseif ((x(2)<=0.75)&(x(2)>=0.25)) then
  f(1,1) = (sin(x(1)) + 0.5 + 0.25*x(2)+3/16.0);
  f(1,2) = (cos(x(1)) + 0.5 + 0.25*x(2)+3/16.0);
elseif ((x(2)<=1)&(x(2)>=0.75)) then
  f(1,1) = (sin(x(1)) + 0.5 + 2.5*x(2)-1.5);
  f(1,2) = (cos(x(1)) + 0.5 + 2.5*x(2)-1.5);
end
endfunction

//
// Probleme test TRIGO6 
//

function Res = min_bd_trigo_6()
Res = [0;0];
endfunction

function Res = max_bd_trigo_6()
Res = [%pi/2;1];
endfunction

function f = trigo_6(x)
if ((x(1)<=(%pi/8.0))&(x(1)>=0)) then
  f(1,1) = (sin(1.5*x(1)) + x(2));
  f(1,2) = (cos(1.5*x(1)) + x(2));
elseif ((x(1)<=(3*%pi/8.0))&(x(1)>=(%pi/8.0))) then
  f(1,1) = (sin(0.5*x(1)+%pi/8) + x(2));
  f(1,2) = (cos(0.5*x(1)+%pi/8) + x(2));
elseif ((x(1)<=%pi/2.0)&(x(1)>=(3*%pi/8.0))) then
  f(1,1) = (sin(1.5*x(1)-%pi/4.0) + x(2));
  f(1,2) = (cos(1.5*x(1)-%pi/4.0) + x(2));
else 
  f(1,1) = (%pi/2);
  f(1,2) = (%pi/2);
end
endfunction

//
// Probleme test TRIGO6bis
//

function Res = min_bd_trigo_6_bis()
Res = [-%pi/6;0];
endfunction

function Res = max_bd_trigo_6_bis()
Res = [4*%pi/6;1];
endfunction

function f = trigo_6_bis(x)
if ((x(1)<=(%pi/8.0))&(x(1)>=-%pi/6.0)) then
  f(1,1) = (sin(1.5*x(1)) + 0.5 + x(2));
  f(1,2) = (cos(1.5*x(1)) + 0.5 + x(2));
elseif ((x(1)<=(3*%pi/8.0))&(x(1)>=(%pi/8.0))) then
  f(1,1) = (sin(0.5*x(1)+%pi/8) + 0.5 + x(2));
  f(1,2) = (cos(0.5*x(1)+%pi/8) + 0.5 + x(2));
elseif ((x(1)<=4*%pi/6.0)&(x(1)>=(3*%pi/8.0))) then
  f(1,1) = (sin(1.5*x(1)-%pi/4.0) + 0.5 + x(2));
  f(1,2) = (cos(1.5*x(1)-%pi/4.0) + 0.5 + x(2));
end
endfunction

//
// Probleme test TRIGO7
//

function Res = min_bd_trigo_7()
Res = [0;0];
endfunction

function Res = max_bd_trigo_7()
Res = [%pi/2;1];
endfunction

function f = trigo_7(x)
f(1,1) = ((1-0.05+0.05*sin(16*x(1)))*sin(x(1)) + x(2));
f(1,2) = ((1-0.05+0.05*sin(16*x(1)))*cos(x(1)) + x(2));
endfunction


//
// Probleme test TRIGO8
//

function Res = min_bd_trigo_8()
Res = [0;0];
endfunction

function Res = max_bd_trigo_8()
Res = [%pi/2;1];
endfunction

function f = trigo_8(x)
if ((x(2)<=0.25)&(x(2)>=0)) then
  f(1,1) = ((1-0.05+0.05*sin(16*x(1)))*sin(x(1)) + x(2));
  f(1,2) = ((1-0.05+0.05*sin(16*x(1)))*cos(x(1)) + x(2));
elseif ((x(2)<=0.75)&(x(2)>=0.25)) then
  f(1,1) = ((1-0.05+0.05*sin(16*x(1)))*sin(x(1)) + 0.25*x(2)+3/16.0);
  f(1,2) = ((1-0.05+0.05*sin(16*x(1)))*cos(x(1)) + 0.25*x(2)+3/16.0);
elseif ((x(2)<=1)&(x(2)>=0.75)) then
  f(1,1) = ((1-0.05+0.05*sin(16*x(1)))*sin(x(1)) + 2.5*x(2)-1.5);
  f(1,2) = ((1-0.05+0.05*sin(16*x(1)))*cos(x(1)) + 2.5*x(2)-1.5);
else 
  f(1,1) = (1.0);
  f(1,2) = (1.0);
end


//
// Probleme test TRIGO9  
//

function Res = min_bd_trigo_9()
Res = [0;0];
endfunction

function Res = max_bd_trigo_9()
Res = [%pi/2;1];
endfunction

function f = trigo_9(x)
if ((x(1)<=(%pi/8.0))&(x(1)>=0)) then
  f(1,1) = ((1-0.05+0.05*sin(16*x(1)))*sin(1.5*x(1)) + x(2));
  f(1,2) = ((1-0.05+0.05*sin(16*1.5*x(1)))*cos(1.5*x(1)) + x(2));
elseif ((x(1)<=(3*%pi/8.0))&(x(1)>=(%pi/8.0))) then
  f(1,1) = ((1-0.05+0.05*sin(16*x(1)))*sin(0.5*x(1)+%pi/8) + x(2));
  f(1,2) = ((1-0.05+0.05*sin(16*(0.5*x(1)+%pi/8)))*cos(0.5*x(1)+%pi/8) + x(2));
elseif ((x(1)<=%pi/2.0)&(x(1)>=(3*%pi/8.0))) then
  f(1,1) = ((1-0.05+0.05*sin(16*x(1)))*sin(1.5*x(1)-%pi/4.0) + x(2));
  f(1,2) = ((1-0.05+0.05*sin(16*(1.5*x(1)-%pi/4.0)))*cos(1.5*x(1)-%pi/4.0) + x(2));
else 
  f(1,1) = (%pi/2);
  f(1,2) = (%pi/2);
end
endfunction

