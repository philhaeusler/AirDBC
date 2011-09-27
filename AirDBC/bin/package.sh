#
read -p "Replace SDK_PATH before running"
echo Starting build of AirDBC.swc - `date`
cp ../../AirDBCExtension/Debug/AirDBCExtension.dll .
SDK_PATH/bin/adt -package -storetype pkcs12 -keystore ../certs/test.p12 -storepass test -target ane AirDBC.ane extension.xml -swc AirDBC.swc -platform Windows-x86 library.swf AirDBCExtension.dll
