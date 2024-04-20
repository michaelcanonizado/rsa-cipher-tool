// C program to create loading bar 
#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX_STRING_LENGTH 1000000

const int PROGRESS_BAR_LENGTH = 50;

// Function to creating loading bar 
void loadingBar(int percentDone) { 
	int numChar = percentDone * PROGRESS_BAR_LENGTH / 100;
	int numSpace = PROGRESS_BAR_LENGTH - numChar;
  // int start = (width - PROGRESS_BAR_LENGTH) / 2;

  // moveCursor(start, (height * 3) / 2);
	printf("\r");
#ifdef _WIN32
	char a = 219, b = 177;
	for (int i = 0; i < numChar; i++) {
		printf("%c", a);
	}
	for (int i = 0; i < numSpace; i++) {
		printf("%c", b);
	}
#else
	char *a = "█", *b = "░";
	for (int i = 0; i < numChar; i++) {
		printf("%s", a);
	}
	for (int i = 0; i < numSpace; i++) {
		printf("%s", b);
	}
#endif

	printf(" %d%% Done", percentDone);
	fflush(stdout);
}

// Function to convert a very long string to their ASCII value
void convertToASCII(const char* str) {
	int length = strlen(str);
	for (int i = 0; i < length; i++) {
		// printf("%d", str[i]);
		if (i != length - 1) {
			// printf(", ");
		}
	}
}

// Driver Code 
int main() 
{ 
	char plainText[MAX_STRING_LENGTH] = "You are about to generate an RSA private key with this option. Additionally, it will display the public key of a generated private key. Do you agree to save a copy of your private and public keys? [Y/N]. Encryption includes the message to be encrypted and the public key of the recipient. The txt file of the message must be in the same folder of the C program. Is the txt file in the same folder of the C program? [Y/N] Do you have the public key of the recipient? [Y/N] The RSA Cipher Tool program generates, encrypts, and decrypts messages using the RSA algorithm. The program uses the RSA (Rivest, Shamir, Adleman) Algorithm to generate public and private keys needed to encrypt and decrypt messages and a custom bignum library to handle large numbers. The user will use the program to encrypt a message in a file, where it will then return the encrypted/secret message. Then, they can now send it to the recipient of the message safely, where they can also use the same program to decrypt the message and reveal the actual message. This is a programming project for Computer Programming 2 during the Academic Year 2023-2024 at the Bicol University College of Science.";
	int plainTextLength = strlen(plainText);
	printf("Length: %d\n", plainTextLength);
	loadingBar(0);
	for (int i = 0; i < plainTextLength; i++) {
		// convertToASCII(plainText);
		loadingBar((i+1)*100/plainTextLength);
		usleep(10000);
	}
	printf("\n");
	return 0; 
}