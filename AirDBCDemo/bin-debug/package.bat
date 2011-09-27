echo Replace SDK_PATH before running
pause

@echo off
copy ..\db\demo.mdb .
copy ..\icons\*.png .
"SDK_PATH\bin\adt.bat" -package -XnoAneValidate -storetype pkcs12 -keystore test.p12 -storepass test -target native InstallAirDBCDemo AirDBCDemo-app.xml AirDBCDemo.swf demo.mdb icon_*.png -extdir "..\..\AirDBC\bin"
