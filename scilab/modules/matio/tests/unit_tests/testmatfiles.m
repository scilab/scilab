% =============================================================================
% Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
% Copyright (C) 2008 - INRIA - Vincent Couvert <vincent.couvert@inria.fr>
%
%  This file is distributed under the same license as the Scilab package.
% =============================================================================

% This Matlab script generates MAT-files to test the MATIO module

clear all

%
% TESTS FOR CELL ARRAYS
%
emptycell = {};
scalarcell = {1.23};
rowcell = {'abc', [1.23+7.89i 4.56-1.23i 7.89+4.56i], 1.23};
colcell = {[1.23+7.89i 4.56-1.23i 7.89+4.56i]; 'abc'; 1.23};
arraycell = {'a', [1.23+7.89i 4.56-1.23i 7.89+4.56i], int8(1); 'abc', 1.23, eye(100,100)};
stringcell = {'abc', 'def', 'ghi';'jkl', 'mno', 'pqr'};

%
% TESTS FOR STRUCTURE ARRAYS
%
emptystruct = struct();
scalarstruct = struct('f1', 10, 'ftwo', 'Hello', 'field3', int8(12));
rowstruct = struct('field1', 10, 'field2', 'Hello', 'field3', int8(12));
rowstruct(1,2).field1 = 'test';
rowstruct(1,2).field2 = eye(10, 10);
rowstruct(1,3).field2 = 'a field contents';
rowstruct(1,3).field3 = 1.23+4.56i;
colstruct = struct('name', 10, 'phone', 'Hello', 'address', int8(12));
colstruct(2,1).name = 'test';
colstruct(2,1).phone = eye(10, 10);
colstruct(3,1).phone = 'a field contents';
colstruct(3,1).address = 1.23+4.56i;
arraystruct = struct('field1', 10, 'field2', 'Hello', 'field3', int8(12));
arraystruct(1,2).field1 = 'test';
arraystruct(1,2).field2 = eye(10, 10);
arraystruct(1,3).field2 = 'a field contents';
arraystruct(1,3).field3 = 1.23+4.56i;
arraystruct(2,1).name = 'test';
arraystruct(2,1).phone = eye(10, 10);
arraystruct(3,1).phone = 'a field contents';
arraystruct(3,1).address = 1.23+4.56i;

%
% TESTS FOR OBJECTS
%

% TODO

%
% TESTS FOR CHARACTER ARRAYS
%

emptystringmatrix = '';
stringscalar = 'a';
stringrowvector = ['a' 'b' 'c'];
stringcolvector = ['a';'b';'c'];
stringmatrix = ['a' 'b' 'c';'d' 'e' 'f'];

%
% TESTS FOR DOUBLE PRECISION ARRAYS
%

emptydoublematrix = [];

realdoublescalar = 1.23;
cplxdoublescalar = 1.23 + 4.56i;

realdoublerowvector = [1.23 -4.56 7.89];
cplxdoublerowvector = [1.23+7.89i 4.56-1.23i 7.89+4.56i];

realdoublecolvector = [1.23;-4.56;7.89];
cplxdoublecolvector = [1.23+7.89i;4.56-1.23i;7.89+4.56i];

realdoublematrix = [1.23 -4.56 7.89;9.87 6.54 -3.21];
cplxdoublematrix = [1.23+7.89i 4.56-1.23i 7.89+4.56i;9.87+3.21i 6.54+9.87i 3.21-6.54i];

%
% TESTS FOR SINGLE PRECISION ARRAYS
%

% TODO

%
% TESTS FOR SPARSE MATRICES
%
% Tests for complex matrices are not done because of a bug in MATIO

emptysparse = sparse([]);

realscalarsparse = sparse(1.23);
%cplxscalarsparse = sparse(1.23 + 4.56i);

realrowvectorsparse = sparse([1.23 0 7.89]);
%cplxrowvectorsparse = sparse([1.23+7.89i 0 7.89+4.56i]);

realcolvectorsparse = sparse([1.23;0;7.89]);
%cplxcolvectorsparse = sparse([1.23+7.89i;0;7.89+4.56i]);

realmatrixsparse = sparse([1.23 0 7.89;0 6.54 -3.21]);
%cplxmatrixsparse = sparse([1.23+7.89i 0 7.89+4.56i;0 6.54+9.87i 3.21-6.54i]);

%
% TESTS FOR 8-BITS SIGNED INTEGERS
%

emptyint8matrix = int8([]);
int8scalar = int8(1);
int8rowvector = int8([1 -4 7]);
int8colvector = int8([1;-4;7]);
int8matrix = int8([1 -4 7;-9 6 -3]);

%
% TESTS FOR 16-BITS SIGNED INTEGERS
%

emptyint16matrix = int16([]);
int16scalar = int16(1);
int16rowvector = int16([1 -4 7]);
int16colvector = int16([1;-4;7]);
int16matrix = int16([1 -4 7;-9 6 -3]);

%
% TESTS FOR 32-BITS SIGNED INTEGERS
%

emptyint32matrix = int32([]);
int32scalar = int32(1);
int32rowvector = int32([1 -4 7]);
int32colvector = int32([1;-4;7]);
int32matrix = int32([1 -4 7;-9 6 -3]);

%
% TESTS FOR 64-BITS SIGNED INTEGERS
%

emptyint64matrix = int64([]);
int64scalar = int64(1);
int64rowvector = int64([1 -4 7]);
int64colvector = int64([1;-4;7]);
int64matrix = int64([1 -4 7;-9 6 -3]);

%
% TESTS FOR 8-BITS UNSIGNED INTEGERS
%

emptyuint8matrix = uint8([]);
uint8scalar = uint8(1);
uint8rowvector = uint8([1 -4 7]);
uint8colvector = uint8([1;-4;7]);
uint8matrix = uint8([1 -4 7;-9 6 -3]);

%
% TESTS FOR 16-BITS UNSIGNED INTEGERS
%

emptyuint16matrix = uint16([]);
uint16scalar = uint16(1);
uint16rowvector = uint16([1 -4 7]);
uint16colvector = uint16([1;-4;7]);
uint16matrix = uint16([1 -4 7;-9 6 -3]);

%
% TESTS FOR 32-BITS UNSIGNED INTEGERS
%

emptyuint32matrix = uint32([]);
uint32scalar = uint32(1);
uint32rowvector = uint32([1 -4 7]);
uint32colvector = uint32([1;-4;7]);
uint32matrix = uint32([1 -4 7;-9 6 -3]);

%
% TESTS FOR 64-BITS UNSIGNED INTEGERS
%

emptyuint64matrix = uint64([]);
uint64scalar = uint64(1);
uint64rowvector = uint64([1 -4 7]);
uint64colvector = uint64([1;-4;7]);
uint64matrix = uint64([1 -4 7;-9 6 -3]);

%
% TESTS FOR ND-ARRAYS
%
emptyNDarray = reshape([], 0, 0, 0);
realdoubleNDarray = reshape(1:24, 2, 3, 4);
complexdoubleNDarray = reshape((1:24) + (1:24)*i, 2, 3, 4);

%
% MISC
%
eye100x100 = eye(100,100);

%
% Create files
%
varNames = who();
binFormats = {'-v4';'-v6';'-v7'};
for varIndex=1:length(varNames)
     for formatIndex=1:length(binFormats)
        % Integers and NDarrays not saved in -v4 format
       if formatIndex>1 | ...
             (formatIndex==1 & isempty(strfind(varNames{varIndex}, 'int')) ...
              & isempty(strfind(varNames{varIndex}, 'NDarray')) & isempty(strfind(varNames{varIndex}, 'cell')) & isempty(strfind(varNames{varIndex}, 'struct')) & isempty(strfind(varNames{varIndex}, 'sparse')))
         if exist([pwd filesep varNames{varIndex} binFormats{formatIndex} ...
                   '.mat']) ~= 2 % If file does not already exist it is created
           saveCmd = ['save ' pwd filesep varNames{varIndex} binFormats{formatIndex} '.mat ' varNames{varIndex} ' ' binFormats{formatIndex}];
           eval(saveCmd);
         else
           %disp([pwd filesep varNames{varIndex} binFormats{formatIndex} '.mat already exists.'])
         end
       end
     end
end

% TODO: save all variables in a single file ?
