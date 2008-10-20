function  barh(varargin)

	// Copyright INRIA
	// barh(x,y,width,style,color) 
	// Intput :
	// x : a scalar or a vector of reals
	// y : a sclar, a vector or a matrix of reals
	// width : a double, the bar width, it's the percentage (0<width<1) of the max width of one bar which is wanted (default: width=0.8)
	// style : a string, 'grouped' or 'stacked' (default: style='grouped')
	// F.B
	
	styletab=["grouped","stacked"]
	COLORBOOL=%f
	
	// Default values
	STYLE="grouped"
	WIDTH=0.8
	COLORBOOL=%f
	
	// Check RHS arguments
	ListArg = varargin;
	
	// Detect and set the current axes now:
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
	
	// Set the double argument : x,y,width
	// barh(y,...)
	if size(argdb,"*")==1
		Y=ListArg(1)
		if or(size(Y)==1) then
			Y=Y(:)
		end 
		X=1:size(Y,1)
	end
	
	if size(argdb,"*")==2
		if size(ListArg(2),"*")==1 then
			// barh(x,y,...)
			if size(ListArg(1),"*")==1 then
				X=ListArg(1)
				Y=ListArg(2)
			else
				//barh(y,width,...)
				WIDTH=ListArg(2)
				Y=ListArg(1)
				if or(size(Y)==1) then
					Y=Y(:)
				end
				X=1:size(Y,1)
			end
		else
			// barh(x,y,...) 
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
		end    
	end
	
	// barh(x,y,width,...)      
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
	
	// Set the string argument
	for i=1:size(argstr,"*") 
		// barh(...,style)
		if or(ListArg(argstr(i))==styletab) then
			STYLE=ListArg(argstr(i))
		else
			COLOR=ListArg(argstr(i))
			COLORBOOL=%t
		end
	end
	
	// Verify if there are data bounds which are defined before creation the horizontal bars creation, in order to merge the data bounds
	a=gca()
	if size(a.children)<>0 then
		gca_children_empty=%t
		a_data_bounds=a.data_bounds
	else
		gca_children_empty=%f
	end
	
	drawlater();
	
	if COLORBOOL
		plot(X,Y,COLOR)
	else 
		plot(X,Y)
	end
	
	barh_number=size(Y,2)
	
	if size(X,"*")>1 then
		Xtemp=gsort(X,'r','i')
		inter=Xtemp(2)-Xtemp(1)
		for i=2:size(Xtemp,"*")-1
			inter=min(Xtemp(i+1)-Xtemp(i),inter)
		end
		if barh_number>1
			inter=inter*0.9
		end
	else
		inter=1
	end
	
	wmax=inter/barh_number
	y_shift=zeros(size(X,"*"),1)
	bar_number= size(Y,2)
	e=gce()
	a=gca()
	a.sub_ticks(2) = 0
	
	for i=bar_number:-1:1
		
		ei = e.children(i)
		
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
		
		// Update axes data bounds 
		// case 'grouped'
		if STYLE=="grouped"
			if i <> bar_number then
				a.data_bounds=[min(a.data_bounds(1,1),min(Y(:,bar_number-i+1)),0) min(a.data_bounds(1,2),min(X)+x_shift-wmax/2); max(a.data_bounds(2,1),max(Y(:,bar_number-i+1)),0) max(a.data_bounds(2,2),max(X)+x_shift+wmax/2)]
			else
				if ~gca_children_empty
					a.data_bounds=[min(min(Y(:,bar_number-i+1)),0) min(X)+x_shift-wmax/2; max(max(Y(:,bar_number-i+1)),0) max(X)+x_shift+wmax/2]
				else
					a.data_bounds=[min(a_data_bounds(1,1),min(Y(:,bar_number-i+1)),0) min(a_data_bounds(1,2),min(X)+x_shift-wmax/2); max(a_data_bounds(2,1),max(Y(:,bar_number-i+1)),0) max(a_data_bounds(2,2),max(X)+x_shift+wmax/2)]
				end
			end
			ei.x_shift=x_shift*ones(size(X,"*"),1)
		else  // case 'stacked'
			wmax=inter
			if i <> bar_number then
				a.data_bounds=[min(a.data_bounds(1,1),min(Y(:,bar_number-i+1)+y_shift)) min(a.data_bounds(1,2),0,min(X-wmax/2)); max(a.data_bounds(2,1),max(Y(:,bar_number-i+1)+y_shift)) max(a.data_bounds(2,2),0,max(X+wmax/2))]
			else
				if ~gca_children_empty
					a.data_bounds=[min(Y(:,bar_number-i+1)+y_shift) min(0,min(X-wmax/2)); max(Y(:,bar_number-i+1)+y_shift) max(0,max(X+wmax/2))]
				else
					a.data_bounds=[min(a_data_bounds(1,1),min(Y(:,bar_number-i+1)+y_shift)) min(a_data_bounds(1,2),0,min(X-wmax/2)); max(a_data_bounds(2,1),max(Y(:,bar_number-i+1)+y_shift)) max(a_data_bounds(2,2),0,max(X+wmax/2))]
				end
			end
			ei.y_shift=y_shift
		end
		
		a.y_ticks=tlist("ticks",Xtemp,string(Xtemp))
		w=WIDTH*wmax
		ei.bar_width=w
		ei.background=ei.foreground 
		ei.polyline_style=7; // bar type
		ei.background=ei.foreground  
		ei.foreground = -1; // black by default
		ei.line_mode='off';
	end
	
	drawnow();

endfunction
