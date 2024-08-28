setup:
	gcc -shared -o paintcolor_tapper.dll -Wl,--out-implib,libtstdll.a paintcolor_tapper.c -lgdiplus
	# g++ .\capture_image.cpp -o capture_image.exe -lgdiplus -lgdi32