function Plot(varargin)
// Try to build a new better parser that could manage things like:
// plot(x,y,'X',1:10); // where X stands for Xdata (Matlab recognize
//it and treats it well...




//default values
Marker=[];
MarkerSize=1;
Color=0;
LineStyle=1; 






ListArg = varargin;


nv = size(ListArg)


T=[];
d=[];

typeOfPlot = 'plot';
provided_data = 2;

for k=1:nv
 T(k,1) = type(ListArg(k))
end

Ttmp=T;

for i=1:nv-1
 e=[];
 e=find(Ttmp(i,1)==1 & or(Ttmp(i+1,1)==[1,11,13,15]))

 if(e<>[]) then
  d=[d i];
  Ttmp(i,1)  = 99; // Replace a known type by 99 (no meaning) to count it once only!
  Ttmp(i+1,1)= 99; // to avoid having (x1,y1,x2,y2) ->d=[1,2,3]
                   // With this trick, d=[1,3];
 end

 disp("i=");
 disp(i);
 disp("d vaut:");
 disp(d)
end


if(d==[]) // No data couple found
// Search for at least a single data , i.e.: plot(y)
disp(T)
disp(type(T(1,1)))

 if(T(1,1)==1 & ListArg(1)<>[]) then // case plot(SINGLE y,...)
  disp("ICICICICI");
  d = 1; 
  provided_data = 1;

  if(modulo(nv-d,2)<>0) then
   P1 = d+2 // Position of the first PropertyName field
  else
   P1 = d+1
  end

 elseif (T(2,1)==1 & ListArg(2)<>[] & T(1,1)==9) then // case plot(handle,SINGLE y,..
  disp("Handle case *-*-*-*-");
  sca(ListArg(1)); // set current axes
  d = 2;
  provided_data = 1;

  if(modulo(nv-d,2)<>0) then
   P1 = d+2 // Position of the first PropertyName field
  else
   P1 = d+1
  end

 else
  disp("Error inside input argument : no data");
  return;
 end

disp("ICII--------------------")
disp("d vaut:")
disp(d);
disp("P1=")
disp(P1)

else

 // Some test to check wrong inputs
 //
 // 1. Test if 2 data couples (type==1) are at least separated by 2 indices
 if(d(2:$)-d(1:$-1)<2)
  disp("Error inside input argument !");
  return;
 end

 // 2. Test if no string couples happen before P1 (see below for P1 definition)
 for i=1:d($)
  e=[];
  e=find(Ttmp(i,1)==10 & Ttmp(i+1,1)==10)
 
  if(e<>[]) then
   disp("Error inside input argument : String argument is an unknown option.");
   return;
  end
 end


 
 if(modulo(nv-(d($)+1),2)<>0) then
  P1 = d($)+3 // Position of the first PropertyName field
 else
  P1 = d($)+2
 end
 
end

numplot = size(d,'*');

P = zeros(numplot,3); 
// P is a matrix storing the index of x, y and linespec
// if one of these indices is 0 => it does not exist
// (which is possible for x and linepsec, not for y)

disp("T vaut:");
disp(T)

if (provided_data == 2) then

 for k=1:size(d,'*')
  P(k,1:2) = d(k) +[0,1] // x,y index storage

  if(d(k)+2 < P1)
   if(T(d(k)+2,1)==10) then // LineSpec treatment
    P(k,3) = d(k)+2;
   end
  end
 disp(P);
 end
else
// we are in the case where: plot(SINGLE y,... x not specified
// or plot(handle,SINGLE y,...
 P(1,1) = 0; // no x specified
 P(1,2) = d;

 if(d+2 < P1)
  if(T(d+2,1)==10) then // LineSpec treatment
   P(1,3) = d+2;
  end
 end
end

disp("P1 vaut:");
disp(P1);








//Now we have an array P [numplot x 3] containing indices pointing on T for :
// - x (<>0 if existing)
// - y
// - linespec (<>0 if existing)
// for each plot passed in argument
//       x | y | linespec
//       ----------------
//plot1   0|i1 |0    <=> plot(y)
//plot2  i2|i3 |0    <=> plot(x,y)
//plot3  i4|i5 |i6   <=> plot(x,y,LINESPEC)
//...







//Now, we plot the decomposed plots one by one with their own linespec
// provided_data = 2 : x and y are provided


disp("P=")
disp(P)

disp("ListArg=")
disp(ListArg)

if(provided_data == 2) then
 for i=1:numplot
   [X,Y] = checkXYPair(typeOfPlot,ListArg(P(i,1)),ListArg(P(i,2)))

   if(P(i,3)<>0) then // if we have a line spec <=> index <> 0
    [Color,Line,LineStyle,Marker,MarkerStyle,MarkerSize,fail] = getLineSpec(ListArg(P(i,3))); 
   end

   plot2d(X,Y);
   e=gce();  // when using plot2d, we always have agregation as current entity
   e=e.children; // we apply linespec to the only available line

disp(Color)

   if(Line == %T)
    e.mark_mode ='off'
    e.foreground = Color;
    e.line_style = LineStyle;
   else
    e.line_mode='off';
    e.mark_foreground = Color;
    e.mark_style=MarkerStyle;
    e.mark_size=MarkerSize;
   end

 end
end

//for i=0:numplot
// if(type(T(P(i,1),1) == 0) // no x provided => We have to build the x component...
//  if or(size(T(P(i,2)),1)==1)  // If this is a vector
//    T(P(i,1),1)=1:length(argList(1)); // insert an abcsissa vector of same length,
//
//         else                                  // if this is a matrix,
//
//            argList(0)=1:size(argList(1),1); // insert an abcsissa vector with 
//
//         end    
//
//
//
//end





//Global Property treatment





endfunction



