//Copyright INRIA
//DASSL
// PROBLEM 1:   LINEAR DIFFERENTIAL/ALGEBRAIC SYSTEM
//
//X1DOT + 10.0*X1 = 0  
//X1 + X2 = 1
//X1(0) = 1.0, X2(0) = 0.0
//
t=1:10;t0=0;y0=[1;0];y0d=[-10;0];
info=list([],0,[],[],[],0,0);
// with default atol and rtol parameters
//    Calling Scilab functions
deff('[r,ires]=dres1(t,y,ydot)','r=[ydot(1,:)+10*y(1,:);y(2,:)+y(1,:)-1];ires=0')
deff('pd=djac1(t,y,ydot,cj)','pd=[cj+10,0;1,1]')
//   scilab function, without jacobian
yy0=dassl([y0,y0d],t0,t,dres1,info);
if norm(dres1(yy0(1,:),yy0(2:3,:),yy0(4:5,:)),1)>1.d-6 then pause,end
//   scilab functions, with jacobian
yy1=dassl([y0,y0d],t0,t,dres1,djac1,info);
if norm(dres1(yy1(1,:),yy1(2:3,:),yy1(4:5,:)),1)>1.d-6 then pause,end
// fortran routine dres1 in dir. routines/default, without jocabian
yy2=dassl([y0,y0d],t0,t,'dres1',info);   //=yy0
if norm(dres1(yy2(1,:),yy2(2:3,:),yy2(4:5,:)),1)>1.4d-6 then pause,end
// fortran routines dres1 and djac1 in dir. routines/default, with jacobian
yy3=dassl([y0,y0d],t0,t,'dres1','djac1',info);  //=yy1
if norm(dres1(yy3(1,:),yy3(2:3,:),yy3(4:5,:)),1)>1.d-6 then pause,end
//if norm(yy3-yy1,1)  > Eps then pause,end
yy3bis=dassl([y0,y0d],t0,t,'dres1',djac1,info); 
if norm(dres1(yy3bis(1,:),yy3bis(2:3,:),yy3bis(4:5,:)),1)>1.d-6 then pause,end
// call fortran dres1 and scilab's djac1
yy3ter=dassl([y0,y0d],t0,t,dres1,'djac1',info);
if norm(dres1(yy3ter(1,:),yy3ter(2:3,:),yy3ter(4:5,:)),1)>1.d-6 then pause,end
//
// with specific atol and rtol parameters
atol=1.d-6;rtol=0;
//   scilab function, without jacobian
yy4=dassl([y0,y0d],t0,t,atol,rtol,dres1,info);
if norm(dres1(yy4(1,:),yy4(2:3,:),yy4(4:5,:)),1)>1.d-5 then pause,end
//   fortran routine dres1 in dir. routines/default, without jocabian
yy5=dassl([y0,y0d],t0,t,atol,rtol,'dres1',info); //=yy4
if norm(dres1(yy5(1,:),yy5(2:3,:),yy5(4:5,:)),1)>1.d-5 then pause,end
//   scilab functions,
yy6=dassl([y0,y0d],t0,t,atol,rtol,dres1,djac1,info); 
if norm(dres1(yy6(1,:),yy6(2:3,:),yy6(4:5,:)),1)>1.d-5 then pause,end
//   fortran routines 
yy7=dassl([y0,y0d],t0,t,atol,rtol,'dres1','djac1',info); //==yy6
if norm(dres1(yy7(1,:),yy7(2:3,:),yy7(4:5,:)),1)>1.d-5 then pause,end

