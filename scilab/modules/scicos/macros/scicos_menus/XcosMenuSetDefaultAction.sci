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
function Setdefaultaction_()
// Copyright INRIA
  xinfo('Set Default Action')
  if %scicos_action==%f then repp=2, else repp=1, end
  repp=x_choices('Set Default Action',list(list('Type',repp,["Free","Smart"])))
  if repp==2 then %scicos_action=%f, else %scicos_action=%t, end
  xinfo(' ')
  Cmenu = [] ; %pt = [];
endfunction
