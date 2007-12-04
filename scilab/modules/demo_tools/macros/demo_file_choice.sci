// =============================================================================
// Author : Ghislain HELIOT
// Scilab Project
// Copyright INRIA 2007
// =============================================================================

function []=demo_file_choice(path,ch)
	
	if ch=='anim' then
		deff('[]=demoexc(num)','exec(path+demolist(num,2),-1)')
		fs=get('figure_style');
		
		while %t then
			num=tk_choose(demolist(:,1),'Choose a demo');
			if num==0 then
				set('figure_style',fs);
				lines(oldln(1));
				return
			else
				set('figure_style','new');
				delete(gcf());//new style
				clf();
				demoexc(num);
			end
		end
		
	else
		
		if ch=='root' then
			deff('[]=demoexec(num)','exec(demolist(num,2),-1)')
			while %t then
			num=tk_choose(demolist(:,1),['Click to choose a demo';
			'(see also in SCIDIR/demos/...)']);
			if num==0 then
				lines(oldln(1))
				return
			else
				demoexec(num);
			end,
		end
		
		else
			deff('[]=demoex(num)','exec(path+demolist(num,2),-1)');
			while %t then
				num=tk_choose(demolist(:,1),'Choose a demo');
				if num==0 then
					lines(oldln(1))
					return
				else
					xdel(0);
					clf();
					demoex(num);
				end
			end
		end
	end
	mode(0);
endfunction
