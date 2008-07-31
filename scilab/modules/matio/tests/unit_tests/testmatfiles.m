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

% TODO

%
% TESTS FOR STRUCTURE ARRAYS
%

% TODO

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

% TODO: ND-Array tests ?

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

% TODO: ND-Array tests ?

%
% TESTS FOR SINGLE PRECISION ARRAYS
%

% TODO

%
% TESTS FOR SPARSE MATRICES
%

% TODO

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

% TODO: int64 tests ?

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

% TODO: uint64 tests ?

% TODO: ND-Array tests ?
realdoubleNDarray = reshape(1:24, 2, 3, 4);

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
             (formatIndex==1 & isempty(strfind(varNames{varIndex}, 'int')) & isempty(strfind(varNames{varIndex}, 'NDarray')))
         saveCmd = ['save ' pwd filesep varNames{varIndex} binFormats{formatIndex} '.mat ' varNames{varIndex} ' ' binFormats{formatIndex}];
         eval(saveCmd);
       end
     end
end

% TODO: save all variables in a single file ?
