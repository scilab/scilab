% Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
% Copyright (C) 2002-2004 - INRIA - Vincent COUVERT
% Copyright (C) 2009-2010 - DIGITEO - Vincent COUVERT <vincent.couvert@scilab.org>
% 
% This file must be used under the terms of the CeCILL.
% This source file is licensed as described in the file COPYING, which
% you should have received as part of this distribution.  The terms
% are also available at    
% http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
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
  