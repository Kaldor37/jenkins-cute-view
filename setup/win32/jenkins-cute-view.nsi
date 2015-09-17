; Jenkins cute view Win32 installer NSIS script

!include "MUI2.nsh"

; Change those constants to meet your configuration:
; Path to git directory
!define JCV_SRC_DIR "C:\jenkins-cute-view"
; Path to build directory (containing compiled jenkins-cute-view.exe)
!define JCV_BUILD_DIR "C:\build\jenkins-cute-view"
; Path to Qt libraries and binaries
!define QT_DIR "C:\Qt\5.5\mingw492_32"
; Version of this setup
!define JCV_VERSION "0.2"

; The name of the installer
Name "Jenkins Cute View"

; The file to write
OutFile "jenkins-cute-view-v${JCV_VERSION}-setup.exe"

RequestExecutionLevel user
InstallDir $APPDATA\JenkinsCuteView
InstallDirRegKey HKCU "Software\JenkinsCuteView" "Install_Dir"

!define MUI_ABORTWARNING

;Finish page
;--------------------------------
;Finish
Function finishpageaction
CreateShortcut "$desktop\JenkinsCuteView.lnk" "$instdir\jenkins-cute-view.exe"
FunctionEnd

!define MUI_FINISHPAGE_SHOWREADME ""
!define MUI_FINISHPAGE_SHOWREADME_CHECKED
!define MUI_FINISHPAGE_SHOWREADME_TEXT "Create a shortcut on desktop"
!define MUI_FINISHPAGE_SHOWREADME_FUNCTION finishpageaction

; Pages
!insertmacro MUI_PAGE_WELCOME
!insertmacro MUI_PAGE_LICENSE "${JCV_SRC_DIR}\LICENSE.md"
!insertmacro MUI_PAGE_DIRECTORY
!insertmacro MUI_PAGE_INSTFILES
!insertmacro MUI_PAGE_FINISH

!insertmacro MUI_UNPAGE_CONFIRM
!insertmacro MUI_UNPAGE_INSTFILES

; Languages
!insertmacro MUI_LANGUAGE "English"
!insertmacro MUI_LANGUAGE "French"
!insertmacro MUI_LANGUAGE "German"
!insertmacro MUI_LANGUAGE "Spanish"

; Files to install
Section "Main section"

	SetOutPath $INSTDIR
	File "${JCV_BUILD_DIR}\jenkins-cute-view.exe"
	File "${QT_DIR}\bin\Qt5Core.dll"
	File "${QT_DIR}\bin\Qt5Gui.dll"
	File "${QT_DIR}\bin\Qt5Network.dll"
	File "${QT_DIR}\bin\Qt5Widgets.dll"
	File "${QT_DIR}\bin\libstdc++-6.dll"
	File "${QT_DIR}\bin\libwinpthread-1.dll"
	File "${QT_DIR}\bin\libgcc_s_dw2-1.dll"
	File "${JCV_SRC_DIR}\LICENSE.md"
	File "${JCV_SRC_DIR}\README.md"
	
	SetOutPath "$INSTDIR\platforms"
	File "${QT_DIR}\plugins\platforms\qwindows.dll"
	
	WriteUninstaller "$INSTDIR\Uninstall.exe"  
SectionEnd

Section "Uninstall"

	Delete "$INSTDIR\platforms\*.*"
	RMDir "$INSTDIR\platforms"
	Delete "$INSTDIR\*.*"
	RMDir "$INSTDIR"

	DeleteRegKey /ifempty HKCU "Software\JenkinsCuteView"

SectionEnd