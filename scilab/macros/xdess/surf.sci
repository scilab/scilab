 //
//Author : F.Leray
//Copyright INRIA
//

function surf(varargin)

[lhs,rhs]=argn(0);

isold=get('figure_style')=='old'

if isold
  disp("Not supported under old graphic style");
  return;
end


if ~rhs
  //  clf();
  //  //LineSpec and PropertySpec examples:
  //  clf();
  //  t=0:%pi/20:2*%pi;
  //  subplot(211)
  //  plot(t,sin(t),'ro-.',t,cos(t),'cya+',t,abs(sin(t)),'--mo')
  //  subplot(212)
  //  plot([t ;t],[sin(t) ;cos(t)],'xdat',[1:2])
  //  disp("clf();")
  //  disp("t=0:%pi/20:2*%pi;");
  //  disp("subplot(211)")
  //  disp("plot(t,sin(t),''ro-.'',t,cos(t),''cya+'',t,abs(sin(t)),''--mo'')");
  //  disp("subplot(212)");
  //  disp("plot([t ;t],[sin(t) ;cos(t)],''xdat'',[1:2])")
  return;
end


// differ the drawing command
// smart drawlater
f=gcf();
cur_draw_mode = f.immediate_drawing;
f.immediate_drawing = 'off';


CurColor = 0; // current color used if no color specified via LineSpec
// nor PropertyName

ListArg = varargin;

//detect and set the current axes now:
if type(ListArg(1)) == 9
  hdle = ListArg(1);
  if (hdle.type == "Axes")
    sca(ListArg(1));
    ListArg(1) = null(); // remove this parameter from the list
  else
    disp("Handle should be an Axes handle")
    return;
  end
end


nv = size(ListArg)


T=[];
d=[];

typeOfPlot = 'surf';
//given_data = 2;

for k=1:nv
  T(k,1) = type(ListArg(k))
end

given_data = 0;
P1 = 0;

for i=1:nv
  if T(i) == 1
    given_data = given_data +1;
  else
    disp("le ieme element n est pas un double:");
    disp(i);
    P1 = i;  // Position of the first PropertyName field
    break;
  end
end

// set some defaults here
f=gcf();
f.color_map=jetcolormap(64);
  
if given_data == 1 //surf(Z) with Z giving us data + color info.
  // ---------------------------------------------------------- //
  if or(size(ListArg(1))==1)
    disp("Z must not be a scalar or vector, not rendering surface.")
    return;
  end
  
  X = 1:size(ListArg(1),2);
  Y = 1:size(ListArg(1),1);
  Z = ListArg(1)'; // here a transposition is needed
  
  [XX,YY,ZZ] = genfac3d(X,Y,Z);
  CC = ZZ; // Add a color matrix based on Z values
  
  // => interp/shading mode: NO! by default, even if color number == zdata number, 
  // we are in flat mode.
  colormap_size = size(f.color_map,1);
  CC = build_interp_color(CC,colormap_size);
  plot3d(XX,YY,list(ZZ,CC));
  a=gca();
  a.cube_scaling = 'on';
  a.rotation_angles = [51 -125];
  e=gce();
  e.hiddencolor=0;
  e.color_flag=4; // Matlab special flat mode by default (different from mode 2)
    
elseif given_data == 2 //surf(Z,COLOR)
  // ---------------------------------------------------------- //
  if or(size(ListArg(1))==1)
    disp("Z must not be a scalar or vector, not rendering surface.")
    return;
  end
  
  if ((size(ListArg(1)) <> size(ListArg(2))) & (size(ListArg(1))-1 <> size(ListArg(2))))
    disp("Color Data must equal size(ZData) or size(ZData)-1 for flat shading ; not rendering surface.")
    return;
  end
  
  X = 1:size(ListArg(1),2);
  Y = 1:size(ListArg(1),1);
  Z = ListArg(1)'; // here a transposition is needed
  C = ListArg(2)';
  
  [XX,YY,ZZ] = genfac3d(X,Y,Z);
    
  if (size(ListArg(2)) == size(ListArg(1))) // color number == zdata number
    [XX,YY,CC] = genfac3d(X,Y,C);     // CC must be a color matrix of size nf x n
  elseif ((size(ListArg(2))) == size(ListArg(1))-1) // color number -1 == zdata number => ONLY flat mode can be enabled
    Ctmp=[];
    Ctmp = [C [C(:,$)]] ;
    Ctmp = [Ctmp; Ctmp($,:)];
    [XX,YY,CC] = genfac3d(X,Y,Ctmp);     // CC must be a color matrix of size nf x n
  end
  
  // => interp/shading mode: NO! by default, even if color number == zdata number, 
  // we are in flat mode.
  colormap_size = size(f.color_map,1);
  CC = build_interp_color(CC,colormap_size);
  plot3d(XX,YY,list(ZZ,CC));
  a=gca();
  a.cube_scaling = 'on';
  a.rotation_angles = [51 -125];
  e=gce();
  e.hiddencolor=0;
  e.color_flag=4; // Matlab special flat mode by default (different from mode 2)

elseif given_data == 3 //surf(X,Y,Z) with Z giving us data + color info.
  // ---------------------------------------------------------- //
  
  X = ListArg(1)
  Y = ListArg(2);
  
  if or(size(X)==1) & or(size(Y)==1) // X and Y are vector
    
    Z = ListArg(3)'; // here a transposition is needed
    
    if size(X,'*') ~= size(Z,2) | size(Y,'*') ~= size(Z,1)
      error('surf : Vectors X, Y must match Z matrix dimensions');
      return;
    end 
    
    [XX,YY,ZZ] = genfac3d(X,Y,Z);
    
    // COLOR treatment
    CC = ZZ;
  
  elseif and(size(X)>1) & and(size(Y)>1) // X and Y are matrix
    
    Z = ListArg(3);
  
    if or(size(X) ~= size(Y)) | or(size(X) ~= size(Z))
      error('surf : Matrices must be the same size');
      return;
    end 
        
    [XX,YY,ZZ] = nf3d(X,Y,Z);
    
    // COLOR treatment
    CC = ZZ;
        
  else
    disp("Error: X and Y must be of the same type\n")
    return;
  end
  
  colormap_size = size(f.color_map,1);
  CC = build_interp_color(CC,colormap_size);
  //pause;
  plot3d(XX,YY,list(ZZ,CC));
  
  a=gca();
  a.cube_scaling = 'on';
  a.rotation_angles = [51 -125];
  e=gce();
  e.hiddencolor=0;
  e.color_flag=4; // Matlab special flat mode by default (different from mode 2)

  
elseif given_data == 4 //surf(X,Y,Z,COLOR)
  // ---------------------------------------------------------- //

  if ((size(ListArg(1)) <> size(ListArg(2))) & (size(ListArg(1))-1 <> size(ListArg(2))))
    disp("Color Data must equal size(ZData) or size(ZData)-1 for flat shading ; not rendering surface.")
    return;
  end
    
  X = ListArg(1)
  Y = ListArg(2);
  
  if or(size(X)==1) & or(size(Y)==1) // X and Y are vector
        
    Z = ListArg(3)'; // here a transposition is needed
    C = ListArg(4)'; // here a transposition is needed
    
    if size(X,'*') ~= size(Z,2) | size(Y,'*') ~= size(Z,1)
      error('surf : Vectors X, Y must match Z matrix dimensions');
      return;
    end 
    
    [XX,YY,ZZ] = genfac3d(X,Y,Z);
    
    // COLOR treatment
    if (size(ListArg(4)) == size(ListArg(3))) // color number == zdata number
      [XX,YY,CC] = genfac3d(X,Y,C);     // CC must be a color matrix of size nf x n
    elseif ((size(ListArg(4))) == size(ListArg(3))-1) // color number -1 == zdata number => ONLY flat mode can be enabled
      Ctmp=[];
      Ctmp = [C [C(:,$)]] ;
      Ctmp = [Ctmp; Ctmp($,:)];
      [XX,YY,CC] = genfac3d(X,Y,Ctmp);     // CC must be a color matrix of size nf x n
    end
    
  elseif and(size(X)>1) & and(size(Y)>1) // X and Y are matrix
    
    Z = ListArg(3);
    C = ListArg(4);

    if or(size(X) ~= size(Y)) | or(size(X) ~= size(Z))
      error('surf : Matrices must be the same size');
      return;
    end 
        
    [XX,YY,ZZ] = nf3d(X,Y,Z);
    
    // COLOR treatment
    if (size(ListArg(4)) == size(ListArg(3))) // color number == zdata number
      [XX,YY,CC] = nf3d(X,Y,C);     // CC must be a color matrix of size nf x n
    elseif ((size(ListArg(4))) == size(ListArg(3))-1) // color number -1 == zdata number => ONLY flat mode can be enabled
      Ctmp=[];
      Ctmp = [C [C(:,$)]] ;
      Ctmp = [Ctmp; Ctmp($,:)];
      [XX,YY,CC] = nf3d(X,Y,Ctmp);     // CC must be a color matrix of size nf x n
    end
        
  else
    disp("Error: X and Y must be of the same type\n")
    return;
  end
  
  colormap_size = size(f.color_map,1);
  CC = build_interp_color(CC,colormap_size);
  //pause;
  plot3d(XX,YY,list(ZZ,CC));
  
  a=gca();
  a.cube_scaling = 'on';
  a.rotation_angles = [51 -125];
  e=gce();
  e.hiddencolor=0;
  e.color_flag=4; // Matlab special flat mode by default (different from mode 2)

end




///////////////////////////////////
//Global Property treatment      //
//PropertyName and PropertyValue //
///////////////////////////////////


//postponed drawings are done now !
// smart drawnow
f.immediate_drawing = cur_draw_mode;

endfunction





function [xx]=nf3d_one_matrix(x)
// 3d coding transformation 
// from facets coded in three matrices x,y,z to scilab code for facets 
// accepted by plot3d 
//---------------------------------------------------------
// Copyright INRIA
[n1,n2]=size(x)
ind=ones(1,n1-1).*.[0 1 n1+1 n1]+ (1:n1-1).*.[1 1 1 1];
// ind=[1,2,n1+2,n1+1 , 2,3,n1+3,n1+2, ....  ,n1-1,n1,2n1,2n1-1
ind2=ones(1,n2-1).*.ind+((0:n2-2)*n1).*.ones(ind);
nx=prod(size(ind2))
xx=matrix(x(ind2),4,nx/4);

xx2=zeros(xx);

pause;
for i=2:size(xx,1)
  xx2(i-1,:) = xx(i,:)
end

xx2($,:) = xx(1,:);
xx = xx2;

//permut = eye(size(xx,1),size(xx,1));
//permut(1,1:$) = 0
//permut(1,$) = 1;
//
//permut($,1:$) = 0
//permut($,1) = 1;
//
//disp(xx);
//disp("-----------------------------");
//xx = permut * xx;
//disp(xx);
//pause;
endfunction


function [C] = build_interp_color(C,colormap_size)
// C is considered as a data value in Matlab
MIN = min(C);
MAX = max(C);
NCOLMIN = 1;
NCOLMAX = colormap_size;

if MIN <> MAX
  C = (NCOLMIN-NCOLMAX)/(MIN-MAX) * C + (MIN*NCOLMAX - NCOLMIN*MAX)/(MIN-MAX);
  C = round(C);
else
  C = ones(C) * (NCOLMIN+NCOLMAX)/2;
end
endfunction 
