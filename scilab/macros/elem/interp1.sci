function yi=interp1(varargin)
// Copyright INRIA
// yi=interp1(x,y,xi[,method[,interpolation)
// This function performs the yi values corresponding to xi by interpolation defined by x and y.
// Inputs :
// x , y : given data, x is a reals vector, y is a vector, matrix, or hypermatrix of reals
// if y is a vector, the length of x must be equal to the length of y, 
// else the size of the first dimension of y must be equal to length of x.
// xi : a vector, matrix, or hypermatrix of reals 
// Output
// yi : reals vector, matrix or hypermatrix, the values corresponding to xi by interpolation defined by x and y
// if size(y)=[C,N1,N2,N3,....] and size(xi)=[M1,M2,M3,M4] then size(xi)=[M1,M2,M3,M4,N1,N2,N3,N4,..], and length of x must be equal to C. 
// Several kind of intepolations may be computed by selecting the appropriate method  parameter:
// The methods are:
// linear : this is the default method (using the interp Scilab function) 
// spline : this is the cubic spline interpolation  (using interpln and splin Scilab functions)
// nearest : yi take the values corresponding to the nearest neighbor of xi  
//
// Several kind of extrapolations may be computed :
// 'extrap' : the extrapolation points is performed by the defined method 
//  real value : you can choose a real value for extrapolation, in this way yp(i) takes this value for xp(i) not in [x1,xn] interval, for example 0 (but also nan or inf).
// by default the extrapolation is performed by the defined method (for spline method), and by nan for linear and nearest method. 
// F.B

  rhs=size(varargin)
  // 2 < Number of inputs arguments < 5
  if rhs<2 | rhs>5 then 
    error("wrong number of arguments RHS")
  end

  //if yi=interp1(x,y,xi,..) not change
  //if yi=interp1(y,xi,...) replace input argument by yi=interp1(x,y,xi,..),whith x=1:size(y,1) by default
  if rhs==2 | (rhs>2 & type(varargin(3))==10) then
    if isvector(varargin(1)) then
      X=1:size(varargin(1),"*")
    elseif size(size(varargin(1)),"*")==2 then
      if (size(varargin(1),1)>1 & size(varargin(1),2)>1) then
	X=1:size(varargin(1),1)
      end
    else 
      error("y must be a vector or a matrix")
    end
    for i=rhs:-1:1
      varargin(i+1)=varargin(i)
    end
    varargin(1)=X
  end

  //............................
  //ininialisation of xi
  //............................
  //xi components must be reals
  xi=varargin(3)
  if type(xi)<>1 then
    if typeof(xi)=="hypermat" then
      if type(xi.entries)<>1 then
	error("xi must be a reals array ")
      end
    else
      error("xi must be a reals array")
    end
  end
  //delete the dimension of xi equal to one after the second dimension
  //or the first dimension 
  xisize=size(xi);
  while size(xisize,"*")>=2 & xisize($)==1
    xisize=xisize(1:$-1);
  end
  xisizetemp=xisize
  if size(xisize,"*")>=2 then
    if xisize(1)==1 then
      xisize=xisize(2:$);
    end
  end

  //-------------------------
  //Initialisation of x, y
  //-------------------------
  x=varargin(1);
  y=varargin(2);
  //x must be real vector
  if type(x)<>1 then
    error("x must be a reals array")
  end
  //y components must be reals
  if type(y)<>1 then
    if typeof(y)=="hypermat" then
      if type(y.entries)<>1 then
	error("y must be a reals array ")
      end
    else
      error("y must be a reals array")
    end
  end
  //verification of x,y line/column 
  if isvector(x) then
    if find(isnan(x))<>[] then
      error("x values must be real ")
    end
    if isvector(y) then 
      if size(x,"*")<>size(y,"*") then
	error("x,y:wrong line/column dimension")
      end
    elseif size(size(y),"*")>=2 then
      if size(x,"*")<>size(y,1) then
	error("x,y:wrong line/column dimension")
      end
    else
      error("y must be a vector or a matrix")
    end
  else
    error("x must be a vector")
  end

  // xi : increase order sorting (for xi)
  [xtemp,p]=gsort(matrix(x,1,-1),'c','i')
  x=matrix(xtemp,size(x))
  x=matrix(x,1,-1)
  if isvector(y) then
    y=y(p)
  elseif size(size(y),"*") then
    for l=1:size(y,"*")/size(y,1)
      y(:,l)=y(p,l)
    end
  else
    error("y must be a real or a matrix")
  end

  //-------------------------------------------------
  // CASE : 3 intputs arguments : yi=interp1(x,y,xi) 
  //-------------------------------------------------

  //default method : linear method is used
  if size(varargin)==3 then
    yi=interp1(x,y,xi,'linear',%nan)
  end

  //--------------------------------------------------
  // CASE : 4 intputs arguments : yi=interp1(x,y,xi,method) 
  //--------------------------------------------------

  if size(varargin)==4 then
    select part(varargin(4),1)
      //-------------------------------------------  
      // Linear method : yi=linear(x,y,xi,'linear')
      //-------------------------------------------
      // the values of extrapolation points are nan for linear method  
    case "l"
      
      yi=interp1(x,y,xi,'linear',%nan) 
      
      //-------------------------------------------
      // Spline method  yi=interp1(x,y,xi,'spline')
      //-------------------------------------------
      // the extrapolation used the spline method
    case "s"
      if xi==[] then
	yi=[]
	return
      end

      yi=interp1(x,y,xi,'spline','extrap')  
      //----------------------------------------------
      // Nearest method  yi=interp1(x,y,xi,'nearest')
      //----------------------------------------------
      // the values of extrapolation points are nan for nearest method  
    case "n"
      if xi==[] then
	yi=[]
	return
      end
      yi=interp1(x,y,xi,'nearest',%nan)
    else
      error("wrong interpolation")
    end
  end

  //----------------------------------------------------------------------------------------------------------
  // CASE : 5 intputs arguments :  yi=interp1(x,y,xi,method,'extrap')  or  yi=interp1(x,y,xi,method,extrapval)
  //----------------------------------------------------------------------------------------------------------

  if size(varargin)==5 then
    select part(varargin(4),1)
      //--------------------------------------------------------------------------------------------  
      // Linear method : yi=linear(x,y,xi,'linear','extrap') or  yi=interp1(x,y,xi,method,extrapval)
      //---------------------------------------------------------------------------------------------
    case "l"    
      xitemp=matrix(xi,-1,1)
      // y is a vector
      if isvector(y) then
	yi=hypermat(size(xitemp))
	[x,ind]=gsort(matrix(x,1,-1),"c","i")
	if varargin(5)==%nan then
	  yi=linear_interpn(xitemp,x,y(ind),"by_nan");
	end
	if type(varargin(5))==10 then 
	  if varargin(5)<>"extrap" then
	    error("wrong extrapolation")
	  else
	    yi=linear_interpn(xitemp,x,y(ind),"natural");
	  end
	elseif type(varargin(5))==1  then
	  yi=linear_interpn(xitemp,x,y(ind),"by_nan");
	  if ~isnan(varargin(5)) then
	    k=find(xitemp>max(x)|xitemp<min(x)) 
	    yi(k)=varargin(5)
	  end
	end
	if size(xisize,"*")>=2
	  yi=matrix(yi,xisize)
	else
	  yi=matrix(yi,xisizetemp)
	end
        
	// y is matrix or hypermatrix
      elseif size(size(y),"*")>=2 then
	ysize=size(y)
	ky=ysize(2:$)
	yi=hypermat([size(xitemp),ky])
	[x,ind]=gsort(matrix(x,1,-1),"c","i")
	//extrapolation
	if type(varargin(5))==10 then
	  if varargin(5)<>"extrap" then
	    error("wrong extrapolation")
	  else
	    if xitemp==[] then 
	      yi=[]
	      return
	    end
	    for l=1:size(y,"*")/size(y,1)
	      ytemp=y(:,l)
	      yi(:,l)=matrix(linear_interpn(xitemp,x,ytemp(ind),"natural"),size(xitemp)) 
	    end 
	  end
	elseif type(varargin(5))==1 then
	  if xitemp==[] then 
	    yi=[]
	    return
	  end
	  for l=1:size(y,"*")/size(y,1)
	    ytemp=y(:,l)
	    yi(:,l)=matrix(linear_interpn(xitemp,x,ytemp(ind),"by_nan"),size(xitemp)) 
	  end 
	  if ~isnan(varargin(5)) then
	    k=find(xitemp>max(x)|xitemp<min(x)) 
	    yi(k,:)=varargin(5)
	  end
	end  
	yi=matrix(yi,[xisize,ky])
      else 
	error("y must be a vector or a matrix")
      end   
      
      //----------------------------------------------------------------------------------------------
      // Spline method  yi=interp1(x,y,xi,'spline','extrap') or  yi=interp1(x,y,xi,'spline',extrapval) 
      //----------------------------------------------------------------------------------------------    
    case "s"
      if xi==[] then 
	if varargin(5)=="extrap"|type(varargin(5))==1 then
	  yi=[]
	  return
	else
	  error("wrong extrapolation")
	end
      end
      xitemp=matrix(xi,-1,1)
      //y is a vector
      if isvector(y) then
	yi=hypermat(size(xitemp))
	yi=interp(xitemp,matrix(x,1,-1),matrix(y,1,-1),splin(matrix(x,1,-1),matrix(y,1,-1)),"natural");
	if type(varargin(5))==10 then
	  if varargin(5)<>"extrap" then
	    error("wrong extrapolation")
	  end
	elseif type(varargin(5))==1  then
	  k=find(xitemp>max(x)|xitemp<min(x)) 
	  yi(k)=varargin(5)
	end 
	if size(xisize,"*")>=2
	  yi=matrix(yi,xisize)
	else
	  yi=matrix(yi,xisizetemp)
	end
	//y is a matrix or a hypermatrix
      elseif size(size(y),"*")>=2 then 
	ky=size(y)
	ky=ky(2:$)
	yi=hypermat([size(xitemp),ky])
	for l=1:size(y,"*")/size(y,1) 
	  yi(:,l)=matrix(interp(matrix(xi,-1,1),matrix(x,-1,1),y(:,l),splin(matrix(x,-1,1),y(:,l)),"natural"),size(xitemp))//les composante de yi
	end
	//extrapolation
	if type(varargin(5))==10 then
	  if varargin(5)<>"extrap" then
	    error("wrong extrapolation")
	  end
	elseif type(varargin(5))==1  then
	  k=find(xitemp>max(x)|xitemp<min(x)) 
	  yi(k,:)=varargin(5)
	end  
	yi=matrix(yi,[xisize,ky])  
      else 
	error("y must be a vector or a matrix")
      end
      
      //------------------------------------------------------------------------------------------------
      // Nearest method  yi=interp1(x,y,xi,'nearest','extrap') or yi=interp1(x,y,xi,'nearest',extrapval)
      //------------------------------------------------------------------------------------------------    
    case "n"
      //if all xi values are nan, retuns nan values for yi
      if size(find(isnan(xi)),"*")==size(xi,"*") then
	if varargin(5)=="extrap"|type(varargin(5))==1 then
	  yi=xi
	  return
	else
	  error("wrong extrapolation") 
	end
      end
      if xi==[] then 
	if varargin(5)=="extrap"|type(varargin(5))==1 then
	  yi=[]
	  return
	else
	  error("wrong extrapolation")
	end
      end
      //y is vector
      if isvector(y) then
	xitemp=matrix(xi,1,-1)
	knan=find(isnan(xitemp))
	knotnan=find(~isnan(xitemp))
	[xitemp,p]=gsort(matrix(xitemp(knotnan),1,-1),"c","i")
	yi=matrix(xi,1,-1)
	k=zeros(xitemp)   
	x_size=size(x,"*")
	j=size(xitemp,"*")
	i=x_size
	while j>=1 & i>=1
	  if xitemp(j)>=x(i)  then
	    if i<>x_size then
	      k(j)=i
	    end
	    j=j-1
	  else 
	    i=i-1 
	  end
	end
	k(xitemp<x(1)) = 1;
	k(xitemp>=x(x_size)) = x_size-1;
	i = find(xitemp >= matrix((x(k)+x(k+1))/2,size(k)));
	k(i) = k(i)+1;
	yi=y(k)     
	yi=matrix(yi,1,-1)
	//extrapolation
	if type(varargin(5))==10 then
	  if varargin(5)<>"extrap" then
	    error("wrong extrapolation")
	  end
	elseif type(varargin(5))==1 then
	  n=find(xitemp>max(x)|xitemp<min(x)|isnan(xitemp)|isnan(xitemp)) 
	  yi(n)=varargin(5)
	else
	  error("wrong extrapolation")
	end
	yitemp=yi
	yi(p)=yitemp
	ytemp=yi
	yi=matrix(xi,1,-1)
	yi(knan)=%nan
	yi(knotnan)=ytemp
	if size(xisize,"*")>=2
	  yi=matrix(yi,xisize)
	else
	  yi=matrix(yi,xisizetemp)
	end
	//y is a matrix or a hypermatrix
      elseif size(size(y),"*")>=2 then 
	xitemp=matrix(xi,1,-1)
	knan=find(isnan(xitemp))
	knotnan=find(~isnan(xitemp))
	[xitemp,p]=gsort(xitemp(knotnan),"c","i")
	ind=size(y)
	ind=ind(2:$)
	yi=hypermat([size(xitemp,"*"),ind])
	k=zeros(xitemp)
	x_size=size(x,"*")
	j=size(xitemp,"*")
	i=x_size
	while j>=1 & i>=1
	  if xitemp(j)>=x(i)  then
	    if i<>x_size then
	      k(j)=i
	    end
	    j=j-1
	  else 
	    i=i-1 
	  end
	end
	k(xitemp<x(1)) = 1;
	k(xitemp>=x(x_size)) = x_size-1;
	i = find(xitemp >= matrix((x(k)+x(k+1))/2,size(k)));
	k(i) = k(i)+1;
	for l=1:size(y,"*")/size(y,1)
	  ytemp=matrix(y(:,l),1,-1)
	  yi(:,l) =ytemp(k)
	end
	//extrapolation
	if type(varargin(5))==10 then
	  if varargin(5)<>"extrap" then
	    error("wrong extrapolation")
	  end
	elseif type(varargin(5))==1 then
	  n=find(xitemp>max(x)|xitemp<min(x))
	  for l=1:size(y,"*")/size(y,1)
	    yi(n,l)=varargin(5)
	  end
	else
	  error("wrong extrapolation")
	end 
	yitemp=yi
	for l=1:size(y,"*")/size(y,1)
	  yi(p,l)=yitemp(:,l)
	end
	yitemp=yi
	yi=hypermat([size(xi,"*"),ind])
	for l=1:size(y,"*")/size(y,1)
	  yi(knan,l)=%nan
	  yi(knotnan,l)=yitemp(:,l)
	end
	yi=matrix(yi,[xisize,ind])
      else
	error("y must be a vector or a matrix")
      end  
    else error("wrong interpolation") 
    end
  end
endfunction 
