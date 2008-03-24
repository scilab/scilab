// Test of the Parallel Tempering 

// Loading test problems
getf('../../test_problems/cont_funcs.sci');

//functoplot='rosenbrock';
//functoplot='mccormic';
//functoplot='sixhumpcamelback';
//functoplot='branin';
//functoplot='schubert';
//functoplot='hansen';
//functoplot='paviani';
//functoplot='booth';
//functoplot='matyas';
functoplot='rastrigin';
//functoplot='griewank2';
//functoplot='exp2';
//functoplot='treccani';
//functoplot='branin2';
//functoplot='chichinadze';
//functoplot='price';
//functoplot='goldsteinprice';
//functoplot='dejongf2';
//functoplot='dejongf5';
//functoplot='dejongf7';
//functoplot='schafferf6';
//functoplot='schafferf7';
//functoplot='stuckman';
//functoplot='easom';
//functoplot='bohachevsky1';
//functoplot='bohachevsky2';
//functoplot='beale';
//functoplot='holtzmann';
//functoplot='griewank';
//functoplot='sphere';
//functoplot='weierstrass';
//functoplot='ackley';
//functoplot='ellipsoid';
//functoplot='rotell';
//functoplot='abspow';
//functoplot='michalewicz';
//functoplot='levy5';
//functoplot='levy13';

deff('y=fobj(x)','y='+functoplot+'(x)');

// example of use of the parallel tempering method


Proba_start = [0.1 0.3 0.6 0.9];
ItMax    = 100;
It_Pre   = 100;
st_proba = 0.05;
NbTemp   = 4;

//////////////////////////////////////////

printf('\n*************** start example %s ***************\n\n', functoplot);

x_min = eval('min_bd_'+functoplot+'()');
x_max = eval('max_bd_'+functoplot+'()');

x0 = (x_max - x_min) .* rand(size(x_min,1),size(x_min,2)) + x_min;

Z = [];
x = x_min(1):(x_max(1) - x_min(1))/ 10:x_max(1);
y = x_min(2):(x_max(2) - x_min(2))/ 10:x_max(2);

[X,Y]=meshgrid(x,y);
for i=1:size(X,1)
  for j=1:size(X,2)
    Z(i,j) = eval(functoplot+'([X(i,j) Y(i,j)])');
  end
end

////////////////////////
// Parallel Tempering //
////////////////////////

delta = [-0.1*(x_max - x_min)' 0.1*(x_max - x_min)']';

sa_params = init_param();
sa_params = add_param(sa_params,'delta',delta);

printf('PT: parallel tempering\n');

T0 = compute_initial_temp(x0, fobj, Proba_start, It_Pre, sa_params);
printf('Initial temperature T0 = '); disp(T0);
T = T0;
//// Generation of the temperature list
//T = zeros(1,NbTemp);
//T(NbTemp) = T0;
//for i=NbTemp-1:-1:1
//  T(i) = T(i+1) / 10.0;
//end

[x_opt, x_history] = optim_pt(x0, fobj, ItMax, T, st_proba, Log = %T, sa_params);

printf('Solution found:'); disp(x_opt');
printf('value of the objective function = %f\n', fobj(x_opt));
printf('Optimal solution:'); disp(eval('opti_'+functoplot+'()')');

drawlater;
Index = 1;
for j=1:length(x_history)
  printf('curve %d / %d drawn\n',j,length(x_history));
  
  subplot(2,2,Index);
  xset('fpf',' ');
  contour(x,y,Z,10);
  _axes = get("current_axes");
  _axes.data_bounds = [x_min(1) x_max(1) x_min(2) x_max(2)];
  
  for i=1:length(x_history(j))-1
    plot([x_history(j)(i)(1) x_history(j)(i+1)(1)],[x_history(j)(i)(2) x_history(j)(i+1)(2)],'k-');
    plot(x_history(j)(i)(1),x_history(j)(i)(2),'ro');
  end
  plot(x_history(j)($)(1),x_history(j)($)(2),'ro');
  xtitle(sprintf('%s - Markov chain %f',functoplot,T(j)),'x1','x2');
  Index = Index + 1;
end
drawnow;

