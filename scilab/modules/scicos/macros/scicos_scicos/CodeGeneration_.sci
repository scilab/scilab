//  Scicos
//
//  Copyright (C) INRIA - METALAU Project <scicos@inria.fr>
//                      - Alan Layec <alan.layec@inria.fr>
//                      - Ramine Nikoukhah <ramine.nikoukhah@inria.fr>
//                      - Rachid Djenidi
//
//                      - Scilab 5 update by Simone Mannori 
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

function CodeGeneration_()
//Input editor function of Scicos code generator
//
//@l@n, 14/10/07
//
//
//** 10 Set 2007 : cleaner startup code by Simone Mannori  

    k = [] ; //** index of the CodeGen source superbloc candidate

    xc = %pt(1); //** last valid click position 
    yc = %pt(2); 
    
    %pt = []   ;
    Cmenu = [] ;

    k  = getobj(scs_m,[xc;yc]) ; //** look for a block
    //** check if we have clicked near an object
    if k==[] then
      return
    //** check if we have clicked near a block
    elseif typeof(scs_m.objs(k))<>'Block' then
      return
    end

//** If the clicked/selected block is really a superblock 
//**             <k>
    if scs_m.objs(k).model.sim(1)=='super' then
      
        XX = scs_m.objs(k);
        [ok, XX, alreadyran, flgcdgen, szclkINTemp, freof] = do_compile_superblock42(XX, scs_m, k, alreadyran);
        
        //**quick fix for sblock that contains scope
        gh_curwin = scf(curwin); 

      if ok then
        scs_m = changeports(scs_m,list('objs',k), XX);  //scs_m.objs(k)=XX
        if flgcdgen <> szclkINTemp then
           // XX.graphics.pein($)=size(scs_m.objs)+2
            XX.graphics.pein = [XX.graphics.pein ; size(scs_m.objs)+2]
            scs_m.objs(k) = XX
            bk = SampleCLK('define');
            [posx,posy] = getinputports(XX)
             posx = posx($); posy = posy($);
             teta = XX.graphics.theta
             pos  = rotate([posx;posy],teta*%pi/180, ...
                           [XX.graphics.orig(1)+XX.graphics.sz(1)/2,...
                            XX.graphics.orig(2)+XX.graphics.sz(2)/2]) ; 
             posx = pos(1); posy = pos(2);
             bk.graphics.orig = [posx posy]+[-30 20]
             bk.graphics.sz = [60 40]
             bk.graphics.exprs = [sci2exp(freof(1));sci2exp(freof(2))]
             bk.model.rpar = freof;
             bk.graphics.peout = size(scs_m.objs)+2
             scs_m.objs($+1) = bk;
             [posx2,posy2] = getoutputports(bk);
             lnk    = scicos_link();
             lnk.xx = [posx2;posx];
             lnk.yy = [posy2;posy];
             lnk.ct = [5 -1]
             lnk.from = [size(scs_m.objs) 1 0]
             lnk.to = [k flgcdgen 1]
             scs_m.objs($+1) = lnk;
        end
        edited      = %t ;
        needcompile = 4  ;
        Cmenu = "Replot" ;
      else
        Cmenu = "Open/Set"
      end
    
    else
      //** the clicked/selected block is NOT a superblock 
      message("Generation Code only work for a Superblock ! ")
    end
endfunction