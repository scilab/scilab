// This file is released under the 3-clause BSD license. See COPYING-BSD.

path = get_absolute_file_path("cleaner_help.sce");
langdirs = dir(path);
langdirs = langdirs.name(langdirs.isdir);

masterfiles = ls(path + "/" + langdirs + "/master_help.xml");
for i = 1:size(masterfiles,'*')
  mdelete(masterfiles(i));
end

helpdirs = ls(path + "/" + langdirs + "/scilab_*_help");
for i = 1:size(helpdirs,'*')
  rmdir(helpdirs(i), 's');
end

jarhelpfiles = ls(path + "/../jar/scilab_*_help.jar");
for i = 1:size(jarhelpfiles,'*')
  mdelete(jarhelpfiles(i));
end

