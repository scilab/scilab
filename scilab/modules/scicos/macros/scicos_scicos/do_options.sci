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

function [edited,options] = do_options(opt,flag)
//
  colors    = string(1:xget("lastpattern") + 2);
  
  fontsSiz  = [ "08", ..
                "10", ..
                "12", ..
                "14", ..
                "18", ..
                "24"];
                
  fontsIds  = [ "Courrier", ..
                "Symbol", ..
                "Times", ..
                "Times Italic", ..
                "Times Bold", ..
                "Times B. It."];
                
  marksIds  = [ ".", ..
                "+", ..
                "x", ..
                "*", ..
                "diamond fill.", ..
                "diamond", ..
                "triangle up", ..
                "triangle down", ..
                "trefle", ..
                "circle"];

  DashesIds = [ "Solid", ..
                "-2-  -2-", ..
                "-5-  -5-", ..
                "-5-  -2-", ..
                "-8-  -2-", ..
                "-11- -2-", ..
                "-11- -5-"];

  ok      = %f
  edited  = %f
  options = opt
  
  if flag == "3D" then
  
    With3D = options("3D")(1)
    if type(With3D) == 4 then
      with3d = 0; with3d(With3D) = 1
    else
      with3d = With3D
    end
    if with3d then tt = "Yes", else tt = "No", end
    rep1 = message(["Use 3D aspect?" ; "current choice is " + tt], ["Yes" ; "No"])
    
    if rep1 <> 1 then rep1 = 0, end
    if rep1 then
      Color3D = options("3D")(2)
      rep2 = getcolor("3D color", Color3D)
      if rep2 == [] then rep2 = Color3D, end
    else
      rep2 = options("3D")(2)
    end
    ok = %t
    options("3D")(1) = (rep1 == 1)
    options("3D")(2) = rep2
  
  elseif flag == "Background" then
  
    bac = options("Background")
    if bac == [] then bac = [8 1], end        // for compatibility
    if size(bac,"*") < 2 then bac(2) = 1, end // for compatibility
    //lcols_bg = list("Background colors", bac(1), colors) //++ no longer used
    //lcols_fg = list("Foreground colors", bac(2), colors) //++ no longer used
    //rep = x_choices("Default background and foreground colors", list(lcols_bg, lcols_fg)) //++ no longer used
    
    rep = [bac(1), bac(2)]
    rep1 = getcolor("Background color ? :", bac(1))
    if rep1 <> [] then rep(1) = rep1, end
    rep2 = getcolor("Foreground color ? :", bac(2))
    if rep2 <> [] then rep(2) = rep2, end
    if or(rep <> [bac(1) bac(2)]) then
      ok = %t
      options("Background") = rep
    end
  
  elseif flag == "LinkColor" then
  
    //++ Default link colors (regular and event ones)
    //++ "rep" contains the indexes of chosen colors in current colormap 
    
    // lcols_rl = list("Regular links colors", options("Link")(1), colors) //++ no longer used
    // lcols_el = list("Event links colors  ", options("Link")(2), colors) //++ no longer used
    // rep = x_choices("Default regular and event link colors", list(lcols_rl, lcols_el)) //++ no longer used
    
    linkColors = options("Link")
    if linkColors == [] then linkColors = [1 5], end // defaults = [black red] 
    rep = [linkColors(1), linkColors(2)]
    
    rep1 = getcolor("Regular links color ? :", linkColors(1))
    if rep1 <> [] then rep(1) = rep1, end
    
    rep2 = getcolor("Event links color? :",   linkColors(2))
    if rep2 <> [] then rep(2) = rep2, end
    
    if or(rep <> [linkColors(1) linkColors(2)]) then
      ok = %t
      options("Link") = rep
    end
  
  elseif flag == "ID" then
  
    lfid_l = list("Link ID font name",  options("ID")(2)(1)+1, fontsIds);
    lfiz_l = list("Link ID font size",  options("ID")(2)(2)+1, fontsSiz);
    lfid_b = list("Block ID font name", options("ID")(1)(1)+1, fontsIds);
    lfiz_b = list("Block ID font size", options("ID")(1)(2)+1, fontsSiz);
    rep    = x_choices("ID font definitions", list(lfid_l, lfiz_l, lfid_b, lfiz_b))
    if rep <> [] then
      ok = %t
      options("ID")(1) = rep(3:4) - 1
      options("ID")(2) = rep(1:2) - 1
    end
    
  elseif flag == "Cmap" then
  //** --------------------- Add a new color ------------------------------------------
 
  [R, G, B] = uigetcolor("Add a new color at the Scicos palette");

  if R<>[] & G<>[] & B<>[] then 
     R= R/255; G= G/255; B= B/255 ; //** normalize to one 
    options("Cmap") = [options("Cmap") ; [R, G, B]]
  end

//** ---------------------- OLD CODE---------------------------------------------------- 
//     while %t do
//       [ok, R,G,B] = getvalue(["Enter RGB description of new colors"; ..
//                               "Each component must be greater or equal to 0"; ..
//                               "and less or equal to 1"], ..
//                               ["R","G","B"], ..
//                               list("vec", -1, "vec", -1, "vec", -1), ..
//                               [" ", " ", " "]) ; 
//       if ~ok then break, end
//       if or(R < 0 | R > 1) | or(G < 0 | G > 1) | or(B < 0 | B > 1) then
//         messagebox("One or several RGB components are out of [0 1]","modal","error");
//       else
//         break; //** EXIT from the input loop 
//       end
//     end
//     if ok then
//       //** add one or more color ad the palette 
//       options("Cmap") = [options("Cmap") ; [R(:), G(:), B(:)]]
//     end

  end // if flag == ...
  
  if ok then
    edited = or(opt <> options)
  end

endfunction
