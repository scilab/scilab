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

function txt=do_newblk(o,nam)
//create text of GUI function associated with a a block
// o     : block data structure
// nam   : name of the created GUI function

//for  k=5:8,o(2)(k)=0*o(2)(k),end // disconnect links
o.graphics.pin=0*o.graphics.pin;o.graphics.pout=0*o.graphics.pout;
o.graphics.pein=0*o.graphics.pein;;o.graphics.peout=0*o.graphics.peout;
//o(2)(1)=0*o(2)(1) // set origin to zero
o.graphics.orig=0*o.graphics.orig // set origin to zero
//o(2)(2)=o(2)(2)/20 // set definition size ?????
o.graphics.sz=o.graphics.sz/20 // set definition size ?????
// form text of the macro
nam=stripblanks(nam)
txt=[
    'function [x,y,typ]='+nam+'(job,arg1,arg2)';
    'x=[];y=[],typ=[]';
    'select job';
    'case ''plot'' then';
    '  standard_draw(arg1)';
    'case ''getinputs'' then';
    '  [x,y,typ]=standard_inputs(arg1)';
    'case ''getoutputs'' then';
    '  [x,y,typ]=standard_outputs(arg1)';
    'case ''getorigin'' then';
    '  [x,y]=standard_origin(arg1)';
    'case ''set'' then';
    'case ''define'' then';
    sci2exp(o,'x');
    'end']
endfunction
