objdump -d ./hello.exe |grep '[0-9a-f]:'|grep -v 'file'|cut -f2 -d :|cut -f1-6 -d' '|tr -s ' '|tr '\t' ' '|sed 's/ $//g'|sed 's/ /\\x/g'|paste -d '' -s |sed 's/^/"/'|sed 's/$/"/g' > shellcode.txt

objdump -d ./hello.exe |grep '[0-9a-f]:'|grep -v 'file'|cut -f2 -d :|cut -f1-6 -d' '|tr -s ' '|tr '\t' ' '|sed 's/ $//g'|sed 's/ /\\x/g'

gcc -O2 test2.c -o test2.exe -mconsole -I C:\msys64\ucrt64\include -s -ffunction-sections -fdata-sections -Wall -fno-exceptions -fmerge-all-constants -static-libstdc++ -static-libgcc