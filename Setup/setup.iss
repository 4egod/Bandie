#define MyAppName "Bandie"
#define MyAppVersion "0.18.11.1"
#define MyAppPublisher "Digital Resistance"

[Setup]
DisableWelcomePage=no
AppId={{8AC7BC22-69E0-49F2-AB59-766AA5C67115}
AppName={#MyAppName}
AppVersion={#MyAppVersion}
AppVerName={#MyAppName} {#MyAppVersion}
AppPublisher={#MyAppPublisher}
OutputBaseFilename=bandie_setup
Compression=lzma2/ultra64
SolidCompression=yes
InternalCompressLevel=ultra64
SetupIconFile=icon.ico
WizardImageFile=164x314.bmp
WizardSmallImageFile=55x55.bmp
AppCopyright=Copyright (C) {#MyAppPublisher}, 2018
OutputDir=..\bin
AlwaysShowGroupOnReadyPage=True
AlwaysShowDirOnReadyPage=True
AppMutex=2465A86D-0A16-48F7-9C7A-D616564C7777
AppComments={#MyAppName}
VersionInfoVersion={#MyAppVersion}
VersionInfoCompany={#MyAppPublisher}
VersionInfoDescription={#MyAppName}
VersionInfoTextVersion={#MyAppVersion}
VersionInfoCopyright=Copyright (C) {#MyAppPublisher}, 2018
VersionInfoProductName={#MyAppName}
VersionInfoProductVersion={#MyAppVersion}
VersionInfoProductTextVersion={#MyAppVersion}
ShowLanguageDialog=no
LanguageDetectionMethod=none
WizardImageStretch=False
CloseApplications=False
Uninstallable=yes
UsePreviousSetupType=False
RestartApplications=False
UninstallDisplayName={#MyAppName}
UninstallDisplayIcon={app}\Bandie.exe
DisableProgramGroupPage=yes
ArchitecturesInstallIn64BitMode=x64
PrivilegesRequired=lowest
DefaultDirName={commonappdata}\Bandai
AppendDefaultDirName=False
UsePreviousGroup=False

[Files]
Source: "..\x64\Release\Bandie.exe"; DestDir: "{app}"; Flags: ignoreversion replacesameversion 64bit; Check: Is64BitInstallMode
Source: "..\x64\Release\WinDivert.dll"; DestDir: "{app}"; Flags: ignoreversion replacesameversion 64bit; Check: Is64BitInstallMode
Source: "..\x64\Release\WinDivert64.sys"; DestDir: "{app}"; Flags: ignoreversion replacesameversion 64bit; Check: Is64BitInstallMode
Source: "..\Release\Bandie.exe"; DestDir: "{app}"; Flags: ignoreversion replacesameversion 32bit; Check: not Is64BitInstallMode
Source: "..\Release\WinDivert.dll"; DestDir: "{app}"; Flags: ignoreversion replacesameversion 32bit; Check: not Is64BitInstallMode
Source: "..\Release\WinDivert32.sys"; DestDir: "{app}"; Flags: ignoreversion replacesameversion 32bit; Check: not Is64BitInstallMode

[Icons]
Name: "{userdesktop}\{#MyAppName}"; Filename: "{app}\Bandie.exe"; WorkingDir: "{app}"; IconFilename: "{app}\Bandie.exe"; IconIndex: 0
Name: "{userprograms}\{#MyAppName}"; Filename: "{app}\Bandie.exe"; WorkingDir: "{app}"; IconFilename: "{app}\Bandie.exe"; IconIndex: 0

[Run]
Filename: "{app}\Bandie.exe"; WorkingDir: "{app}"; Flags: postinstall nowait shellexec hidewizard; Description: "Start Bandie"; Check: Is64BitInstallMode
Filename: "{app}\Bandie.exe"; WorkingDir: "{app}"; Flags: postinstall nowait shellexec hidewizard; Description: "Start Bandie"; Check: not Is64BitInstallMode
