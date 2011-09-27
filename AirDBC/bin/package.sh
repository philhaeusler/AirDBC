#
echo Starting build of AirDBC.swc - `date`
cp ../../AirDBCExtension/Debug/AirDBCExtension.dll .
/Applications/Adobe\ Flash\ Builder\ 4.5/sdks/4.5.1\ +\ AIR\ 3.0/bin/adt -package -storetype pkcs12 -keystore ../certs/test.p12 -storepass test -target ane AirDBC.ane extension.xml -swc AirDBC.swc -platform Windows-x86 library.swf AirDBCExtension.dll
