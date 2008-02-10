mode(3)
clear
lines(28,72)
lines(0)

baseDir = "/home/bourrik/dev/scilab-5.0/modules/scicos/tests/nonreg_tests"

if getversion() == 'scilab-4.1.2' | getversion() == 'Scilab-4.1.2-SVN'
	diaryName = fullfile(baseDir,"constant_v4.log")
elseif getversion() == 'scilab-trunk-SVN' | getversion() == 'scilab-5.0'
	diaryName = fullfile(baseDir,"constant_v5.log")
else
	diaryName = fullfile(baseDir,"constant_diary.log")
end

diary(diaryName);

load(fullfile(baseDir,"constant.cos"))
disp(scs_m)
Info = list();
Info = scicos_simulate(scs_m,Info);
disp(Info)

diary(0)
exit

