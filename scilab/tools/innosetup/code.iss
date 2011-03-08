//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) DIGITEO - 2010 - Allan CORNET
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

function IsProcessorFeaturePresent(ProcessorFeature: DWORD): Boolean;
external 'IsProcessorFeaturePresent@kernel32.dll stdcall';

function GetModuleHandle(lpModuleName: LongInt): LongInt;
external 'GetModuleHandleA@kernel32.dll stdcall';

var
        AboutModulesButton: TButton;
//------------------------------------------------------------------------------
function isCLIType(): Boolean;
    begin
        Result := true;
        if (IsComponentSelected( ExpandConstant('{#COMPN_JVM_MODULE}'))) then
            begin
                Result := false;
            end;
    end;
//------------------------------------------------------------------------------
function getExecNameForDesktop(Param: String): String;
    begin
        if (isCLIType() = true) then
            begin
                Result := ExpandConstant('{app}') + '\bin\scilex.exe';
            end
        else
            begin
                Result := ExpandConstant('{app}') + '\bin\wscilex.exe';
            end;
    end;
//------------------------------------------------------------------------------
function DoTasksJustAfterInstall: Boolean;
    var
        bOK : Boolean;
    begin
        Result := true;

        bOK := CreateModulesFile();
        if (bOK = false) then
            begin
                Result := false;
            end;

        if (IsComponentSelected( ExpandConstant('{#COMPN_MKL_CPU_LIBRARY}') ) = true) then
            begin
                bOK := checkMD5Files_MKL();
                if (bOK = false) then
                    begin
                        SuppressibleMsgBox(CustomMessage('DoTasksJustAfterInstallMsg2'),mbError, MB_OK, MB_OK );
                        Result := false;
                    end else begin
                        bOK := Install_MKL();
                        if (bOK = false) then
                            begin
                                Result := false;
                            end;
                    end;
            end;

        if (IsComponentSelected( ExpandConstant('{#COMPN_FFTW_MKL_LIBRARY}') ) = true) then
            begin

                bOK := checkMD5Files_MKL_FFTW();
                if (bOK = false) then
                    begin
                        SuppressibleMsgBox(CustomMessage('DoTasksJustAfterInstallMsg3'),mbError, MB_OK, MB_OK );
                        Result := false;
                    end else begin

                        bOK := Install_MKL_FFTW();
                        if (bOK = false) then
                            begin
                                Result := false;
                            end;
                    end;
            end;

    end;
//------------------------------------------------------------------------------
function haveProgramFiles64(): Boolean;
    var
        strEnv : String;
    begin
        strEnv := GetEnv('ProgramW6432');
        result := DirExists(strEnv);
    end;
//------------------------------------------------------------------------------
function isWow64(): Boolean;
    begin
        result := false;
        if (RegKeyExists(HKLM, 'SOFTWARE\Wow6432Node') = true) and (haveProgramFiles64() = true) then
          begin
              result := true;
          end;
    end;
//------------------------------------------------------------------------------
function GetJREVersion(): String;
    begin
        Result := '';
#ifdef SCILAB_X64
        RegQueryStringValue( HKLM, 'SOFTWARE\JavaSoft\Java Runtime Environment', 'CurrentVersion', Result );
#else

// 2 cas :
    if (isWow64() = true) then
        begin
            // Scilab 32 bits sur Windows 64 bits
            RegQueryStringValue( HKLM, 'SOFTWARE\Wow6432Node\JavaSoft\Java Runtime Environment', 'CurrentVersion', Result );
        end else
        begin
            // Scilab 32 bits sur Windows 32 bits
            RegQueryStringValue( HKLM, 'SOFTWARE\JavaSoft\Java Runtime Environment', 'CurrentVersion', Result );
        end;
#endif

    end;
//------------------------------------------------------------------------------
 function CheckJREVersion(): Boolean;
    var
        jreVersion:             String;
        minJREVersionRegistry:  String;
    begin
        //
        // Initialize min java version constant
        //
        minJREVersionRegistry := ExpandConstant('{#javaSpecificationVersion}');
        //
        //
        // now we check the version of the installed JRE
        jreVersion := GetJREVersion();
        //
        if ( jreVersion = '' ) then begin
            Result := false;
        end else if ( jreVersion < minJREVersionRegistry ) then begin
            Result := false;
        end else begin
            Result := true;
        end;
    end;
//------------------------------------------------------------------------------
 function VerifyJREVersion() : Boolean;
    var
        bJREVersion: Boolean;
    begin
        bJREVersion := CheckJREVersion();

        if ( bJREVersion <> true ) then begin
                SuppressibleMsgBox( CustomMessage('MsgBoxJavaDetection1') + #13 +
                    CustomMessage('MsgBoxJavaDetection2') + ExpandConstant('{#javaSpecificationVersion}') + '.',
                    mbError, MB_OK, MB_OK );
        end;

        Result := bJREVersion;
    end;
//------------------------------------------------------------------------------

procedure ButtonAboutModulesOnClick(Sender: TObject);
var
    ErrorCode: Integer;

begin
      if not ShellExec('', ExpandConstant('{#MODULES_LIST_WEB_PAGE}'),
                       '', '', SW_SHOW, ewNoWait, ErrorCode) then
        begin
            // handle failure if necessary
            SuppressibleMsgBox( CustomMessage('MsgBoxWebOpen'),mbError, MB_OK, MB_OK );
        end;
end;
//------------------------------------------------------------------------------
 function BackButtonClick(CurPageID: Integer): Boolean;
    begin
        Result := true;
        if (CurPageId = wpSelectProgramGroup) then
            begin
                AboutModulesButton.Visible := true;
            end else begin
                AboutModulesButton.Visible := false;
            end;
    end;
//------------------------------------------------------------------------------
 function NextButtonClick(CurPageID: Integer): Boolean;
    Var
        bRes : Boolean;
        ErrorCode: Integer;


    begin
        Result := true;

        if (CurPageID = wpWelcome) then
            begin
                #ifndef SCILAB_X64
                if (isWow64() = true) then
                    begin
                        SuppressibleMsgBox(CustomMessage('MsgBoxX64Ready'), mbInformation, MB_OK, MB_OK );
                    end;
                #endif

                if (IsProcessorFeaturePresent(10) = false) then
                    begin
                        bRes := false;
                        SuppressibleMsgBox(CustomMessage('MsgBoxSSERequired'), mbError, MB_OK, MB_OK );
                        Result := false;
                    end;
            end;

        if (CurPageId = wpSelectDir) then
            begin
                AboutModulesButton.Visible := true;
            end else begin
                AboutModulesButton.Visible := false;
            end;

        if (CurPageID =  wpReady) then
            begin
                if (IsComponentSelected( ExpandConstant('{#COMPN_MKL_CPU_LIBRARY}') ) = true) then
                    begin
                        bRes := Download_MKL();
                        if (bRes = false) then
                            begin
                                SuppressibleMsgBox(CustomMessage('NextButtonClickwpReadyMsg1'), mbError, MB_OK, MB_OK );
                            end;
                    end;

                if (IsComponentSelected( ExpandConstant('{#COMPN_FFTW_MKL_LIBRARY}') ) = true) then
                    begin
                        bRes := Download_MKL_FFTW();
                        if (bRes = false) then
                            begin
                                SuppressibleMsgBox(CustomMessage('NextButtonClickwpReadyMsg2'), mbError, MB_OK, MB_OK );
                            end;
                    end
            end;

        if (CurPageId = wpSelectComponents) then
            begin
              if ( IsComponentSelected( ExpandConstant('{#COMPN_JRE}') ) = false ) then
                  begin
                      bRes := VerifyJREVersion();
                      if ( bRes = false ) then
                          begin
                              Result := false;
                          end;
                  end;

                if ( (IsComponentSelected( ExpandConstant('{#COMPN_DEVTOOLS}') ) = false) and (IsComponentSelected( ExpandConstant('{#COMPN_TOOLBOX_SKELETON}') ) = true) ) then
                    begin
                            SuppressibleMsgBox( CustomMessage('MsgBoxDevToolsRequired1') + #13 +
                                CustomMessage('MsgBoxDevToolsRequired2'),
                                mbError, MB_OK, MB_OK );
                        Result := false;
                    end;

                    if ( (IsComponentSelected( ExpandConstant('{#COMPN_DEVTOOLS}') ) = false) and (IsComponentSelected( ExpandConstant('{#COMPN_TESTS}') ) = true) ) then
                    begin
                        SuppressibleMsgBox( CustomMessage('MsgBoxDevToolsRequired3') + #13 +
                                CustomMessage('MsgBoxDevToolsRequired2'),
                                mbError, MB_OK, MB_OK );
                        Result := false;
                    end;

            end;
    end;
//------------------------------------------------------------------------------
 function InitializeSetup: Boolean;
    Var
        Version: TWindowsVersion;
#ifdef SCILAB_WITHOUT_JRE
        bRes : Boolean;
#endif
    begin
        Result := True;
        GetWindowsVersionEx(Version);

        if Version.NTPlatform and (Version.Major > 4) then
            begin
                Result := True;
            end else begin
                    SuppressibleMsgBox(CustomMessage('MsgBoxWinVer'), mbCriticalError, MB_OK, MB_OK);
                  Result := False;
                  Exit;
            end
#ifdef SCILAB_WITHOUT_JRE
        bRes := CheckJREVersion();
        if ( bRes = false ) then
        begin
            SuppressibleMsgBox(CustomMessage('MsgBoxJRENotFound')+ '(' +ExpandConstant('{#javaSpecificationVersion}') + ')' + #13 +
                CustomMessage('MsgBoxJREURL')+ #13 +
                CustomMessage('MsgBoxJREReinstall')
                , mbCriticalError, MB_OK, MB_OK);
             Result := False;
             Exit;
        end else begin
            Result := True;
        end
#endif
    end;
//------------------------------------------------------------------------------
procedure CreateTheWizardPages;
var
  CancelButton: TButton;

begin

    CancelButton := WizardForm.CancelButton;

    AboutModulesButton := TButton.Create(WizardForm);
    AboutModulesButton.Left := WizardForm.ClientWidth - CancelButton.Left - CancelButton.Width;
    AboutModulesButton.Top := CancelButton.Top;
    AboutModulesButton.Width := CancelButton.Width * 2;

    AboutModulesButton.Caption := CustomMessage('ButtonAboutModules');

    AboutModulesButton.Height := CancelButton.Height;

    AboutModulesButton.OnClick := @ButtonAboutModulesOnClick;
    AboutModulesButton.Parent := CancelButton.Parent;
    AboutModulesButton.Visible := false;
end;
//------------------------------------------------------------------------------
procedure InitializeWizard();

begin
  CreateTheWizardPages;
end;
//------------------------------------------------------------------------------
