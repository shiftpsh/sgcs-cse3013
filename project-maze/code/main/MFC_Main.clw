; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CMFC_MainView
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "MFC_Main.h"
LastPage=0

ClassCount=5
Class1=CMFC_MainApp
Class2=CMFC_MainDoc
Class3=CMFC_MainView
Class4=CMainFrame

ResourceCount=2
Resource1=IDR_MAINFRAME
Class5=CAboutDlg
Resource2=IDD_ABOUTBOX

[CLS:CMFC_MainApp]
Type=0
HeaderFile=MFC_Main.h
ImplementationFile=MFC_Main.cpp
Filter=N
BaseClass=CWinApp
VirtualFilter=AC
LastObject=CMFC_MainApp

[CLS:CMFC_MainDoc]
Type=0
HeaderFile=MFC_MainDoc.h
ImplementationFile=MFC_MainDoc.cpp
Filter=N
BaseClass=CDocument
VirtualFilter=DC
LastObject=ID_FILE_CLOSE

[CLS:CMFC_MainView]
Type=0
HeaderFile=MFC_MainView.h
ImplementationFile=MFC_MainView.cpp
Filter=C
BaseClass=CView
VirtualFilter=VWC
LastObject=CMFC_MainView


[CLS:CMainFrame]
Type=0
HeaderFile=MainFrm.h
ImplementationFile=MainFrm.cpp
Filter=T
BaseClass=CFrameWnd
VirtualFilter=fWC
LastObject=ID_FILE_CLOSE




[CLS:CAboutDlg]
Type=0
HeaderFile=MFC_Main.cpp
ImplementationFile=MFC_Main.cpp
Filter=D

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[MNU:IDR_MAINFRAME]
Type=1
Class=CMainFrame
Command1=ID_FILE_OPEN
Command2=ID_FILE_SAVE
Command3=ID_FILE_SAVE_AS
Command4=ID_FILE_CLOSE
Command5=ID_FILE_PRINT
Command6=ID_FILE_PRINT_PREVIEW
Command7=ID_FILE_PRINT_SETUP
Command8=ID_FILE_MRU_FILE1
Command9=ID_APP_EXIT
Command10=ID_VIEW_FULLVIEW
Command11=ID_VIEW_TOOLBAR
Command12=ID_VIEW_STATUS_BAR
Command13=ID_TOOLS_GO
Command14=ID_APP_ABOUT
CommandCount=14

[ACL:IDR_MAINFRAME]
Type=1
Class=CMainFrame
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_FILE_PRINT
Command5=ID_EDIT_UNDO
Command6=ID_EDIT_CUT
Command7=ID_EDIT_COPY
Command8=ID_EDIT_PASTE
Command9=ID_EDIT_UNDO
Command10=ID_EDIT_CUT
Command11=ID_EDIT_COPY
Command12=ID_EDIT_PASTE
Command13=ID_NEXT_PANE
Command14=ID_PREV_PANE
CommandCount=14

[TB:IDR_MAINFRAME]
Type=1
Class=?
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_VIEW_FULLVIEW
Command5=ID_VIEW_ZOOM
Command6=ID_TOOLS_GO
Command7=ID_FILE_PRINT
Command8=ID_APP_ABOUT
CommandCount=8

