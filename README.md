
# RSA Cipher Tool

A cipher tool that uses the RSA (Rivest, Shamir, Adleman) encryption algorithm to generate keys, encryptm and decrypt. It also uses a custom number precision library made from scratch!

## Backstory
This is a 1st year capstone project for Computer Programming 2. This project was chosen as the members wanted to explore the complex algorithms used in cryptography and mathematical operations, while implementing the lessons learned this academic year. A custom number precision library was built to overcome the limitations of C's native data types. The current state of the precision library ([bignum.h](src/bignum.c)) is enough to efficiently handle a maximum of 128-bit RSA key size. Super small i know hehe. You can view the project instructions [here](CS103-Programming-Project.pdf).

## Installation
Clone the repository
```
git clone https://github.com/michaelcanonizado/rsa-cipher-tool.git
```
Compile ./rsa-cipher-tool.c
```
gcc rsa-cipher-tool.c -o rsa-cipher-tool src/bignum.c -lm
```
Run executable file
```
./rsa-cipher-tool
```

## Usage
The program is pretty straight forward, having 3 main actions: (1) Generating Keys, (2) Encrypting Text, and (3) Decrypting Text.

Therefore, upon running the program, you will be greeted with the following options:

![image](https://github.com/michaelcanonizado/rsa-cipher-tool/assets/100785846/603b1231-2e2c-4d28-85cb-1f08f3c39d09)


### Generating Keys
You will be asked for your desired key size. The bigger the key size, the more secure the encrypted message will be, but the longer it will take to encrypt and decrypt. Smaller key sizes where implemented for the sake of demonstration, as the custom number precision library is not efficient enough to perform calculations on super large numbers. Further optimization can be made to bignum.h to enable it to handle longer key sizes efficiently.

![image](https://github.com/michaelcanonizado/rsa-cipher-tool/assets/100785846/4504f147-4b5c-4b55-b9c5-7ef25a058b8a)

After choosing a key size, the program will start generating the keys. They will be printed out in the screen, so make sure you properly copy and secure them before exiting!

![image](https://github.com/michaelcanonizado/rsa-cipher-tool/assets/100785846/75de7288-6373-416b-8974-1f8642a95fd0)


### Encrypting text

Make sure to have the txt file you want to encrypt at the top most level of the directory!

![image](https://github.com/michaelcanonizado/rsa-cipher-tool/assets/100785846/8902ff66-90be-4ddd-b658-34d0951c5f25)

Then simply enter the file name and the public key!

**REMEMBER:** Only the corresponding private key to the public key used to encrypt the text can decrypt it back to the original message! Ensure that you are using the right public key of the recipient.

![image](https://github.com/michaelcanonizado/rsa-cipher-tool/assets/100785846/2bc835f8-592f-4279-bc75-d45e34df2fb2)

The encrypted file will be named as ./encrypted.txt, and can now be safely stored or be sent to the recipient.

![image](https://github.com/michaelcanonizado/rsa-cipher-tool/assets/100785846/3ad9911c-0c5b-4919-97f8-a6e72e89243b)


### Decrypting text

Similar to encrypting, make sure to have the txt file you want to decrypt at the top most level of the directory!

![image](https://github.com/michaelcanonizado/rsa-cipher-tool/assets/100785846/165119d0-ee52-471b-a818-0683db902c84)

Then simply enter the file name and the private key!

![image](https://github.com/michaelcanonizado/rsa-cipher-tool/assets/100785846/89007ede-89cf-4476-83b8-12cb43299978)

The decrypted file will be named as ./decrypted.txt

![image](https://github.com/michaelcanonizado/rsa-cipher-tool/assets/100785846/8268414b-3286-4575-a1af-998f505a5bec)








