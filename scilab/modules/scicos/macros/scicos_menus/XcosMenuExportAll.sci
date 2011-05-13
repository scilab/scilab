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

function XcosMenuExportAll()
  // any old scicos macros, will be removed on 5.4.0
  warnobsolete(scilabRemovedVersion="5.4.0")


  Cmenu = []

  // Select output directory
  %exp_dir = uigetdir('./',_("Select a directory path"))
  
  if %exp_dir=="" then return; end //user cancelled
  info=fileinfo(%exp_dir)
  if isempty(info) then
    mkdir(%exp_dir)
  elseif ~isdir(%exp_dir) then
    messagebox(msprintf(_("""%s"" is not a valid directory\n"),%exp_dir),"error","modal")
    return
  end
  // Select output format
  %exp_format=choose_export_format();
  if isempty(%exp_format) then  // User cancelled
    return
  end
  // Export full diagram tree
  systexport(scs_m, %exp_dir, %exp_format)

  // Export a 'navigator' file showing full diagram tree
  ok=navigatorexport(scs_m, %exp_dir,%exp_format)
  
 
endfunction

