// exec("e:\testNonReg\bug600.sce")


disp("activate your sound card, and run this file");
disp("if you hear something, that means the playsnd & PlaySnd work");

y=wavread("SCI/demos/sound/chimes.wav");
playsnd(y);

PlaySound("SCI/demos/sound/chimes.wav"); 

affich_result(%T,600);