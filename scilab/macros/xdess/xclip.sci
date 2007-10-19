function []=xclip(x,y,w,h)
	
	// fixe une zone de clipping en cordonnees reelles
	// (x,y,w,h) (Upper-Left,wide,Height)
	//
	// Copyright INRIA
	
	[lhs,rhs]=argn(0)
	
	if get("figure_style") =='old' then
		
		if rhs<=0 then
			xset("clipoff");
			return;
		end
		
		if rhs==1 then
			if typeof(x)<>"string" then 
				xset("clipping",x(1),x(2),x(3),x(4));
			else
				xset(x);
			end
		else 
			xset("clipping",x,y,w,h);
		end
	else
		ax = gca();
		if rhs <= 0 then
			ax.clip_state = "off"
		elseif rhs==1 then
			if typeof(x) <> "string" then 
				ax.clip_state = "on"
				ax.clip_box  = x;
			elseif x == "clipon" then
				ax.clip_state = "on"
			elseif x == "clipoff" then
				ax.clip_state = "off"
			elseif x == "clipgrf" then
				ax.clip_state = "clipgrf"
			else
				error("Invalid argument")
			end
		else
			ax.clip_state = "on";
                        disp(ax);
			ax.clip_box  = [x,y,w,h];
		end
	end
	
endfunction
