#
read -p "Replace SDK_PATH before running"
SDK_PATH/bin/adt -certificate -cn SelfSign -ou QE -o "Example, Co" -c US 2048-RSA test.p12 test
