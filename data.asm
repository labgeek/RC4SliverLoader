.CODE
RunData PROC

<hexdump -v -e '1/2 "dw 0%.4xh\n"' https50.bin.enc| tee out.txt>

RunData ENDP
END
