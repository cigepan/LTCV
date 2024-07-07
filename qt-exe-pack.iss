; Script generated by the Inno Setup Script Wizard.
; SEE THE DOCUMENTATION FOR DETAILS ON CREATING INNO SETUP SCRIPT FILES!

#define MyAppName "LTCV"
#define MyAppVersion "2024.0706.1854"
#define MyAppPublisher "CaiPeng"
#define MyAppURL "http://www.cigepan.com/"
#define MyAppExeName "LTCV.exe"

[Setup]
; NOTE: The value of AppId uniquely identifies this application. Do not use the same AppId value in installers for other applications.
; (To generate a new GUID, click Tools | Generate GUID inside the IDE.)
AppId={{CB792668-3460-473A-A4C2-797AE364F7CC}
AppName={#MyAppName}
AppVersion={#MyAppVersion}
;AppVerName={#MyAppName} {#MyAppVersion}
AppPublisher={#MyAppPublisher}
AppPublisherURL={#MyAppURL}
AppSupportURL={#MyAppURL}
AppUpdatesURL={#MyAppURL}
DefaultDirName={autopf}\{#MyAppName}
DisableProgramGroupPage=yes
; Remove the following line to run in administrative install mode (install for all users.)
PrivilegesRequired=lowest
OutputDir=D:\0Project\LTCV\build\Release
OutputBaseFilename=Step-LTCV-Windows-X64-{#MyAppVersion}
SetupIconFile=D:\0Project\LTCV\2D.bmp
Compression=lzma
SolidCompression=yes
WizardStyle=modern

[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked

[Files]
Source: "D:\0Project\LTCV\build\Desktop_Qt_6_7_2_MinGW_64_bit-Release\LTCV.exe"; DestDir: "{app}"; Flags: ignoreversion
Source: "D:\0Project\LTCV\build\Desktop_Qt_6_7_2_MinGW_64_bit-Release\*.dll"; DestDir: "{app}"; Flags: ignoreversion recursesubdirs
Source: "D:\0Project\LTCV\build\Desktop_Qt_6_7_2_MinGW_64_bit-Release\way\*.json"; DestDir: "{app}"; Flags: ignoreversion recursesubdirs
; NOTE: Don't use "Flags: ignoreversion" on any shared system files

[Icons]
Name: "{autoprograms}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"
Name: "{autodesktop}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"; Tasks: desktopicon

[Run]
Filename: "{app}\{#MyAppExeName}"; Description: "{cm:LaunchProgram,{#StringChange(MyAppName, '&', '&&')}}"; Flags: nowait postinstall skipifsilent

