function subplot(n,m,p)

global myWorld

if length(myWorld)==0
   graphinit()
end

//
// Like in Matlab. The only difference is that you cannot replot in a subplot,
// because it is impossible to destroy plots selectively (no axes structure). 
// An example :
//
// t=linspace(0,2*%pi);
// subplot(2,1,1)
// plot(t,cos(t))
// subplot(2,1,2)
// plot(t,sin(t))

if (p<1) | (p>n*m)
  str=sprintf('subplot : number of subplot must be between 1 and %d',n*m);
  error(str);
end

lx=1/m;
ly=1/n;

x=int((p-1)/n);
y=(p-1)-x*n;

x=x*lx;
y=y*ly;

if n*m>1
  if winsid()==[]
     fig()
  end
  win=xget('window');
  s=loadGraphicState(win);
  s('viewport')=[x y lx ly];
  s('nextPlot')='erase';
  if s('subplotState')~='other';
     s('subplotState')='first';
  end
  saveGraphicState(s,win);  
end

// end of subplot
