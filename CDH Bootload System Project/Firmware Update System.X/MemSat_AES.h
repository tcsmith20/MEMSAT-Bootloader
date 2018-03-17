#ifndef AES_H
#define	AES_H

/*
 *	The encryption uses AES standard to encrypt a 256 bytes data.
 *	The function will encrypt the data and store in its original address.
 *	The varible *state is the the pointer to the data,
 *	It will accept more than 256 bytes of data,
 *	however, the data beyond 256 bytes will NOT be encrypted.
 *	
*/

void aes_encrypt(unsigned char *state, unsigned char *data, unsigned char *passkey, unsigned char *key);
int aes_decrypt(unsigned char *state, unsigned char *passkey, unsigned char *key);

#endif	/* AES_H */

