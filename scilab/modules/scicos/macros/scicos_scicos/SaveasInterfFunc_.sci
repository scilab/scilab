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

function SaveasInterfFunc_()

    Cmenu = []
    ok = %f
    
    while ~ok then
      fname = savefile('*.sci')
      if fname<>emptystr() then 
	[Dir,name,ext] = splitfilepath_cos(fname)
	select ext
	case 'sci' then
	  ok=%t
	else
	  message('Only *.sci files allowed');
	  ok = %f
	end
      else
	ok = %t
      end
    end
    
    if fname<>emptystr() then
      scs_m.props.title(1) = name
      [ok, %wh] = getvalue(['Enter the type of Interf. Func. to create'],..
	                   ['Block(0) or SuperBlock(1)'],list('vec','1'),..
	                   ['1'])
      if ok then
	scs_m.props.title(1) = name
	if  %wh==0 then
	  %path = save_csuper(scs_m,Dir)
	else
	  %path = save_super(scs_m,Dir)
	end
	
	if %path<>[] then 
	  if execstr('exec(%path)','errcatch')<>0 then
	     message([name+': error:'
		      lasterror()])
	  end
	end
      end
    end
endfunction
