# Microsoft Developer Studio Project File - Name="DEMIST" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=DEMIST - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Simul.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Simul.mak" CFG="DEMIST - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "DEMIST - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "DEMIST - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName "DEMIST"
# PROP Scc_LocalPath "."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "DEMIST - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GR /GX /O2 /I "D:\DEV\DEMIST" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x809 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x809 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo /o"Release/demist.bsc"
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 Shlwapi.lib /nologo /subsystem:windows /machine:I386 /out:"Release/demist.exe"

!ELSEIF  "$(CFG)" == "DEMIST - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GR /GX /ZI /Od /I "D:\DEV\DEMIST" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x809 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x809 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo /o"Debug/demist.bsc"
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 msimg32.lib Shlwapi.lib /nologo /subsystem:windows /debug /machine:I386 /out:"Debug/demist.exe" /pdbtype:sept
# SUBTRACT LINK32 /verbose /profile

!ENDIF 

# Begin Target

# Name "DEMIST - Win32 Release"
# Name "DEMIST - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\AnimCtrl\3DMeterCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\MSimulation\AddModelDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\Animation\AnimControl.cpp
# End Source File
# Begin Source File

SOURCE=.\Animation\AnimDensityPage.cpp
# End Source File
# Begin Source File

SOURCE=.\Animation\AnimInterPage.cpp
# End Source File
# Begin Source File

SOURCE=.\Animation\AnimLink.cpp
# End Source File
# Begin Source File

SOURCE=.\Animation\AnimLinkPage.cpp
# End Source File
# Begin Source File

SOURCE=.\Animation\AnimObject.cpp
# End Source File
# Begin Source File

SOURCE=.\Animation\AnimObjectPage.cpp
# End Source File
# Begin Source File

SOURCE=.\Animation\AnimObjSizePage.cpp
# End Source File
# Begin Source File

SOURCE=.\Animation\AnimPage.cpp
# End Source File
# Begin Source File

SOURCE=.\Animation\AnimPict.cpp
# End Source File
# Begin Source File

SOURCE=.\Animation\AnimPictDensPage.cpp
# End Source File
# Begin Source File

SOURCE=.\Animation\AnimPictPage.cpp
# End Source File
# Begin Source File

SOURCE=.\Animation\AnimPolygonPage.cpp
# End Source File
# Begin Source File

SOURCE=.\Animation\AnimSector.cpp
# End Source File
# Begin Source File

SOURCE=.\Animation\AnimSectorPage.cpp
# End Source File
# Begin Source File

SOURCE=.\Animation\AnimSelectDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\Animation\AnimShapePage.cpp
# End Source File
# Begin Source File

SOURCE=.\Animation\AnimText.cpp
# End Source File
# Begin Source File

SOURCE=.\Animation\AnimTextPage.cpp
# End Source File
# Begin Source File

SOURCE=.\Animation\AnimValuePage.cpp
# End Source File
# Begin Source File

SOURCE=.\MSimulation\AuthorModel.cpp
# End Source File
# Begin Source File

SOURCE=.\Tools\AutoFont.cpp
# End Source File
# Begin Source File

SOURCE=.\Tools\BCMenu.cpp
# End Source File
# Begin Source File

SOURCE=.\Tools\Bezier.cpp
# End Source File
# Begin Source File

SOURCE=.\Tools\BitmapPickerCombo.cpp
# End Source File
# Begin Source File

SOURCE=.\BPInfoDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\GridCtrl\GridBtn\BtnDataBase.cpp
# End Source File
# Begin Source File

SOURCE=.\Tools\BtnST.cpp
# End Source File
# Begin Source File

SOURCE=.\Tools\Bubble.cpp
# End Source File
# Begin Source File

SOURCE=.\MSimulation\BuildModelDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ChildFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\Tools\ColorPickerCB.cpp
# End Source File
# Begin Source File

SOURCE=.\Tools\ColourPicker.cpp
# End Source File
# Begin Source File

SOURCE=.\Tools\ColourPopup.cpp
# End Source File
# Begin Source File

SOURCE=.\AnimCtrl\ComponentStatic.cpp
# End Source File
# Begin Source File

SOURCE=.\Tools\CopyPasteBuffer.cpp
# End Source File
# Begin Source File

SOURCE=.\AnimCtrl\CRadialCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\MInstruction\DefineERPropSheet.cpp
# ADD CPP /I ".."
# End Source File
# Begin Source File

SOURCE=.\MInstruction\DefineExpSetDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\MInstruction\DefineLUPropSheet.cpp
# End Source File
# Begin Source File

SOURCE=.\hlp\demist.hpj

!IF  "$(CFG)" == "DEMIST - Win32 Release"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Making help file...
OutDir=.\Release
InputPath=.\hlp\demist.hpj
InputName=demist

"$(OutDir)\$(InputName).hlp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	start /wait hcw /C /E /M "hlp\$(InputName).hpj" 
	if errorlevel 1 goto :Error 
	if not exist "hlp\$(InputName).hlp" goto :Error 
	copy "hlp\$(InputName).hlp" $(OutDir) 
	goto :done 
	:Error 
	echo hlp\$(InputName).hpj(1) : error: 
	type "hlp\$(InputName).log" 
	:done 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "DEMIST - Win32 Debug"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Making help file...
OutDir=.\Debug
InputPath=.\hlp\demist.hpj
InputName=demist

"$(OutDir)\$(InputName).hlp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	start /wait hcw /C /E /M "hlp\$(InputName).hpj" 
	if errorlevel 1 goto :Error 
	if not exist "hlp\$(InputName).hlp" goto :Error 
	copy "hlp\$(InputName).hlp" $(OutDir) 
	goto :done 
	:Error 
	echo hlp\$(InputName).hpj(1) : error: 
	type "hlp\$(InputName).log" 
	:done 
	
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Demist.rc
# End Source File
# Begin Source File

SOURCE=.\MInteraction\DlgFormat.cpp
# End Source File
# Begin Source File

SOURCE=.\MInteraction\DlgTranslation.cpp
# End Source File
# Begin Source File

SOURCE=.\MultiSlider\EditInterval.cpp
# End Source File
# Begin Source File

SOURCE=.\MInteraction\Format.cpp
# End Source File
# Begin Source File

SOURCE=.\MInteraction\FormatPage.cpp
# End Source File
# Begin Source File

SOURCE=.\Tools\Graph.cpp
# End Source File
# Begin Source File

SOURCE=.\Tools\Graphics.cpp
# End Source File
# Begin Source File

SOURCE=.\Tools\GraphLegend.cpp
# End Source File
# Begin Source File

SOURCE=.\Tools\GraphSeries.cpp
# End Source File
# Begin Source File

SOURCE=.\GridCtrl\GridBtn\GridBtnCell.cpp
# End Source File
# Begin Source File

SOURCE=.\GridCtrl\GridBtn\GridBtnCellBase.cpp
# End Source File
# Begin Source File

SOURCE=.\GridCtrl\GridBtn\GridBtnCellCombo.cpp
# End Source File
# Begin Source File

SOURCE=.\GridCtrl\GridCell.cpp
# End Source File
# Begin Source File

SOURCE=.\GridCtrl\GridCellBase.cpp
# End Source File
# Begin Source File

SOURCE=.\GridCtrl\NewCellTypes\GridCellColor.cpp
# End Source File
# Begin Source File

SOURCE=.\GridCtrl\NewCellTypes\GridCellCombo.cpp
# End Source File
# Begin Source File

SOURCE=.\GridCtrl\GridCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\GridCtrl\GridDropTarget.cpp
# End Source File
# Begin Source File

SOURCE=.\GridCtrl\GridTree\GridTreeBtnCell.cpp
# End Source File
# Begin Source File

SOURCE=.\GridCtrl\TreeColumn\GridTreeCell.cpp
# End Source File
# Begin Source File

SOURCE=.\GridCtrl\TreeColumn\GridTreeCellBase.cpp
# End Source File
# Begin Source File

SOURCE=.\GridCtrl\NewCellTypes\GridURLCell.cpp
# End Source File
# Begin Source File

SOURCE=.\Tools\HelpDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\Tools\HelpPrSheet.cpp
# End Source File
# Begin Source File

SOURCE=.\IdentityDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\GridCtrl\InPlaceEdit.cpp
# End Source File
# Begin Source File

SOURCE=.\GridCtrl\GridBtn\InPlaceList.cpp
# End Source File
# Begin Source File

SOURCE=.\MInstruction\InstructView.cpp
# End Source File
# Begin Source File

SOURCE=.\LearnerTrace.cpp
# End Source File
# Begin Source File

SOURCE=.\MInstruction\LearningUnit.cpp
# End Source File
# Begin Source File

SOURCE=.\MathParser\Lexer.cpp
# End Source File
# Begin Source File

SOURCE=.\Tools\Line.cpp
# End Source File
# Begin Source File

SOURCE=.\Tools\LinePicker.cpp
# End Source File
# Begin Source File

SOURCE=.\Tools\LineWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\MainFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\MathParser\MathParser.cpp
# End Source File
# Begin Source File

SOURCE=.\MSimulation\Model.cpp
# End Source File
# Begin Source File

SOURCE=.\Animation\MRTracker.cpp
# End Source File
# Begin Source File

SOURCE=.\MultiSlider\MultiSlider.cpp
# End Source File
# Begin Source File

SOURCE=.\Tools\MvDocTemplate.cpp
# End Source File
# Begin Source File

SOURCE=.\COptionTree\OptionTree.cpp
# End Source File
# Begin Source File

SOURCE=.\COptionTree\OptionTreeCheckButton.cpp
# End Source File
# Begin Source File

SOURCE=.\COptionTree\OptionTreeColorPopUp.cpp
# End Source File
# Begin Source File

SOURCE=.\COptionTree\OptionTreeIconPopUp.cpp
# End Source File
# Begin Source File

SOURCE=.\COptionTree\OptionTreeInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\COptionTree\OptionTreeItem.cpp
# End Source File
# Begin Source File

SOURCE=.\COptionTree\OptionTreeItemCheckBox.cpp
# End Source File
# Begin Source File

SOURCE=.\COptionTree\OptionTreeItemColor.cpp
# End Source File
# Begin Source File

SOURCE=.\COptionTree\OptionTreeItemComboBox.cpp
# End Source File
# Begin Source File

SOURCE=.\COptionTree\OptionTreeItemDate.cpp
# End Source File
# Begin Source File

SOURCE=.\COptionTree\OptionTreeItemEdit.cpp
# End Source File
# Begin Source File

SOURCE=.\COptionTree\OptionTreeItemIcon.cpp
# End Source File
# Begin Source File

SOURCE=.\COptionTree\OptionTreeItemRadio.cpp
# End Source File
# Begin Source File

SOURCE=.\COptionTree\OptionTreeItemSpinner.cpp
# End Source File
# Begin Source File

SOURCE=.\COptionTree\OptionTreeItemStatic.cpp
# End Source File
# Begin Source File

SOURCE=.\COptionTree\OptionTreeList.cpp
# End Source File
# Begin Source File

SOURCE=.\COptionTree\OptionTreeRadioButton.cpp
# End Source File
# Begin Source File

SOURCE=.\COptionTree\OptionTreeSpinnerButton.cpp
# End Source File
# Begin Source File

SOURCE=.\COptionTree\OptionTreeSpinnerEdit.cpp
# End Source File
# Begin Source File

SOURCE=.\Prefs\Pref.cpp
# End Source File
# Begin Source File

SOURCE=.\Prefs\PreferencePage.cpp
# End Source File
# Begin Source File

SOURCE=.\Prefs\prefsimulpage.cpp
# End Source File
# Begin Source File

SOURCE=.\MSimulation\PreyPredModel.cpp
# End Source File
# Begin Source File

SOURCE=.\Tools\PushPin.cpp
# End Source File
# Begin Source File

SOURCE=.\ControlBar\scbarcf.cpp
# End Source File
# Begin Source File

SOURCE=.\ControlBar\scbarg.cpp
# End Source File
# Begin Source File

SOURCE=.\SelectParamDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\Tools\SelSliderCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\Simul.cpp
# End Source File
# Begin Source File

SOURCE=.\MInteraction\SimulAnimat.cpp
# End Source File
# Begin Source File

SOURCE=.\SimulDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\MInteraction\SimulGraph.cpp
# End Source File
# Begin Source File

SOURCE=.\MInteraction\SimulParam.cpp
# End Source File
# Begin Source File

SOURCE=.\MInteraction\SimulPie.cpp
# End Source File
# Begin Source File

SOURCE=.\MInteraction\SimulTable.cpp
# End Source File
# Begin Source File

SOURCE=.\MInteraction\SimulText.cpp
# End Source File
# Begin Source File

SOURCE=.\MInteraction\SimulTimer.cpp
# End Source File
# Begin Source File

SOURCE=.\MSimulation\SimulView.cpp
# End Source File
# Begin Source File

SOURCE=.\MInteraction\SimulXYGraph.cpp
# End Source File
# Begin Source File

SOURCE=.\ControlBar\sizecbar.cpp
# End Source File
# Begin Source File

SOURCE=.\MInstruction\SortERDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\Tools\ST_SplitterWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\MathParser\StrMap.cpp
# End Source File
# Begin Source File

SOURCE=.\MathParser\SymTable.cpp
# End Source File
# Begin Source File

SOURCE=.\GridCtrl\TitleTip.cpp
# End Source File
# Begin Source File

SOURCE=.\Prefs\TraceConfigPage.cpp
# End Source File
# Begin Source File

SOURCE=.\Prefs\TraceInfoPage.cpp
# End Source File
# Begin Source File

SOURCE=.\GridCtrl\TreeColumn\TreeColumn.cpp
# End Source File
# Begin Source File

SOURCE=.\opTree\TreeOptionsCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\Tools\TWScriptEdit.cpp
# End Source File
# Begin Source File

SOURCE=.\UserOutput.cpp
# End Source File
# Begin Source File

SOURCE=.\MSimulation\ViewAggreg.cpp
# End Source File
# Begin Source File

SOURCE=.\MSimulation\ViewBuildModel.cpp
# End Source File
# Begin Source File

SOURCE=.\MInteraction\ViewController.cpp
# End Source File
# Begin Source File

SOURCE=.\MSimulation\ViewListModel.cpp
# End Source File
# Begin Source File

SOURCE=.\MSimulation\ViewModelDiag.cpp
# End Source File
# Begin Source File

SOURCE=.\ViewWelcome.cpp
# End Source File
# Begin Source File

SOURCE=.\VisualFx\VisualFx.cpp
# End Source File
# Begin Source File

SOURCE=.\Tools\xShadeButton.cpp
# End Source File
# Begin Source File

SOURCE=.\MInteraction\XYGraphData.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\AnimCtrl\3DMeterCtrl.h
# End Source File
# Begin Source File

SOURCE=.\MSimulation\AddModelDlg.h
# End Source File
# Begin Source File

SOURCE=.\Animation\AnimControl.h
# End Source File
# Begin Source File

SOURCE=.\Animation\AnimDensityPage.h
# End Source File
# Begin Source File

SOURCE=.\Animation\AnimInterPage.h
# End Source File
# Begin Source File

SOURCE=.\Animation\AnimLinkPage.h
# End Source File
# Begin Source File

SOURCE=.\Animation\AnimObject.h
# End Source File
# Begin Source File

SOURCE=.\Animation\AnimObjectPage.h
# End Source File
# Begin Source File

SOURCE=.\Animation\AnimObjSizePage.h
# End Source File
# Begin Source File

SOURCE=.\Animation\AnimPage.h
# End Source File
# Begin Source File

SOURCE=.\Animation\AnimPictDensPage.h
# End Source File
# Begin Source File

SOURCE=.\Animation\AnimPictPage.h
# End Source File
# Begin Source File

SOURCE=.\Animation\AnimPolygonPage.h
# End Source File
# Begin Source File

SOURCE=.\Animation\AnimSectorPage.h
# End Source File
# Begin Source File

SOURCE=.\Animation\AnimSelectDlg.h
# End Source File
# Begin Source File

SOURCE=.\Animation\AnimShapePage.h
# End Source File
# Begin Source File

SOURCE=.\Animation\AnimTextPage.h
# End Source File
# Begin Source File

SOURCE=.\Animation\AnimValuePage.h
# End Source File
# Begin Source File

SOURCE=.\MSimulation\AuthorModel.h
# End Source File
# Begin Source File

SOURCE=.\Tools\AutoFont.h
# End Source File
# Begin Source File

SOURCE=.\Tools\BCMenu.h
# End Source File
# Begin Source File

SOURCE=.\Tools\Bezier.h
# End Source File
# Begin Source File

SOURCE=.\Tools\BitmapPickerCombo.h
# End Source File
# Begin Source File

SOURCE=.\BPInfoDlg.h
# End Source File
# Begin Source File

SOURCE=.\GridCtrl\GridBtn\BtnDataBase.h
# End Source File
# Begin Source File

SOURCE=.\Tools\BtnST.h
# End Source File
# Begin Source File

SOURCE=.\Tools\Bubble.h
# End Source File
# Begin Source File

SOURCE=.\MSimulation\BuildModelDlg.h
# End Source File
# Begin Source File

SOURCE=.\GridCtrl\CellRange.h
# End Source File
# Begin Source File

SOURCE=.\ChildFrm.h
# End Source File
# Begin Source File

SOURCE=.\Tools\ColorPickerCB.h
# End Source File
# Begin Source File

SOURCE=.\Tools\ColourPicker.h
# End Source File
# Begin Source File

SOURCE=.\Tools\ColourPopup.h
# End Source File
# Begin Source File

SOURCE=.\AnimCtrl\ComponentStatic.h
# End Source File
# Begin Source File

SOURCE=.\Tools\CopyPasteBuffer.h
# End Source File
# Begin Source File

SOURCE=.\AnimCtrl\CRadialCtrl.h
# End Source File
# Begin Source File

SOURCE=.\MInstruction\DefineERPropSheet.h
# End Source File
# Begin Source File

SOURCE=.\MInstruction\DefineExpSetDlg.h
# End Source File
# Begin Source File

SOURCE=.\MInstruction\definelupropsheet.h
# End Source File
# Begin Source File

SOURCE=.\hlp\demist.hm
# End Source File
# Begin Source File

SOURCE=.\MInteraction\DlgFormat.h
# End Source File
# Begin Source File

SOURCE=.\MInteraction\DlgTranslation.h
# End Source File
# Begin Source File

SOURCE=.\MultiSlider\EditInterval.h
# End Source File
# Begin Source File

SOURCE=.\MInteraction\Format.h
# End Source File
# Begin Source File

SOURCE=.\MInteraction\FormatPage.h
# End Source File
# Begin Source File

SOURCE=.\Tools\Graph.h
# End Source File
# Begin Source File

SOURCE=.\Tools\Graphics.h
# End Source File
# Begin Source File

SOURCE=.\GraphInfoDlg.h
# End Source File
# Begin Source File

SOURCE=.\Tools\GraphLegend.h
# End Source File
# Begin Source File

SOURCE=.\Tools\GraphSeries.h
# End Source File
# Begin Source File

SOURCE=.\GridCtrl\GridBtn\GridBtnCell.h
# End Source File
# Begin Source File

SOURCE=.\GridCtrl\GridBtn\GridBtnCellBase.h
# End Source File
# Begin Source File

SOURCE=.\GridCtrl\GridBtn\GridBtnCellCombo.h
# End Source File
# Begin Source File

SOURCE=.\GridCtrl\GridCell.h
# End Source File
# Begin Source File

SOURCE=.\GridCtrl\GridCellBase.h
# End Source File
# Begin Source File

SOURCE=.\GridCtrl\NewCellTypes\GridCellColor.h
# End Source File
# Begin Source File

SOURCE=.\GridCtrl\NewCellTypes\GridCellCombo.h
# End Source File
# Begin Source File

SOURCE=.\GridCtrl\GridCtrl.h
# End Source File
# Begin Source File

SOURCE=.\GridCtrl\GridDropTarget.h
# End Source File
# Begin Source File

SOURCE=.\GridCtrl\GridTree\GridTreeBtnCell.h
# End Source File
# Begin Source File

SOURCE=.\GridCtrl\TreeColumn\GridTreeCell.h
# End Source File
# Begin Source File

SOURCE=.\GridCtrl\TreeColumn\GridTreeCellBase.h
# End Source File
# Begin Source File

SOURCE=.\GridCtrl\NewCellTypes\GridURLCell.h
# End Source File
# Begin Source File

SOURCE=.\Tools\HelpDlg.h
# End Source File
# Begin Source File

SOURCE=.\Tools\HelpPrSheet.h
# End Source File
# Begin Source File

SOURCE=.\HMXChart.h
# End Source File
# Begin Source File

SOURCE=.\HMXDataset.h
# End Source File
# Begin Source File

SOURCE=.\IdentityDlg.h
# End Source File
# Begin Source File

SOURCE=.\GridCtrl\InPlaceEdit.h
# End Source File
# Begin Source File

SOURCE=.\GridCtrl\GridBtn\InPlaceList.h
# End Source File
# Begin Source File

SOURCE=.\MInstruction\InstructView.h
# End Source File
# Begin Source File

SOURCE=.\LearnerTrace.h
# End Source File
# Begin Source File

SOURCE=.\MInstruction\LearningUnit.h
# End Source File
# Begin Source File

SOURCE=.\MathParser\Lexer.h
# End Source File
# Begin Source File

SOURCE=.\Tools\Line.h
# End Source File
# Begin Source File

SOURCE=.\Tools\LinePicker.h
# End Source File
# Begin Source File

SOURCE=.\Tools\LineWnd.h
# End Source File
# Begin Source File

SOURCE=.\MainFrm.h
# End Source File
# Begin Source File

SOURCE=.\MathParser\MathParser.h
# End Source File
# Begin Source File

SOURCE=.\GridCtrl\MemDC.h
# End Source File
# Begin Source File

SOURCE=.\MSimulation\Model.h
# End Source File
# Begin Source File

SOURCE=.\Animation\MRTracker.h
# End Source File
# Begin Source File

SOURCE=.\MultiSlider\MultiSlider.h
# End Source File
# Begin Source File

SOURCE=.\Tools\MvDocTemplate.h
# End Source File
# Begin Source File

SOURCE=.\COptionTree\OptionTree.h
# End Source File
# Begin Source File

SOURCE=.\COptionTree\OptionTreeCheckButton.h
# End Source File
# Begin Source File

SOURCE=.\COptionTree\OptionTreeColorPopUp.h
# End Source File
# Begin Source File

SOURCE=.\COptionTree\OptionTreeDef.h
# End Source File
# Begin Source File

SOURCE=.\COptionTree\OptionTreeIconPopUp.h
# End Source File
# Begin Source File

SOURCE=.\COptionTree\OptionTreeInfo.h
# End Source File
# Begin Source File

SOURCE=.\COptionTree\OptionTreeItem.h
# End Source File
# Begin Source File

SOURCE=.\COptionTree\OptionTreeItemCheckBox.h
# End Source File
# Begin Source File

SOURCE=.\COptionTree\OptionTreeItemColor.h
# End Source File
# Begin Source File

SOURCE=.\COptionTree\OptionTreeItemComboBox.h
# End Source File
# Begin Source File

SOURCE=.\COptionTree\OptionTreeItemDate.h
# End Source File
# Begin Source File

SOURCE=.\COptionTree\OptionTreeItemEdit.h
# End Source File
# Begin Source File

SOURCE=.\COptionTree\OptionTreeItemIcon.h
# End Source File
# Begin Source File

SOURCE=.\COptionTree\OptionTreeItemRadio.h
# End Source File
# Begin Source File

SOURCE=.\COptionTree\OptionTreeItemSpinner.h
# End Source File
# Begin Source File

SOURCE=.\COptionTree\OptionTreeItemStatic.h
# End Source File
# Begin Source File

SOURCE=.\COptionTree\OptionTreeList.h
# End Source File
# Begin Source File

SOURCE=.\COptionTree\OptionTreeRadioButton.h
# End Source File
# Begin Source File

SOURCE=.\COptionTree\OptionTreeSpinnerButton.h
# End Source File
# Begin Source File

SOURCE=.\COptionTree\OptionTreeSpinnerEdit.h
# End Source File
# Begin Source File

SOURCE=.\Prefs\Pref.h
# End Source File
# Begin Source File

SOURCE=.\Prefs\PreferencePage.h
# End Source File
# Begin Source File

SOURCE=.\Prefs\prefsimulpage.h
# End Source File
# Begin Source File

SOURCE=.\MSimulation\PreyPredModel.h
# End Source File
# Begin Source File

SOURCE=.\Tools\PushPin.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h

!IF  "$(CFG)" == "DEMIST - Win32 Release"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Making help include file...
TargetName=demist
InputPath=.\Resource.h

"hlp\$(TargetName).hm" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	echo. >"hlp\$(TargetName).hm" 
	echo // Commands (ID_* and IDM_*) >>"hlp\$(TargetName).hm" 
	makehm ID_,HID_,0x10000 IDM_,HIDM_,0x10000 resource.h >>"hlp\$(TargetName).hm" 
	echo. >>"hlp\$(TargetName).hm" 
	echo // Prompts (IDP_*) >>"hlp\$(TargetName).hm" 
	makehm IDP_,HIDP_,0x30000 resource.h >>"hlp\$(TargetName).hm" 
	echo. >>"hlp\$(TargetName).hm" 
	echo // Resources (IDR_*) >>"hlp\$(TargetName).hm" 
	makehm IDR_,HIDR_,0x20000 resource.h >>"hlp\$(TargetName).hm" 
	echo. >>"hlp\$(TargetName).hm" 
	echo // Dialogs (IDD_*) >>"hlp\$(TargetName).hm" 
	makehm IDD_,HIDD_,0x20000 resource.h >>"hlp\$(TargetName).hm" 
	echo. >>"hlp\$(TargetName).hm" 
	echo // Frame Controls (IDW_*) >>"hlp\$(TargetName).hm" 
	makehm IDW_,HIDW_,0x50000 resource.h >>"hlp\$(TargetName).hm" 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "DEMIST - Win32 Debug"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Making help include file...
TargetName=demist
InputPath=.\Resource.h

"hlp\$(TargetName).hm" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	echo. >"hlp\$(TargetName).hm" 
	echo // Commands (ID_* and IDM_*) >>"hlp\$(TargetName).hm" 
	makehm ID_,HID_,0x10000 IDM_,HIDM_,0x10000 resource.h >>"hlp\$(TargetName).hm" 
	echo. >>"hlp\$(TargetName).hm" 
	echo // Prompts (IDP_*) >>"hlp\$(TargetName).hm" 
	makehm IDP_,HIDP_,0x30000 resource.h >>"hlp\$(TargetName).hm" 
	echo. >>"hlp\$(TargetName).hm" 
	echo // Resources (IDR_*) >>"hlp\$(TargetName).hm" 
	makehm IDR_,HIDR_,0x20000 resource.h >>"hlp\$(TargetName).hm" 
	echo. >>"hlp\$(TargetName).hm" 
	echo // Dialogs (IDD_*) >>"hlp\$(TargetName).hm" 
	makehm IDD_,HIDD_,0x20000 resource.h >>"hlp\$(TargetName).hm" 
	echo. >>"hlp\$(TargetName).hm" 
	echo // Frame Controls (IDW_*) >>"hlp\$(TargetName).hm" 
	makehm IDW_,HIDW_,0x50000 resource.h >>"hlp\$(TargetName).hm" 
	
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\resource.hm
# End Source File
# Begin Source File

SOURCE=.\Tools\Scatter.h
# End Source File
# Begin Source File

SOURCE=.\ControlBar\scbarcf.h
# End Source File
# Begin Source File

SOURCE=.\ControlBar\scbarg.h
# End Source File
# Begin Source File

SOURCE=.\SelectParamDlg.h
# End Source File
# Begin Source File

SOURCE=.\Tools\SelSliderCtrl.h
# End Source File
# Begin Source File

SOURCE=.\Simul.h
# End Source File
# Begin Source File

SOURCE=.\MInteraction\SimulAnimat.h
# End Source File
# Begin Source File

SOURCE=.\SimulDoc.h
# End Source File
# Begin Source File

SOURCE=.\MInteraction\SimulGraph.h
# End Source File
# Begin Source File

SOURCE=.\MInteraction\SimulParam.h
# End Source File
# Begin Source File

SOURCE=.\MInteraction\SimulPie.h
# End Source File
# Begin Source File

SOURCE=.\MInteraction\SimulTable.h
# End Source File
# Begin Source File

SOURCE=.\MInteraction\SimulText.h
# End Source File
# Begin Source File

SOURCE=.\MInteraction\SimulTimer.h
# End Source File
# Begin Source File

SOURCE=.\MSimulation\SimulView.h
# End Source File
# Begin Source File

SOURCE=.\MInteraction\SimulXYGraph.h
# End Source File
# Begin Source File

SOURCE=.\ControlBar\sizecbar.h
# End Source File
# Begin Source File

SOURCE=.\MInstruction\SortERDlg.h
# End Source File
# Begin Source File

SOURCE=.\Tools\ST_SplitterWnd.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\MathParser\StrMap.h
# End Source File
# Begin Source File

SOURCE=.\MathParser\SymTable.h
# End Source File
# Begin Source File

SOURCE=.\GridCtrl\TitleTip.h
# End Source File
# Begin Source File

SOURCE=.\Prefs\TraceConfigPage.h
# End Source File
# Begin Source File

SOURCE=.\Prefs\TraceInfoPage.h
# End Source File
# Begin Source File

SOURCE=.\GridCtrl\TreeColumn\TreeColumn.h
# End Source File
# Begin Source File

SOURCE=.\opTree\TreeOptionsCtrl.h
# End Source File
# Begin Source File

SOURCE=.\Tools\TWScriptEdit.h
# End Source File
# Begin Source File

SOURCE=.\UserOutput.h
# End Source File
# Begin Source File

SOURCE=.\MSimulation\ViewAggreg.h
# End Source File
# Begin Source File

SOURCE=.\MSimulation\ViewBuildModel.h
# End Source File
# Begin Source File

SOURCE=.\MInteraction\ViewController.h
# End Source File
# Begin Source File

SOURCE=.\MSimulation\ViewListModel.h
# End Source File
# Begin Source File

SOURCE=.\MSimulation\ViewModelDiag.h
# End Source File
# Begin Source File

SOURCE=.\ViewWelcome.h
# End Source File
# Begin Source File

SOURCE=.\VisualFx\VisualFx.h
# End Source File
# Begin Source File

SOURCE=.\Tools\xShadeButton.h
# End Source File
# Begin Source File

SOURCE=.\MInteraction\XYGraphData.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\anim_cpn.bmp
# End Source File
# Begin Source File

SOURCE=.\res\anim_sel.bmp
# End Source File
# Begin Source File

SOURCE=.\res\antblack.bmp
# End Source File
# Begin Source File

SOURCE=.\res\antred.bmp
# End Source File
# Begin Source File

SOURCE=.\res\barnacle.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Beany.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bitmap1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00001.bmp
# End Source File
# Begin Source File

SOURCE=.\res\books.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bp_actio.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bp_hypot.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bp_info.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bp_test.bmp
# End Source File
# Begin Source File

SOURCE=.\res\component.bmp
# End Source File
# Begin Source File

SOURCE=.\res\contr_ti.bmp
# End Source File
# Begin Source File

SOURCE=.\res\controll.bmp
# End Source File
# Begin Source File

SOURCE=.\res\delete.cur
# End Source File
# Begin Source File

SOURCE=.\res\Dialbk.bmp
# End Source File
# Begin Source File

SOURCE=.\res\fingreen.bmp
# End Source File
# Begin Source File

SOURCE=.\res\finred.bmp
# End Source File
# Begin Source File

SOURCE=.\res\fond.bmp
# End Source File
# Begin Source File

SOURCE=.\res\funcdiag2.bmp
# End Source File
# Begin Source File

SOURCE=.\res\funcdiag3.bmp
# End Source File
# Begin Source File

SOURCE=.\res\funcdiag4.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ico00001.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00003.ico
# End Source File
# Begin Source File

SOURCE=.\res\iconnext.ico
# End Source File
# Begin Source File

SOURCE=.\res\iconprev.ico
# End Source File
# Begin Source File

SOURCE=.\res\iconrun.ico
# End Source File
# Begin Source File

SOURCE=.\res\idr_book.ico
# End Source File
# Begin Source File

SOURCE=.\res\idr_desi.ico
# End Source File
# Begin Source File

SOURCE=.\res\idr_glass.ico
# End Source File
# Begin Source File

SOURCE=.\res\idr_glog.ico
# End Source File
# Begin Source File

SOURCE=.\res\idr_node.ico
# End Source File
# Begin Source File

SOURCE=.\res\idr_ruler.ico
# End Source File
# Begin Source File

SOURCE=.\res\idr_simu.ico
# End Source File
# Begin Source File

SOURCE=.\res\idr_therm.ico
# End Source File
# Begin Source File

SOURCE=.\res\idr_view.ico
# End Source File
# Begin Source File

SOURCE=.\res\idr_welc.ico
# End Source File
# Begin Source File

SOURCE=.\res\mainfram.bmp
# End Source File
# Begin Source File

SOURCE=.\mdi.ico
# End Source File
# Begin Source File

SOURCE=.\res\mdianim.ico
# End Source File
# Begin Source File

SOURCE=.\res\mdichart.ico
# End Source File
# Begin Source File

SOURCE=.\res\mdictrl.ico
# End Source File
# Begin Source File

SOURCE=.\res\mdigraph.ico
# End Source File
# Begin Source File

SOURCE=.\res\mdihisto.ico
# End Source File
# Begin Source File

SOURCE=.\res\mdiparam.ico
# End Source File
# Begin Source File

SOURCE=.\res\mditable.ico
# End Source File
# Begin Source File

SOURCE=.\res\mditext.ico
# End Source File
# Begin Source File

SOURCE=.\res\mditimer.ico
# End Source File
# Begin Source File

SOURCE=.\res\mdixygraph.ico
# End Source File
# Begin Source File

SOURCE=.\res\MERseq.ico
# End Source File
# Begin Source File

SOURCE=.\res\mersort.bmp
# End Source File
# Begin Source File

SOURCE=.\res\mod_TWCC.bmp
# End Source File
# Begin Source File

SOURCE=.\res\model_de.bmp
# End Source File
# Begin Source File

SOURCE=.\res\model_er.bmp
# End Source File
# Begin Source File

SOURCE=.\res\model_ic.bmp
# End Source File
# Begin Source File

SOURCE=.\res\model_lu.bmp
# End Source File
# Begin Source File

SOURCE=.\res\pushpin.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ratkang.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ratpock.bmp
# End Source File
# Begin Source File

SOURCE=.\res\round.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Simul.ico
# End Source File
# Begin Source File

SOURCE=.\res\Simul.rc2
# End Source File
# Begin Source File

SOURCE=.\res\SimulDoc.ico
# End Source File
# Begin Source File

SOURCE=.\res\smallcpn.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Smokes.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Splitter.cur
# End Source File
# Begin Source File

SOURCE=.\res\square.bmp
# End Source File
# Begin Source File

SOURCE=.\res\taskbar.bmp
# End Source File
# Begin Source File

SOURCE=.\res\taskbar1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\time_add.bmp
# End Source File
# Begin Source File

SOURCE=.\res\time_bre.bmp
# End Source File
# Begin Source File

SOURCE=.\res\time_smb.bmp
# End Source File
# Begin Source File

SOURCE=.\res\toolani.bmp
# End Source File
# Begin Source File

SOURCE=.\res\toolani_align.bmp
# End Source File
# Begin Source File

SOURCE=.\res\toolani_cpnt.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Toolbar.bmp
# End Source File
# Begin Source File

SOURCE=.\res\toolgraph.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ToolInst.bmp
# End Source File
# Begin Source File

SOURCE=.\res\toolpar.bmp
# End Source File
# Begin Source File

SOURCE=.\res\tooltimer.bmp
# End Source File
# Begin Source File

SOURCE=.\res\tree_ctrl.bmp
# End Source File
# Begin Source File

SOURCE=.\res\viewicon.ico
# End Source File
# Begin Source File

SOURCE=.\res\viewlist.ico
# End Source File
# End Group
# Begin Group "Help Files"

# PROP Default_Filter "cnt;rtf"
# Begin Source File

SOURCE=.\hlp\AfxCore.rtf
# End Source File
# Begin Source File

SOURCE=.\hlp\AfxPrint.rtf
# End Source File
# Begin Source File

SOURCE=.\hlp\AppExit.bmp
# End Source File
# Begin Source File

SOURCE=.\hlp\Bullet.bmp
# End Source File
# Begin Source File

SOURCE=.\hlp\CurArw2.bmp
# End Source File
# Begin Source File

SOURCE=.\hlp\CurArw4.bmp
# End Source File
# Begin Source File

SOURCE=.\hlp\CurHelp.bmp
# End Source File
# Begin Source File

SOURCE=.\hlp\demist.cnt

!IF  "$(CFG)" == "DEMIST - Win32 Release"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Copying contents file...
OutDir=.\Release
InputPath=.\hlp\demist.cnt
InputName=demist

"$(OutDir)\$(InputName).cnt" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy "hlp\$(InputName).cnt" $(OutDir)

# End Custom Build

!ELSEIF  "$(CFG)" == "DEMIST - Win32 Debug"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Copying contents file...
OutDir=.\Debug
InputPath=.\hlp\demist.cnt
InputName=demist

"$(OutDir)\$(InputName).cnt" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy "hlp\$(InputName).cnt" $(OutDir)

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\hlp\EditCopy.bmp
# End Source File
# Begin Source File

SOURCE=.\hlp\EditCut.bmp
# End Source File
# Begin Source File

SOURCE=.\hlp\EditPast.bmp
# End Source File
# Begin Source File

SOURCE=.\hlp\EditUndo.bmp
# End Source File
# Begin Source File

SOURCE=.\hlp\FileNew.bmp
# End Source File
# Begin Source File

SOURCE=.\hlp\FileOpen.bmp
# End Source File
# Begin Source File

SOURCE=.\hlp\FilePrnt.bmp
# End Source File
# Begin Source File

SOURCE=.\hlp\FileSave.bmp
# End Source File
# Begin Source File

SOURCE=.\hlp\HlpSBar.bmp
# End Source File
# Begin Source File

SOURCE=.\hlp\HlpTBar.bmp
# End Source File
# Begin Source File

SOURCE=.\hlp\RecFirst.bmp
# End Source File
# Begin Source File

SOURCE=.\hlp\RecLast.bmp
# End Source File
# Begin Source File

SOURCE=.\hlp\RecNext.bmp
# End Source File
# Begin Source File

SOURCE=.\hlp\RecPrev.bmp
# End Source File
# Begin Source File

SOURCE=.\hlp\Scmax.bmp
# End Source File
# Begin Source File

SOURCE=.\hlp\ScMenu.bmp
# End Source File
# Begin Source File

SOURCE=.\hlp\Scmin.bmp
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# Begin Source File

SOURCE=.\Simul.reg
# End Source File
# Begin Source File

SOURCE=.\ToDo.txt
# End Source File
# End Target
# End Project
