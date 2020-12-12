#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "AES_UTILITY.h"

size keyRead(byte * key){	
	// Read Key
	char * keyPath = "./TestFolder/Key/key.txt";
	FILE * keyFile = fopen(keyPath, "r");
	
	counter c;
	byte temp;
	int fail;
	
	size keySize;
	
	fail = 0;
	
	if(keyFile == NULL){
		printf("Unable to open key file \"%s\"\n", keyPath);
		return -1;
	}
	
	printf("Key:\n");
	for(keySize=0; fail==0; keySize+=1){
		key = realloc(key, 8*keySize+8);
		for(c=0; c < 8; c+=1){
			if(fscanf(keyFile, "%c", &temp)==EOF){ fail=1; break; }
			printf("%02x ", temp);
			key[keySize*8+c] = temp;
		}
		
		if(c==0){ keySize -= 1; c=8; }
		else printf("\n");
	}
	
	printf("\n");
	fclose(keyFile);
	
	// Validate Key length
	if(c!=8){
		keySize -= 1;
		printf("Error: Invalid key length (%u) in \"%s\"\n", 8*keySize+c, keyPath);
		return -1;
	}

	return keySize;
}

void encryptFile(byte* key, size keySize, char* dataPath, char* outPath, long* timeEncrypt){
	FILE * dataFile = fopen(dataPath, "r");
	FILE * outFile = fopen(outPath, "w");
	
	EncryptStream(dataFile, key, keySize, outFile, timeEncrypt);
	
	fclose(outFile);
	fclose(dataFile);
}

void decryptFile(byte* key, size keySize, char* dataPath, char* outPath, long* timeDecrypt){
	FILE * dataFile = fopen(dataPath, "r");
	FILE * outFile = fopen(outPath, "w");
	
	DecryptStream(dataFile, key, keySize, outFile, timeDecrypt);
	
	fclose(outFile);
	fclose(dataFile);
}

int main(int argc, const char * argv[])
{
	byte* key = malloc(8);;
	size keySize;
	
	char* dataPath = "./TestFolder/File/test.txt";
	char* encryptPath = "./TestFolder/EncryptFile/test.txt";
	char* decryptPath =  "./TestFolder/DecryptFile/test.txt";
	
	long timeEncrypt = 0;
	long timeDecrypt = 0;
	
	keySize = keyRead(key);
	
	if(!keySize) return -1;
	
	encryptFile(key,  keySize, dataPath, encryptPath, &timeEncrypt);
	decryptFile(key,  keySize, encryptPath, decryptPath, &timeDecrypt);
	
	printf("Time encrypt: %ld milisecond\n", timeEncrypt);
	printf("Time decrypt: %ld milisecond\n", timeDecrypt);

	return 0;
}


