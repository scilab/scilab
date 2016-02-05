% Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
% Copyright (C) DIGITEO - 2009 - Vincent COUVERT
% Copyright (C) INRIA - 2002-2004 - Vincent COUVERT
% 
 % Copyright (C) 2012 - 2016 - Scilab Enterprises
 %
 % This file is hereby licensed under the terms of the GNU GPL v2.0,
 % pursuant to article 5.3.4 of the CeCILL v.2.1.
 % This file was originally licensed under the terms of the CeCILL v2.1,
 % and continues to be available under such terms.
 % For more information, see the COPYING file which you should have received
 % along with this program.

% Batch file which generate test files for M2SCI

% Last Matlab version used to generate the test files: 7.6.0.324 (R2008a)

definetestcells;

global TESTS_SUITE_DIR;
TESTS_SUITE_DIR='../mfiles/';

disp('Generating functions test files...');

% PROTO0
genfunctestfile('abs',{{1,{testcell}}});
genfunctestfile('cos',{{1,{numerictestcell}}});
genfunctestfile('cosh',{{1,{numerictestcell}}});
genfunctestfile('double',{{1,{testcell}}});
genfunctestfile('erf',{{1,{numericrealtestcell}}});
genfunctestfile('erfc',{{1,{numericrealtestcell}}});
genfunctestfile('erfcx',{{1,{numericrealtestcell}}});
genfunctestfile('gamma',{{1,{numericrealtestcell}}});
genfunctestfile('gammaln',{{1,{numericrealtestcell}}});
genfunctestfile('imag',{{1,{testcell}}});
genfunctestfile('real',{{1,{testcell}}});

% PROTO1
genfunctestfile('max',{{1,{testcell}},{2,{testcell}},...
		    {1,{testcell,testcell1}},{1,{testcell,{'[],1','[],2','[],3'}}},{2,{testcell,{'[],1','[],2','[],3'}}}});
genfunctestfile('min',{{1,{testcell}},{2,{testcell}},...
		    {1,{testcell,testcell1}},{1,{testcell,{'[],1','[],2','[],3'}}},{2,{testcell,{'[],1','[],2','[],3'}}}});

% PROTO2
genfunctestfile('cumprod',{{1,{numerictestcell,{'','1','2','3'}}}});
genfunctestfile('cumsum',{{1,{testcellnostring,{'','1','2','3'}}}});

% PROTO3
genfunctestfile('eye',{{1,{{'2','3','4','[10,10]','size([1 2])','m2sciUnknownDims([1,2])'},{'','5','4','','',''}}}});
genfunctestfile('false',{{1,{{'2','3','4','[10,10]','size([1 2])','m2sciUnknownDims([1,2])'},{'','5','4','','',''}}}});
genfunctestfile('ones',{{1,{{'2','3','4','[10,10]','size([1 2])','m2sciUnknownDims([1,2])'},{'','5','4','','',''}}}});
genfunctestfile({'rand';'size'},{{1,{{'2','3','4','[10,10]','size([1 2])','m2sciUnknownDims([1,2])'},{'','5','4','','',''}}}});
genfunctestfile({'randn';'size'},{{1,{{'2','3','4','[10,10]','size([1 2])','m2sciUnknownDims([1,2])'},{'','5','4','','',''}}}});
genfunctestfile('true',{{1,{{'2','3','4','[10,10]','size([1 2])','m2sciUnknownDims([1,2])'},{'','5','4','','',''}}}});
genfunctestfile('zeros',{{1,{{'2','3','4','[10,10]','size([1 2])','m2sciUnknownDims([1,2])'},{'','5','4','','',''}}}});

% PROTO4
genfunctestfile('acos',{{1,{numerictestcell}}});
genfunctestfile('acosh',{{1,{numerictestcell}}});
genfunctestfile('asinh',{{1,{numerictestcell}}});
genfunctestfile('atan',{{1,{numerictestcell}}});
genfunctestfile('conj',{{1,{testcellnoboolean}}});
genfunctestfile('cot',{{1,{numerictestcell}}});
genfunctestfile('coth',{{1,{numerictestcell}}});
genfunctestfile('exp',{{1,{numerictestcell}}});
genfunctestfile('log',{{1,{numerictestcell}}});
genfunctestfile('sign',{{1,{testcellfloatnoboolean}}});
genfunctestfile('sin',{{1,{numerictestcell}}});
genfunctestfile('sinh',{{1,{numerictestcell}}});
genfunctestfile('sqrt',{{1,{numerictestcell}}});
genfunctestfile('tanh',{{1,{numerictestcell}}});
genfunctestfile('tan',{{1,{numerictestcell}}});

% PROTO5
genfunctestfile('asin',{{1,{numerictestcell}}});
genfunctestfile('atanh',{{1,{numerictestcell}}});

% PROTO6
genfunctestfile('ceil',{{1,{testcellfloatnoboolean}}});
genfunctestfile('fix',{{1,{testcellfloatnoboolean}}});
genfunctestfile('floor',{{1,{testcellfloatnoboolean}}});
genfunctestfile('round',{{1,{testcellfloatnoboolean}}});

% PROTO7
genfunctestfile('tril',{{1,{testcell,{'','1','2','10','''a'''}}}});
genfunctestfile('triu',{{1,{testcell,{'','1','2','10','''a'''}}}});

% PROTO8
genfunctestfile('prod',{{1,{numerictestcell,{'','1','2','3'}}}});
genfunctestfile('sum',{{1,{testcell,{'','1','2','3'}}}});

% PROTO9
genfunctestfile({'int8';'double'},{{1,{testcell}},{1,{{'inf','-inf','nan'}}}});
genfunctestfile({'int16';'double'},{{1,{testcell}},{1,{{'inf','-inf','nan'}}}});
genfunctestfile({'int32';'double'},{{1,{testcell}},{1,{{'inf','-inf','nan'}}}});
genfunctestfile({'uint8';'double'},{{1,{testcell}},{1,{{'inf','-inf','nan'}}}});
genfunctestfile({'uint16';'double'},{{1,{testcell}},{1,{{'inf','-inf','nan'}}}});
genfunctestfile({'uint32';'double'},{{1,{testcell}},{1,{{'inf','-inf','nan'}}}});

% PROTO10
genfunctestfile('lower',{{1,{testcellnoboolean}}});
genfunctestfile('upper',{{1,{testcellnoboolean}}});

% Other functions
genfunctestfile('atan2',{{1,{numerictestcell,numerictestcell1}}});
genfunctestfile('acot',{{1,{testcell}}});
genfunctestfile('acoth',{{1,{testcell}}});
genfunctestfile('acsc',{{1,{testcell}}});
genfunctestfile('acsch',{{1,{testcell}}});
genfunctestfile('angle',{{1,{testcell}}});
genfunctestfile('asec',{{1,{testcell}}});
genfunctestfile('asech',{{1,{testcell}}});
genfunctestfile('beta',{{1,{numericrealtestcell,numericrealtestcell1}},{1,{numericrealtestcell,{1}}},{1,{{1},numericrealtestcell1}}});
genfunctestfile('conv',{{1,{numerictestcellvectnoempty,numerictestcellvectnoempty}}});
genfunctestfile('csc',{{1,{numerictestcell}}});
genfunctestfile('csch',{{1,{numerictestcell}}});
% Bug of Matlab dec2bin with matrices!!
genfunctestfile('dec2bin',{{1,{testcellvectnocomplex}},{1,{testcellvectnocomplex,{'10'}}}});
genfunctestfile('dec2hex',{{1,{testcellnocomplex}},{1,{testcellnocomplex,{'10'}}}});
genfunctestfile('diag',{{1,{testcell,{'','1','2'}}}});
genfunctestfile('diff',{{1,{testcellnostring,{'','2','3'}}},{1,{testcellnostring,{'1,1','2,1','1,2','2,2'}}}});
genfunctestfile('find',{{1,{testcell}},{2,{testcell}},{3,{testcell}}});
tmptestcell={'[''str1'']','[''str1 str2'']','[''str1'',''str8'']','[''123'']'};
tmptestcell1={'''s''','''str14''','[''8'']','''12''','[''str2'',''str1'']'};
genfunctestfile('findstr',{{1,{tmptestcell,tmptestcell1}}});
genfunctestfile('fft',{{1,{testcellnostring,{'','1','5','[],1','[],2','1,1','1,2','5,1','5,2'}}}});
genfunctestfile('fftshift',{{1,{testcell,{'','1','5'}}}});
genfunctestfile('fliplr',{{1,{testcell}}});
genfunctestfile('flipud',{{1,{testcell}}});
genfunctestfile('hankel',{{1,{numerictestcell1}},{1,{numerictestcellnotempty,numerictestcellnotempty}}});
genfunctestfile('ifft',{{1,{testcellnostring,{'','1','5','[],1','[],2','1,1','5,1'}}}});
genfunctestfile('ischar',{{1,{testcell}}});
genfunctestfile('isempty',{{1,{testcell}}});
genfunctestfile('isfinite',{{1,{testcell}},{1,{{'inf','-inf','nan','-nan'}}}})
genfunctestfile('isinf',{{1,{testcell}},{1,{{'inf','nan','[inf,nan,1;nan,inf,-inf]'}}}});
genfunctestfile('isletter',{{1,{testcellnoboolean}}});
genfunctestfile('islogical',{{1,{testcell}}});
genfunctestfile('isnan',{{1,{testcell}},{1,{{'inf','nan','[inf,nan,1;nan,inf,-inf]'}}}});
genfunctestfile('isreal',{{1,{testcell}}});
genfunctestfile('isscalar',{{1,{testcell}}});
genfunctestfile('isspace',{{1,{testcellnoboolean}},{1,{{''' ''','''  str st t r   '''}}}});
genfunctestfile('isstr',{{1,{testcell}}});
genfunctestfile('isvector',{{1,{testcell}}});
genfunctestfile('kron',{{1,{testcell,testcell}}});
genfunctestfile('length',{{1,{testcell}}});
tmptestcell1={'1','1+i','1','''a''','1','1','m2sciUnknownType(''a'')','1','1'};
tmptestcell2={'10','10+10i','''a''','''z''','1000','m2sciUnknownType(''a'')','m2sciUnknownType(''z'')','1000','1000'};
tmptestcell3={'','','1','10','''a''','1','10','m2sciUnknownType(''a'')','[]'};
genfunctestfile('linspace',{{1,{tmptestcell1,tmptestcell2,tmptestcell3}}});
tmptestcell=[emptymatcell,intmatcell,booleanmatcell];
genfunctestfile('logical',{{1,{tmptestcell}}});
genfunctestfile('log2',{{1,{numerictestcell}},{2,{numerictestcell}}});
genfunctestfile('log10',{{1,{numerictestcell}}});
genfunctestfile('logm',{{1,{numerictestcellsq}}});
tmptestcell=[emptymatcell,intmatcell,stringmatcell];
genfunctestfile('mod',{{1,{tmptestcell,tmptestcell}}});
genfunctestfile('ndims',{{1,{testcell}}});
genfunctestfile('pow2',{{1,{numerictestcell,{''}}},{1,{numerictestcell,numerictestcell1}}});
tmptestcell=[emptymatcell,intmatcell,stringmatcell,{0}];
genfunctestfile('rem',{{1,{tmptestcell,tmptestcell}}});
tmptestcell={'2','[2,2]','2,3'};
genfunctestfile('repmat',{{1,{testcell,tmptestcell}}});
genfunctestfile('reshape',{{1,{{'''abc''','''abc''','[1,2,3;4,5,6]','[1,2,3]==[1,0,3]'},{'3,[]','3,1','[1,6]','1,3'}}}});
tmptestcell=[emptymatcell,intmatcell,stringmatcell];
genfunctestfile('setstr',{{1,{tmptestcell}}});
genfunctestfile('size',{{1,{testcell,{'','1','2','3'}}},{2,{testcell}}});
genfunctestfile('sort',{{1,{testcellnoboolean,{'','1','2'}}},{2,{testcellnoboolean,{'','1','2'}}}});
tmptestcell={'[''str1'']','[''str1 str2'']','[''str1'',''str8'']','[''123'']','[''idem'']','0','1'};
tmptestcell1={'''s''','''str14''','[''8'']','''12''','[''str2'',''str1'']','[''idem'']','0','0'};
genfunctestfile('strcmp',{{1,{tmptestcell,tmptestcell1}}});
genfunctestfile('strcmpi',{{1,{tmptestcell,tmptestcell1}}});
tmptestcell={'[''str1'']','[''str1 str2'']','[''str1'',''str8'']'};
tmptestcell1={'''s''','''str14''','[''8'']'};
genfunctestfile('strfind',{{1,{tmptestcell,tmptestcell1}}});
tmptestcell={'''str1''','''str1''','''str1''','''[1,2,3]''','''[1,2,3;4,5,6]'''};
tmptestcell1={'''s''','''st''','''s''','''1''','''2'''};
tmptestcell2={'''a''','''b''','''cd''','''0''','''10'''};
genfunctestfile('strrep',{{1,{tmptestcell,tmptestcell1,tmptestcell2}}});
tmptestcell1=[intmatcell1,complexmatcell1];
genfunctestfile('toeplitz',{{1,{numerictestcellnotempty}},{1,{numerictestcellnotempty,tmptestcell1}}});

% matfun
genfunctestfile('balance',{{1,{numerictestcellsq}},{2,{numerictestcellsq}}});
tmptestcell=[emptymatcell,{'1','eye(3,3)'}];
genfunctestfile('chol',{{1,{tmptestcell}}}); % Should be tested for 2 lhs
genfunctestfile('det',{{1,{numerictestcellsq}}});
genfunctestfile('eig',{{1,{numerictestcellsq}},{2,{numerictestcellsq}},{1,{[numerictestcellsq,{'1'}],[numerictestcellsq,{'''nobalance'''}]}}});
genfunctestfile('expm',{{1,{numerictestcellsq}}});
genfunctestfile('hess',{{1,{numerictestcellsq}},{2,{numerictestcellsq}}});
genfunctestfile('inv',{{1,{numerictestcellsq}}});
genfunctestfile('lu',{{2,{numerictestcellsq}},{3,{numerictestcellsq}}});
genfunctestfile('norm',{{1,{numerictestcellsq,{'','1','2','''fro''','inf'}}},{1,{numerictestcellvectnoempty,{'','1','2','''fro''','inf','-inf'}}}});
genfunctestfile('qr',{{2,{numerictestcell}},{3,{numerictestcell}},{2,{numerictestcell,{'0'}}},{3,{numerictestcell,{'0'}}}});
genfunctestfile('rcond',{{1,{numerictestcellsq}}});
genfunctestfile('schur',{{1,{numerictestcellsq}}});
genfunctestfile('svd',{{1,{numerictestcellsq,{'0'}}},{2,{numerictestcellsq,{'0'}}},{3,{numerictestcellsq,{'0'}}}});

% ops
genfunctestfile('all',{{1,{testcell,{'','1','2','3'}}}});
genfunctestfile('any',{{1,{testcell,{'','1','2','3'}}}});

% sparfun
genfunctestfile('full',{{1,{[testcell,{'sparse([0,1,2])'}]}}});
genfunctestfile('issparse',{{1,{[testcell,{'sparse([0,1,2])'}]}}});
tmptestcell={'1','5',''' ''','1==1',''' '''};
tmptestcell1={'1','5',''' ''','1==1','1==1'};
tmptestcell2={'1','5',''' ''','1==1',''' ''','[1,2,3]'};
tmptestcell3={'1','5',''' ''','1==1','1==1','[3,2,1]'};
tmptestcell4={'1','5',''' ''','1==1','1==1','[4,4,4]'};
genfunctestfile('sparse',{{1,{testcell}},{1,{tmptestcell,tmptestcell1}},{1,{tmptestcell2,tmptestcell3,tmptestcell4}},{1,{tmptestcell,tmptestcell1,tmptestcell,tmptestcell,tmptestcell1}}});

