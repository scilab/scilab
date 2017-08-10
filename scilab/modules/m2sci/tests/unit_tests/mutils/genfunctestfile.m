% Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
% Copyright (C) 2002-2004 - INRIA - Vincent COUVERT
% Copyright (C) 2009-2010 - DIGITEO - Vincent COUVERT <vincent.couvert@scilab.org>
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

function []=genfunctestfile(funnames,datacell)

% INPUT ARGUMENTS DESCRIPTION :

% funnames = cell containing : 
%    - the name of the function to test
%    - an other optional function name which is used to modify
%      Scilab result (e.g. : abs...)

% datacell = cell which contain data used to test the function
% datacell is composed of n cells each containing :
%    - the number of output argument
%    - a  cell in which we can find a variable number of cells containing the input arguments
%      (e.g. : 2 cells for 2 input arguments)

global TESTS_SUITE_DIR;

% Reading functions names
if size(funnames,1)==1
  funname=char(funnames);
else
  funname=char(funnames(1));
end
FUNNAME=upper(funname);

% Test file header writing (FUNNAME.m)
disp(['Creating test file for ',funname,'() function...']);
testfile = fopen([TESTS_SUITE_DIR,FUNNAME,'.m'],'w');
fprintf(testfile,'%s\n',['% Test file for function ',funname,'()']);
fprintf(testfile,'%s\n\n',['% Matlab version: ',version ]);

resind = 1;
testnb = 0;
% Beginning of general loop
for testindex = 1:size(datacell,2)
  % If datacell{k} contains 2 cells (fonction called with 2 input arguments)
  %     - if they have the same size tests are done for
  %       argcell1{k} and argcell2{k} 1 <= k <= size(argcell1,2)
  %     - else tests are done for all combinations of argcell1 and
  %       argcell2 components
  % Else tests are done with argcell1{k}, argcell2{k},... argcelln{k}
  % (assuming they have all same dimensions)

  % Number of output arguments in the test
  argoutnb = datacell{testindex}{1};
  
  % Number of input arguments in the test
  argcell=datacell{testindex}{2};
  arginnb = size(argcell,2);
  
  % This matrix will tell us which argcell{arginindex} value will be use
  argindex=ones(1,arginnb);
  
  % Poosibility of testing with combinations of argcell
  combinate = 'no';
  % When 2 input arguments
  if arginnb==2
    argcell1size = size(argcell{1},2);
    argcell2size = size(argcell{2},2);
    if argcell1size ~= argcell2size
      combinate = 'yes';
    end
  end
  
  % Beginning of loop following number of different value in argcell
  while argindex(1) <= size(argcell{1},2)
    testnb = testnb + 1;
    
    % Creating command which will be translated
    if argoutnb>1
      teststr = ['['];
    else
       teststr = [''];
    end     
    toadd='';
    for k = 1:argoutnb
      %teststr = [teststr,'res',mat2str(k+(testnb-1)*argoutnb)];
      teststr = [teststr,'res',mat2str(resind)];
      resind=resind+1;
      if size(funnames,1)>=k+1
	if ~strcmp(funnames(k+1),'')
	  toadd=[toadd,'res',mat2str(k+(testnb-1)*argoutnb),' = ',char(funnames(k+1)),'(res',mat2str(k+(testnb-1)*argoutnb),');'];
	end
      end
      if k ~= argoutnb
	teststr = [teststr,','];
      end
    end
    if argoutnb>1
      teststr = [teststr,']'];
    end
    teststr = [teststr,' = ',funname,'('];
  
    for arginindex = 1:arginnb
      if ~strcmp(char(argcell{arginindex}{argindex(arginindex)}),'')
	if arginindex ~= 1
	  teststr = [teststr,','];
	end
	if ischar(argcell{arginindex}{argindex(arginindex)})
          teststr = [teststr, ...
                     argcell{arginindex}{argindex(arginindex)}];
        else
          teststr = [teststr, ...
                     mat2str(argcell{arginindex}{argindex(arginindex)})];
         end
       else
	break
      end
    end
    teststr = [teststr,');',toadd];
    fprintf(testfile,'%s\n%s\n',['% TEST ',mat2str(testnb)],teststr);

    % Ajusting indexes for next loop execution
    if strcmp(combinate,'no')
      argindex = argindex + 1;
    else
      argindex = argindex + 1;
      argindex(2) = argindex(2) - 1;
      if argindex(1) == (size(argcell{1},2)+1)
	argindex(2) = argindex(2) + 1;	
	argindex(1) = 1;
      end
      if argindex(2) == (size(argcell{2},2)+1)
	break
      end
    end
    
  end  % End of while loop
end % End of for loop

% Closing test file
fclose(testfile);

