(*
   *  Translator 
   *
   *  Copyright (C) 2005 - 2007 Imagine S.A.
   *  For more information or commercial use please contact us at www.amesim.com
   *
   *  This program is free software; you can redistribute it and/or
   *  modify it under the terms of the GNU General Public License
   *  as published by the Free Software Foundation; either version 2
   *  of the License, or (at your option) any later version.
   *
   *  This program is distributed in the hope that it will be useful,
   *  but WITHOUT ANY WARRANTY; without even the implied warranty of
   *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   *  GNU General Public License for more details.
   *
   *  You should have received a copy of the GNU General Public License
   *  along with this program; if not, write to the Free Software
   *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.   
   *
   *  This file is a template file that will be transformed to versiondate.ml 
   *  using sed that will replace the values (AUTO_UPDATE_DATE and AUTO_UPDATE_REV) 
   *  of the variables auto_updated_date and auto_updated_rev with the actual SVN 
   *  last update date and version. 
   *
   *  On computers that doesn't have svn grep or sed you can simply copy
   *  this file to versiondate.ml and modify it if you like.
   *  
   *  Since this file is automatically transformed using a sed command
   *  the lines which set the variables auto_updated_date and auto_updated_rev 
   *  must be kept as they are (unless you want to change the sed command). 
   *  AJN 20/02/2008.
*)

let default_date = "2008-04-01"

let auto_updated_date = "AUTO_UPDATE_DATE"
let auto_updated_rev = "AUTO_UPDATE_REV"

let print_versiondate () =
  if auto_updated_date = "AUTO_UPDATE_DATE" then
    Printf.printf "Build date is not known (%s).\n" default_date
  else
    Printf.printf "%s.\n" auto_updated_date;
  if auto_updated_rev = "AUTO_UPDATE_REV" then
    Printf.printf ""
  else
    Printf.printf "%s.\n" auto_updated_rev;

