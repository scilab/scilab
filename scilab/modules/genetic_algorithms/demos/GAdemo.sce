// example of use of the genetic algorithm

lines(0);
old_funcprot = funcprot();
funcprot(0);

// Definition of the Rastrigin test problem

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

func = 'rastrigin';

deff('y=f(x)','y = '+func+'(x)');

PopSize     = 100;
Proba_cross = 0.7;
Proba_mut   = 0.1;
NbGen       = 10;
NbCouples   = 110;
Log         = %T;
nb_disp     = 10; // Nb point to display from the optimal population
pressure    = 0.05;

ga_params = init_param();
// Parameters to adapt to the shape of the optimization problem
ga_params = add_param(ga_params,'minbound',eval('min_bd_'+func+'()'));
ga_params = add_param(ga_params,'maxbound',eval('max_bd_'+func+'()'));
ga_params = add_param(ga_params,'dimension',2);
ga_params = add_param(ga_params,'beta',0);
ga_params = add_param(ga_params,'delta',0.1);
// Parameters to fine tune the Genetic algorithm. All these parameters are optional for continuous optimization
// If you need to adapt the GA to a special problem, you 
ga_params = add_param(ga_params,'init_func',init_ga_default);
ga_params = add_param(ga_params,'crossover_func',crossover_ga_default);
ga_params = add_param(ga_params,'mutation_func',mutation_ga_default);
ga_params = add_param(ga_params,'codage_func',coding_ga_identity);
ga_params = add_param(ga_params,'selection_func',selection_ga_elitist);
//ga_params = add_param(ga_params,'selection_func',selection_ga_random);
ga_params = add_param(ga_params,'nb_couples',NbCouples);
ga_params = add_param(ga_params,'pressure',pressure);

Min = get_param(ga_params,'minbound');
Max = get_param(ga_params,'maxbound');
x0  = (Max - Min) .* rand(size(Min,1),size(Min,2)) + Min;

//////////////////////////////////////////

x = Min(1):(Max(1)-Min(1))/20:Max(1); y = Min(2):(Max(2)-Min(2))/20:Max(2);
[X,Y]=meshgrid(x,y);
for i=1:size(X,1)
  for j=1:size(X,2)
    Z(i,j) = eval(func+'([X(i,j) Y(i,j)])');
  end
end

scf();
drawlater;
xset('fpf',' ');
contour(x,y,Z', 10);
_axes = get("current_axes");
_axes.data_bounds = [Min(1) Max(1) Min(2) Max(2)];
xtitle('Genetic Algorithm - real variable','x1','x2');

drawnow;

///////////////////////
// Genetic Algorithm //
///////////////////////

[pop_opt, fobj_pop_opt, pop_init, fobj_pop_init] = optim_ga(f, PopSize, NbGen, Proba_mut, Proba_cross, Log, ga_params);

if (size(pop_opt(1)',2)==2) then
  drawlater;
  printf('plotting init population ...\n');
  for i=1:length(pop_init)
    plot(pop_init(i)(1),pop_init(i)(2),'r.');
  end
  printf('plotting result population ...\n');
  for i=1:length(pop_opt)
    plot(pop_opt(i)(1),pop_opt(i)(2),'g.');
  end
  drawnow;
end

printf('Genetic Algorithm: %d points from pop_opt\n', nb_disp);
for i=1:nb_disp 
  printf('Individual %d: x(1) = %f x(2) = %f -> f = %f\n', i, pop_opt(i)(1), pop_opt(i)(2), fobj_pop_opt(i));
end

/////////////////////////////////////////
// Genetic Algorithm for binary codage //
/////////////////////////////////////////

deff('y=f(x)','BinLen = get_param(ga_params,''binary_length''); ...
               tmp    = convert_to_float(x, BinLen, Max, Min); ...
               y      = '+func+'(tmp);','n');

ga_params = add_param(ga_params,'binary_length',8);
ga_params = set_param(ga_params,'crossover_func',crossover_ga_binary);
ga_params = set_param(ga_params,'mutation_func',mutation_ga_binary);
ga_params = set_param(ga_params,'codage_func',coding_ga_binary);
ga_params = add_param(ga_params,'multi_cross',%T);
ga_params = add_param(ga_params,'multi_cross_nb',3);

[pop_opt, fobj_pop_opt, pop_init, fobj_pop_init] = optim_ga(f, PopSize, NbGen, Proba_mut, Proba_cross, Log, ga_params);

if (size(pop_opt(1)',2)==2) then
  scf();
  drawlater;
  xset('fpf',' ');
  contour(x,y,Z', 10);
  _axes = get("current_axes");
  _axes.data_bounds = [Min(1) Max(1) Min(2) Max(2)];
  xtitle('Genetic Algorithm - binary variables','x1','x2');
  
  printf('plotting init population ...\n');
  for i=1:length(pop_init)
    plot(pop_init(i)(1),pop_init(i)(2),'r.');
  end
  printf('plotting result population ...\n');
  for i=1:length(pop_opt)
    plot(pop_opt(i)(1),pop_opt(i)(2),'g.');
  end
  drawnow;
end

printf('Genetic Algorithm - binary: %d points from pop_opt\n', nb_disp); 
for i=1:nb_disp 
  printf('Individual %d: x(1) = %f x(2) = %f -> f = %f\n', i, pop_opt(i)(1), pop_opt(i)(2), fobj_pop_opt(i));
end

funcprot(old_funcprot);

