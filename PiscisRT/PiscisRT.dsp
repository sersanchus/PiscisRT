# Microsoft Developer Studio Project File - Name="PiscisRT" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=PiscisRT - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "PiscisRT.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "PiscisRT.mak" CFG="PiscisRT - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "PiscisRT - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "PiscisRT - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "PiscisRT - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "lib"
# PROP Intermediate_Dir "lib"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "WINDOWSVC6_EXPORTS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "WINDOWSVC6_EXPORTS" /YX /FD /c
# SUBTRACT CPP /Fr
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0xc0a /d "NDEBUG"
# ADD RSC /l 0xc0a /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# SUBTRACT LINK32 /pdb:none
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=copy lib\PiscisRT.dll ..\Elements\out\PiscisRT.dll
# End Special Build Tool

!ELSEIF  "$(CFG)" == "PiscisRT - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "lib"
# PROP Intermediate_Dir "lib"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "WINDOWSVC6_EXPORTS" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "WINDOWSVC6_EXPORTS" /FR /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0xc0a /d "_DEBUG"
# ADD RSC /l 0xc0a /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# SUBTRACT LINK32 /pdb:none
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=copy lib\PiscisRT.dll ..\Elements\out\PiscisRT.dll
# End Special Build Tool

!ENDIF 

# Begin Target

# Name "PiscisRT - Win32 Release"
# Name "PiscisRT - Win32 Debug"
# Begin Group "Object"

# PROP Default_Filter "cpp"
# Begin Source File

SOURCE=.\source\PRTCircle.cpp
# End Source File
# Begin Source File

SOURCE=.\source\PRTCircle.h
# End Source File
# Begin Source File

SOURCE=.\source\PRTCone.cpp
# End Source File
# Begin Source File

SOURCE=.\source\PRTCone.h
# End Source File
# Begin Source File

SOURCE=.\source\PRTCsgObject.cpp
# End Source File
# Begin Source File

SOURCE=.\source\PRTCsgObject.h
# End Source File
# Begin Source File

SOURCE=.\source\PRTCylinder.cpp
# End Source File
# Begin Source File

SOURCE=.\source\PRTCylinder.h
# End Source File
# Begin Source File

SOURCE=.\source\PRTLine.cpp
# End Source File
# Begin Source File

SOURCE=.\source\PRTLine.h
# End Source File
# Begin Source File

SOURCE=.\source\PRTMesh.cpp
# End Source File
# Begin Source File

SOURCE=.\source\PRTMesh.h
# End Source File
# Begin Source File

SOURCE=.\source\PRTObject.cpp
# End Source File
# Begin Source File

SOURCE=.\source\PRTObject.h
# End Source File
# Begin Source File

SOURCE=.\source\PRTPlane.cpp
# End Source File
# Begin Source File

SOURCE=.\source\PRTPlane.h
# End Source File
# Begin Source File

SOURCE=.\source\PRTPolygon.cpp
# End Source File
# Begin Source File

SOURCE=.\source\PRTPolygon.h
# End Source File
# Begin Source File

SOURCE=.\source\PRTQuadric.cpp
# End Source File
# Begin Source File

SOURCE=.\source\PRTQuadric.h
# End Source File
# Begin Source File

SOURCE=.\source\PRTQuartic.cpp
# End Source File
# Begin Source File

SOURCE=.\source\PRTQuartic.h
# End Source File
# Begin Source File

SOURCE=.\source\PRTQuatJulia.cpp
# End Source File
# Begin Source File

SOURCE=.\source\PRTQuatJulia.h
# End Source File
# Begin Source File

SOURCE=.\source\PRTRefObject.cpp
# End Source File
# Begin Source File

SOURCE=.\source\PRTRefObject.h
# End Source File
# Begin Source File

SOURCE=.\source\PRTSphere.cpp
# End Source File
# Begin Source File

SOURCE=.\source\PRTSphere.h
# End Source File
# Begin Source File

SOURCE=.\source\PRTTriangle.cpp
# End Source File
# Begin Source File

SOURCE=.\source\PRTTriangle.h
# End Source File
# End Group
# Begin Group "Data"

# PROP Default_Filter "cpp"
# Begin Source File

SOURCE=.\source\PRTConvexHull.cpp
# End Source File
# Begin Source File

SOURCE=.\source\PRTConvexHull.h
# End Source File
# Begin Source File

SOURCE=.\source\PRTDinamicList.cpp
# End Source File
# Begin Source File

SOURCE=.\source\PRTDinamicList.h
# End Source File
# Begin Source File

SOURCE=.\source\PRTMatrix.cpp
# End Source File
# Begin Source File

SOURCE=.\source\PRTMatrix.h
# End Source File
# Begin Source File

SOURCE=.\source\PRTOcTree.cpp
# End Source File
# Begin Source File

SOURCE=.\source\PRTOcTree.h
# End Source File
# Begin Source File

SOURCE=.\source\PRTPhotonMap.cpp
# End Source File
# Begin Source File

SOURCE=.\source\PRTPhotonMap.h
# End Source File
# Begin Source File

SOURCE=.\source\PRTQuaternion.cpp
# End Source File
# Begin Source File

SOURCE=.\source\PRTQuaternion.h
# End Source File
# Begin Source File

SOURCE=.\source\PRTVector.cpp
# End Source File
# Begin Source File

SOURCE=.\source\PRTVector.h
# End Source File
# End Group
# Begin Group "Light"

# PROP Default_Filter "cpp"
# Begin Source File

SOURCE=.\source\PRTDirectionalLight.cpp
# End Source File
# Begin Source File

SOURCE=.\source\PRTDirectionalLight.h
# End Source File
# Begin Source File

SOURCE=.\source\PRTLight.cpp
# End Source File
# Begin Source File

SOURCE=.\source\PRTLight.h
# End Source File
# Begin Source File

SOURCE=.\source\PRTPointLight.cpp
# End Source File
# Begin Source File

SOURCE=.\source\PRTPointLight.h
# End Source File
# Begin Source File

SOURCE=.\source\PRTSphereLight.cpp
# End Source File
# Begin Source File

SOURCE=.\source\PRTSphereLight.h
# End Source File
# Begin Source File

SOURCE=.\source\PRTTriangleLight.cpp
# End Source File
# Begin Source File

SOURCE=.\source\PRTTriangleLight.h
# End Source File
# End Group
# Begin Group "Material"

# PROP Default_Filter "cpp"
# Begin Source File

SOURCE=.\source\PRTBitmap.cpp
# End Source File
# Begin Source File

SOURCE=.\source\PRTBitmap.h
# End Source File
# Begin Source File

SOURCE=.\source\PRTMaterial.cpp
# End Source File
# Begin Source File

SOURCE=.\source\PRTMaterial.h
# End Source File
# Begin Source File

SOURCE=.\source\PRTPerlin.cpp
# End Source File
# Begin Source File

SOURCE=.\source\PRTPerlin.h
# End Source File
# Begin Source File

SOURCE=.\source\PRTTexCoord.cpp
# End Source File
# Begin Source File

SOURCE=.\source\PRTTexCoord.h
# End Source File
# Begin Source File

SOURCE=.\source\PRTTexture.cpp
# End Source File
# Begin Source File

SOURCE=.\source\PRTTexture.h
# End Source File
# End Group
# Begin Group "Main"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\source\PRTMain.cpp
# End Source File
# Begin Source File

SOURCE=.\source\PRTMain.h
# End Source File
# Begin Source File

SOURCE=.\source\PRTMath.cpp
# End Source File
# Begin Source File

SOURCE=.\source\PRTMath.h
# End Source File
# Begin Source File

SOURCE=.\source\PRTRandom.cpp
# End Source File
# Begin Source File

SOURCE=.\source\PRTRandom.h
# End Source File
# Begin Source File

SOURCE=.\source\PRTRay.cpp
# End Source File
# Begin Source File

SOURCE=.\source\PRTRay.h
# End Source File
# Begin Source File

SOURCE=.\source\PRTThread.cpp
# End Source File
# Begin Source File

SOURCE=.\source\PRTThread.h
# End Source File
# Begin Source File

SOURCE=.\source\PRTUtil.cpp
# End Source File
# Begin Source File

SOURCE=.\source\PRTUtil.h
# End Source File
# End Group
# Begin Group "Plugin"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\source\PRTBitmapImpExp.cpp
# End Source File
# Begin Source File

SOURCE=.\source\PRTBitmapImpExp.h
# End Source File
# Begin Source File

SOURCE=.\source\PRTMeshImpExp.cpp
# End Source File
# Begin Source File

SOURCE=.\source\PRTMeshImpExp.h
# End Source File
# Begin Source File

SOURCE=.\source\PRTPlugin.cpp
# End Source File
# Begin Source File

SOURCE=.\source\PRTPlugin.h
# End Source File
# End Group
# Begin Group "Render"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\source\PRTPatches.cpp
# End Source File
# Begin Source File

SOURCE=.\source\PRTPatches.h
# End Source File
# Begin Source File

SOURCE=.\source\PRTPhotonMapping.cpp
# End Source File
# Begin Source File

SOURCE=.\source\PRTPhotonMapping.h
# End Source File
# Begin Source File

SOURCE=.\source\PRTRayTracing.cpp
# End Source File
# Begin Source File

SOURCE=.\source\PRTRayTracing.h
# End Source File
# Begin Source File

SOURCE=.\source\PRTRender.cpp
# End Source File
# Begin Source File

SOURCE=.\source\PRTRender.h
# End Source File
# End Group
# Begin Group "External Libs"

# PROP Default_Filter ""
# End Group
# End Target
# End Project
