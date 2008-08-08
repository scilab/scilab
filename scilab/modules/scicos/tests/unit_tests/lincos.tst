exec(fullfile(SCI,'modules','scicos','tests','unit_tests','PENDULUM_ANIM.sci'))
exec(fullfile(SCI,'modules','scicos','tests','unit_tests','anim_pen.sci'))
load(fullfile(SCI,'modules','scicos','tests','unit_tests','pendulum_anim45.cos'))
M=10;
m=3;
l=3;
ph=0.1; 
scs_m = scs_m.objs(5).model.rpar;
[X,U,Y,XP] = steadycos(scs_m,[],[],[],[],1,1:$);
sys = lincos(scs_m,X,U);

