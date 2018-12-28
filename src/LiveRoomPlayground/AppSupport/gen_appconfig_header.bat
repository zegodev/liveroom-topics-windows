
@echo off
setlocal enabledelayedexpansion
cd  %~dp0
set file=./../../app_config.ini
set appid_keyname=app_key
set appsign_keynname=app_sign
for /f "tokens=1,2* delims==" %%i in (%file%) do if "%%i"=="%appid_keyname%" set appid_value=%%j
::echo %appid_value%  
for /f "tokens=1,2* delims==" %%i in (%file%) do if "%%i"=="%appsign_keynname%" set appsign_value=%%j
::echo %appsign_value% 

set file=./AppIDHelper.h.in
set file_tmp=./AppIDHelper.h.tmp
set file_out=./AppIDHelper.h

set source1=$appid$
set source2=$appkey$

set replaced1=%appid_value%  
set replaced2=%appsign_value% 

for /f "delims=" %%i in (%file%) do (
    set str=%%i
        set "str=!str:%source1%=%replaced1%!"
        set "str=!str:%source2%=%replaced2%!"
        echo !str!>>%file_tmp%
)
move "%file_tmp%" "%file_out%"


