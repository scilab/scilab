function pie(varargin)
// Copyright INRIA
// This function draws a pie, if size of x is N then pie function draws a pie with N parts, the area of the ith part is equal to (x(i)/sum(x))*( surface of the unit cercle). 
//
//
// syntax : pie(x[,sp[,txt]])
// Intput : The input arguments must have the same size
// x : a scalar or a vector of positive reals. 
// sp : a scalar or a vector of reals. The sp vector allows to cut one or several parts of the pie
// txt : a cell or a vector of strings. The txt vector allows to write a text for each part of the pie
// F.B

varlist = varargin;
// Input arguments must have the same length

for i=2:size(varlist)
  if size(varlist(i-1),"*") <> size(varlist(i),"*") then
    error("input arguments must have the same length")
  end
end

txt = [];
esp = [];

// Detect and set the RHS arguments
// case : pie(x)
if size(varlist) == 1 then
  if type(varlist(1))==1 & and(varlist(1)>0) then
    x = varlist(1)
  else
    error("first argument must be a scalar or a vector of positve reals");
  end
  // case : pie(x,sp) or pie(x,txt)
elseif size(varlist) == 2 then
  if type(varlist(1)) == 1 & and(varlist(1)>0) then
    x = varlist(1);
  else
    error("first argument must be a scalar or a vector of positve reals");   
  end
  if type(varlist(2)) == 1 | type(varlist(2)) == 4 
    esp = varlist(2);
  elseif type(varlist(2)) == 10 | iscellstr(varlist(2)) then
    txt = varlist(2);
  end
  // case : pie(x,sp,txt)
elseif size(varlist) == 3 then
  if type(varlist(1)) == 1 & and(varlist(1)>0) then
    x = varlist(1);
  else
    error("first argument must be a scalar or a vector of positve reals");
  end
  if type(varlist(2)) == 1 | type(varlist(2)) == 4 
    esp = varlist(2);
  else 
    error("second argument must be a scalar or a vector of reals");
  end
  if type(varlist(3)) == 10 
    txt = varlist(3);
  elseif iscellstr(varlist(3)) then
    for j=1:size(varlist(3),"*")
      txt(j) = varlist(3).entries(j);
    end
  else
    error("third argument must be a cell or a vector of strings");
  end
else 
  error("wrong number of input arguments")
end

if isempty(x) then
  error("first argument must be a scalar or a vector of positve reals")
end
// xi and yi represents the coordinates of each polyline
// iesp is the index of the part of i which are separated of the pie
// teta_1 is the start angle of the arc polyline, teta_2 is the end angle of the arc polyline
iesp = find(esp>0 | esp<0);
teta_2 = 0 + %pi/2;
yi = [];
xi = [];
CurColor = 0;

drawlater();
// Create a close polyline for every parts of pie, the polyline inside color is determinated by the plot colormap 
for i=1:size(x,"*")
  xi = [];
  yi = [];
  teta_1 = teta_2;
  teta_i = (x(i)/sum(x))*2*%pi;
  teta_2 = teta_1 + teta_i
  if size(x,"*") <> 1 then
    xi(1) = 0;
    yi(1) = 0;
    xi(2) = cos(teta_1);
    yi(2) = sin(teta_1);
  else
    xi(1) = cos(teta_1);
    yi(1) = sin(teta_1);
  end
  inter = 1/(100*x(i)/sum(x));
  k = inter;
  while k<1
    xi($+1) = cos((1-k)*teta_1 +k*teta_2);
    yi($+1) = sin((1-k)*teta_1 +k*teta_2);
    k= k + inter;
  end 
  xi($+1) = cos(teta_2);
  yi($+1) = sin(teta_2);
  
  xfpolys(xi,yi);
  e = gce();
  a = gca();
  ei = e.children;
  if or(i == iesp) then
    ei.x_shift = ones(1,size(xi,"*")) * (1/10) * cos((teta_2+teta_1)/2);
    ei.y_shift = ones(1,size(yi,"*")) * (1/10) * sin((teta_2+teta_1)/2);
    if txt <> []
      xstring(cos((teta_2+teta_1)/2)*1.2+ei.x_shift(1)-0.1*(cos((teta_2+teta_1)/2)<0),sin((teta_2+teta_1)/2)*1.2+ei.y_shift(1),txt(i));
    else
      xstring(cos((teta_2+teta_1)/2)*1.2+ei.x_shift(1)-0.1*(cos((teta_2+teta_1)/2)<0),sin((teta_2+teta_1)/2)*1.2+ei.y_shift(1),string(round((x(i)/sum(x))*100)) + "%");
    end
  else
    if txt <> []
      xstring(cos((teta_2+teta_1)/2)*1.2-0.1*(cos((teta_2+teta_1)/2)<0),sin((teta_2+teta_1)/2)*1.2,txt(i));
    else
      xstring(cos((teta_2+teta_1)/2)*1.2-0.1*(cos((teta_2+teta_1)/2)<0),sin((teta_2+teta_1)/2)*1.2,string(round((x(i)/sum(x))*100)) + "%");
    end
  end
  
  [Color,CurColor] = setDefaultColor(CurColor);
  ei.background = Color;
  ei.fill_mode = "on";
  a.isoview = 'on';
  a.box = 'off'; 
  a.axes_visible = 'off';
  // Update data_bounds
  a.data_bounds = [min(-1.3,a.data_bounds(1,1)) min(-1.3,a.data_bounds(1,2));max(1.3,a.data_bounds(2,1)) max(1.3,a.data_bounds(2,2))];
end
drawnow();

endfunction
