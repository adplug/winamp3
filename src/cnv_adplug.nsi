;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
;;  NSIS Template for Winamp3 Components
;;
;;  Feel free to modify and reuse.
;;
;;  Originally by Ghislain 'Aus' Lacroix
;;  aus@nullsoft.com
;;
;;  If you want more info about NSIS
;;  or you need NSIS to use this template, 
;;  go here: 
;;
;;    http://www.nullsoft.com/free/nsis/
;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;  These are the defines you should change to
;;  properly package your component.
;;
;;  COMPONENT_NAME Is the long name of your component.
;;
;;  COMPONENT_FILE Is the short name of your component.
;;    It will be used for the filename of your .wac
;;    and your readme file (which must be named 
;;    'readme-COMPONENT_FILE.txt').
;;
;;  COMPONENT_XML Is the base name of your preferences
;;    panel to configure your component.
;;
;;  LICENSE_FILE Is the file that contains the license
;;    text to be displayed at install.
;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

!define COMPONENT_NAME  "Nameless"      ;Name of the component.
!define COMPONENT_FILE  "component"     ;The base filename you're using (without extension).
!define COMPONENT_XML   "component"     ;The base name for your xml config file (without extension).

!define LICENSE_FILE    "license.txt"   ;File to use as the license text.

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;  This is where the real magic happens.
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

Name "${COMPONENT_NAME} Component for Winamp3"

OutFile "wa3_${COMPONENT_FILE}.exe"

InstallDir $PROGRAMFILES\Winamp3

ShowInstDetails Show

InstallDirRegKey HKLM \
                 "Software\Microsoft\Windows\CurrentVersion\Uninstall\Winamp3" \
                 "UninstallString"

AutoCloseWindow false

LicenseText "This will install the ${COMPONENT_NAME} Component for Winamp3."
LicenseData ${LICENSE_FILE}

DirShow Show
DirText "Setup has determined the optimal location to install. If you would like to change the directory, do so now." \
        "Select the directory to install ${COMPONENT_NAME} for Winamp3 in:"

Function .onVerifyInstDir
  IfFileExists $INSTDIR\Winamp3.exe Good  ;Check for main Winamp3.exe
  IfFileExists $INSTDIR\Studio.exe Good   ;LEGACY! Check for Studio.exe as backup.
    Abort
  Good:
FunctionEnd

Section "Installation"
 
  SetOutPath $INSTDIR\Wacs
  File "${COMPONENT_FILE}.wac"
  
  SetOutPath $INSTDIR\Wacs\Docs
  File "readme-${COMPONENT_FILE}.txt"
  
  SetOutPath $INSTDIR\Wacs\Xml\${COMPONENT_FILE}
  File "${COMPONENT_XML}.xml"
  
  
  MessageBox MB_YESNO|MB_ICONQUESTION \
           "Would you like to view the Readme file?" \
           IDNO NoReadme
  ExecShell open '$INSTDIR\Wacs\Docs\readme-${COMPONENT_FILE}.txt'
  
  NoReadme:

  WriteRegStr HKLM SOFTWARE\${COMPONENT_FILE}Winamp3 "Install_Dir" "$INSTDIR"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${COMPONENT_FILE}Winamp3" "DisplayName" "${COMPONENT_NAME} Component for Winamp3 (remove only)"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${COMPONENT_FILE}Winamp3" "UninstallString" '"$INSTDIR\uninst-${COMPONENT_FILE}.exe"'
  WriteUninstaller "$INSTDIR\uninst-${COMPONENT_FILE}.exe"

  CreateDirectory "$SMPROGRAMS\Winamp3\Component Uninstallers"
  CreateShortCut "$SMPROGRAMS\Winamp3\Component Uninstallers\${COMPONENT_FILE} Uninstall.lnk" "$INSTDIR\uninst-${COMPONENT_FILE}.exe" "" "$INSTDIR\uninst-${COMPONENT_FILE}.exe" 0
  
SectionEnd

UninstallText "This will uninstall the ${COMPONENT_NAME} Component for Winamp3. Hit next to continue."
ShowUninstDetails Show

Section "Uninstall"
  
  DeleteRegKey HKLM SOFTWARE\${COMPONENT_FILE}Winamp3
  DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${COMPONENT_FILE}Winamp3"
  
  Delete $INSTDIR\Wacs\${COMPONENT_FILE}.wac
  Delete $INSTDIR\Wacs\Docs\readme-${COMPONENT_FILE}.txt
  Delete $INSTDIR\Wacs\Xml\${COMPONENT_FILE}\${COMPONENT_XML}.xml
  Delete $INSTDIR\uninst-${COMPONENT_FILE}.exe
  
  RMDir $INSTDIR\Wacs\Xml\${COMPONENT_FILE}

  Delete "$SMPROGRAMS\Winamp3\Component Uninstallers\${COMPONENT_FILE} Uninstall.lnk"
  
  RMDir "$SMPROGRAMS\Winamp3\Component Uninstallers"

SectionEnd
