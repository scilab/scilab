//
// Function names:    Dimension     x_opt
//
// rosenbrock             2        [1 1]                0
// mccormic               2        [-0.54719 -1.54719]  -1.9133
// sixhumpcamelb          2        [-0.0898 0.7127]
//                                 [0.0898 -0.7127]     1.0316
// branin2                2
// schubert               2        [? ?]                -186.7309
// hansen                 2        [-7.589893 -7.708314]
//                                 [-7.589893 -1.425128]
//                                 [-7.589893  4.858057]
//                                 [-1.306708 -7.708314]
//                                 [-1.306708 -1.425128]
//                                 [-1.306708 -1.425128]
//                                 [4.976478  -7.708314]
//                                 [4.976478  -1.425128]
//                                 [4.976478  -1.425128] -176.541793
// paviani                2
// booth                  2        [1 3]                0
// matyas                 2        [0 0]                0
// rastrigin              2        [0 0]                0
// griewank2              2        [0 0]                0
// exp2                   3        [1 10 1]             0
// treccani               2        [0 0]                0
// branin                 2        [-%pi 12.275]        0.39788
//                                 [%pi  2.275]         0.39788
//                                 [9.42478, 2.475]     0.39788
// colville               4        [1 1 1 1]            0
// chichinadze            2        [5.90133 0.5]        -43.3159
// hartmann34             3        [0.114614 0.555649 0.852547]  - 3.86278
// hartmann64             6        [0.20169  0.150011 0.476874
//                                  0.275332 0.311652 0.6573]    - 3.32237
// price                  2
// goldsteinprice         2        [0 1]                3
// dixonprice             n (25)   [? ? ?]              0
// hump                   2        [0.0898 -0.7126]
//                                 [-0.0898 0.7126]     0
// dejongf1 (see sphere)
// dejongf2               2        [1 1]                0
// dejongf5               2        [-32 -32]            ~1
// dejongf7               2        [0 0]                0
// schafferf6             2        [0 0]                0
// schafferf7             2
// stuckman               2
// easom                  2        [%pi %pi]           -1
// bohachevsky1           2        [0 0]                0
// bohachevsky2           2        [0 0]                0
// bohachevsky3           2        [0 0]                0
// beale                  2        [3 0.5]              0
// levy13                 2        [1 1]                0
// levy8                  2
// levy5                  2        [-1.30685 -1.424845] -1.761375
// levy2                  30       [1 ... 1]            0
// holtzmann              2
// gen_rosen              n        [1 ... 1]            0
// shekel                 4        [4 4 4 4]            m = 5 -10.1532
//                                                      m = 7 -10.4029
//                                                      m = 10 -10.5364
// griewank               n        [0 ... 0]            0
// sphere                 n        [0 ... 0]            0
// weierstrass            n
// ackley                 n        [0 ... 0]            0
// ellipsoid              n        [0 ... 0]            0
// rotell                 n
// abspow                 n
// michalewicz            n        [? ... ?]               n = 2  f(x*) = -1.8013
//                                                         n = 5  f(x*) = -4.687658
//                                                         n = 10 f(x*) = -9.66015
// powell                 n (24)   [3 -1 0 3 ... -1 0]     0
// power                  4        [1 2 3 4]               0
// gen_rastrigin          n        [0 ... 0]               0
// schwefel               n        [420.9687 ... 420.9687] 0
// trid                   n        [? ... ?]               n = 6  f(x*) = -50
//                                                         n = 10 f(x*) = -200
// zhakarov               n        [0 ... 0]               0
// freudroth              2        [5 4]                   0
// himmelblau             2        [3 2]                   0
// jensamp                2        [0.257825 0.257825]     124.362
// zhufu                  n        [1 ... 1]               0
// cola                   17       [? ... ?]               11.7464
// leon                   2        [1 1]                   0
// giunta                 2        [0.45834282 0.45834282] 0.0602472184
// bukin2                 2        [-10 0]                 0
// bukin4                 2        [-10 0]                 0
// bukin6                 2        [-10 1]                 0
// stybtang               2        [-2.903534 -2.903534]   -78.332
// zettl                  2        [-0.0299 0]             -0.003791
// threehumpcamelb        2        [0 0]                   0


function f_list = get_function_list()
f_list = list();
f_list($+1) = 'rosenbrock';
f_list($+1) = 'mccormic';
f_list($+1) = 'sixhumpcamelb';
f_list($+1) = 'branin2';
f_list($+1) = 'schubert';
f_list($+1) = 'hansen';
f_list($+1) = 'paviani';
f_list($+1) = 'booth';
f_list($+1) = 'matyas';
f_list($+1) = 'rastrigin';
f_list($+1) = 'griewank2';
f_list($+1) = 'exp2';
f_list($+1) = 'treccani';
f_list($+1) = 'branin';
f_list($+1) = 'colville';
f_list($+1) = 'chichinadze';
f_list($+1) = 'hartmann34';
f_list($+1) = 'hartmann64';
f_list($+1) = 'price';
f_list($+1) = 'goldsteinprice';
f_list($+1) = 'dixonprice';
f_list($+1) = 'hump';
f_list($+1) = 'dejongf2';
f_list($+1) = 'dejongf5';
f_list($+1) = 'dejongf7';
f_list($+1) = 'schafferf6'
f_list($+1) = 'schafferf7';
f_list($+1) = 'stuckman';
f_list($+1) = 'easom';
f_list($+1) = 'bohachevsky1';
f_list($+1) = 'bohachevsky2';
f_list($+1) = 'bohachevsky3';
f_list($+1) = 'beale';
f_list($+1) = 'levy13';
f_list($+1) = 'levy8';
f_list($+1) = 'levy5';
f_list($+1) = 'levy2';
f_list($+1) = 'holtzmann';
f_list($+1) = 'gen_rosen';
f_list($+1) = 'shekel';
f_list($+1) = 'griewank';
f_list($+1) = 'sphere';
f_list($+1) = 'weierstrass';
f_list($+1) = 'ackley';
f_list($+1) = 'ellipsoid';
f_list($+1) = 'rotell';
f_list($+1) = 'abspow';
f_list($+1) = 'michalewicz';
f_list($+1) = 'powell';
f_list($+1) = 'power';
f_list($+1) = 'gen_rastrigin';
f_list($+1) = 'schwefel';
f_list($+1) = 'trid';
f_list($+1) = 'zhakarov';
f_list($+1) = 'freudroth';
f_list($+1) = 'himmelblau';
f_list($+1) = 'jensamp';
f_list($+1) = 'zhufu';
f_list($+1) = 'cola';
f_list($+1) = 'leon';
f_list($+1) = 'giunta';
f_list($+1) = 'bukin2';
f_list($+1) = 'bukin4';
f_list($+1) = 'bukin6';
f_list($+1) = 'stybtang';
f_list($+1) = 'zettl';
f_list($+1) = 'threehumpcamelb';
endfunction

//
// The Rosenbrock function
//
function Res = min_bd_rosenbrock()
Res = [-2 -2]';
endfunction
function Res = max_bd_rosenbrock()
Res = [2 2]';
endfunction
function Res = opti_rosenbrock()
Res = [1 1]';
endfunction
function y = rosenbrock(x)
y = 100*(x(2)-x(1)^2)^2+(1-x(1))^2;
endfunction

//
// The McCormic function
//
function Res = min_bd_mccormic()
Res = [-1.5 -3]';
endfunction
function Res = max_bd_mccormic()
Res = [4 4]';
endfunction
function Res = opti_mccormic()
Res = [-0.54719 -1.54719]';
endfunction
function y = mccormic(x)
y = sin(x(1)+x(2))+(x(2)-x(1))^2-1.5*x(1)+2.5*x(2)+1;
endfunction

//
// The Six Hump Camel Back function
//
function Res = min_bd_sixhumpcamelb()
Res = [-5 -5]';
endfunction
function Res= max_bd_sixhumpcamelb()
Res = [5 5]';
endfunction
function Res = opti_sixhumpcamelb()
Res = [-0.0898 0.7127; ...
        0.0898 -0.7127]';
endfunction
function y = sixhumpcamelb(x)
y = 4*x(1)^2-2.1*x(1)^4+1.0/3.0*x(1)^6+x(1)*x(2)-4*x(2)^2+4*x(2)^4;
endfunction

//
// The Branin 2 function
//
function Res = min_bd_branin2()
Res = [-10 -10]';
endfunction
function Res = max_bd_branin2()
Res = [10 10]';
endfunction
function Res = opti_branin2()
Res = [1         0; ...
       0.148696  0.402086; ...
       0.402537  0.287408; ...
       1.59746  -0.287408; ...
       1.85130  -0.402086]';
endfunction
function y=branin2(x)
y = (1-2*x(1)+1.0/20.0*sin(4*%pi*x(2))-x(1))^2+(x(2)-1.0/2.0*sin(2*%pi*x(1)));
endfunction

//
// Schubert function
//
function Res = min_bd_schubert()
Res = [-10 -10]';
endfunction
function Res = max_bd_schubert()
Res = [10 10]';
endfunction
function Res = opti_schubert()
Res = [-6.774576 -6.774576; ...
       -6.774576 -0.491391; ...
       -6.774576  5.791794; ...
       -0.491391 -6.774576; ...
       -0.491391 -0.491391; ...
       -0.491391  5.791794; ...
        5.791794 -6.774576; ...
        5.791794 -0.491391; ...
        5.791794 5.791794]';
endfunction
function y = schubert(x)
y = 0;
for j=1:5
  y = y - j*sin((j+1)*x(1)+j) - j*sin((j+1)*x(2)+j);
end
endfunction

//
// The Hansen function
//
function Res = min_bd_hansen()
Res = [-10 -10]';
endfunction
function Res = max_bd_hansen()
Res = [10 10]';
endfunction
function Res = opti_hansen()
Res = [-7.589893 -7.708314; ...
       -7.589893 -1.425128; ...
       -7.589893  4.858057; ...
       -1.306708 -7.708314; ...
       -1.306708 -1.425128; ...
       -1.306708  4.858057; ...
        4.976478 -7.708314; ...
        4.976478 -1.425128; ...
        4.976478  4.858057]';
endfunction
function y = hansen(x)
s1 = 0;
s2 = 0;
for i=1:5
  s1 = s1 + i*cos((i-1)*x(1)+i);
  s2 = s2 + i*cos((i+1)*x(2)+i);
end
y = s1.*s2;
endfunction

//
// Paviani function
//
function Res = min_bd_paviani(n)
if (~isdef('n','local')) then n = 2; end
Res = 2.001*ones(1,n)';
endfunction
function Res = max_bd_paviani(n)
if (~isdef('n','local')) then n = 2; end
Res = 9.999*ones(1,n)';
endfunction
function Res = opti_paviani(n)
if (~isdef('n','local')) then n = 2; end
Res = 9.350266*ones(1,n)';
endfunction
function y = paviani(x)
y = sum(log(x-2).^2 + log(10-x).^2) - prod(x)^0.2;
endfunction

//
// Booth function
//
function Res = min_bd_booth()
Res = [-10 -10]';
endfunction
function Res = max_bd_booth()
Res = [10 10]';
endfunction
function Res = opti_booth()
Res = [1 3]';
endfunction
function y = booth(x)
  y = (1.5-x(1)+x(1)*x(2))^2+(2.25-x(1)+x(1)*x(2)^2)^2+(2.625-x(1)*x(2)^3)^2;
endfunction

//
// Matyas function
//
function Res = min_bd_matyas()
Res = [-10 -10]';
endfunction
function Res = max_bd_matyas()
Res = [10 10]';
endfunction
function Res = opti_matyas()
Res = [0 0]';
endfunction
function y = matyas(x)
  y = 0.26*(x(1)^2+x(2)^2)-0.48*x(1)*x(2);
endfunction

//
// Rastrigin function
//
function Res = min_bd_rastrigin()
Res = [-1 -1]';
endfunction
function Res = max_bd_rastrigin()
Res = [1 1]';
endfunction
function Res = opti_rastrigin()
Res = [0 0]';
endfunction
function y = rastrigin(x)
  y = x(1)^2+x(2)^2-cos(12*x(1))-cos(18*x(2));
endfunction

//
// Griewank 2 function
//
function Res = min_bd_griewank2()
Res = [-100 -100]';
endfunction
function Res = max_bd_griewank2()
Res = [100 100]';
endfunction
function Res = opti_griewank2()
Res = [0 0]';
endfunction
function y = griewank2(x)
  y = (x(1)^2+x(2)^2)/200.0-200*cos(x(1)).*cos(x(2)/sqrt(2))+1;
endfunction

//
// Exp 2 function
//
function Res = min_bd_exp2()
Res = [0.9 9 0.9]';
endfunction
function Res = max_bd_exp2()
Res = [1.2 11.2 1.2]';
endfunction
function Res = opti_exp2()
Res = [1 10 1]';
endfunction
function y = exp2(x)
y=0;
for i=1:10
  y = y + (exp(-i*x(1)/10.0)-exp(-i*x(2)/10.0))-(exp(-i/10.0)-exp(-i))*x(3);
end
endfunction

//
// Treccani function
//
function Res = min_bd_treccani()
Res = [-5 -5]';
endfunction
function Res = max_bd_treccani()
Res = [5 5]';
endfunction
function Res = opti_treccani()
Res = [0 0]';
endfunction
function y = treccani(x)
y = x(1)^4+4*x(1)^3+4*x(1)^2+x(2)^2;
endfunction

//
// Branin function
//
function Res = min_bd_branin()
Res = [-5 0]';
endfunction
function Res = max_bd_branin()
Res = [15 10]';
endfunction
function Res = opti_branin()
Res = [-%pi     12.275; ...
        %pi     12.275; ...
        9.42478  2.475]';
endfunction
function y = branin(x)
y = (x(2)-(5.1/(4*%pi^2))*x(1)^2+5/%pi*x(1)-6)^2+10*(1-1.0/(8*%pi))*cos(x(1))+10;
endfunction

//
// Chichinadze function
//
function Res = min_bd_chichinadze()
Res = [-30 -10]';
endfunction
function Res = max_bd_chichinadze()
Res = [30 10]';
endfunction
function Res = opti_chichinadze()
Res = [5.90133 0.5]';
endfunction
function y = chichinadze(x)
y = x(1)^2-12*x(1)+11+10*cos(%pi/2.0*x(1))+8*sin(5*%pi*x(1))-1.0/sqrt(5)*exp(-(x(2)-1.0/2.0)^2/2.0);
endfunction

//
// Price function
//
function Res = min_bd_price()
Res = [-10 -10]';
endfunction
function Res = max_bd_price()
Res = [10 10]';
endfunction
function Res = opti_price()
Res = [0         0; ...
       2         4; ...
       1.464352 -2.506012]';
endfunction
function y = price(x)
y = (2*(x(1)^3).*x(2)-x(2)^3)^2+(6*x(1)-x(2)^2+x(2))^2;
endfunction

//
// Dixon-price function
//
function Res = min_bd_dixonprice(n)
if (~isdef('n','local')) then n = 2; end
Res = -10*ones(1,n)';
endfunction
function Res = max_bd_dixonprice(n)
if (~isdef('n','local')) then n = 2; end
Res = 10*ones(1,n)';
endfunction
function Res = opti_dixonprice(n)
if (~isdef('n','local')) then n = 2; end
Res = %nan*ones(1,n)';
endfunction
function y=dixonprice(x)
n = length(x);
s1 = 0;
for j=2:n
  s1 = s1+j*(2*x(j)^2-x(j-1))^2;    
end
y = s1+(x(1)-1)^2;
endfunction

//
// Goldstein-Price function
//
function Res = min_bd_goldsteinprice()
Res = [-2 -2]';
endfunction
function Res = max_bd_goldsteinprice()
Res = [2 2]';
endfunction
function Res = opti_goldsteinprice()
Res = [0 1]';
endfunction
function y = goldsteinprice(x)
y = (1+((x(1)+x(2)+1)^2)*(19-14*x(1)+3*x(1)^2-14*x(2)-6*x(1)*x(2)+3*x(2)^2))*(20+((2*x(1)-3*x(2))^2)*(18-32*x(1)+12*x(1)^2+48*x(2)-36*x(1)*x(2)+27*x(2)^2));
endfunction

//
// De Jong F2 function
//
function Res = min_bd_dejongf2()
Res = [-2.048 -2.048]';
endfunction
function Res = max_bd_dejongf2()
Res = [2.048 2.048]';
endfunction
function Res = opti_dejongf2()
Res = [1 1]';
endfunction
function y = dejongf2(x)
y = 100*(x(1)^2-x(2))^2+(1-x(1))^2;
endfunction

//
// De Jong F5
//
function Res = min_bd_dejongf5()
Res = [-65.536 -65.536]';
endfunction
function Res = max_bd_dejongf5()
Res = [65.536 65.536]';
endfunction
function Res = opti_dejongf5()
Res = [-32 -32]';
endfunction
function y = dejongf5(x)
A = [-32,-32;-16,-32;0,-32; 16,-32;32,-32;-32,-16;-16,-16;0,-16;16,-16;32,-16;-32,0;-16,0;0,0;16,0;32,0;-32,16;-16,16;0,16;16,16;32,16;-32,32;-16,32;0,32;16,32;32,32];
s = 0;
for i=1:25
  s = s + i + (x(1)-A(i,1))^6 + (x(2)-A(i,2))^6;
end
y = 1.0/(1.0/500.0 + s);
endfunction

//
// De Jong F7 function
//
function Res = min_bd_dejongf7()
Res = [-100 -100]';
endfunction
function Res = max_bd_dejongf7()
Res = [100 100]';
endfunction
function Res = opti_dejongf7()
Res = [0 0]';
endfunction
function y = dejongf7(x)
y = (x(1)^2 + x(2)^2)^0.25 * (sin(50*(x(1)^2+x(2)^2)^0.1 + 1)^2);
endfunction

//
// Schaffer F6 function
//
function Res = min_bd_schafferf6()
Res = [-100 -100]';
endfunction
function Res = max_bd_schafferf6()
Res = [100 100]';
endfunction
function Res = opti_schafferf6()
Res = [0 0]';
endfunction
function y = schafferf6(x)
y = 0.5 + (sin(sqrt(x(1)^2+x(2)^2))^2-0.5)/(1+0.001*(x(1)^2+x(2)^2))^2;
endfunction

//
// Schaffer F7 function
//
function Res = min_bd_schafferf7()
Res = [-100 -100]';
endfunction
function Res = max_bd_schafferf7()
Res = [100 100]';
endfunction
function Res = opti_schafferf7()
Res = [%nan %nan]';
endfunction
function y = schafferf7(x)
y = (x(1)^2+x(2)^2)^0.25 .* (sin(50*(x(1)^2+x(2)^2)^0.1)+1);
endfunction

//
// Stuckman function
//
function Res = min_bd_stuckman()
Res = [-10 -10]';
endfunction
function Res = max_bd_stuckman()
Res = [10 10]';
endfunction
function Res = opti_stuckman()
Res = [%nan %nan]';
endfunction
function y = stuckman(x)
m1  = rand(1,1,'uniform')*100;
m2  = rand(1,1,'uniform')*100;
b   = rand(1,1,'uniform')*10;
r11 = rand(1,1,'uniform')*b;
r12 = ((10-b)*rand(1,1,'uniform')+b);
r21 = rand(1,1,'uniform')*10;
r22 = rand(1,1,'uniform')*10;

a1 = ceil(abs(x(1)-r11)) + ceil(abs(x(2)-r21));
a2 = ceil(abs(x(1)-r12)) + ceil(abs(x(2)-r22));

if ((x(1)<=b)&(x(1)>=0)) then
  y = ceil((ceil(m1)+0.5).*sin(a1)/a1);
else
  y = ceil((ceil(m2)+0.5).*sin(a2)/a2);
end
endfunction

//
// Easom function
//
function Res = min_bd_easom()
Res = [-100 -100]';
endfunction
function Res = max_bd_easom()
Res = [100 100]';
endfunction
function Res = opti_easom()
Res = [%pi %pi]';
endfunction
function y = easom(x)
y = -cos(x(1))*cos(x(2))*exp(-(x(1)-%pi)^2-(x(2)-%pi)^2);
endfunction

//
// Bohachevsky 1 function
//
function Res = min_bd_bohachevsky1()
Res = [-50 -50]';
endfunction
function Res = max_bd_bohachevsky1()
Res = [50 50]';
endfunction
function Res = opti_bohachevsky1()
Res = [0 0]';
endfunction
function y = bohachevsky1(x)
y = x(1)^2+2*x(2)^2-0.3*cos(3*%pi*x(1)) - 0.4*cos(4*%pi*x(2)) + 0.7;
endfunction

//
// Bohachevsky 2 function
//
function Res = min_bd_bohachevsky2()
Res = [-50 -50]';
endfunction
function Res = max_bd_bohachevsky2()
Res = [50 50]';
endfunction
function Res = opti_bohachevsky2()
Res = [0 0]';
endfunction
function y = bohachevsky2(x)
y = x(1)^2+2*x(2)^2-0.3*cos(3*%pi*x(1))*cos(4*%pi*x(2)) + 0.3;
endfunction

//
// Bohachevsky 3 function
//
function Res = min_bd_bohachevsky3()
Res = [-50 -50]';
endfunction
function Res = max_bd_bohachevsky3()
Res = [50 50]';
endfunction
function Res = opti_bohachevsky3()
Res = [0 0]';
endfunction
function y = bohachevsky3(x)
y = x(1)^2+2*x(2)^2-0.3*cos(3*%pi*x(1) + 4*%pi*x(2)) + 0.3;
endfunction

//
// Beale function
//
function Res = min_bd_beale()
Res = [-4.5 -4.5]';
endfunction
function Res = max_bd_beale()
Res = [4.5 4.5]';
endfunction
function Res = opti_beale()
Res = [3 0.5]';
endfunction
function y = beale(x)
y = (1.5-x(1)+x(1)*x(2))^2+(2.25-x(1)+x(1)*x(2)^2)^2+(2.625-x(1)+x(1)*x(2)^3)^2;
endfunction

//
// Levy 13 function
//
function Res = min_bd_levy13()
Res = [-10 -10]';
endfunction
function Res = max_bd_levy13()
Res = [10 10]';
endfunction
function Res = opti_levy13()
Res = [1 1]';
endfunction
function y = levy13(x)
y = sin(3*%pi*x(1))^2 + (x(1)-1)^2*(1+sin(3*%pi*x(2))^2) + (x(2)-1)^2*(1+sin(2*%pi*x(2))^2);
endfunction

//
// Levy 8 function
//
function Res = min_bd_levy8(n)
if (~isdef('n','local')) then n = 2; end
if n==4 then
  Res = -10*ones(1,4)';
else
  Res = -5*ones(1,n)';
end
endfunction
function Res = max_bd_levy8(n)
if (~isdef('n','local')) then n = 2; end
if n==4 then
  Res = 10*ones(1,4)';
else
  Res = 5*ones(1,n)';
end
endfunction
function Res = opti_levy8(n)
if (~isdef('n','local')) then n = 2; end
if n==4 then
  Res = [1,1,1,-9.75235]';
else
  Res = ones(1,n);
  Res($) = -4.754402;
  Res = Res';
end
endfunction
function y = levy8(x)
y = sin(3*%pi*x(1))^2;
for i=1:n-1
 y = y + (x(i)-1)^2*(1+sin(3*%pi*x(i+1))^2);
end
y = y + (x($) - 1)^2 * (1+sin(2*%pi*x($))^2);
endfunction

//
// Levy5 function
//
function Res = min_bd_levy5()
Res = [-100 -100]';
endfunction
function Res = max_bd_levy5()
Res = [100 100]';
endfunction
function Res = opti_levy5()
Res = [-1.30685 -1.424845]';
endfunction
function y = levy5(x)
y1 = 0;
y2 = 0;
for i=1:5
  y1 = y1 + i*cos((i-1)*x(1) + i);
  y2 = y2 + i*cos((i+1)*x(2) + i);
end
y = y1*y2 + (x(1) + 1.42513)^2 + (x(2) + 0.80032)^2;
endfunction


//
// Levy2 function
//
function Res = min_bd_levy2(n)
if (~isdef('n','local')) then n = 2; end
Res = -10*ones(1,n)';
endfunction
function Res = max_bd_levy2(n)
if (~isdef('n','local')) then n = 2; end
Res = 10*ones(1,n)';
endfunction
function Res = opti_levy2()
if (~isdef('n','local')) then n = 2; end
Res = ones(1,n)';
endfunction
function y = levy2(x)
n = length(x);
for i=1:n
  z(i) = 1+(x(i)-1)/4;
end
s = sin(%pi*z(1))^2;
for i=1:n-1
  s = s+(z(i)-1)^2*(1+10*(sin(%pi*z(i)+1))^2);
end 
y = s+(z(n)-1)^2*(1+(sin(2*%pi*z(n)))^2);
endfunction

//
// Holtzmann function
//
function Res = min_bd_holtzmann(n)
if (~isdef('n','local')) then n = 2; end
Res = -10*ones(1,n)';
endfunction
function Res = max_bd_holtzmann(n)
if (~isdef('n','local')) then n = 2; end
Res = 10*ones(1,n)';
endfunction
function Res = opti_holtzmann(n)
if (~isdef('n','local')) then n = 2; end
Res = zeros(1,n)';
endfunction
function y=holtzmann(x)
n = length(x);
C = 1:n;
y = sum(C.*x(:)'.^4);
endfunction

//
// Generalized Rosenbrock function
//
function Res = min_bd_gen_rosen(n)
if (~isdef('n','local')) then n = 2; end
Res = -n*ones(1,n)';
endfunction
function Res = max_bd_gen_rosen(n)
if (~isdef('n','local')) then n = 2; end
Res = n*ones(1,n)';
endfunction
function Res = opti_gen_rosen(n)
if (~isdef('n','local')) then n = 2; end
Res = ones(1,n)';
endfunction
function y=gen_rosen(x)
n = length(x);
y = 0;
for i=1:n-1
  y = y + 100*(x(i+1) - x(i)^2)^2 + (x(i) - 1)^2;
end
endfunction

//
// Shekel function
//
function Res = min_bd_shekel()
Res = zeros(1,4)';
endfunction
function Res = max_bd_shekel()
Res = 10*ones(1,4)';
endfunction
function Res = opti_shekel()
Res = 4*ones(1,4)';
endfunction
function y=shekel(x, m)
if ~isdef('m','local') then m = 5; end
// The parameter m should be adjusted m = 5,7,10.
a = ones(10,4);
a(1,:) = 4.0*a(1,:);
a(2,:) = 1.0*a(2,:);
a(3,:) = 8.0*a(3,:);
a(4,:) = 6.0*a(4,:);
for j = 1:2
  a(5,2*j-1) = 3.0; a(5,2*j) = 7.0;
  a(6,2*j-1) = 2.0; a(6,2*j) = 9.0;
  a(7,j)     = 5.0; a(7,j+2) = 3.0;
  a(8,2*j-1) = 8.0; a(8,2*j) = 1.0;
  a(9,2*j-1) = 6.0; a(9,2*j) = 2.0;
  a(10,2*j-1)= 7.0; a(10,2*j)= 3.6;
end
c(1) = 0.1; c(2) = 0.2; c(3) = 0.2; c(4) = 0.4; c(5) = 0.4;
c(6) = 0.6; c(7) = 0.3; c(8) = 0.7; c(9) = 0.5; c(10)= 0.5;
s = 0;
for j = 1:m;
  p = 0;
  for i = 1:4
    p = p+(x(i)-a(j,i))^2;
  end
  s = s+1/(p+c(j));
end
y = -s;
endfunction

//
// Function Griewank
//
function Res = min_bd_griewank(n)
if (~isdef('n','local')) then n = 2; end
Res = -500*ones(1,n)';
endfunction
function Res = max_bd_griewank(n)
if (~isdef('n','local')) then n = 2; end
Res = 700*ones(1,n)';
endfunction
function Res = opti_griewank(n)
if (~isdef('n','local')) then n = 2; end
Res = zeros(1,n)';
endfunction
function y=griewank(x)
a = sum(x.^2/4000);
b = 1;
for i=1:length(x)
  b = b * cos(x(i)/sqrt(i));
end
y = a - b + 1;
endfunction

//
// Function Sphere
//
function Res = min_bd_sphere(n)
if (~isdef('n','local')) then n = 2; end
Res = -100*ones(1,n)';
endfunction
function Res = max_bd_sphere(n)
if (~isdef('n','local')) then n = 2; end
Res = 100*ones(1,n)';
endfunction
function Res = opti_sphere(n)
if (~isdef('n','local')) then n = 2; end
Res = zeros(1,n)';
endfunction
function y=sphere(x)
y = sum(x.^2);
endfunction

//
// Function Weierstrass
//
function Res = min_bd_weierstrass(n)
if (~isdef('n','local')) then n = 2; end
Res = -0.5*ones(1,n)';
endfunction
function Res = max_bd_weierstrass(n)
if (~isdef('n','local')) then n = 2; end
Res = 0.5*ones(1,n)';
endfunction
function Res = opti_weierstrass(n)
if (~isdef('n','local')) then n = 2; end
Res = %nan*ones(1,n)';
endfunction
function y=weierstrass(x)
a = 0.5;
b = 3;
kmax = 20;
y = 0;
for i=1:length(x)
  for j=1:kmax
    y = y + a^j*cos(2*%pi*b^j*(x(i) + 0.5)) - length(x) * a^j*cos(2*%pi*b^j*0.5);
  end
end
endfunction

//
// Function Ackley
//
function Res = min_bd_ackley(n)
if (~isdef('n','local')) then n = 2; end
Res = -32*ones(1,n)';
endfunction
function Res = max_bd_ackley(n)
if (~isdef('n','local')) then n = 2; end
Res = 32*ones(1,n)';
endfunction
function Res = opti_ackley(n)
if (~isdef('n','local')) then n = 2; end
Res = zeros(1,n)';
endfunction
function y = ackley(x)
y = -20*exp(-0.2*sqrt(1/length(x)*sum(x.^2))) - exp(1/length(x)*sum(cos(2*%pi*x))) + 20 + exp(1);
endfunction

//
// Ellipsoid function
//
function Res = min_bd_ellipsoid(n)
if (~isdef('n','local')) then n = 2; end
Res = -5.12*ones(1,n)';
endfunction
function Res = max_bd_ellipsoid(n)
if (~isdef('n','local')) then n = 2; end
Res = 5.12*ones(1,n)';
endfunction
function Res = opti_ellipsoid(n)
if (~isdef('n','local')) then n = 2; end
Res = zeros(1,n)';
endfunction
function y = ellipsoid(x)
C = 1:length(x);
y = sum(C.*x(:)'.^2);
endfunction

//
// Rotated Ellipsoid function
//
function Res = min_bd_rotell(n)
if (~isdef('n','local')) then n = 2; end
Res = -65.536*ones(1,n)';
endfunction
function Res = max_bd_rotell(n)
if (~isdef('n','local')) then n = 2; end
Res = 65.536*ones(1,n)';
endfunction
function Res = opti_rotell(n)
if (~isdef('n','local')) then n = 2; end
Res = zeros(1,n)';
endfunction
function y = rotell(x)
y = 0;
for i=1:length(x)
  y = y + sum(x(1:i))^2;
end
endfunction

//
// absolute power function
//
function Res = min_bd_abspow(n)
if (~isdef('n','local')) then n = 2; end
Res = -1*ones(1,n)';
endfunction
function Res = max_bd_abspow(n)
if (~isdef('n','local')) then n = 2; end
Res = 1*ones(1,n)';
endfunction
function Res = opti_abspow(n)
if (~isdef('n','local')) then n = 2; end
Res = %nan*ones(1,n)';
endfunction
function y = abspow(x)
x = x(:);
C=(1:length(x)) + 1;
y = sum(abs(x).^(C'));
endfunction

//
// Michalewicz function
//
function Res = min_bd_michalewicz(n)
if (~isdef('n','local')) then n = 2; end
Res = zeros(1,n)';
endfunction
function Res = max_bd_michalewicz(n)
if (~isdef('n','local')) then n = 2; end
Res = %pi*ones(1,n)';
endfunction
function Res = opti_michalewicz(n)
if (~isdef('n','local')) then n = 2; end
Res = %nan*ones(1,n)';
endfunction
function y = michalewicz(x)
C = 1:length(x);
m = 10;
y = sum(sin(x(:)').*sin(C.*x(:)'.^2/%pi).^(2*m));
endfunction

//
// Colville function
//
function Res = min_bd_colville()
Res = -10*ones(1,4)';
endfunction
function Res = max_bd_colville()
Res = 10*ones(1,4)';
endfunction
function Res = opti_colville()
Res = ones(1,4)';
endfunction
function y = colville(x)
y  = 100*(x(1)^2-x(2))^2 + (x(1)-1)^2+(x(3)-1)^2 + 90*(x(3)^2-x(4))^2 + 10.1*((x(2)-1)^2 + (x(4)-1)^2) + 19.8*(x(2)-1)*(x(4)-1);
endfunction

//
// Hartmann34 function
//
function Res = min_bd_hartmann34()
Res = [0 0 0]';
endfunction
function Res = max_bd_hartmann34()
Res = [1 1 1]';
endfunction
function Res = opti_hartmann34()
Res = [0.114614 0.555649 0.852547]';
endfunction
function y = hartmann34(x)
a(:,2)=10.0*ones(4,1);
for j=1:2;
  a(2*j-1,1) = 3.0;
  a(2*j,1)   = 0.1; 
  a(2*j-1,3) = 30.0;
  a(2*j,3)   = 35.0; 
end
c = [1.0 1.2 3.0 3.2];
p = [0.36890 0.11700 0.26730; ...
     0.46990 0.43870 0.74700; ...
     0.10910 0.87320 0.55470; ...
     0.03815 0.57430 0.88280];

s = 0;
for i=1:4;
  sm = 0;
  for j=1:3;
    sm = sm + a(i,j)*(x(j)-p(i,j))^2;
  end
  s = s + c(i)*exp(-sm);
end
y = -s;
endfunction

//
// Hartmann64 function
//
function Res = min_bd_hartmann64()
Res = zeros(1,6)';
endfunction
function Res = max_bd_hartmann64()
Res = ones(1,6)';
endfunction
function Res = opti_hartmann64()
Res = [0.20169 0.150011 0.476874; ...
       0.275332 0.311652 0.6573]';
endfunction
function y = hartmann64(x)
a = [10.0 3.0  17.0 3.5  1.7  8.0;  ...
     0.05 10.0 17.0 0.1  8.0  14.0; ...
     3.0  3.5  1.7  10.0 17.0 8.0;  ...
     17.0 8.0  0.05 10.0 0.1  14.0];
c = [1.0 1.2 3.0 3.2];
p = [0.1312 0.1696 0.5569 0.0124 0.8283 0.5886; ...
     0.2329 0.4135 0.8307 0.3736 0.1004 0.9991; ...
     0.2348 0.1451 0.3522 0.2883 0.3047 0.6650; ...
     0.4047 0.8828 0.8732 0.5743 0.1091 0.0381];
s = 0;
for i=1:4;
  sm = 0;
  for j=1:6;
    sm = sm + a(i,j)*(x(j)-p(i,j))^2;
  end
  s = s + c(i)*exp(-sm);
end
y = -s;
endfunction

//
// Hump function
//
function Res = min_bd_hump()
Res = [-5 -5]';
endfunction
function Res = max_bd_hump()
Res = [5 5]';
endfunction
function Res = opti_hump()
Res = [0.0898 -0.7126; ...
       -0.0898 0.7126]';
endfunction
function y = hump(x)
y=1.0316285+4*x(1)^2-2.1*x(1)^4+x(1)^6/3+x(1)*x(2)-4*x(2)^2+4*x(2)^4;
endfunction

//
// Powell function
//
function Res = min_bd_powell(n)
if (~isdef('n','local')) then n = 2; end
Res = -4*ones(1,n)';
endfunction
function Res = max_bd_powell(n)
if (~isdef('n','local')) then n = 2; end
Res = 5*ones(1,n)';
endfunction
function Res = opti_powell()
Res = %nan*ones(1,n)';
endfunction
function y = powell(x)  
m = length(x);
fvec = [];
for i = 1:m/4
  fvec(4*i-3) = x(4*i-3)+10*(x(4*i-2));
  fvec(4*i-2) = sqrt(5)*(x(4*i-1)-x(4*i));
  fvec(4*i-1) = (x(4*i-2)-2*(x(4*i-1)))^2;
  fvec(4*i)   = sqrt(10)*(x(4*i-3)-x(4*i))^2;
end
fvec = fvec';
y = norm(fvec)^2;
endfunction

//
// Power function
//
function Res = min_bd_power()
Res = zeros(1,4)';
endfunction
function Res = max_bd_power()
Res = 4*ones(1,4)';
endfunction
function Res = opti_power()
Res = [1 2 3 4]';
endfunction
function y = power(x)
n = 4;
b = [8,18,44,114];
s_out = 0;
for k=1:n
  s_in = 0;
  for j = 1:n
    s_in = s_in+x(j)^k;
  end
  s_out = s_out+(s_in-b(k))^2;
end
y = s_out;
endfunction

//
// Generalized rastrigin
//
function Res = min_bd_gen_rastrigin(n)
if (~isdef('n','local')) then n = 2; end
Res = -5.12*ones(1,n)';
endfunction
function Res = max_bd_gen_rastrigin(n)
if (~isdef('n','local')) then n = 2; end
Res = 5.12*ones(1,n)';
endfunction
function Res = opti_gen_rastrigin(n)
if (~isdef('n','local')) then n = 2; end
Res = zeros(1,n)';
endfunction
function y = gen_rastrigin(x)
n = length(x);
s = 0;
for j = 1:n
  s = s+(x(j)^2-10*cos(2*%pi*x(j))); 
end
y = 10*n+s;
endfunction

//
// Schwefel function
//
function Res = min_bd_schwefel(n)
if (~isdef('n','local')) then n = 2; end
Res = -500*ones(1,n)';
endfunction
function Res = max_bd_schwefel(n)
if (~isdef('n','local')) then n = 2; end
Res = 500*ones(1,n)';
endfunction
function Res = opti_schwefel(n)
if (~isdef('n','local')) then n = 2; end
Res = 420.9687*ones(1,n)';
endfunction
function y = schwefel(x)
s = sum(-x.*sin(sqrt(abs(x))));
y = 418.9829*n+s;
endfunction

//
// Trid function
//
function Res = min_bd_trid(n)
if (~isdef('n','local')) then n = 2; end
Res = -n^2*ones(1,n)';
endfunction
function Res = max_bd_trid(n)
if (~isdef('n','local')) then n = 2; end
Res = n^2*ones(1,n)';
endfunction
function Res = opti_trid(n)
if (~isdef('n','local')) then n = 2; end
Res = %nan*ones(1,n)';
endfunction
function  y = trid(x)
n = length(x);
s1 = 0;
s2 = 0;
for j=1:n
  s1 = s1+(x(j)-1)^2;    
end
for j=2:n
  s2 = s2+x(j)*x(j-1);    
end
y = s1-s2;
endfunction

//
// Zhakarov function
//
function Res = min_bd_zhakarov(n)
if (~isdef('n','local')) then n = 2; end
Res = -5*ones(1,n)';
endfunction
function Res = max_bd_zhakarov(n)
if (~isdef('n','local')) then n = 2; end
Res = 10*ones(1,n)';
endfunction
function Res = opti_zhakarov(n)
if (~isdef('n','local')) then n = 2; end
Res = zeros(1,n)';
endfunction
function y = zhakarov(x)
n = length(x);
s1 = 0;
s2 = 0;
for j=1:n
  s1 = s1+x(j)^2;
  s2 = s2+0.5*j*x(j);
end
y = s1+s2^2+s2^4;
endfunction

//
// Freudenstein-Roth function
//
function Res = min_bd_freudroth()
Res = [-10 -10]';
endfunction
function Res = max_bd_freudroth()
Res = [10 10]';
endfunction
function Res = opti_freudroth()
Res = [5 4]';
endfunction
function y=freudroth(x)
y = ((x(1) - 13) + ((5 - x(2))*x(2) - 2)*x(2))^2 + ((x(1) - 29) + ((x(2) + 1)*x(2) - 14)*x(2))^2;
endfunction

//
// Himmelblau function
//
function Res = min_bd_himmelblau()
Res = [-5 -5]';
endfunction
function Res = max_bd_himmelblau()
Res = [5 5]';
endfunction
function Res = opti_himmelblau()
Res = [3 2]';
endfunction
function y = himmelblau(x)
y = (x(1)^2 + x(2) - 11)^2 + (x(1) + x(2)^2 - 7)^2;
endfunction

//
// Jennrich-sampson function
//
function Res = min_bd_jensamp()
Res = [-1 -1]';
endfunction
function Res = max_bd_jensamp()
Res = [1 1]';
endfunction
function Res = opti_jensamp()
Res = [0.257825 0.257825]';
endfunction
function y = jensamp(x)
aux = 0;
for i=1:10
  aux = aux + (2+2*i+exp(i*x(1)) + exp(i*x(2)))^2;
end
y = aux;
endfunction

//
// Zhu-fu function
//
function Res = min_bd_zhufu(n)
if (~isdef('n','local')) then n = 2; end
Res = -10*ones(1,n)';
endfunction
function Res = max_bd_zhufu(n)
if (~isdef('n','local')) then n = 2; end
Res = 10*ones(1,n)';
endfunction
function Res = opti_zhufu(n)
if (~isdef('n','local')) then n = 2; end
Res = ones(1,n)';
endfunction
function y = zhufu(x)
n = length(x);
aux = 0;
for i=1:n-1
  aux = aux + (x(i) - 1)^2*(1 + sin(3*%pi*x(i+1))^2);
end
aux = aux + (x(n) - 1)^2*(1 + sin(2*%pi*x(n))^2) + sin(3*%pi*x(1))^2;
y = aux / 10;
endfunction

//
// Cola function
//
function Res = min_bd_cola()
Res    = -4*ones(1,17)';
Res(1) = 0;
endfunction
function Res = max_bd_cola()
Res = 4*ones(1,17)';
endfunction
function Res = opti_cola()
Res = %nan*ones(1,17)';
endfunction
function y=cola(x)
x = x(:)';
d = [0   0    0    0    0    0    0    0    0   ; ...
    1.27 0    0    0    0    0    0    0    0   ; ...
    1.69 1.43 0    0    0    0    0    0    0   ; ...
    2.04 2.35 2.43 0    0    0    0    0    0   ; ...
    3.09 3.18 3.26 2.85 0    0    0    0    0   ; ...
    3.20 3.22 3.27 2.88 1.55 0    0    0    0   ; ...
    2.86 2.56 2.58 2.59 3.12 3.06 0    0    0   ; ...
    3.17 3.18 3.18 3.12 1.31 1.64 3.00 0    0   ; ...
    3.21 3.18 3.18 3.17 1.70 1.36 2.95 1.32 0   ; ...
    2.38 2.31 2.42 1.94 2.85 2.81 2.56 2.91 2.97];
x1 = [0 x(1) x(2:2:$)];
x2 = [0 0    x(3:2:$)];
y  = 0;
for i=2:length(x1)
  for j=1:i-1
    y = y + (((x1(i) - x1(j))^2 + (x2(i) - x2(j))^2) - d(i,j))^2;
  end
end
endfunction

//
// Leon function
//
function Res = min_bd_leon()
Res = [-1.2 -1.2]';
endfunction
function Res = max_bd_leon()
Res = [1.2 1.2]';
endfunction
function Res = opti_leon()
Res = [1 1]';
endfunction
function y=leon(x)
  y = 100*(x(2) - x(1)^3)^2 + (1 - x(1))^2;
endfunction

//
// Giunta function
//
function Res = min_bd_giunta()
Res = [-1.2 -1.2]';
endfunction
function Res = max_bd_giunta()
Res = [1.2 1.2]';
endfunction
function Res = opti_giunta()
Res = [0.45834282 0.45834282]';
endfunction
function y = giunta(x)
y = sin(16/15*x(1)-1) + sin(16/15*x(1)-1)^2 + 1/50*sin(4*(16/15*x(1)-1)) + ...
    sin(16/15*x(2)-1) + sin(16/15*x(2)-1)^2 + 1/50*sin(4*(16/15*x(2)-1)) + 0.6;
endfunction

//
// Bukin2 function
//
function Res = min_bd_bukin2()
Res = [-15 -3]';
endfunction
function Res = max_bd_bukin2()
Res = [-5 3]';
endfunction
function Res = opti_bukin2()
Res = [-10 0]';
endfunction
function y = bukin2(x)
y = 100*(x(2) - 0.01*x(1)^2 + 1) + 0.01*(x(1) + 10)^2;
endfunction

//
// Bukin4 function
//
function Res = min_bd_bukin4()
Res = [-1.2 -1.2]';
endfunction
function Res = max_bd_bukin4()
Res = [1.2 1.2]';
endfunction
function Res = opti_bukin4()
Res = [-10 0]';
endfunction
function y = bukin4(x)
y = 100*x(2)^2 + 0.01*abs(x(1)+10);
endfunction

//
// Bukin6 function
//
function Res = min_bd_bukin6()
Res = [-1.2 -1.2]';
endfunction
function Res = max_bd_bukin6()
Res = [1.2 1.2]';
endfunction
function Res = opti_bukin6()
Res = [-10 1]';
endfunction
function y = bukin6(x)
y = 100*sqrt(abs(x(2) - 0.01*x(1)^2)) + 0.01*abs(x(1) + 10);
endfunction

//
// Styblinsky-tang function
//
function Res = min_bd_stybtang()
Res = [-5 -5]';
endfunction
function Res = max_bd_stybtang()
Res = [15 15]';
endfunction
function Res = opti_stybtang()
Res = [-2.903534 -2.903534]';
endfunction
function y = stybtang(x)
y = 0.5*(x(1)^4-16*x(1)^2+5*x(1) + x(2)^4 - 16*x(2)^2 + 5*x(2));
endfunction

//
// Zettl function
//
function Res = min_bd_zettl()
Res = [-5 -5]';
endfunction
function Res = max_bd_zettl()
Res = [5 5]';
endfunction
function Res = opti_zettl()
Res = [-0.0299 0]';
endfunction
function y = zettl(x)
y = (x(1)^2 + x(2)^2 - 2*x(1))^2 + 0.25*x(1);
endfunction

//
// Three hump camel back
//
function Res = min_bd_threehumpcamelb()
Res = [-5 -5]';
endfunction
function Res= max_bd_threehumpcamelb()
Res = [5 5]';
endfunction
function Res = opti_threehumpcamelb()
Res = [0 0]';
endfunction
function y = threehumpcamelb(x)
y = 2*x(1)^2 - 1.05*x(1)^4 + x(1)^6/6 + x(1)*x(2) + x(2)^2;
endfunction

