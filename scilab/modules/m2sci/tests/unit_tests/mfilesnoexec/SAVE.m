% Test file for function save()
% Copyright INRIA
% Matlab version: 6.5.0.180913a (R13)
% V.C.

savefile='save_env';
save;
save([savefile,'.mat']);
save([savefile,'.mat'],'y');
save([savefile,'.asc'],'y','-ascii','-double','-tabs','-mat','-v4','-append');
save([savefile,'.asc'],'y','-ascii','-double','-tabs','-mat','-v4');
save('filename');
save filename


