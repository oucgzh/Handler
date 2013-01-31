Jeffrey Guan 2013.01.20

A、Requirement needed:

       System: Win7
       Tools:  VS2010,DirectX 10 sdk
  
B、Before using this .sln,configure it by  the following steps:

       1、Add the libs(dxguid.lib,dinput8.lib,comctl32.lib)to the project(vs2010->project->properties->Linker->Input->Additional Dependencies)

       2、Project->Properties->Executable Directories->($ExecutPath)\Utilities\bin\x86

       3、Project->Properties->Include Directories->($IncludePath)\Include

       4、Project->Properties->Library Directories->($LibPath)\Lib\x86



     P.S.:  If you get the error says "LINK : fatal error LNK1104: cannot open file 'dxguid.lib'" ,it shows that you did'nt configure the project according to what I have said above.


