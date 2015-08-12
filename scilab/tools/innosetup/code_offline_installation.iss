//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) DIGITEO - 2010-2011 - Allan CORNET
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
//
//------------------------------------------------------------------------------
// Inno Setup Script (5.3 and more) for Scilab (UNICODE version required)
//
//------------------------------------------------------------------------------

var
  OfflineInstallCheckBox: TNewCheckBox;

//------------------------------------------------------------------------------
// Sets the state of a component given by its english name
function SetComponentState(ComponentEnglishName: String;
  Checked: Boolean; Enabled: Boolean): Boolean;
var
  ComponentLocalizedName: String;
  Index: Integer;
begin
  ComponentLocalizedName := CustomMessage(ComponentEnglishName);
  Index := WizardForm.ComponentsList.Items.IndexOf(ComponentLocalizedName);
  if Index <> -1 then
  begin
    WizardForm.ComponentsList.Checked[Index] := Checked;
    WizardForm.ComponentsList.ItemEnabled[Index] := Enabled;
    Result := True;
  end else
  begin
    Log('SetComponentState: cannot find component ' + ComponentEnglishName);
    Result := False;
  end;
end;
//------------------------------------------------------------------------------
// Offline installation option is checked : disable the components needing
// to be downloaded (MKL, FFTW)
procedure OnOfflineInstallCheckBoxClick(Sender: TObject);
var
  IsOnline: Boolean;
  Res: Boolean;
begin
  // Bug: IsOnline := not ((Sender as TNewCheckBox).Checked) provokes errors
  If (Sender as TNewCheckBox).Checked then
    IsOnline := False
  else
    IsOnline := True;

  // CPU Optimization
  Res := SetComponentState('DescriptionMKLLibrary', IsOnline, IsOnline);
  if not IsOnline then
  begin
#ifndef SCILAB_X64
    Res := Res and SetComponentState('DescriptionATLASLibrary', True, True);
#else
    Res := Res and SetComponentState('DescriptionRefBlasLibrary', True, True);
#endif
  end;

  // FFTW
  Res := Res and SetComponentState('DescriptionFftwMKLDownload', IsOnline,
    IsOnline);
  if not IsOnline then
  begin
    Res := Res and SetComponentState('DescriptionRefFftwLibrary', True, True);
  end;

  if not Res then
  begin
    Log('OfflineInstallCheckBoxOnClick: ' +
      'Error while changing components intallation.');
  end;

  // Refresh installation type (Online => Full, Offline => Custom)
  if IsOnline then
    WizardForm.TypesCombo.ItemIndex := 0
  else
    WizardForm.TypesCombo.ItemIndex := 1;

  // Bug: need to trigger manually the TypesCombo.OnChange event
  WizardForm.TypesCombo.OnChange(WizardForm);
end;
//------------------------------------------------------------------------------
// Adds an "Offline installation" option (check box)in the Select Location panel
procedure CreateOfflineInstallationCheckBox;
var
  LastControl: TWinControl;
begin
  LastControl := WizardForm.DirEdit;

  OfflineInstallCheckBox := TNewCheckBox.Create(WizardForm);
  OfflineInstallCheckBox.Parent := WizardForm.SelectDirPage;
  OfflineInstallCheckBox.Top := LastControl.Top +
    LastControl.Height + ScaleY(8);
  OfflineInstallCheckBox.Width := WizardForm.SelectDirPage.Width;
  OfflineInstallCheckBox.Caption :=
    'Installation without an Internet connection';
  OfflineInstallCheckBox.OnClick := @OnOfflineInstallCheckBoxClick;
end;

