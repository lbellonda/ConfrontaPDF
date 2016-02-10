; define here the location of the sources
!define BuildDir "xxxxxxxx"
!define SourceDir "xxxxxxxxxxxxxxxxx"
!define PopplerDir "xxxxxxxxxxxxxxxx"
!define Version "1.1.0"
!define QtDir "xxxxxxxxxxxxxxx"

Name "ConfrontaPDF"


OutFile "ConfrontaPDF-${Version}.exe"

InstallDir $PROGRAMFILES\ConfrontaPDF

RequestExecutionLevel admin

Page directory
Page instfiles
;UninstPage uninstConfirm
;UninstPage instfiles


Section ""

        SetOutPath $INSTDIR

        File "${BuildDir}\confrontapdf.exe"
        File "${BuildDir}\confrontapdfc.exe"
        File "${PopplerDir}\zlib1.dll"
        File "${PopplerDir}\freetype6.dll"
        File "${PopplerDir}\liblcms2-2.dll"
        File "${PopplerDir}\libpng16.dll"
        File "${PopplerDir}\libpoppler.dll"
        File "${PopplerDir}\libpoppler-qt5.dll"
        File "${PopplerDir}\openjpeg.dll"
        File "${SourceDir}\README"
        File "${SourceDir}\gpl-2.0.txt"
        File "${SourceDir}\confrontapdf_cz.qm"
        File "${SourceDir}\confrontapdf_de.qm"
        File "${SourceDir}\confrontapdf_es.qm"
        File "${SourceDir}\confrontapdf_fr.qm"
        File "${QtDir}\bin\libwinpthread-1.dll"
        File "${QtDir}\bin\libstdc++-6.dll"
        File "${QtDir}\bin\libgcc_s_dw2-1.dll"
        File "${QtDir}\bin\icudt52.dll"
        File "${QtDir}\bin\icuin52.dll"
        File "${QtDir}\bin\icuuc52.dll"
        File "${QtDir}\bin\Qt5Core.dll"
        File "${QtDir}\bin\Qt5Xml.dll"
        File "${QtDir}\bin\Qt5Gui.dll"
        File "${QtDir}\bin\Qt5PrintSupport.dll"
        File "${QtDir}\bin\Qt5Widgets.dll"
        SetOutPath $INSTDIR\imageformats
        File "${QtDir}\plugins\imageformats\qgif.dll"
        File "${QtDir}\plugins\imageformats\qico.dll"
        File "${QtDir}\plugins\imageformats\qjpeg.dll"
        File "${QtDir}\plugins\imageformats\qmng.dll"
        File "${QtDir}\plugins\imageformats\qsvg.dll"
        File "${QtDir}\plugins\imageformats\qtga.dll"
        File "${QtDir}\plugins\imageformats\qtiff.dll"
        File "${QtDir}\plugins\imageformats\qwbmp.dll"
        SetOutPath $INSTDIR\iconengines
        File "${QtDir}\plugins\iconengines\qsvgicon.dll"
        SetOutPath $INSTDIR\platforms
        File "${QtDir}\plugins\platforms\qwindows.dll"
        SetOutPath $INSTDIR\printsupport
        File "${QtDir}\plugins\printsupport\windowsprintersupport.dll"

        WriteRegStr HKLM SOFTWARE\ConfrontaPDF "Install_Dir" "$INSTDIR"

  ; uninstaller
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\ConfrontaPDF" "DisplayName" "ConfrontaPDF"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\ConfrontaPDF" "UninstallString" '"$INSTDIR\uninstall.exe"'
  WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\ConfrontaPDF" "NoModify" 1
  WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\ConfrontaPDF" "NoRepair" 1
  WriteUninstaller "uninstall.exe"

SectionEnd

Section "Start Menu Shortcuts"
  SetShellVarContext all
  CreateDirectory "$SMPROGRAMS\ConfrontaPDF"
  CreateDirectory "$SMPROGRAMS\ConfrontaPDF\Uninstall"
  CreateShortCut "$SMPROGRAMS\ConfrontaPDF\Uninstall\Uninstall.lnk" "$INSTDIR\uninstall.exe" "" "$INSTDIR\uninstall.exe" 0
  CreateShortCut "$SMPROGRAMS\ConfrontaPDF\ConfrontaPDF.lnk" "$INSTDIR\confrontapdf.exe" "" "$INSTDIR\confrontapdf.exe" 0
  
SectionEnd


Section "Uninstall"

  DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\ConfrontaPDF"
  DeleteRegKey HKLM "SOFTWARE\ConfrontaPDF"

  SetShellVarContext all
  Delete "$SMPROGRAMS\ConfrontaPDF\Uninstall\*.*"
  RMDir "$SMPROGRAMS\ConfrontaPDF\Uninstall"
  Delete "$SMPROGRAMS\ConfrontaPDF\*.*"
  RMDir "$SMPROGRAMS\ConfrontaPDF"
  Delete "$INSTDIR\translations\*.*"
  RMDir "$INSTDIR\translations"
  Delete "$INSTDIR\snippets\*.*"
  RMDir "$INSTDIR\snippets"
  Delete "$INSTDIR\*.*"
  RMDir "$INSTDIR"
SectionEnd
