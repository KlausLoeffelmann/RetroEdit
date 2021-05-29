%onedriveconsumer%\C64\Dev\cc65\bin\cc65 -Orsi -t cx16 x16EditMain.c >x16EditMain_cc_output.txt
del x16EditMain.asm
rename x16EditMain.s x16EditMain.asm
%onedriveconsumer%\C64\Dev\cc65\bin\cl65 -Orsi -t cx16 screentools.c x16EditMain.c -o x16EditMain >x16EditMain_cl_output.txt -Wl "--mapfile,x16EditMain.map" -Wl "--dbgfile,x16EditMain.dbg"
del x16EditMain.prg
rename x16EditMain X16EDITMAIN.PRG
copy x16EditMain.prg %onedriveconsumer%\C64\C-X16\Emulator\X16EDITMAIN.PRG
%onedriveconsumer%\C64\C-X16\Emulator\x16emu.exe -prg X16EDITMAIN.PRG -run
