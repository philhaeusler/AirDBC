echo Replace SDK_PATH before running
pause

copy ..\..\AirDBCExtension/Debug/AirDBCExtension.dll . .\
"SDK_PATH\bin\adt.bat" -package -storetype pkcs12 -keystore ../certs/test.p12 -storepass test -target ane AirDBC.ane extension.xml -swc AirDBC.swc -platform Windows-x86 library.swf AirDBCExtension.dll
