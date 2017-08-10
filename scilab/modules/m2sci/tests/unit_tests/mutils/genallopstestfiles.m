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

% Batch file which generate test files for M2SCI

% Last Matlab version used to generate the test files: 7.6.0.324 (R2008a)

definetestcells;

global TESTS_SUITE_DIR;
TESTS_SUITE_DIR='../mfiles/';

disp('Generating operation test files...');

genoptestfile('+',{{'no',{testcell,testcell}},...
                     {'yes',{testcell,{'1','''a'''}}},...
                     {'yes',{{'[]','inf','nan','1'},{'[]','inf','nan','1'}}}});

genoptestfile('-',{{'no',{testcell,testcell}},...
                     {'yes',{testcell,{'1','''a'''}}},...
                     {'yes',{{'[]','inf','nan','1'},{'[]','inf','nan','1'}}}});

genoptestfile('''',{{'yes',{testcell,{''}}}})

genoptestfile('.''',{{'yes',{testcell,{''}}}})

genoptestfile({'*';''''},{{'no',{testcellnoboolean,testcell1noboolean}},...
                    {'yes',{testcellnoboolean,{'1'}}},...
                    {'yes',{{'[]','inf','nan','1'},{'[]','inf','nan','1'}}}})

genoptestfile('.*',{{'no',{testcell,testcell}},...
		    {'yes',{{'[]','inf','nan','1'},{'[]','inf','nan','1'}}}})

genoptestfile('/',{{'no',{testcellnoboolean,testcell1noboolean}},...
                     {'yes',{testcellnoboolean,{'1','''a'''}}},...
                     {'yes',{{'inf','nan','1'},{'inf','nan','1'}}}});

genoptestfile('./',{{'no',{testcellnoboolean,testcell1noboolean}},...
		    {'yes',{testcellnoboolean,{'1','''a'''}}},...
		    {'yes',{{'[]','inf','nan','1'},{'[]','inf','nan','1'}}}})

genoptestfile('\',{{'no',{testcellnoboolean,testcell1noboolean}},...
		   {'yes',{{'inf','nan','1'},{'inf','nan','1'}}}});

genoptestfile('.\',{{'no',{testcellnoboolean,testcell1noboolean}},...
		    {'yes',{testcellnoboolean,{'1','''a'''}}},...
		    {'yes',{{'[]','inf','nan','1'},{'[]','inf','nan','1'}}}})

genoptestfile('^',{{'yes',{testcellsq,{'''a''','1','2','-1','-2','inf','nan'}}},...
                   {'yes',{{'1','2','inf','nan'},testcellsq}},...
		   {'yes',{{'''a''','1','2','-1','-2','inf','nan'},{'''a''','1','2','-1','-2','inf','nan'}}}})
		   
genoptestfile('.^',{{'yes',{testcellsq,{'''a''','1','2','-1','-2','inf','nan'}}},...
		    {'yes',{{'1','''a''','2','inf','nan'},testcellsq}},...
		    {'yes',{{'''a''','1','2','-1','-2','inf','nan'},{'''a''','1','2','-1','-2','inf','nan'}}}});

tmptestcell1 = [emptymatcell,intmatcell,stringmatcell,booleanmatcell];
tmptestcell2 = [emptymatcell,intmatcell1,stringmatcell1,booleanmatcell1];
tmptestcell={'[1==1]','m2sciUnknownDims([1==1])','m2sciUnknownType([1==1])',...
	     '1','m2sciUnknownDims(1)','m2sciUnknownType(1)',...
	     '''a''','m2sciUnknownDims(''a'')','m2sciUnknownType(''a'')'};
genoptestfile('&',{{'no',{tmptestcell1,tmptestcell2}},...
		   {'yes',{tmptestcell,tmptestcell1}},...
		   {'yes',{tmptestcell1,tmptestcell}}});

genoptestfile('|',{{'no',{tmptestcell1,tmptestcell2}},...
		   {'yes',{tmptestcell,tmptestcell1}},...
		   {'yes',{tmptestcell1,tmptestcell}}});

genoptestfile('~',{{'yes',{{''},tmptestcell1}}});

tmptestcell={'[]','nan','m2sciUnknownDims(1)','10','100'};
genoptestfile(':',{{'no',{{'1','''a''','-1'},{'100','''j''','-100'}}},...
		   {'no',{{'1','''f''','-1'},{'1','-1','-10'},{'100','''b''','-100'}}},...
		   {'no',{tmptestcell,tmptestcell}},...
		   {'no',{tmptestcell,tmptestcell,tmptestcell}}});

genoptestfile('==',{{'no',{testcell,testcell1}},{'yes',{testcell,{'1','-1','''a'''}}}});
genoptestfile('<',{{'no',{testcell,testcell1}},{'yes',{testcell,{'1','-1','''a'''}}}});
genoptestfile('>',{{'no',{testcell,testcell1}},{'yes',{testcell,{'1','-1','''a'''}}}});
genoptestfile('<=',{{'no',{testcell,testcell1}},{'yes',{testcell,{'1','-1','''a'''}}}});
genoptestfile('>=',{{'no',{testcell,testcell1}},{'yes',{testcell,{'1','-1','''a'''}}}});
genoptestfile('~=',{{'no',{testcell,testcell1}},{'yes',{testcell,{'1','-1','''a'''}}}});
 
% Special test to see if m2sci_syntax does a good work for digits
% followed by a dot operator
genoptestfile({'.^';'.*'},{{'no',{testcellnoboolean,testcellnoboolean,testcellnoboolean}}})
