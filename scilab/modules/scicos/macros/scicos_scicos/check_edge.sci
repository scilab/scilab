//  Scicos
//
//  Copyright (C) INRIA - METALAU PROJECT <scicos@inria.fr>
//                      - Alan Layec <alan.layec@inria.fr>
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

function Cmenu = check_edge(o, Cmenu, %pt)
  // any old scicos macros, will be removed on 5.4.0
  warnobsolete(scilabRemovedVersion="5.4.0")

//**
//** 24/07/07: Al@n's patch for rotation of blocks
//**

  //** The click is inside the navigator 
  if windows(find(%win==windows(:,2)),1)==100000 then
    return //** click inside the Navigator 
  end

  %xc = %pt(1);
  %yc = %pt(2);

  if typeof(o)=="Block" then
    orig = o.graphics.orig;
    sz   = o.graphics.sz;

    orig = orig(:);
    sz   = sz(:);

    eps  = sz/5
    orig = orig + eps;
    sz   = sz - 2*eps;

    //** Geometrical correction for rotation 
    if %pt<>[] then
      xxx = rotate([ %pt(1);%pt(2)],...
                   -o.graphics.theta*%pi/180,...
                  [ orig(1)+sz(1)/2;orig(2)+sz(2)/2]);
      %xc = xxx(1);
      %yc = xxx(2);
    end

    data = [(orig(1)-%xc)*(orig(1)+sz(1)-%xc),..
            (orig(2)-%yc)*(orig(2)+sz(2)-%yc)]

    if data(1)<0 & data(2)<0 then
      // we have cliked inside the block so it is probably not a link
    else

      //find output port location projected on the block
      [%xout, %yout, typout] = getoutputports(o) ; 

      //** avoid to start a link from a Split 
      if %xout<>[] & ~(o.gui=="SPLIT_f" | o.gui=="CLKSPLIT_f") then

        %xxyymax = o.graphics.orig(:) + o.graphics.sz(:)

        %xout = max(min(%xout,%xxyymax(1)), o.graphics.orig(1))
        %yout = max(min(%yout,%xxyymax(2)), o.graphics.orig(2))

        // how close to the port is considered a link (/5)
        %center = orig+sz/2;
        if or((%xc-%xout).^2+(%yc-%yout).^2 <(%xc-%center(1)).^2+...
              (%yc-%center(2)).^2) then
          //we have clicked near a port
          Cmenu = "XcosMenuLink" 
        end
      
      end //** avoid "Link from a Split"
    
    end //** click inside the box 
    
  end //** the pbject is a block 

endfunction
