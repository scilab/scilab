function dir(varargin)

[lhs,rhs]=argn(0);

if ~rhs
   path='.';
else
  path=varargin(1);
end

printf(' \n\n');
unix_w(sprintf('ls -C %s',path));

function cd(varargin)

[lhs,rhs]=argn(0);

if ~rhs
   chdir('.');
else
  chdir(varargin(1));
end

