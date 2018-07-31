testmake: cir_test.cpp decode_ir_code.cpp cir.h decode_ir_code.h 
	g++ cir_test.cpp decode_ir_code.cpp -I/usr/include/libusb-1.0/ -l usb-1.0 -l sqlite3 -shared -o libcir.so -g

driver: main.cpp
	g++ main.cpp libcir.so -Wl,-R,'./' -o main -g

.PHONY: clean

clean:
	rm a.out

