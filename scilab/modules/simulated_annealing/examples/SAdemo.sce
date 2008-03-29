// example of use of the simulated annealing method

// Loading the test problems
getf('../../test_problems/cont_funcs.sci');

//func = 'rosenbrock';
//func = 'mccormic';
//func = 'sixhumpcamelb';
//func = 'branin2';
//func = 'schubert';
//func = 'hansen';
//func = 'paviani';
//func = 'booth';
//func = 'matyas';
func = 'rastrigin';
//func = 'griewank2';
//func = 'exp2';
//func = 'treccani';
//func = 'branin';
//func = 'colville';
//func = 'chichinadze';
//func = 'hartmann34';
//func = 'hartmann64';
//func = 'price';
//func = 'goldsteinprice';
//func = 'dixonprice';
//func = 'hump';
//func = 'dejongf2';
//func = 'dejongf5';
//func = 'dejongf7';
//func = 'schafferf6'
//func = 'schafferf7';
//func = 'stuckman';
//func = 'easom';
//func = 'bohachevsky1';
//func = 'bohachevsky2';
//func = 'bohachevsky3';
//func = 'beale';
//func = 'levy13';
//func = 'levy8';
//func = 'levy5';
//func = 'levy2';
//func = 'holtzmann';
//func = 'gen_rosen';
//func = 'shekel';
//func = 'griewank';
//func = 'sphere';
//func = 'weierstrass';
//func = 'ackley';
//func = 'ellipsoid';
//func = 'rotell';
//func = 'abspow';
//func = 'michalewicz';
//func = 'powell';
//func = 'power';
//func = 'gen_rastrigin';
//func = 'schwefel';
//func = 'trid';
//func = 'zhakarov';
//func = 'freudroth';
//func = 'himmelblau';
//func = 'jensamp';
//func = 'zhufu';
//func = 'cola';
//func = 'leon';
//func = 'giunta';
//func = 'bukin2';
//func = 'bukin4';
//func = 'bukin6';
//func = 'stybtang';
//func = 'zettl';
//func = 'threehumpcamelb';

Proba_start = 0.8;
It_intern = 1000;
It_extern = 30;
It_Pre    = 100;

DoSA    = %T;
DoFSA   = %F;
DoVFSA  = %F;
DoCSA   = %F;
DoHuang = %T;

//////////////////////////////////////////

Min = eval('min_bd_'+func+'()');
Max = eval('max_bd_'+func+'()');
x0  = (Max - Min).*rand(size(Min,1),size(Min,2)) + Min;

deff('y=f(x)','y='+func+'(x)');

/////////////////////////
// Simulated Annealing //
/////////////////////////

if DoSA then
  printf('SA: geometrical decrease temperature law\n');

  sa_params = init_param();
  sa_params = add_param(sa_params,'min_delta',-0.1*(Max-Min));
  sa_params = add_param(sa_params,'max_delta', 0.1*(Max-Min));
  sa_params = add_param(sa_params,'neigh_func', neigh_func_default); // Optional
  sa_params = add_param(sa_params,'accept_func', accept_func_default); // Optional
  sa_params = add_param(sa_params,'temp_law', temp_law_default); // Optional
  sa_params = add_param(sa_params,'min_bound',Min);
  sa_params = add_param(sa_params,'max_bound',Max);

  T0 = compute_initial_temp(x0, f, Proba_start, It_Pre, sa_params);
  printf('Initial temperatore T0 = %f\n', T0);

  [x_opt, f_opt, sa_mean_list, sa_var_list, temp_list] = optim_sa(x0, f, It_extern, It_intern, T0, Log = %T, sa_params);

  printf('optimal solution:\n'); disp(x_opt);
  printf('value of the objective function = %f\n', f_opt);

  scf();
  drawlater;
  subplot(2,1,1);
  xtitle('Geometrical annealing','Iteration','Mean / Variance');
  t = 1:length(sa_mean_list);
  plot(t,sa_mean_list,'r',t,sa_var_list,'g');
  legend(['Mean','Variance']);
  subplot(2,1,2);
  xtitle('Temperature evolution','Iteration','Temperature');
  for i=1:length(t)-1
    plot([t(i) t(i+1)], [temp_list(i) temp_list(i)],'k-');
  end
  drawnow;
end

/////////
// FSA //
/////////

if DoFSA then
  printf('SA: the FSA algorithm\n');

  sa_params = init_param();
  sa_params = add_param(sa_params,'min_delta',-0.1*(Max-Min));
  sa_params = add_param(sa_params,'max_delta', 0.1*(Max-Min));
  sa_params = add_param(sa_params,'neigh_func', neigh_func_default); // Required for compute_initial_temp
  sa_params = add_param(sa_params,'accept_func', accept_func_default); // Optional
  sa_params = add_param(sa_params,'temp_law', temp_law_fsa); // Required to transform SA into FSA
  sa_params = add_param(sa_params,'min_bound',Min);
  sa_params = add_param(sa_params,'max_bound',Max);

  T0 = compute_initial_temp(x0, f, Proba_start, It_Pre, sa_params);
  
  sa_params = remove_param(sa_params,'neigh_func');
  sa_params = add_param(sa_params,'neigh_func', neigh_func_fsa); // Required to transform SA into FSA
  
  printf('Initial temperatore T0 = %f\n', T0);

  [x_opt, f_opt, sa_mean_list, sa_var_list, temp_list] = optim_sa(x0, f, It_extern, It_intern, T0, Log = %T, sa_params);

  printf('optimal solution:\n'); disp(x_opt);
  printf('value of the objective function = %f\n', f_opt);

  scf();
  drawlater;
  subplot(2,1,1);
  xtitle('FSA','Iteration','Mean / Variance');
  t = 1:length(sa_mean_list);
  plot(t,sa_mean_list,'r',t,sa_var_list,'g');
  legend(['Mean','Variance']);
  subplot(2,1,2);
  xtitle('Temperature evolution','Iteration','Temperature');
  for i=1:length(t)-1
    plot([t(i) t(i+1)], [temp_list(i) temp_list(i)],'k-');
  end
  drawnow;
end

//////////
// VFSA //
//////////

if DoVFSA then
  printf('SA: the VFSA algorithm\n');

  sa_params = init_param();
  sa_params = add_param(sa_params,'min_delta',-0.1*(Max-Min));
  sa_params = add_param(sa_params,'max_delta', 0.1*(Max-Min));
  sa_params = add_param(sa_params,'accept_func', accept_func_vfsa); // Required to transform SA into FSA
  sa_params = add_param(sa_params,'neigh_func', neigh_func_default); // Required for compute_initial_temp
  sa_params = add_param(sa_params,'temp_law', temp_law_vfsa); // Required to transform SA into FSA
  sa_params = add_param(sa_params,'type_accept', 'vfsa'); // Required to compute correctly the starting temperature for VFSA
  sa_params = add_param(sa_params,'dimension', length(x0)); // Required to compute correctly the starting temperature for VFSA
  sa_params = add_param(sa_params,'min_bound',Min);
  sa_params = add_param(sa_params,'max_bound',Max);
  
  T0 = compute_initial_temp(x0, f, Proba_start, It_Pre, sa_params);

  sa_params = remove_param(sa_params,'neigh_func');
  sa_params = add_param(sa_params,'neigh_func', neigh_func_vfsa); // Required to transform SA into VFSA
  sa_params = remove_param(sa_params,'type_accept');
  sa_params = add_param(sa_params,'type_accept', 'sa'); // We go back to the classical method for computing the starting temperature

  printf('Initial temperatore T0 = %f\n', T0);

  [x_opt, f_opt, sa_mean_list, sa_var_list, temp_list] = optim_sa(x0, f, It_extern, It_intern, T0, Log = %T, sa_params);

  printf('optimal solution:\n'); disp(x_opt);
  printf('value of the objective function = %f\n', f_opt);

  scf();
  drawlater;
  subplot(2,1,1);
  xtitle('VFSA','Iteration','Mean / Variance');
  t = 1:length(sa_mean_list);
  plot(t,sa_mean_list,'r',t,sa_var_list,'g');
  legend(['Mean','Variance']);
  subplot(2,1,2);
  xtitle('Temperature evolution','Iteration','Temperature');
  for i=1:length(t)-1
    plot([t(i) t(i+1)], [temp_list(i) temp_list(i)],'k-');
  end
  drawnow;
end

/////////
// CSA //
/////////

if DoCSA then
  printf('SA: the CSA algorithm\n');

  sa_params = init_param();
  sa_params = add_param(sa_params,'min_delta',-0.1*(Max-Min));
  sa_params = add_param(sa_params,'max_delta', 0.1*(Max-Min));
  sa_params = add_param(sa_params,'neigh_func', neigh_func_default); // Required for compute_initial_temp
  sa_params = add_param(sa_params,'accept_func', accept_func_default); // Optional
  sa_params = add_param(sa_params,'temp_law', temp_law_csa); // Required to transform SA into CSA
  sa_params = add_param(sa_params,'min_bound',Min);
  sa_params = add_param(sa_params,'max_bound',Max);

  T0 = compute_initial_temp(x0, f, Proba_start, It_Pre, sa_params);
  
  sa_params = remove_param(sa_params,'neigh_func');
  sa_params = add_param(sa_params,'neigh_func', neigh_func_csa); // Required to transform SA into CSA

  printf('Initial temperatore T0 = %f\n', T0);

  [x_opt, f_opt, sa_mean_list, sa_var_list, temp_list] = optim_sa(x0, f, It_extern, It_intern, T0, Log = %T, sa_params);

  printf('optimal solution:\n'); disp(x_opt);
  printf('value of the objective function = %f\n', f_opt);

  scf();
  drawlater;
  subplot(2,1,1);
  xtitle('Classical simulated annealing','Iteration','Mean / Variance');
  t = 1:length(sa_mean_list);
  plot(t,sa_mean_list,'r',t,sa_var_list,'g');
  legend(['Mean','Variance']);
  subplot(2,1,2);
  xtitle('Temperature evolution','Iteration','Temperature');
  for i=1:length(t)-1
    plot([t(i) t(i+1)], [temp_list(i) temp_list(i)],'k-');
  end
  drawnow;
end

///////////
// Huang //
///////////

if DoHuang then
  printf('SA: the Huang annealing\n');

  sa_params = init_param();
  sa_params = add_param(sa_params,'min_delta',-0.1*(Max-Min));
  sa_params = add_param(sa_params,'max_delta', 0.1*(Max-Min));
  sa_params = add_param(sa_params,'temp_law', temp_law_huang);
  sa_params = add_param(sa_params,'min_bound',Min);
  sa_params = add_param(sa_params,'max_bound',Max);

  T0 = compute_initial_temp(x0, f, Proba_start, It_Pre, sa_params);
  printf('Initial temperatore T0 = %f\n', T0);

  [x_opt, f_opt, sa_mean_list, sa_var_list, temp_list] = optim_sa(x0, f, It_extern, It_intern, T0, Log = %T, sa_params);

  printf('optimal solution:\n'); disp(x_opt);
  printf('value of the objective function = %f\n', f_opt);

  scf();
  drawlater;
  subplot(2,1,1);
  xtitle('Huang annealing','Iteration','Mean / Variance');
  t = 1:length(sa_mean_list);
  plot(t,sa_mean_list,'r',t,sa_var_list,'g');
  legend(['Mean','Variance']);
  subplot(2,1,2);
  xtitle('Temperature evolution','Iteration','Temperature');
  for i=1:length(t)-1
    plot([t(i) t(i+1)], [temp_list(i) temp_list(i)],'k-');
  end
  drawnow;
end

