function [X,Y]=checkXYPair(typeOfPlot,x,y) 

ok=%F;

if type(y)==13 // If y is a function
   f=y;
   if and(size(x)~=1)  // then x *must* be a vector
      error(sprintf('%s : x must be a vector',typeOfPlot));
   end

   t=x(:); // to ensure that t is a column vector

   [nArgOut,vectInput]=check2dFun(typeOfPlot,f,t);

   if nArgOut==1
      X=t;
      if vectInput
         Y=f(t);
      else
         Y=zeros(length(t),1);
         for i=1:length(t)
            Y(i)=f(t(i));
	 end
      end
   elseif nArgOut==2
      if vectInput
         [X,Y]=f(t);
      else
         X=zeros(length(t),1);
	 Y=zeros(length(t),1);
         for i=1:length(t)

// CANNOT DO THE SAME WITH X(i) and Y(i)
// instead of xt and yt (scilab parser sees the stuff 
// as a comparison)

	     [xt,yt]=f(t(i));
            X(i)=xt;Y(i)=yt;
	 end
      end
   end

else // "classical" case

X=x;
Y=y;

if size(X,1)==1, X=X', end;  // si l'un des vecteurs est une ligne
if size(Y,1)==1, Y=Y', end;  // on le transpose.

if and(size(X)==size(Y))  // si les tailles sont egales
   ok=%T;
   return
end

if (size(X,2)==1) & (size(Y,1)==size(X,1))
   ok=%T;
   return;
end

if (size(X,2)==1) & (size(Y,2)==size(X,1))
   Y=Y';
   ok=%T;
   return;
end

if ~ok
    str=sprintf('plot : incompatible dimensions of arguments number %d and %d',argNumber,argNumber+1);
    error(str);
end

end

// end of checkXYPair
