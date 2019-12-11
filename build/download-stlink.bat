@set path=..\BB_TOOLS\stm32toolchain\bin;%path%
@ST-LINK_CLI.exe -c SWD UR -Rst -ME -P ..\Source\build\biobus_fw.hex  -V -Rst -Run
cmd