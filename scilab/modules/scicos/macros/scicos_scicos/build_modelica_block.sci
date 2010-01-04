//  Scicos
//
//  Copyright (C) INRIA - Serge Steer <serge.steer@inria.fr> - 2003
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

function [model,ok] = build_modelica_block(blklstm, cmmat, NiM, NoM, name, path)

// given the blocks definitions in blklstm and connections in cmmat this
// function first create  the associated modelicablock  and writes its code
// in the file named 'imppart_'+name+'.mo' in the directory given by path
// Then modelica compiler is called to produce the C code of scicos block
// associated to this modelica block. filbally the C code is compiled and
// dynamically linked with Scilab.
// The correspondind model data structure is returned.

//** Thanks to Serge Steer for this "superbe" explanation :) 

name = "imppart_" + stripblanks(name);
path = pathconvert(stripblanks(path),%t,%t);
//++ convert space to _
name = strsubst(name, ' ', '_');

[txt,rpar,ipar] = create_modelica1(blklstm,cmmat,name);

mputl(txt, path+name+'.mo');

mprintf(_('   Modelica code generated at ""%s.mo""\n'),path+name); 

[ok,name1,nx,nin,nout,ng,nm,nz] = compile_modelica(path+name+'.mo'); //** here we are 

if ~ok then
   return
end

// nx is the state dimension
// ng is the number of surfaces

// build model data structure of the block equivalent to the implicit part
model = scicos_model(sim = list(name,10004),.. 
	             in  = ones(nin,1),out=ones(nout,1),..
	  	     state  = zeros(nx*2,1),..
		     dstate = zeros(nz,1),..
		     rpar   = rpar,..
		     ipar   = ipar,..
		     dep_ut = [%f %t],..
                     nzcross = ng,..
                     nmode   = nm) ; 

endfunction
