REM set target=cx16
set target=c64

set vice128emu="%onedriveconsumer%\ClassicComputing\C64\WinVice (GTK)\GTK3VICE-3.5-win64\bin\x128"
set vice64emu="%onedriveconsumer%\ClassicComputing\C64\WinVice (GTK)\GTK3VICE-3.5-win64\bin\x64sc"
set cx16emu="%onedriveconsumer%\ClassicComputing\C-X16\x16emu"
set cc65="%onedriveconsumer%\ClassicComputing\cc65"
set cc65=%cc65:"=%

%cc65%\bin\cc65 -Orsi -t %target% screentools.c >screentools_cc_output.txt
%cc65%\bin\cc65 -Orsi -t %target% retroeditmain.c >retroeditmain_cc_output.txt

del screentools.asm
del retroeditmain.asm
rename screentools.s screentools.asm
rename retroeditmain.s retroeditmain.asm

%cc65%\bin\cl65 -Orsi -t %target% screentools.c retroeditmain.c -o retroeditmain >retroeditmain_cl_output.txt -Wl "--mapfile,retroeditmain.map" -Wl "--dbgfile,retroeditmain.dbg"

del RETROEDIT.PRG
rename retroeditmain RETROEDIT.PRG

if "%target%"=="cx16" (
    %cx16emu% -prg RETROEDIT.PRG -run
)

if "%target%"=="c64" (
    %vice64emu% RETROEDIT.PRG
)
