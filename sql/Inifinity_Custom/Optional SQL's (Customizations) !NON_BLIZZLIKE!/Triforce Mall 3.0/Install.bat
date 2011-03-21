:top
@echo off
echo.
color C
echo  Please enter your MySQL info
echo.
set /p host=  MySQL Server Address (Default = localhost): 
if %host%. == . set host=localhost
set /p user=  MySQL Username (Default = root): 
if %user%. == . set user=root
set /p pass=  MySQL Password (Default = ): 
if %pass%. == . set pass=
set /p mangos=  Mangos World Database (Default = mangos): 
if %mangos%. == . set mangos=mangos
set /p arcemu=  ArcEmu World Database (Default = arcemu): 
if %arcemu%. == . set arcemu=arcemu
set /p trinity=  Trinity World Database (Default = trinity): 
if %trinity%. == . set trinity=trinity
set port=3306
set mysqlpath=.\mysql\

:begin
CLS
echo.
echo          /\
echo         /  \
echo        /    \
echo       /______\
echo      /\      /\ Triforce Mall - Version 3
echo     /  \    /  \  Updated by Infinity_Project
echo    /    \  /    \
echo   /______\/______\
echo.
echo 	M - Mangos
echo 	A - ArcEmu
echo 	T - Trinity
echo.
set /p a=	Enter a letter: 
if %a%==* goto error
if %a%==m goto mangos
if %a%==M goto mangos
if %a%==a goto arcemu
if %a%==A goto arcemu
if %a%==t goto trinity
if %a%==T goto trinity
set a=
goto error

:mangos
CLS
echo.
echo          /\
echo         /  \
echo        /    \
echo       /______\
echo      /\      /\ Triforce Mall - Version 3 (Mangos)
echo     /  \    /  \  Updated by Infinity_Project
echo    /    \  /    \
echo   /______\/______\
echo.
echo 	I - Import Vendor Data
echo 	S - Spawn Vendors
echo 	D - Despawn Vendors
echo 	E - Remove Everything
echo 	B - Back
echo.
set /p b=	Enter a letter: 
if %b%==* goto error
if %b%==i goto mangos_import
if %b%==I goto mangos_import
if %b%==s goto mangos_spawn
if %b%==S goto mangos_spawn
if %b%==d goto mangos_despawn
if %b%==D goto mangos_despawn
if %b%==e goto mangos_remove
if %b%==E goto mangos_remove
if %b%==b goto begin
if %b%==B goto begin
set b=
goto error

:mangos_import
CLS
echo.
echo  Importing...
echo.
for %%C in (.\development\vendors_mangos.sql) do (
echo  Import: %%~nxC
%mysqlpath%\mysql --host=%host% --user=%user% --password=%pass% --port=%port% %mangos% < "%%~fC"
)
echo.
for %%C in (.\development\vendordata_mangos.sql) do (
echo  Import: %%~nxC
%mysqlpath%\mysql --host=%host% --user=%user% --password=%pass% --port=%port% %mangos% < "%%~fC"
)
echo.
echo  Done.
echo.
echo  Press any key to exit.
pause >NUL
goto mangos

:mangos_spawn
CLS
echo.
echo          /\
echo         /  \
echo        /    \
echo       /______\
echo      /\      /\ Triforce Mall - Version 3 (Mangos)
echo     /  \    /  \   updated by infinity project
echo    /    \  /    \
echo   /______\/______\
echo.
echo 	1 - GM Island
echo 	2 - (Not implemented yet)
echo 	3 - (Not implemented yet)
echo 	B - Back
echo.
set /p c=	Enter a letter: 
if %c%==* goto error
if %c%==1 goto mangos_spawn1
if %c%==2 goto mangos_spawn2
if %c%==3 goto mangos_spawn3
if %c%==b goto mangos
if %c%==B goto mangos
set c=
goto error

:mangos_spawn1
CLS
echo.
echo  Spawning Vendors...
echo.
for %%C in (.\development\spawn_gmisland_mangos.sql) do (
echo  Import: %%~nxC
%mysqlpath%\mysql --host=%host% --user=%user% --password=%pass% --port=%port% %mangos% < "%%~fC"
)
echo.
echo  Done.
echo.
echo  Press any key to exit.
pause >NUL
goto mangos_spawn

:mangos_spawn2
CLS
echo.
echo  Spawning Vendors...
echo.
for %%C in (.\development\spawn_gmisland_mangos.sql) do (
echo  Import: %%~nxC
%mysqlpath%\mysql --host=%host% --user=%user% --password=%pass% --port=%port% %mangos% < "%%~fC"
)
echo.
echo  Done.
echo.
echo  Press any key to exit.
pause >NUL
goto mangos_spawn

:mangos_spawn3
CLS
echo.
echo  Spawning Vendors...
echo.
for %%C in (.\development\spawn_gmisland_mangos.sql) do (
echo  Import: %%~nxC
%mysqlpath%\mysql --host=%host% --user=%user% --password=%pass% --port=%port% %mangos% < "%%~fC"
)
echo.
echo  Done.
echo.
echo  Press any key to exit.
pause >NUL
goto mangos_spawn

:mangos_despawn
CLS
echo.
echo  Despawning Vendors...
echo.
for %%C in (.\development\despawn_mangos.sql) do (
echo  Import: %%~nxC
%mysqlpath%\mysql --host=%host% --user=%user% --password=%pass% --port=%port% %mangos% < "%%~fC"
)
echo.
echo  Done.
echo.
echo  Press any key to exit.
pause >NUL
goto mangos

:mangos_remove
CLS
echo.
echo  Removing Triforce Mall...
echo.
for %%C in (.\development\removeall_mangos.sql) do (
echo  Import: %%~nxC
%mysqlpath%\mysql --host=%host% --user=%user% --password=%pass% --port=%port% %mangos% < "%%~fC"
)
echo.
echo  Done.
echo.
echo  Press any key to exit.
pause >NUL
goto mangos

:arcemu
CLS
echo.
echo          /\
echo         /  \
echo        /    \
echo       /______\
echo      /\      /\ Triforce Mall - Version 3 (ArcEmu)
echo     /  \    /  \
echo    /    \  /    \
echo   /______\/______\
echo.
echo 	I - Import Vendor Data
echo 	S - Spawn Vendors
echo 	D - Despawn Vendors
echo 	E - Remove Everything
echo 	B - Back
echo.
set /p d=	Enter a letter: 
if %d%==* goto error
if %d%==i goto arcemu_import
if %d%==I goto arcemu_import
if %d%==s goto arcemu_spawn
if %d%==S goto arcemu_spawn
if %d%==d goto arcemu_despawn
if %d%==D goto arcemu_despawn
if %d%==e goto arcemu_remove
if %d%==E goto arcemu_remove
if %d%==b goto begin
if %d%==B goto begin
set d=
goto error

:arcemu_import
CLS
echo.
echo  Importing...
echo.
for %%C in (.\development\vendors_arcemu.sql) do (
echo  Import: %%~nxC
%mysqlpath%\mysql --host=%host% --user=%user% --password=%pass% --port=%port% %arcemu% < "%%~fC"
)
echo.
for %%C in (.\development\vendordata_arcemu.sql) do (
echo  Import: %%~nxC
%mysqlpath%\mysql --host=%host% --user=%user% --password=%pass% --port=%port% %arcemu% < "%%~fC"
)
echo.
echo  Done.
echo.
echo  Press any key to exit.
pause >NUL
goto arcemu

:arcemu_spawn
CLS
echo.
echo          /\
echo         /  \
echo        /    \
echo       /______\
echo      /\      /\ Triforce Mall - Version 3 (ArcEmu)
echo     /  \    /  \
echo    /    \  /    \
echo   /______\/______\
echo.
echo 	1 - GM Island
echo 	2 - (Not implemented yet)
echo 	3 - (Not implemented yet)
echo 	B - Back
echo.
set /p e=	Enter a letter: 
if %e%==* goto error
if %e%==1 goto arcemu_spawn1
if %e%==2 goto arcemu_spawn2
if %e%==3 goto arcemu_spawn3
if %e%==b goto arcemu
if %e%==B goto arcemu
set e=
goto error

:arcemu_spawn1
CLS
echo.
echo  Spawning Vendors...
echo.
for %%C in (.\development\spawn_gmisland_arcemu.sql) do (
echo  Import: %%~nxC
%mysqlpath%\mysql --host=%host% --user=%user% --password=%pass% --port=%port% %arcemu% < "%%~fC"
)
echo.
echo  Done.
echo.
echo  Press any key to exit.
pause >NUL
goto arcemu_spawn

:arcemu_spawn2
CLS
echo.
echo  Spawning Vendors...
echo.
for %%C in (.\development\spawn_gmisland_arcemu.sql) do (
echo  Import: %%~nxC
%mysqlpath%\mysql --host=%host% --user=%user% --password=%pass% --port=%port% %arcemu% < "%%~fC"
)
echo.
echo  Done.
echo.
echo  Press any key to exit.
pause >NUL
goto arcemu_spawn

:arcemu_spawn3
CLS
echo.
echo  Spawning Vendors...
echo.
for %%C in (.\development\spawn_gmisland_arcemu.sql) do (
echo  Import: %%~nxC
%mysqlpath%\mysql --host=%host% --user=%user% --password=%pass% --port=%port% %arcemu% < "%%~fC"
)
echo.
echo  Done.
echo.
echo  Press any key to exit.
pause >NUL
goto arcemu_spawn

:arcemu_despawn
CLS
echo.
echo  Despawning Vendors...
echo.
for %%C in (.\development\despawn_arcemu.sql) do (
echo  Import: %%~nxC
%mysqlpath%\mysql --host=%host% --user=%user% --password=%pass% --port=%port% %arcemu% < "%%~fC"
)
echo.
echo  Done.
echo.
echo  Press any key to exit.
pause >NUL
goto arcemu

:arcemu_remove
CLS
echo.
echo  Removing Triforce Mall...
echo.
for %%C in (.\development\removeall_arcemu.sql) do (
echo  Import: %%~nxC
%mysqlpath%\mysql --host=%host% --user=%user% --password=%pass% --port=%port% %arcemu% < "%%~fC"
)
echo.
echo  Done.
echo.
echo  Press any key to exit.
pause >NUL
goto arcemu

:trinity
CLS
echo.
echo          /\
echo         /  \
echo        /    \
echo       /______\
echo      /\      /\ Triforce Mall - Version 3 (Trinity)
echo     /  \    /  \
echo    /    \  /    \
echo   /______\/______\
echo.
echo 	I - Import Vendor Data
echo 	S - Spawn Vendors
echo 	D - Despawn Vendors
echo 	E - Remove Everything
echo 	B - Back
echo.
set /p f=	Enter a letter: 
if %f%==* goto error
if %f%==i goto trinity_import
if %f%==I goto trinity_import
if %f%==s goto trinity_spawn
if %f%==S goto trinity_spawn
if %f%==d goto trinity_despawn
if %f%==D goto trinity_despawn
if %f%==e goto trinity_remove
if %f%==E goto trinity_remove
if %f%==b goto begin
if %f%==B goto begin
set f=
goto error

:trinity_import
CLS
echo.
echo  Importing...
echo.
for %%C in (.\development\vendors_trinity.sql) do (
echo  Import: %%~nxC
%mysqlpath%\mysql --host=%host% --user=%user% --password=%pass% --port=%port% %trinity% < "%%~fC"
)
echo.
for %%C in (.\development\vendordata_trinity.sql) do (
echo  Import: %%~nxC
%mysqlpath%\mysql --host=%host% --user=%user% --password=%pass% --port=%port% %trinity% < "%%~fC"
)
echo.
echo  Done.
echo.
echo  Press any key to exit.
pause >NUL
goto trinity

:trinity_spawn
CLS
echo.
echo          /\
echo         /  \
echo        /    \
echo       /______\
echo      /\      /\ Triforce Mall - Version 3 (Trinity)
echo     /  \    /  \
echo    /    \  /    \
echo   /______\/______\
echo.
echo 	1 - GM Island
echo 	2 - (Not implemented yet)
echo 	3 - (Not implemented yet)
echo 	B - Back
echo.
set /p g=	Enter a letter: 
if %g%==* goto error
if %g%==1 goto trinity_spawn1
if %g%==2 goto trinity_spawn2
if %g%==3 goto trinity_spawn3
if %g%==b goto trinity
if %g%==B goto trinity
set g=
goto error

:trinity_spawn1
CLS
echo.
echo  Spawning Vendors...
echo.
for %%C in (.\development\spawn_gmisland_trinity.sql) do (
echo  Import: %%~nxC
%mysqlpath%\mysql --host=%host% --user=%user% --password=%pass% --port=%port% %trinity% < "%%~fC"
)
echo.
echo  Done.
echo.
echo  Press any key to exit.
pause >NUL
goto trinity_spawn

:trinity_spawn2
CLS
echo.
echo  Spawning Vendors...
echo.
for %%C in (.\development\spawn_gmisland_trinity.sql) do (
echo  Import: %%~nxC
%mysqlpath%\mysql --host=%host% --user=%user% --password=%pass% --port=%port% %trinity% < "%%~fC"
)
echo.
echo  Done.
echo.
echo  Press any key to exit.
pause >NUL
goto trinity_spawn

:trinity_spawn3
CLS
echo.
echo  Spawning Vendors...
echo.
for %%C in (.\development\spawn_gmisland_trinity.sql) do (
echo  Import: %%~nxC
%mysqlpath%\mysql --host=%host% --user=%user% --password=%pass% --port=%port% %trinity% < "%%~fC"
)
echo.
echo  Done.
echo.
echo  Press any key to exit.
pause >NUL
goto trinity_spawn

:trinity_despawn
CLS
echo.
echo  Despawning Vendors...
echo.
for %%C in (.\development\despawn_trinity.sql) do (
echo  Import: %%~nxC
%mysqlpath%\mysql --host=%host% --user=%user% --password=%pass% --port=%port% %trinity% < "%%~fC"
)
echo.
echo  Done.
echo.
echo  Press any key to exit.
pause >NUL
goto trinity

:trinity_remove
CLS
echo.
echo  Removing Triforce Mall...
echo.
for %%C in (.\development\removeall_trinity.sql) do (
echo  Import: %%~nxC
%mysqlpath%\mysql --host=%host% --user=%user% --password=%pass% --port=%port% %trinity% < "%%~fC"
)
echo.
echo  Done.
echo.
echo  Press any key to exit.
pause >NUL
goto trinity

:error
CLS
echo.
echo  Please enter a correct letter.
echo.
echo  Press any key to exit.
pause >NUL
goto begin
