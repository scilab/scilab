// This file is released under the 3-clause BSD license. See COPYING-BSD.
//=================================
// load toolbox_skeleton
if funptr('c_sum') == 0 then
  root_tlbx_path = SCI+'\contrib\toolbox_skeleton\';
  exec(root_tlbx_path + 'loader.sce',-1); 
end
//=================================
if c_sum(3,5) <> 8 then pause,end
//=================================
