#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <memory.h>
#include "dhexchange.h"

static void _print_key(const char* name, const DH_KEY key) 
{
	int i;

	printf("%s=\t", name);
	for (i = 0; i< DH_KEY_LENGTH; i++)
	{
		printf("%02x", key[i]);
	}
	printf("\n");
}

int main(void)
{
	DH_KEY our_public;
	DH_KEY our_private;
	DH_KEY other_private;
	DH_KEY other_public;
	DH_KEY our_secret; 
	DH_KEY other_secret;

	unsigned char key[3][33] =  {"870d7253bef3e17be12d9738937531dc",
							"45451fae9b3a9d5f463ccb756303557c",
							"ccb1cf316ef3ea8868481472e8385a7e"};

	assign_key(our_public, key[0]);
	assign_key(our_private, key[1]);
	assign_key(other_public, key[2]);

	time_t seed;
	time(&seed);
	srand((unsigned int)seed);

	/*Alice generate her private key and public key */
	// DH_generate_key_pair(our_public, our_private);

	/*Bob generate his private key and public key */
	// DH_generate_key_pair(other_public, other_private);

	/*Bob send his public key to Alice, Alice generate the secret key */
	DH_generate_key_secret(our_secret, our_private, other_public);

	/*Alice send her public key to Bob, Bob generate the secret key */
	// DH_generate_key_secret(other_secret, other_private, our_public);

	_print_key("our_private", our_private);
	_print_key("our_public", our_public);
	_print_key("other_private", other_private);
	_print_key("other_public", other_public);
	_print_key("our_secret", our_secret);
	_print_key("other_secret", other_secret);

	if (memcmp(our_secret, other_secret, DH_KEY_LENGTH) != 0) 
	{
		printf("ERROR!\n");
		return 1;
	}
	return 0;
}
