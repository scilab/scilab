% Test file for function load()
% Copyright INRIA
% Matlab version: 6.5.0.180913a (R13)
% V.C.

savefile = 'savefile'

load;
load([savefile,'.mat']);
load([savefile,'.mat'],'y');
load([savefile,'.asc'],'y','-ascii','-mat');
load('filename')
load filename
