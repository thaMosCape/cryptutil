#pragma once
#include "cryptutil.h"



using namespace std;


class Cipher
{
public:
	virtual byte* encrypt(const byte* block, const size_t block_size, const TKey& key) = 0;
	virtual	byte* decrypt(const byte* block, const size_t block_size, const TKey& key) = 0;

	enum algorithm { TRANSPOSITION, VIGENERE, HILL };
	static size_t get_block_size(size_t key_size, algorithm alg) {
		switch (alg)
		{
		case Cipher::TRANSPOSITION:
		case Cipher::VIGENERE:
			return key_size;
		case Cipher::HILL:
			if (floor(sqrt(key_size)) == sqrt(key_size))
				throw InvalidKeySize();
			return static_cast<size_t>(sqrt(key_size));
		}
	}
};


class Transposition : Cipher
{
public:
	static byte* encrypt(const byte* block, const size_t block_size, const TKey& key)
	{
		byte* result = new byte[block_size + 1];
		result[block_size] = '\0';

		for (size_t i = 0; i < block_size; i++)
			result[key[i]] = block[i];

		return result;
	}


	static byte* decrypt(const byte* block, const size_t block_size, const TKey& key)
	{
		byte* result = new byte[block_size + 1];
		result[block_size] = '\0';

		for (size_t i = 0; i < block_size; i++)
			result[i] = block[key[i]];

		return result;
	}
};


class Vigenere : Cipher
{
public:
	static byte* encrypt(const byte* block, const size_t block_size, const TKey& key)
	{
		byte* result = new byte[block_size + 1];
		result[block_size] = '\0';

		for (size_t i = 0; i < block_size; i++)
			result[i] = (block[i] + key[i]) % 256;

		return result;
	}


	static byte* decrypt(const byte* block, const size_t block_size, const TKey& key)
	{
		byte* result = new byte[block_size + 1];
		result[block_size] = '\0';

		for (size_t i = 0; i < block_size; i++)
			result[i] = (block[i] - key[i] + 256) % 256;

		return result;
	}
};


class Hill : Cipher
{
public:
	static byte* encrypt(const byte* block, const size_t block_size, const TKey& key)
	{
		byte* result = new byte[block_size + 1];
		result[block_size] = '\0';
		for (size_t i = 0; i < block_size; i++)
			result[i] = 0;

		for (size_t i = 0; i < block_size; i++)
			for (size_t j = 0; j < block_size; j++)
				result[i] += key[i*block_size + j] * block[j] % 257;

		return result;
	}


	static byte* decrypt(const byte* block, const size_t block_size, const TKey& key)
	{
		byte* result = new byte[block_size + 1];
		result[block_size] = '\0';

		cout << "to decrypt Hill cipher buy full version" << endl;

		return result;
	}
};