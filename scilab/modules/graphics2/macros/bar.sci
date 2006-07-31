function  bar(varargin)
// Copyright INRIA
// bar(x,y,width,style,color)
// This function  ...
// 
// Intput :
// x : a real scalar or a vector
// y : a real sclar, or a vector
// width : a double, the bar width, it's the percentage (0<width<1) of the width max of one bar which is wanted (default: width=0.8)
// style : a string 'grouped' or 'stacked' (default: style='grouped')
// F.B


styletab=["grouped","stacked"]
COLORBOOL=%f
STYLE="grouped"

//Check RHS argument
ListArg = varargin;

//detect and set the current axes now:
if type(ListArg(1)) == 9
  hdle = ListArg(1);
  if (hdle.type == "Axes")
    sca(ListArg(1));
    ListArg(1) = null(); // remove this parameter from the list
  else
    warning("Handle should be an Axes handle")
    return;
  end
end

nv = size(ListArg)

T=[];

// Number of inputs arguments < 6
if  size(ListArg)>5 then 
  error("wrong number of arguments RHS")
end

for k=1:nv
  T(k) = type(ListArg(k))
end

argdb=find(T==1)
argstr=find(T==10)

if size(argdb,"*")<> argdb($) then
  error("wrong argument type")
end

if size(argstr,"*") <> nv-argdb($) then
  error("wrong argument type")
end

//set the double argument : x,y,width
// bar(y,...)
if size(argdb,"*")==1
  Y=ListArg(1)
  WIDTH=0.8
  if or(size(Y)==1) then
    Y=Y(:)
  end 
  X=1:size(Y,1)
end

if size(argdb,"*")==2
  if size(ListArg(2),"*")==1 then
    // bar(x,y,...)
    if size(ListArg(1),"*")==1 then
      WIDTH=0.8
      X=ListArg(1)
      Y=ListArg(2)
    else
      //bar(y,width,...)
      WIDTH=ListArg(2)
      Y=ListArg(1)
      if or(size(Y)==1) then
	Y=Y(:)
      end
      X=1:size(Y,1)
    end
  else
    // bar(x,y,...) 
    X=ListArg(1)
    Y=ListArg(2)
    if or(size(X)==1) then  
      if size(X,"*")<>1 then // X is a vector  
	if or(size(Y)==1) then // Y is a vector
	  Y=Y(:)
	end  
	if size(X,"*")<>size(Y,1)
	  error("x and y dims : no match")    
	end
      elseif size(Y,1)>1 then
	error("x and y dims : no match") 
      end 
    else 
      error("x must be a scalar or a vector")
    end 
    WIDTH=0.8
  end    
end

// bar(x,y,width,...)      
if size(argdb,"*")==3
  X=ListArg(1)
  Y=ListArg(2)
  WIDTH=ListArg(3)
  if size(WIDTH,"*")<>1 then
    error("width must be a scalar")
  elseif or(size(X)==1) then  
    if size(X,"*")<>1 then // X is a vector  
      if or(size(Y)==1) then // Y is a vector
	Y=Y(:)
      end  
      if size(X,"*")<>size(Y,1)
	error("x and y dims : no match")    
      end
    elseif size(Y,1)>1 then
      error("x and y dims : no match") 
    end 
  else 
    error("x must be a scalar or a vector")
  end 
end
X=X(:)

// set the string argument
for i=1:size(argstr,"*") 
  // bar(...,style)
  if or(ListArg(argstr(i))==styletab) then
    STYLE=ListArg(argstr(i))
  else
    COLOR=ListArg(argstr(i))
    COLORBOOL=%t
  end
end

drawlater();

if COLORBOOL
  plot(X,Y,COLOR)
else 
  plot(X,Y)
end
bar_number=size(Y,2)
if size(X,"*")>1 then
  Xtemp=gsort(X,'r','i')
  inter=Xtemp(2)-Xtemp(1)
  for i=2:size(Xtemp,"*")-1
    inter=min(Xtemp(i+1)-Xtemp(i),inter)
  end
  if bar_number>1
    inter=inter*0.9
  end
else
  Xtemp=X
  inter=1
end

wmax=inter/bar_number

y_shift=zeros(size(X,"*"),1)

bar_number= bar_number

  e=gce()
  a=gca()
  
  a.sub_ticks(1) = 0; // bar (barh => a.sub_ticks(2) = 0;)

for i=bar_number:-1:1

  ei = e.children(i);
    
  // Perform x_shift
  if modulo(bar_number,2)==0 then  
    x_shift=(-i+bar_number/2)*wmax+wmax/2
  elseif modulo(bar_number,2)==1 then  
    x_shift=(-i+1+floor(bar_number/2))*wmax
  end
  
  // Perform y_shift 
  if i==bar_number then
    y_shift=zeros(size(X,"*"),1)
  else
    y_shift=Y(:,bar_number-i)+y_shift 
  end
  
  // Udate the axes data bounds 
  if STYLE=="grouped"
    a.data_bounds=[min(a.data_bounds(1,1),min(X)+x_shift-wmax/2) min(a.data_bounds(1,2),0,min(y_shift+Y(:,bar_number-i+1))); max(a.data_bounds(2,1),max(X)+x_shift+wmax/2) max(a.data_bounds(2,2),0)]
    ei.x_shift=x_shift*ones(size(X,"*"),1)
  else
    wmax=inter
    a.data_bounds=[min(a.data_bounds(1,1),min(X)-wmax/2) min(a.data_bounds(1,2),0,min(y_shift+Y(:,bar_number-i+1))); max(a.data_bounds(2,1),max(X)+wmax/2) max(a.data_bounds(2,2),0,max(y_shift+Y(:,bar_number-i+1)))]
    ei.y_shift=y_shift
  end
  
  a.x_ticks=tlist("ticks",Xtemp,string(Xtemp))
  
  w=WIDTH*wmax
  
  ei.bar_width=w
  ei.background=ei.foreground 
  ei.polyline_style=6; // bar type
  ei.background=ei.foreground  
  ei.foreground = -1; // black by default
  ei.line_mode='off';
end

drawnow();

endfunction