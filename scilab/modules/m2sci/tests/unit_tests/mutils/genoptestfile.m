% Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
% Copyright (C) 2002-2004 - INRIA - Vincent COUVERT
% Copyright (C) 2010 - DIGITEO - Vincent COUVERT <vincent.couvert@scilab.org>
% 
 % Copyright (C) 2012 - 2016 - Scilab Enterprises
 %
 % This file is hereby licensed under the terms of the GNU GPL v2.0,
 % pursuant to article 5.3.4 of the CeCILL v.2.1.
 % This file was originally licensed under the terms of the CeCILL v2.1,
 % and continues to be available under such terms.
 % For more information, see the COPYING file which you should have received
 % along with this program.
%

function []=genoptestfile(opnames,datacell)

% INPUT ARGUMENTS DESCRIPTION :

% opnames = cell containing : 
%    - the name of the operator to test
%    - an other optional operator name
%      Scilab result (e.g. : abs...)

% datacell = cell which contain data used to test the operator
% datacell is composed of one cell containing :
%    - a cell in which we can find a variable number of cells containing the input arguments

global TESTS_SUITE_DIR

% Reading operator names
if size(opnames,1)==1
  opname=char(opnames);
  otherop='';
  closing=[];
elseif  size(opnames,1)==2
  opname=char(opnames(1));
  otherop=char(opnames(2));
else
  disp('Wrong number of operator names!');
  return;
end

if strcmp(opname,'+') OPNAME=upper('addition');
elseif strcmp(opname,'-') OPNAME=upper('subtraction');
elseif strcmp(opname,'*') OPNAME=upper('multiplication');
elseif strcmp(opname,'.*') OPNAME=upper('dot_multiplication');
elseif strcmp(opname,'/') OPNAME=upper('right_division');
elseif strcmp(opname,'./') OPNAME=upper('dot_right_division');
elseif strcmp(opname,'\') OPNAME=upper('left_division');
elseif strcmp(opname,'.\') OPNAME=upper('dot_left_division');
elseif strcmp(opname,'^') OPNAME=upper('power');
elseif strcmp(opname,'==') OPNAME=upper('equal');
elseif strcmp(opname,'<') OPNAME=upper('less');
elseif strcmp(opname,'>') OPNAME=upper('greater');
elseif strcmp(opname,'<=') OPNAME=upper('less_equal');
elseif strcmp(opname,'>=') OPNAME=upper('greater_equal');
elseif strcmp(opname,'~=') OPNAME=upper('not_equal');
elseif strcmp(opname,'''') OPNAME=upper('transpose');
elseif strcmp(opname,'|') OPNAME=upper('logical_or');
elseif strcmp(opname,'&') OPNAME=upper('logical_and');
elseif strcmp(opname,'~') OPNAME=upper('negation');
elseif strcmp(opname,':') OPNAME=upper('colon');
elseif strcmp(opname,'.^') & strcmp(otherop,'') OPNAME=upper('dot_power');
elseif strcmp(opname,'.^') OPNAME=upper('dot_and_digits');
elseif strcmp(opname,'.''') OPNAME=upper('dot_transpose');
else disp('Undefined operator !'),return;end

% Test file header writing (OPNAME.m)
disp(['Creating test file for ',OPNAME,' operator...']);
testfile = fopen([TESTS_SUITE_DIR,OPNAME,'.m'],'w');
fprintf(testfile,'%s\n',['% Test file for operator ',opname]);
fprintf(testfile,'%s\n\n',['% Matlab version: ',version ]);

testnb = 0;
% Beginning of general loop
for testindex = 1:size(datacell,2)
  data=datacell{testindex};
  combinate=char(data{1});
  argcell1=data{2}{1};
  argcell2=data{2}{2};
  if size(data{2},2) == 3
    argcell3=data{2}{3};
  end
  % Number of input arguments in the test 
  % (Can be one (for transposition) or 2)
  arginnb = size(data{2},2);
  argindex1=1;
  argindex2=1;
  
  % Beginning of loop following number of different value in argcell
  while (argindex2 <= size(argcell2,2))&(argindex1 <= size(argcell1,2))
    testnb = testnb + 1;
    
    % Creating command which will be translated
    teststr = ['res',mat2str(testnb),' = ',argcell1{argindex1},opname];
  
    if arginnb>=2
      teststr = [teststr,argcell2{argindex2}];
    end
    if (size(data{2},2) == 3) &  strcmp(otherop,'') % Used for : operator
      teststr = [teststr,opname];
    end
    if ~strcmp(otherop,'')
      teststr = [teststr,otherop];
    end
    if arginnb==3
      teststr = [teststr,argcell3{argindex2}];
    end
    
    teststr = [teststr,';'];
    fprintf(testfile,'%s\n%s\n',['% TEST ',mat2str(testnb)],teststr);

    % Ajusting indexes for next loop execution
    if strcmp(combinate,'yes')
      if size(argcell1,2)>=size(argcell2,2)
	if argindex1==size(argcell1,2)
	  argindex2 = argindex2 + 1;
	  argindex1 = 1;
	else
	  argindex1 = argindex1 + 1;
	end
      else
	if argindex2==size(argcell2,2)
	  argindex1 = argindex1 + 1;
	  argindex2 = 1;
	else
	  argindex2 = argindex2 + 1;
	end
      end
    else
 	argindex1 = argindex1 + 1;
	argindex2 = argindex2 + 1;
    end
  end  % End of while loop
end % End of for loop

% Closing test file
fclose(testfile);
