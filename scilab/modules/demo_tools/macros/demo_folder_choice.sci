
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2007-2008 - INRIA - Ghislain HELIOT
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function []=demo_folder_choice(path,ch)

	if ch=='anim' then
		deff('[]=demoexc(num)','exec(path+demolist(num,2),-1)')
		fs = get('figure_style');
		while %t then
			num=x_choose_modeless(demolist(:,1),'Choose a demo');
			if num==0 then
				set('figure_style',fs);
				lines(oldln(1));
				return
			else
				set('figure_style','old');
				xdel();
				clf();
				demoexc(num);
			end
		end
	
	else
	
		if ch=='root' then
			deff('[]=demoexec(num)','exec(demolist(num,2),-1)')
			while %t then
				num=x_choose_modeless(demolist(:,1),['Click to choose a demo';
				'(see also in SCIDIR/demos/...)']);
				if num==0 then
					lines(oldln(1))
					return
				else
					demoexec(num);
				end
			end
			
		else
			deff('[]=demoex(num)','exec(path+demolist(num,2),-1)')
			while %t then
				num=x_choose_modeless(demolist(:,1),'Choose a demo');
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
