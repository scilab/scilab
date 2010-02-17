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

function [ind]=modelica_cind_from_name(txt)
//Copyright INRIA
//## return indices of a modelica block in the
//## compiled modelica structure given its name(s)
//## inputs :
//##   txt : a vector of strings that gives the names
//##         of the modelica blocks to find in the compiled
//##         modelica structure. if is not given all indexes are returned.
//##
//## output :
//##   ind : the indices of modelica blocks in the
//##         compiled modelica structure
//## initial rev 14/12/07, Alan

 //## default value for output
 ind = []

 //## Get number of Right Hand Side arguments
 rhs = argn(2)

 //## check if %cpr exists
 if ~exists('%cpr') then
   return; //## silent exit(nothing is done)
 end

 //## check if the diagram have been compiled
 if %cpr==list() then
   return; //## silent exit(nothing is done)
 end

 //## get the corinv of the modelica blocks
 corinvm = %cpr.corinv($);

 //## check the type of corinvm
 if type(corinvm)<>15 then
   return; //## silent exit(nothing is done)
 end

 //## get the number of modelica blocks
 nb = lstsize(corinvm);

 //## check if txt is given
 if rhs==1 then
   //## check type of txt
   if (type(txt)<>10) then
     return; //## silent exit(nothing is done)
   end

   //## format txt
   txt=txt(:)';
 //## no argument : return all the indices of modelica blocks
 else
   ind = 1:nb;
   return;
 end

 //## Bnames are the names of the modelica blocks
 //## in the modelica compiled structure
 Bnam = [];
 Bnames = [];

 //## loop on numbers of modelica blocks
 //## to build the vector of names of modelica blocks
 //## in the compiled modelica structure
 for k=1:nb
    //## get the path in the scs_m structure
    path = scs_full_path(corinvm(k));

    //## get the objs of the modelica block number k
    o = scs_m(path);

    //## get the model of the modelica block number k
    omod = o.model;

    //## get the graphics of the modelica block number k
    ogra = o.graphics;

    //## get the list equations
    mo = omod.equations;

    //## update Bnames
    Bnam=[Bnam, get_model_name(mo.model,Bnam)];

    //## get the identification field
    id = stripblanks(ogra.id)

    if id<>'' then
      Bnames = [Bnames, Bnam($)+'_'+...
                stripblanks(ogra.id)]
    else
      Bnames = [Bnames, Bnam($)]
    end
 end

 //## return the indices of the researched modelica blocks names

 //## loop on size of txt
 for i=1:size(txt,2)
   ind = [ind;
          find(Bnames==txt(i))]
 end
 //## format ind
 ind=ind';

endfunction
