#
read -p "Replace SDK_PATH before running"
/Applications/Adobe\ Flash\ Builder\ 4.5/sdks/4.5.1\ +\ AIR\ 3.0/bin/adt -certificate -cn SelfSign -ou QE -o "Example, Co" -c US 2048-RSA test.p12 test
