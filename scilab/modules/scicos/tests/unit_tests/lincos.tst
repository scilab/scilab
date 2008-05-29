getf PENDULUM_ANIM.sci
getf anim_pen.sci
load ("pendulum_anim45.cos")
M=10;
m=3;
l=3;
ph=0.1; 
scs_m = scs_m.objs(5).model.rpar;
[X,U,Y,XP] = steadycos(scs_m,[],[],[],[],1,1:$);
sys = lincos(scs_m,X,U);

