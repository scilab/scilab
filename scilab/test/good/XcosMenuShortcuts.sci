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

function XcosMenuShortcuts()

%rr=lines()
%scicos_short

mess= ['[Ctrl]+[x] --> Cut'
       '[Ctrl]+[c] --> Copy'
       '[Ctrl]+[v] --> Paste '
       '[Delete] or [backspace]--> Delete '
       '[Ctrl]+[z] --> Undo '
       '[Ctrl]+[s] --> Save the diagram '
       '[Ctrl]+[a] --> Select all '
       ' '
       %scicos_short(:,1)+' --> '+%scicos_short(:,2)];
messagebox(mess,_("Defined shortcuts"),'info',_("Close"))

xinfo('Select an item in a menu to set shortcut')
EnableAllMenus()
[btn,%pt,cwin,Cmenu]=cosclick()
if Cmenu<>'XcosMenuQuit' then
  %okay=%f
  if Cmenu<>[] then
    %koko=find(%scicos_short(:,2)==Cmenu)
    if %koko<>[] then //already defined shortcut
      txt=x_mdialog(msprintf(_('Edit the short cut,\n only letters allowed')),..
	  %scicos_short(%koko,2),%scicos_short(%koko,1))
      if txt<>[] then 
	if txt<>emptystr() then txt=part(txt(1),1);end
	if find(txt==%scicos_short(:,1))<>[] then
	  messagebox(txt+_(' is already in use for ')+ ..
	      %scicos_short( find(txt==%scicos_short(:,1)),2), 'modal')
	elseif txt==emptystr() then
	  %scicos_short=[%scicos_short(1:%koko-1,:);
	      %scicos_short(%koko+1:$,:)];%okay=%t;
	elseif (ascii(txt)>122|ascii(txt)<97)&(ascii(txt)>90|ascii(txt)<65) then
	  messagebox(txt+_(' is not a letter'), 'modal')
	else
	  %scicos_short(%koko,1)=txt;%okay=%t;
	end
      end
    else
      label=XcosGetMenuLabel(Cmenu,%scicos_menu)
      if label<>[] then
         txt=x_mdialog(msprintf(_('Edit the short cut,\n only letters allowed')),..
	    label,emptystr())
	if txt<>[] then 
	  key=part(txt(1),1)
	  k=find(txt==%scicos_short(:,1))
	  if k<>[] then
	    klabel=XcosGetMenuLabel(%scicos_short(k,1),%scicos_menu)
	    messagebox(msprintf(_("The key %s is already in use for %s"),key,keylabel),'modal')
	  elseif (ascii(txt)>122|ascii(txt)<97)&(ascii(txt)>90|ascii(txt)<65) ...
	  then
	    messagebox(msprintf(_("The key %s is not a valid letter"),key),'modal')
	  else
	    %scicos_short=[%scicos_short; [key,Cmenu]];
	    %okay=%t
	  end
	end
      end
    end 
    if %okay then 
      if execstr('save(''.scicos_short'',%scicos_short)','errcatch') <>0 then
	messagebox(msprintf(_('Cannot save ""%s"" in current directory'),'.scicos_short'),'modal')
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
