//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) DIGITEO - 2011 - Allan CORNET
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
//
//------------------------------------------------------------------------------
// Inno Setup Script (5.3 and more) for Scilab (UNICODE version required)
//
//------------------------------------------------------------------------------
function Unzip(const ZipFile: String): Boolean;
var
  ErrorCode: Integer;
  TmpDirName: String;
  sUNZIP: String;
  sParams: String;

begin
  ExtractTemporaryFile('unzip.exe');
  Result := False;
  TmpDirName := ExpandConstant('{tmp}') + '\';
  sUNZIP := TmpDirName + 'unzip.exe';
  sParams := '-e -o ' + ZipFile + ' -d ' + TmpDirName;

  Result := Exec(sUNZIP, sParams, TmpDirName, SW_HIDE, ewWaitUntilTerminated, ErrorCode);
  if (ErrorCode = 0) then
    begin
      Result := True;
    end;
end;
//------------------------------------------------------------------------------
