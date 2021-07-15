% Test file for COMMENTS
%{
  Matlab block-comment
  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
  Copyright (c) 2020 - Samuel GOUGEON
  
fprintf('%d %s\n', 2, 'percents in strings'); % A difficult case
%}

a = 1;  % Appended comment

% Percent symbol % in a comment
A=a; B = 2*a        % Case 3: Symbol % in a comment
fprintf('%d\n', 2);                           % Case 4
fprintf('%d\n', 2);                           % Case 4 with % in comment
fprintf('%s\n', 'percents % in strings');     % Case 5
fprintf('%d %s\n', 2, 'percents in strings'); % Case 6
fprintf('%d %s\n', 2, 'percents in strings'); % Case 6 with % in comment

switch a % This comment disappears
case 1
    % Anything to do
    disp('OK')
end
