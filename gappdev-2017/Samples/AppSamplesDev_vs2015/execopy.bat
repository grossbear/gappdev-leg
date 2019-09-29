rem mm_exe_copy.bat <solution dir> <configuration name> <out dir> <target file name>
rem 
rem where:
rem solution dir - is solution directory. VS $(SolutionDir)
rem configuration name - project configuration name: debug/release. VS $(ConfigurationName)
rem out dir - project outout directory. VS $(OutDir)
rem target file name - path of the exe outputed by project. VS $(TargetFileName)

set SolutionDir=%1
set SolutionDir=%SolutionDir:"=%
set ConfigurationName=%2
set ConfigurationName=%ConfigurationName:"=%
set OutDir=%3
set OutDir=%OutDir:"=%
set TargetFileName=%4
set TargetFileName=%TargetFileName:"=%

@echo copy exe file to bin directory

set CurrDir = %CD%
cd %SolutionDir%
echo %CD%
cd..
cd..
cd Bin
set BinDirName=%CD%
set BinDirName=%BinDirName:"=%
copy "%OutDir%\%TargetFileName%" "%BinDirName%\%TargetFileName%"