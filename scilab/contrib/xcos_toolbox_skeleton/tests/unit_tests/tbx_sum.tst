// This file is released under the 3-clause BSD license. See COPYING-BSD.
//=================================
// load xcos toolbox_skeleton
if funptr('tbx_sum') == 0 then
  root_tlbx_path = SCI+'\contrib\xcos_toolbox_skeleton\';
  exec(root_tlbx_path + 'loader.sce',-1); 
end

if tbx_sum(3,5) <> 8 then pause,end

