function [f_History, x_History] = Lumped(f,x0,T,NbIter,NbBar,Log,Neigh_func,Neigh_func_param)

[nargout, nargin] = argn();

x_Hist_defined = (nargout==2);

if ~isdef('f','local') | ~isdef('x0','local') then
  error('Lumped: f nd x0 are mandatory');
end
if ~isdef('T','local') then
  T = %inf;
end
if ~isdef('NbIter','local') then
  NbIter = 1000;
end
if ~isdef('NbBar','local') then
  NbBar = 10;
end
if (~isdef('neigh_func','local')) then
  neigh_func = neigh_func_default;
end
if (~isdef('param_neigh_func','local')) then
  param_neigh_func = [];
end
if (~isdef('Log','local')) then
  Log = %F;
end

x    = x0;
f_x  = f(x0);
f_History = f_x;

if (x_Hist_defined) then
  x_History = list(x0);
end

if Log then
  printf('Lumped: performing the random walk for %d iterations\n',NbIter);
end

for i=1:NbIter
  x_Neigh = neigh_func(x,T,param_neigh_func);
  f_Neigh = f(x_Neigh);
  if ((f_Neigh<=f_x) | (exp(-(f_Neigh - f_x)/T)>rand(1,1))) then
    x   = x_Neigh;
    f_x = f_Neigh;
  end
  
  if (x_Hist_defined) then
    x_History($+1) = x;
  end
  
  f_History = [f_History f_x];
  
  if i==NbIter then
    Time_Relax = int_autocorr(f_History);
    if 3*Time_Relax>NbIter then
      if Log then
        printf('Lumped: Correlation time too small: restarting\n');
      end
      i = 1;
      f_History = [];
    else
      if Log then
        printf('Lumped: Correlation time = %f\n', Time_Relax);
      end
    end
  end
end

f_aux = f_History(ceil(3*Time_Relax):$);

if Log then
  printf('Lumped: Building the Lumped matrix (size %d x %d) using %d samples\n',NbBar,NbBar,length(f_aux));
end

fmin = min(f_aux);
fmax = max(f_aux);
Binf = (fmin*NbBar-fmax)/(NbBar-1);
Bsup = (fmax*NbBar-fmin)/(NbBar-1);
Bstep = (Bsup-Binf)/(NbBar-1);
f_interv = Binf:Bstep:Bsup;
f_aux = ceil(((f_aux - fmin) / (fmax - fmin)) * (NbBar - 1)) + 1;

// We build the lumped matrix
Matrix = zeros(NbBar,NbBar);

for i=1:length(f_aux)-1
  //Matrix(f_aux(i+1),f_aux(i)) = Matrix(f_aux(i+1),f_aux(i)) + 1;
  Matrix(f_aux(i),f_aux(i+1)) = Matrix(f_aux(i),f_aux(i+1)) + 1;
end

// Normalization of the lines of the lumped matrix
for i=1:NbBar
  _coeff = max([%eps sum(Matrix(:,i))]);
  Matrix(:,i) = Matrix(:,i) / _coeff;
end

// Extraction of the eigen values
[v_res, e_res] = spec(Matrix');
[e_res, Index] = sort(diag(e_res));
e_res = abs(e_res); // Eigen values are always real
v_res = v_res(:,Index);

DensOfStates = v_res(:,1)';
RelaxTime    = -1 / log(max([%eps e_res(2)]));
Z_T = sum(DensOfStates .* exp(-f_interv / T));
E_T = sum(DensOfStates .* f_interv .* exp(-f_interv / T)) / Z_T;
C_T = (Z_T * sum(DensOfStates .* f_interv^2 .* exp(-f_interv / T)) - sum(DensOfStates .* f_interv .* exp(-f_interv / T))^2 ...
       - 2*T*Z_T*sum(DensOfStates .* f_interv .* exp(-f_interv / T))) / (T^4*Z_T^2)
printf('Density of states:'); disp(DensOfStates);
printf('Relaxation time = %f\n', RelaxTime);
printf('Z(T) = %f \nE(T) = %f \nC(T) = %f\n',Z_T, E_T, C_T);
endfunction
