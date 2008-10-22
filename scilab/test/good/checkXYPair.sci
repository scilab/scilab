function [X,Y]=checkXYPair(typeOfPlot,x,y,current_figure,cur_draw_mode)

  ok=%F;

  if type(y)==13 // If y is a function
    f=y;
    if and(size(x)~=1)  // then x *must* be a vector
      warning(sprintf('%s : x must be a vector',typeOfPlot));
      ResetFigureDDM(current_figure, cur_draw_mode);
      return;
    end

    t=x(:); // to ensure that t is a column vector

    [nArgOut,vectInput]=check2dFun(typeOfPlot,f,t,current_figure,cur_draw_mode);

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
    
    if size(X)==[0 0] | size(Y)==[0 0]
      ok=%F
      str='plot error : empty input data';
      warning(str);
      ResetFigureDDM(current_figure, cur_draw_mode)
      return;
    end
    
    if and(size(X)==size(Y))  // si les tailles sont egales
      ok=%T;
      //    disp("I.)")
      return
    end

    if (size(X,2)==1) & (size(Y,1)==size(X,1))
      ok=%T;
      //    pause;
      //    disp("II.)")
      return;
    end

    if (size(X,2)==1) & (size(Y,2)==size(X,1))
      Y=Y';
      ok=%T;
      //    disp("III.)")
      //    return;
    end
    

    // new case : plot(MAT4x4,[1 2 3 4]) HERE Y is a vector and X a matrix
    // extend y to be a 4x4 matrix defined as [1 1 1 1;2 2 2 2;3 3 3 3;4 4 4 4]
    if or(size(Y) == 1)
      if size(X,1) == size(Y,1)
	//      disp("ENTRE 1!")
	//      pause;
	y=Y;
	Y=[];
	for i=1:size(X,2)
	  Y = [Y y]; 
	end
	//      disp("FINI")
	//      pause;
      elseif size(X,1) == size(Y,2)
	//      disp("ENTRE 2!")
	//      pause;
	y=Y(:);
	Y=[];
	for i=1:size(X,2)
	  Y = [Y y]; 
	end
      elseif size(X,2) == size(Y,1)
	//      disp("ENTRE 3!")
	//      pause;
	X=X';
	y=Y(:);
	Y=[];
	for i=1:size(X,2)
	  Y = [Y y]; 
	end
	//      disp("FIN...!")
	//      pause;
      elseif size(X,2) == size(Y,2)
	//      disp("ENTRE 4!")
	//      pause;
	y=Y;
	Y=[];
	for i=1:size(X,1)
	  Y = [Y y]; 
	end
      end
      
      //    disp("IV.)")
      ok=%T;
      return;
    end
    
    
    if ~ok
      str='plot : incompatible dimensions of data arguments';
      warning(str);
      ResetFigureDDM(current_figure, cur_draw_mode)
    end

  end

  // end of checkXYPair
endfunction

