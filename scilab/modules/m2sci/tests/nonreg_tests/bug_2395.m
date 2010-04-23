function x_nS_n = bug_2395(n,T_e)

% function x_nS_n = bug_2395(n,T_e)
%
% generator-function used by a_i_n to calculate the rate coefficient for
% radiative recombination.
%
% n = principle quantum #
% T_e = kinetic electron temperature
%
% based on Seaton 1959
%
% valid for:
%
% a) 0.02 >= x_n => 20 using eqs. 25 and 26
%
% b) 0.02 <= x_n <= 20 using table I and linear-interpolation
%
% where x_n = lambda/n^2, lambda = h*R_inf*c/(k*T_e)
% 
% jordan alexander AUT 24/4/07

global seaton_1959

x_n = X_n1(n,T_e);

if x_n <= 0.02

    xS0 = x_n*exp(x_n)*(-log(x_n)-0.5772+x_n);
    xS1 = 0.4629*x_n*(1+4*x_n)-1.0368*x_n^(4/3)*(1+15/8*x_n);
    xS2 = -0.0672*x_n*(1+3*x_n)+0.1488*x_n^(5/3)*(1+9/5*x_n);

elseif x_n < 20

    xS0 = interp1(seaton_1959(:,1),seaton_1959(:,2),x_n);
    xS1 = interp1(seaton_1959(:,1),seaton_1959(:,3),x_n);
    xS2 = interp1(seaton_1959(:,1),seaton_1959(:,4),x_n);

elseif x_n >= 20

    xS0 = 1-1/x_n+2/x_n^2-factorial(3)/x_n^3+factorial(4)/x_n^4;
    xS1 = -0.1728*x_n^(1/3)*(1-8/(3*x_n)+70/(3*x_n)^2-800/(3*x_n)^3+11440/(3*x_n)^4);
    xS2 = -0.0496*x_n^(2/3)*(1-3/(3*x_n)+32/(3*x_n)^2-448/(3*x_n)^3);

else

    xS0 = nan;
    xS1 = nan;
    xS2 = nan;

end

x_nS_n = xS0+xS1+xS2;