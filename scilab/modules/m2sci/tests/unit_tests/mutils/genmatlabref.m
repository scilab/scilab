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

% Global test variables
TESTS_SUITE_DIR='../mfiles/';
addpath(TESTS_SUITE_DIR);

% List of all test files
test_files=dir([TESTS_SUITE_DIR,'*.m']);

% Just one function
%test_files=dir([TESTS_SUITE_DIR, 'DEC2BIN.m']);;

% Create reference files
for file_ind=1:size(test_files,1)
  
  testfilename=test_files(file_ind).name;
  if strfind(testfilename,'_ref.m') 
    continue
  end
  
  % ref file
  point=strfind(testfilename,'.');
  
  % Evaluate the file
  run(testfilename(1:point-1))
  
  allRes = whos('res*');
  
  for varIndex = 1:size(allRes,1)
      varSize=mat2str(allRes(varIndex).size);
      varSize=varSize(2:end-1);
      eval([allRes(varIndex).name,'_Infos=''',varSize,'|',type_infos(allRes(varIndex)),''';']);
  end
  
  save([TESTS_SUITE_DIR, testfilename(1:point-1), '.mat'], '-v6', 'res*')
  
  clear res*

end

clear TESTS_SUITE_DIR allRes file_ind point test_files testfilename varIndex varSize
  