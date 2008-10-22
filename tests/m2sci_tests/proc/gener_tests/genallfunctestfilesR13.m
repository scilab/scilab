% Copyright INRIA
% Batch file which generate test files for M2SCI
% V.C.

definetestcells;

global TESTS_SUITE_DIR;
TESTS_SUITE_DIR='../../generated_files/';

disp('Generating functions test files...');

testcellnoboolean=[emptymatcell,intmatcell,complexmatcell,stringmatcell];
testcell1noboolean=[emptymatcell,intmatcell1,complexmatcell1,stringmatcell1];
testcellsqnoboolean=[emptymatcell,intsqmatcell,complexsqmatcell,stringsqmatcell];
testcellvectnoboolean=[emptymatcell,intvectcell,complexvectcell,stringvectcell];		
testcellfloatnoboolean=[emptymatcell,intmatcell,complexmatcell,stringmatcell,floatmatcell];

% PROTO0
genfunctestfile('abs',{{1,{testcell}}});
tmptestcell=[emptymatcell,intmatcell,complexmatcell];
genfunctestfile('cos',{{1,{tmptestcell}}});
genfunctestfile('cosh',{{1,{tmptestcell}}});
genfunctestfile('double',{{1,{testcell}}});
tmptestcell = [emptymatcell,intmatcell]; 
genfunctestfile('erf',{{1,{tmptestcell}}});
tmptestcell = [emptymatcell,intmatcell]; 
genfunctestfile('erfc',{{1,{tmptestcell}}});
tmptestcell = [emptymatcell,intmatcell]; 
genfunctestfile('erfcx',{{1,{tmptestcell}}});
tmptestcell = [emptymatcell,intmatcell]; % Other cells removed because too big values
genfunctestfile('gamma',{{1,{tmptestcell}}});
genfunctestfile('gammaln',{{1,{tmptestcell}}});
genfunctestfile('imag',{{1,{testcell}}});
genfunctestfile('real',{{1,{testcell}}});

% PROTO1
genfunctestfile('max',{{1,{testcell}},{2,{testcell}},...
		    {1,{testcell,testcell1}},{1,{testcell,{'[],1','[],2','[],3'}}},{2,{testcell,{'[],1','[],2','[],3'}}}});
genfunctestfile('min',{{1,{testcell}},{2,{testcell}},...
		    {1,{testcell,testcell1}},{1,{testcell,{'[],1','[],2','[],3'}}},{2,{testcell,{'[],1','[],2','[],3'}}}});

% PROTO2
tmptestcell=[emptymatcell,intmatcell,complexmatcell];
genfunctestfile('cumprod',{{1,{tmptestcell,{'','1','2','3'}}}});
tmptestcell=[emptymatcell,intmatcell,complexmatcell,booleanmatcell];
genfunctestfile('cumsum',{{1,{tmptestcell,{'','1','2','3'}}}});

% PROTO3
genfunctestfile('eye',{{1,{{'2','3','4','[10,10]','size([1 2])','1+i','Unknown_Dims([1,2])'},{'','5','4','','','',''}}}});
genfunctestfile('false',{{1,{{'2','3','4','[10,10]','size([1 2])','1+i','Unknown_Dims([1,2])'},{'','5','4','','','',''}}}});
genfunctestfile('ones',{{1,{{'2','3','4','[10,10]','size([1 2])','1+i','Unknown_Dims([1,2])'},{'','5','4','','','',''}}}});
genfunctestfile({'rand';'size'},{{1,{{'2','3','4','[10,10]','size([1 2])','1+i','Unknown_Dims([1,2])'},{'','5','4','','','',''}}}});
genfunctestfile({'randn';'size'},{{1,{{'2','3','4','[10,10]','size([1 2])','1+i','Unknown_Dims([1,2])'},{'','5','4','','','',''}}}});
genfunctestfile('true',{{1,{{'2','3','4','[10,10]','size([1 2])','1+i','Unknown_Dims([1,2])'},{'','5','4','','','',''}}}});
genfunctestfile('zeros',{{1,{{'2','3','4','[10,10]','size([1 2])','1+i','Unknown_Dims([1,2])'},{'','5','4','','','',''}}}});

% PROTO4
tmptestcell=[emptymatcell,intmatcell,complexmatcell];
genfunctestfile('acos',{{1,{tmptestcell}}});
genfunctestfile('acosh',{{1,{tmptestcell}}});
genfunctestfile('asinh',{{1,{tmptestcell}}});
genfunctestfile('atan',{{1,{tmptestcell}}});
genfunctestfile('conj',{{1,{testcellnoboolean}}});
genfunctestfile('cot',{{1,{tmptestcell}}});
genfunctestfile('coth',{{1,{tmptestcell}}});
tmptestcell=[emptymatcell,intmatcell,complexmatcell];
genfunctestfile('exp',{{1,{tmptestcell}}});
tmptestcell=[emptymatcell,intmatcell,complexmatcell];
genfunctestfile('log',{{1,{tmptestcell}}});
genfunctestfile('sign',{{1,{testcellfloatnoboolean}}});
tmptestcell=[emptymatcell,intmatcell,complexmatcell];
genfunctestfile('sin',{{1,{tmptestcell}}});
tmptestcell=[emptymatcell,intmatcell,complexmatcell];
genfunctestfile('sinh',{{1,{tmptestcell}}});
tmptestcell=[emptymatcell,intmatcell,complexmatcell];
genfunctestfile('sqrt',{{1,{tmptestcell}}});
tmptestcell=[emptymatcell,intmatcell,complexmatcell];
genfunctestfile('tanh',{{1,{tmptestcell}}});
tmptestcell=[emptymatcell,intmatcell,complexmatcell];
genfunctestfile('tan',{{1,{tmptestcell}}});

% PROTO5
tmptestcell=[emptymatcell,intmatcell,complexmatcell];
genfunctestfile('asin',{{1,{tmptestcell}}});
genfunctestfile('atanh',{{1,{tmptestcell}}});

% PROTO6
genfunctestfile('ceil',{{1,{testcellfloatnoboolean}}});
genfunctestfile('fix',{{1,{testcellfloatnoboolean}}});
genfunctestfile('floor',{{1,{testcellfloatnoboolean}}});
genfunctestfile('round',{{1,{testcellfloatnoboolean}}});

% PROTO7
genfunctestfile('tril',{{1,{testcell,{'','1','2','10','''a''','1+i'}}}});
genfunctestfile('triu',{{1,{testcell,{'','1','2','10','''a''','1+i'}}}});

% PROTO8
tmptestcell=[emptymatcell,intmatcell,complexmatcell];
genfunctestfile('prod',{{1,{tmptestcell,{'','1','2','3'}}}});
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
tmptestcell=[emptymatcell,intmatcell,complexmatcell];
tmptestcell1=[emptymatcell,intmatcell1,complexmatcell1];
genfunctestfile('atan2',{{1,{tmptestcell,tmptestcell1}}});
genfunctestfile('acot',{{1,{testcell}}});
genfunctestfile('acoth',{{1,{testcell}}});
genfunctestfile('acsc',{{1,{testcell}}});
genfunctestfile('acsch',{{1,{testcell}}});
genfunctestfile('angle',{{1,{testcell}}});
genfunctestfile('asec',{{1,{testcell}}});
genfunctestfile('asech',{{1,{testcell}}});
tmptestcell = [emptymatcell,intmatcell];
tmptestcell1 = [emptymatcell,intmatcell1];
genfunctestfile('beta',{{1,{tmptestcell,tmptestcell1}},{1,{tmptestcell,{1}}},{1,{{1},tmptestcell1}}});
tmptestcellvect=[intvectcell,complexvectcell,booleanvectcell];		
genfunctestfile('conv',{{1,{tmptestcellvect,tmptestcellvect}}});
tmptestcell=[emptymatcell,intmatcell,complexmatcell];
genfunctestfile('csc',{{1,{tmptestcell}}});
tmptestcell=[emptymatcell,intmatcell,complexmatcell];
genfunctestfile('csch',{{1,{tmptestcell}}});
tmptestcell=[emptymatcell,intvectcell,stringvectcell,booleanvectcell];		
genfunctestfile('dec2hex',{{1,{tmptestcell}},{1,{tmptestcell,{'10'}}}});
genfunctestfile('diag',{{1,{testcell,{'','1','2'}}}});
tmptestcell=[emptymatcell,intmatcell,complexmatcell,booleanmatcell];
genfunctestfile('diff',{{1,{tmptestcell,{'','2','3'}}},{1,{tmptestcell,{'1,1','2,1','1,2','2,2'}}}});
genfunctestfile('find',{{1,{testcell}},{2,{testcell}},{3,{testcell}}});
tmptestcell={'[''str1'']','[''str1 str2'']','[''str1'',''str8'']','[''123'']'};
tmptestcell1={'''s''','''str14''','[''8'']','''12''','[''str2'',''str1'']'};
genfunctestfile('findstr',{{1,{tmptestcell,tmptestcell1}}});
tmptestcell=[emptymatcell,intmatcell,complexmatcell,booleanmatcell];
genfunctestfile('fft',{{1,{tmptestcell,{'','1','5','[],1','[],2','1,1','1,2','5,1','5,2'}}}});
genfunctestfile('fftshift',{{1,{testcell,{'','1','5'}}}});
genfunctestfile('fliplr',{{1,{testcell}}});
genfunctestfile('flipud',{{1,{testcell}}});
tmptestcell=[intmatcell,complexmatcell,stringmatcell,booleanmatcell];
genfunctestfile('hankel',{{1,{testcell}},{1,{tmptestcell,tmptestcell}}});
tmptestcell=[emptymatcell,intmatcell,complexmatcell,booleanmatcell];
genfunctestfile('ifft',{{1,{tmptestcell,{'','1','5','[],1','[],2','1,1','5,1'}}}});
genfunctestfile('ischar',{{1,{testcell}}});
genfunctestfile('isempty',{{1,{testcell}}});
genfunctestfile('isfinite',{{1,{testcell}},{1,{{'inf','-inf','nan','-nan'}}}})
genfunctestfile('isinf',{{1,{testcell}},{1,{{'inf','nan','[inf,nan,1;nan,inf,-inf]'}}}});
tmptestcell=[emptymatcell,intmatcell,complexmatcell,stringmatcell];
genfunctestfile('isletter',{{1,{tmptestcell}}});
genfunctestfile('islogical',{{1,{testcell}}});
genfunctestfile('isnan',{{1,{testcell}},{1,{{'inf','nan','[inf,nan,1;nan,inf,-inf]'}}}});
genfunctestfile('isreal',{{1,{testcell}}});
genfunctestfile('isscalar',{{1,{testcell}}});
tmptestcell=[emptymatcell,intmatcell,complexmatcell,stringmatcell];
genfunctestfile('isspace',{{1,{tmptestcell}},{1,{{''' ''','''  str st t r   '''}}}});
genfunctestfile('isstr',{{1,{testcell}}});
genfunctestfile('isvector',{{1,{testcell}}});
genfunctestfile('kron',{{1,{testcell,testcell}}});
genfunctestfile('length',{{1,{testcell}}});
tmptestcell1={'1','1+i','1','''a''','1','1','Unknown_Type(''a'')','1','1'};
tmptestcell2={'10','10+10i','''a''','''z''','1000','Unknown_Type(''a'')','Unknown_Type(''z'')','1000','1000'};
tmptestcell3={'','','1','10','''a''','1','10','Unknown_Type(''a'')','[]'};
genfunctestfile('linspace',{{1,{tmptestcell1,tmptestcell2,tmptestcell3}}});
tmptestcell=[emptymatcell,intmatcell,booleanmatcell];
genfunctestfile('logical',{{1,{tmptestcell}}});
tmptestcell=[emptymatcell,intmatcell,complexmatcell];
genfunctestfile('log2',{{1,{tmptestcell}},{2,{tmptestcell}}});
tmptestcell=[emptymatcell,intmatcell,complexmatcell];
genfunctestfile('log10',{{1,{tmptestcell}}});
tmptestcell=[emptymatcell,intmatcell,stringmatcell];
genfunctestfile('mod',{{1,{tmptestcell,tmptestcell}}});
genfunctestfile('ndims',{{1,{testcell}}});
tmptestcell=[emptymatcell,intmatcell,complexmatcell];
tmptestcell1=[emptymatcell,intmatcell1,complexmatcell1];
genfunctestfile('pow2',{{1,{tmptestcell,{''}}},{1,{tmptestcell,tmptestcell1}}});
tmptestcell=[emptymatcell,intmatcell,stringmatcell,{0}];
genfunctestfile('rem',{{1,{tmptestcell,tmptestcell}}});
tmptestcell={'2','[2,2]','2,3','2+2i'};
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
tmptestcell=[intmatcell,complexmatcell,stringmatcell];
tmptestcell1=[intmatcell1,complexmatcell1,stringmatcell1];
genfunctestfile('toeplitz',{{1,{tmptestcell}},{1,{tmptestcell,tmptestcell1}}});

% matfun
tmptestcellsq=[emptymatcell,intsqmatcell,complexsqmatcell];
genfunctestfile('balance',{{1,{tmptestcellsq}},{2,{tmptestcellsq}}});
tmptestcell=[emptymatcell,{'1','eye(3,3)'}];
genfunctestfile('chol',{{1,{tmptestcell}}}); % Should be tested for 2 lhs
tmptestcell=[emptymatcell,intsqmatcell,complexsqmatcell];
genfunctestfile('det',{{1,{tmptestcell}}});
tmptestcell=[emptymatcell,intsqmatcell,complexsqmatcell];
genfunctestfile('eig',{{1,{tmptestcell}},{2,{tmptestcell}},{1,{[tmptestcell,{'1'}],[tmptestcell,{'''nobalance'''}]}}});
tmptestcell=[emptymatcell,intsqmatcell,complexsqmatcell];
genfunctestfile('expm',{{1,{tmptestcell}}});
tmptestcell=[emptymatcell,intsqmatcell,complexsqmatcell];
genfunctestfile('hess',{{1,{tmptestcell}},{2,{tmptestcell}}});
tmptestcell=[emptymatcell,intsqmatcell,complexsqmatcell];
genfunctestfile('inv',{{1,{tmptestcell}}});
tmptestcell=[emptymatcell,intsqmatcell,complexsqmatcell];
genfunctestfile('lu',{{2,{tmptestcell}},{3,{tmptestcell}}});
tmptestcell=[emptymatcell,intsqmatcell,complexsqmatcell];
tmptestcellvect=[emptymatcell,intvectcell,complexvectcell];
genfunctestfile('norm',{{1,{tmptestcell,{'','1','2','''fro''','inf'}}},{1,{tmptestcellvect,{'','1','2','''fro''','inf','-inf'}}}});
tmptestcell=[emptymatcell,intmatcell,complexmatcell];
genfunctestfile('qr',{{2,{tmptestcell}},{3,{tmptestcell}},{2,{tmptestcell,{'0'}}},{3,{tmptestcell,{'0'}}}});
tmptestcell=[emptymatcell,intsqmatcell,complexsqmatcell];
genfunctestfile('rcond',{{1,{tmptestcell}}});
tmptestcell=[emptymatcell,intsqmatcell,complexsqmatcell];
genfunctestfile('schur',{{1,{tmptestcell}}});
tmptestcell=[emptymatcell,intsqmatcell,complexsqmatcell];
genfunctestfile('svd',{{1,{tmptestcell,{'0'}}},{2,{tmptestcell,{'0'}}},{3,{tmptestcell,{'0'}}}});

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

