% Test file for function grid
% Copyright INRIA
% Matlab version: 7.0.0.19901 (R14)
% V.C.

grid on
grid off
grid minor
grid

%m2scideclare ax|1 1|Handle
%m2scideclare ax2|1 1|Handle
grid(ax,'on')
grid(ax,'off')
grid(ax,'minor')
grid(ax)
grid([ax ax2], 'on')
