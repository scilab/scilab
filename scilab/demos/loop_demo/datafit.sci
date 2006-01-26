function demo_datafit()
  demo_help demo_datafit
  
  //create the experimental data
  X=[];Y=[];
  a=34;b=12;c=14;
  for x=0:.1:3, Y=[Y,FF(x)+100*(rand()-.5)];X=[X,x];end
  Z=[Y;X];
  //show the data points
  xbasc();
  SetPosition();
  set figure_style old;
  //xset("wpos",500,16);xset("wdim",600*0.9,400*0.9);xselect()
  plot2d(X',Y',style=-1,leg='Données expérimentales') 
  realtimeinit(0.1);for k=1:30,realtime(k),end
  // solve the non linear data fitting
  [p,err]=datafit(G,Z,[3;5;10])
  // show the fitting curve
  plot2d(X',FF(X)',[5,2],'002','Fonction approximante')
  realtimeinit(0.1);for k=1:30,realtime(k),end
  xdel();
endfunction
function y=FF(x)
//parametric function model
  y=a*(x-b)+c*x.*x,
endfunction
function e=G(p,z)
//datafit external computes the error
  a=p(1),
  b=p(2),
  c=p(3),
  y=z(1),
  x=z(2),
  e=y-FF(x)
endfunction
