
#include "hash_utils.h"

#include "openssl/sha.h"


#ifndef _HASH_LEN
#define _HASH_LEN (32)
#endif




static inline
int _hash( unsigned char * digest , const unsigned char * m , unsigned long long mlen )
{
#if 32 == _HASH_LEN
	SHA256_CTX sha256;
	SHA256_Init( &sha256 );
	SHA256_Update( &sha256 , m , mlen );
	SHA256_Final( digest , &sha256 );
#elif 48 == _HASH_LEN
	SHA512_CTX sha384;
	SHA384_Init( &sha384 );
	SHA384_Update( &sha384 , m , mlen );
	SHA384_Final( digest , &sha384 );
#elif 64 == _HASH_LEN
	SHA512_CTX sha512;
	SHA512_Init( &sha512 );
	SHA512_Update( &sha512 , m , mlen );
	SHA512_Final( digest , &sha512 );
#else
error: un-supported _HASH_LEN
#endif
	return 0;
}





static inline
int expand_hash( unsigned char * digest , unsigned n_digest , const unsigned char * hash )
{
	if( _HASH_LEN >= n_digest ) {
		for(unsigned i=0;i<n_digest;i++) digest[i] = hash[i];
		return 0;
	} else {
		for(unsigned i=0;i<_HASH_LEN;i++) digest[i] = hash[i];
		n_digest -= _HASH_LEN;
	}

	while( _HASH_LEN <= n_digest ) {
		_hash( digest+_HASH_LEN , digest , _HASH_LEN );

		n_digest -= _HASH_LEN;
		digest += _HASH_LEN;
	}
	unsigned char temp[_HASH_LEN];
	if( n_digest ){
		_hash( temp , digest , _HASH_LEN );
		for(unsigned i=0;i<n_digest;i++) digest[_HASH_LEN+i] = temp[i];
	}
	return 0;
}




int hash_msg( unsigned char * digest , unsigned len_digest , const unsigned char * m , unsigned long long mlen )
{
	unsigned char buf[_HASH_LEN];
	_hash( buf , m , mlen );

	return expand_hash( digest , len_digest , buf );
}


