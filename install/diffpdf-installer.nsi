; define here the location of the sources
!define BuildDir "xxxxxxxx"
!define SourceDir "xxxxxxxxxxxxxxxxx"
!define PopplerDir "xxxxxxxxxxxxxxxx"
!define Version "2.1.3c"
!define QtDir "xxxxxxxxxxxxxxx"

Name "DiffPDF"


OutFile "DiffPDF-${Version}.exe"

InstallDir $PROGRAMFILES\DiffPdf

RequestExecutionLevel admin

Page directory
Page instfiles
;UninstPage uninstConfirm
;UninstPage instfiles


Section ""

        SetOutPath $INSTDIR

        File "${BuildDir}\diffpdf.exe"
		File "${PopplerDir}\zlib1.dll"
		File "${PopplerDir}\freetype6.dll"
		File "${PopplerDir}\liblcms2-2.dll"
		File "${PopplerDir}\libpng16.dll"
		File "${PopplerDir}\libpoppler.dll"
		File "${PopplerDir}\libpoppler-qt5.dll"
		File "${PopplerDir}\openjpeg.dll"
        File "${SourceDir}\README"
        File "${SourceDir}\gpl-2.0.txt"
        File "${SourceDir}\diffpdf_cz.qm"
        File "${SourceDir}\diffpdf_de.qm"
        File "${SourceDir}\diffpdf_es.qm"
		File "${SourceDir}\diffpdf_fr.qm"
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

        WriteRegStr HKLM SOFTWARE\DiffPdf "Install_Dir" "$INSTDIR"

  ; uninstaller
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\DiffPdf" "DisplayName" "DiffPdf"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\DiffPdf" "UninstallString" '"$INSTDIR\uninstall.exe"'
  WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\DiffPdf" "NoModify" 1
  WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\DiffPdf" "NoRepair" 1
  WriteUninstaller "uninstall.exe"

SectionEnd

Section "Start Menu Shortcuts"
  SetShellVarContext all
  CreateDirectory "$SMPROGRAMS\DiffPdf"
  CreateDirectory "$SMPROGRAMS\DiffPdf\Uninstall"
  CreateShortCut "$SMPROGRAMS\DiffPdf\Uninstall\Uninstall.lnk" "$INSTDIR\uninstall.exe" "" "$INSTDIR\uninstall.exe" 0
  CreateShortCut "$SMPROGRAMS\DiffPdf\DiffPdf.lnk" "$INSTDIR\DiffPdf.exe" "" "$INSTDIR\DiffPdf.exe" 0
  
SectionEnd


Section "Uninstall"

  DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\PdfDiff"
  DeleteRegKey HKLM "SOFTWARE\QXmlEdit"

  SetShellVarContext all
  Delete "$SMPROGRAMS\QXmlEdit\Uninstall\*.*"
  RMDir "$SMPROGRAMS\QXmlEdit\Uninstall"
  Delete "$SMPROGRAMS\QXmlEdit\*.*"
  RMDir "$SMPROGRAMS\QXmlEdit"
  Delete "$INSTDIR\sqldrivers\*.*"
  RMDir "$INSTDIR\sqldrivers"
  Delete "$INSTDIR\translations\*.*"
  RMDir "$INSTDIR\translations"
  Delete "$INSTDIR\snippets\*.*"
  RMDir "$INSTDIR\snippets"
  Delete "$INSTDIR\*.*"
  RMDir "$INSTDIR"
SectionEnd
