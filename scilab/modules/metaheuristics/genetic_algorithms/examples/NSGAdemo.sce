//
// Definition of the operators
//

// Loading test problems
getf('../../test_problems/mo_cont_funcs.sci');

//funcname = 'binh_1';
//funcname = 'fonseca_1';
//funcname = 'fonseca_2';
//funcname = 'kursawe_1';
//funcname = 'laumanns_1';
//funcname = 'lis_1';
//funcname = 'murata_1';
//funcname = 'poloni_1';
//funcname = 'quagliar_1';
//funcname = 'rendon_1';
//funcname = 'rendon_2';
//funcname = 'schaffer_1';
//funcname = 'schaffer_2';
funcname = 'deb_1';
//funcname = 'deb_2';
//funcname = 'deb_3';
//funcname = 'deb_4';
//funcname = 'deb_5';
//funcname = 'deb_6';
//funcname = 'veldmop_1';
//funcname = 'veldmop_2';
//funcname = 'veldmop_3';
//funcname = 'veldmop_4';
//funcname = 'veldmop_6';
//funcname = 'veldmop_7';
//funcname = 'meca_1';
//funcname = 'trigo_1';
//funcname = 'trigo_2';
//funcname = 'trigo_3';
//funcname = 'trigo_4';
//funcname = 'trigo_5';
//funcname = 'trigo_5_bis';
//funcname = 'trigo_6';
//funcname = 'trigo_6_bis';
//funcname = 'trigo_7';
//funcname = 'trigo_8';
//funcname = 'trigo_9';

// example of use of the genetic algorithm

PopSize     = 100;
Proba_cross = 0.7;
Proba_mut   = 0.1;
NbGen       = 4;
NbCouples   = 110;
Log         = %T;
nb_disp     = 10; // Nb point to display from the optimal population
Sigma       = 0.02; // Sigma_share = Pareto_length / Pop_Size
Pow         = 1;
pressure    = 0.1;

ga_params = init_param();
// Parameters to adapt to the shape of the optimization problem
ga_params = add_param(ga_params,'minbound',eval('min_bd_'+funcname+'(2)'));
ga_params = add_param(ga_params,'maxbound',eval('max_bd_'+funcname+'(2)'));
ga_params = add_param(ga_params,'dimension',2);
ga_params = add_param(ga_params,'beta',0);
ga_params = add_param(ga_params,'delta',0.1);
// Parameters to fine tune the Genetic algorithm. All these parameters are optional for continuous optimization
// If you need to adapt the GA to a special problem, you 
ga_params = add_param(ga_params,'init_func',init_func_default);
ga_params = add_param(ga_params,'crossover_func',crossover_func_default);
ga_params = add_param(ga_params,'mutation_func',mutation_func_default);
ga_params = add_param(ga_params,'codage_func',codage_identity);
ga_params = add_param(ga_params,'selection_func',selection_func_elitist);
//ga_params = add_param(ga_params,'selection_func',selection_func_random);
ga_params = add_param(ga_params,'nb_couples',NbCouples);
ga_params = add_param(ga_params,'pressure',pressure);

deff('y=fobjs(x)','y = ' + funcname + '(x);');

////////////////////
// NSGA Algorithm //
////////////////////

[pop_opt, fobj_pop_opt, pop_init, fobj_pop_init] = optim_nsga(fobjs, PopSize, NbGen, Proba_mut, Proba_cross, Log, ga_params, Sigma, Pow);

[f_pareto,pop_pareto] = pareto_filter(fobj_pop_opt,pop_opt);

if (size(fobj_pop_opt,2)==2) then
  drawlater;
  subplot(2,1,1);
  printf('plotting init population ...\n');
  plot(fobj_pop_init(:,1),fobj_pop_init(:,2),'r.');

  if isdef('get_opti_'+funcname) then
    t = 0:0.01:1;
    for i=1:length(t)
      y_t(i,:) = eval('get_opti_' + funcname + '(t(' + string(i) + '))');
    end
    plot(y_t(:,1), y_t(:,2), 'k-');
  end
  
  legend(['Init']);
  xtitle('Objective function space','f1','f2');

  subplot(2,1,2);
  printf('plotting final population ...\n');
  plot(fobj_pop_opt(:,1),fobj_pop_opt(:,2),'g.');
  printf('plotting Pareto population ...\n');
  plot(f_pareto(:,1),f_pareto(:,2),'k.');
  
  if isdef('get_opti_'+funcname) then
    t = 0:0.01:1;
    for i=1:length(t)
      y_t(i,:) = eval('get_opti_' + funcname + '(t(' + string(i) + '))');
    end
    plot(y_t(:,1), y_t(:,2), 'k-');
  end
  
  legend(['Final','Pareto']);
  xtitle('Objective function space','f1','f2');
  drawnow;
end

