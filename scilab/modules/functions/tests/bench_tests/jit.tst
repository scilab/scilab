function jit_check(init, str)
    execstr(init);
    tic();
    jit(str);
    t1 = toc();

    execstr(init);
    tic();
    execstr(str);
    t2 = toc();
    printf("%f to %f -> %f\n", t2, t1, t2/t1);
endfunction

//just check jit
a=1;jit("a=2");

//small scalar computation
jit_check("a=1;b=1;i=1", "for i=1:1:1e7;a=a+i*3+b;b=b-i*a;end;");

//matrix computation
jit_check("a=[1 2;3 4];b=[5 6;7 8];i=1;c=zeros(a);", "for i=1:1:1e;a=a+b-(a+b);end;");

//double boucle
jit_check("i=1;j=1;c=1;a=1;b=1;", "for i=1:1:2134;for j=1:1:4605;a=a+i-j;end;end;");

