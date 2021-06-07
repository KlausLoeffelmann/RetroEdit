REM set target=cx16
set target=c64

%onedriveconsumer%\C64\Dev\cc65\bin\cc65 -Orsi -t %target% screentools.c >screentools_cc_output.txt
%onedriveconsumer%\C64\Dev\cc65\bin\cc65 -Orsi -t %target% retroeditmain.c >retroeditmain_cc_output.txt
del screentools.asm
del retroeditmain.asm
rename screentools.s screentools.asm
rename retroeditmain.s retroeditmain.asm

%onedriveconsumer%\C64\Dev\cc65\bin\cl65 -Orsi -t %target% screentools.c retroeditmain.c -o retroeditmain >retroeditmain_cl_output.txt -Wl "--mapfile,retroeditmain.map" -Wl "--dbgfile,retroeditmain.dbg"
del RETROEDIT.PRG
rename retroeditmain RETROEDIT.PRG
copy RETROEDIT.PRG %onedriveconsumer%\C64\C-X16\Emulator\RETROEDIT.PRG

if "%target%"=="cx16" (
%onedriveconsumer%\C64\C-X16\Emulator\x16emu.exe -prg X16EDITMAIN.PRG -run
)


