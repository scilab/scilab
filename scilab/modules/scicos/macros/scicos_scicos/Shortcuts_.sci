//  Scicos
//
//  Copyright (C) INRIA - METALAU Project <scicos@inria.fr>
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
//
// See the file ../license.txt
//

function Shortcuts_()

%rr=lines()
%scicos_short
lines(0)
disp(%scicos_short)
lines(%rr(2));
xpause(1000)
xinfo('Select an item in a menu to set shortcut')
EnableAllMenus()
[btn,%pt,cwin,Cmenu]=cosclick()
if Cmenu<>'Quit' then
  %okay=%f
  if Cmenu<>[] then
    %koko=find(%scicos_short(:,2)==Cmenu)
    if %koko<>[] then
      txt=x_mdialog(['Edit the short cut (a-z)';'only lower case letters allowed'],..
	  %scicos_short(%koko,2),%scicos_short(%koko,1))
      if txt<>[] then 
	if txt<>emptystr() then txt=part(txt(1),1);end
	if find(txt==%scicos_short(:,1))<>[] then
	  message(txt+' already in use for '+..
	      %scicos_short( find(txt==%scicos_short(:,1)),2))
	elseif txt==emptystr() then
	  %scicos_short=[%scicos_short(1:%koko-1,:);
	      %scicos_short(%koko+1:$,:)];%okay=%t;
	elseif ascii(txt)>122|ascii(txt)<97 then
	  message(txt+' is not in a-z')
	else
	  %scicos_short(%koko,1)=txt;%okay=%t;
	end
      end
    else
      %koko=find(%cor_item_exec(:,1)==Cmenu)
      if %koko<>[] then
        txt=x_mdialog(['Add new short cut (a-z)';'only lower case letters allowed'],..
	    %cor_item_exec(%koko,1),emptystr())
	if txt<>[] then 
	  txt=part(txt(1),1)
	  if find(txt==%scicos_short(:,1))<>[] then
	    message(txt+' already in use for '+..
		%scicos_short( find(txt==%scicos_short(:,1)),2))
	  elseif ascii(txt)>122|ascii(txt)<97 then
	    message(txt+' is not in a-z')
	  else
	    %scicos_short=[%scicos_short;..
		[part(txt(1),1),%cor_item_exec(%koko,1)]];
	    %okay=%t
	  end
	end
      end
    end 
    if %okay then 
      if execstr('save(''.scicos_short'',%scicos_short)','errcatch') <>0 then
	message('Cannot save .scicos_short in current directory')
      end
      %tableau=emptystr([1:100]);
      for %Y=1:size(%scicos_short,1)
	%tableau(-31+ascii(%scicos_short(%Y,1)))=%scicos_short(%Y,2);
      end
    end
    Cmenu=[]
  end
  xinfo(' ')
end
endfunction
