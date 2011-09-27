echo Replace SDK_PATH before running
pause

copy SDK_PATH\include\FlashRuntimeExtensions.h .\AirDBCExtension\FlashRuntimeExtensions.h
copy SDK_PATH\lib\win\FlashRuntimeExtensions.lib .\AirDBCExtension\FlashRuntimeExtensions.lib
