#pragma once
#include "cryptutil.h"
#include <iostream>
#include <vector>



using namespace std;


class OperationMode
{
public:
	virtual void encrypt(
		istream& ist, ostream& ost,
		const size_t block_size, const vector<byte>& key, const byte *init_vector,
		byte *(*algorithm)(const byte* block, const size_t block_size, const vector<byte>& key),
		void (*padding)(byte *block, const size_t block_size, const size_t filled_blocks)
		) = 0;


	virtual void decrypt(
		istream& ist, ostream& ost,
		const size_t block_size, const vector<byte>& key, const byte *init_vector,
		byte *(*algorithm)(const byte* block, const size_t block_size, const vector<byte>& key),
		void (*padding)(byte *block, const size_t block_size, const size_t filled_blocks)
		) = 0;
};


class ECB : OperationMode
{
public:
	static void encrypt(
		istream& ist, ostream& ost,
		const size_t block_size, const vector<byte>& key, const byte *init_vector,
		byte *(*algorithm)(const byte* block, const size_t block_size, const vector<byte>& key),
		void (*padding)(byte *block, const size_t block_size, const size_t filled_blocks)
		) 
	{
		byte *block = new byte[block_size + 1];
		block[block_size] = '\0';

		while (ist)
		{
			ist.read(block, block_size);
			if (ist.gcount()) {
				if (ist.gcount() != static_cast<streamsize>(block_size))
					padding(block, block_size, static_cast<size_t>(ist.gcount()));
				byte *encrypted = algorithm(block, block_size, key);
				ost.write(encrypted, block_size);
				delete[] encrypted;
			}
		}

		delete[] block;
		return;
	}


	static void decrypt(
		istream& ist, ostream& ost,
		const size_t block_size, const vector<byte>& key, const byte *init_vector,
		byte *(*algorithm)(const byte* block, const size_t block_size, const vector<byte>& key),
		void (*padding)(byte *block, const size_t block_size, const size_t filled_blocks)
		)
	{
		byte *block = new byte[block_size + 1];
		block[block_size] = '\0';

		while (ist)
		{
			ist.read(block, block_size);
			if (ist.gcount()) {
				if (ist.gcount() != static_cast<streamsize>(block_size))
					padding(block, block_size, static_cast<size_t>(ist.gcount()));
				byte *decrypted = algorithm(block, block_size, key);
				ost.write(decrypted, block_size);
				delete[] decrypted;
			}
		}

		delete[] block;
		return;
	}
};


class CFB : OperationMode
{
public:
	static void encrypt(
		istream& ist, ostream& ost,
		const size_t block_size, const vector<byte>& key, const byte *init_vector,
		byte *(*algorithm)(const byte* block, const size_t block_size, const vector<byte>& key),
		void(*padding)(byte *block, const size_t block_size, const size_t filled_blocks)
		)
	{
		byte *block = new byte[block_size + 1];
		block[block_size] = '\0';

		byte *IV = new byte[block_size + 1];
		memcpy(IV, init_vector, block_size);
		IV[block_size] = '\0';

		while (ist) {
			ist.read(block, static_cast<streamsize>(block_size));
			if (ist.gcount()) {
				if (ist.gcount() != static_cast<streamsize>(block_size))
					padding(block, block_size, static_cast<size_t>(ist.gcount()));
				byte *encrypted = algorithm(IV, block_size, key);
				for (size_t i = 0; i < block_size; i++)
					encrypted[i] ^= block[i];
				ost.write(encrypted, static_cast<streamsize>(block_size));
				memmove(IV, encrypted, block_size);
				delete[] encrypted;
			}
		}

		delete[] block;
		delete[] IV;
		return;
	}

	static void decrypt(
		istream& ist, ostream& ost,
		const size_t block_size, const vector<byte>& key, const byte *init_vector,
		byte *(*algorithm)(const byte* block, const size_t block_size, const vector<byte>& key),
		void(*padding)(byte *block, const size_t block_size, const size_t filled_blocks)
		)
	{
		byte *block = new byte[block_size + 1];
		block[block_size] = '\0';

		byte *IV = new byte[block_size + 1];
		memcpy(IV, init_vector, block_size);
		IV[block_size] = '\0';

		while (ist) {
			ist.read(block, block_size);
			if (ist.gcount()) {
				byte *decrypted = algorithm(IV, block_size, key);
				for (size_t i = 0; i < block_size; i++)
					decrypted[i] ^= block[i];
				ost.write(decrypted, block_size);
				memmove(IV, block, block_size);
				delete[] decrypted;
			}
		}

		delete[] block;
		delete[] IV;
		return;
	}
};


class OFB : OperationMode
{
public:
	static void encrypt(
		istream& ist, ostream& ost,
		const size_t block_size, const vector<byte>& key, const byte *init_vector,
		byte *(*algorithm)(const byte* block, const size_t block_size, const vector<byte>& key),
		void(*padding)(byte *block, const size_t block_size, const size_t filled_blocks)
		)
	{
		byte *block = new byte[block_size + 1];
		block[block_size] = '\0';

		byte *IV = new byte[block_size + 1];
		memcpy(IV, init_vector, block_size);
		IV[block_size] = '\0';

		while (ist) {
			ist.read(block, static_cast<streamsize>(block_size));
			if (ist.gcount()) {
				if (ist.gcount() != static_cast<streamsize>(block_size))
					padding(block, block_size, static_cast<size_t>(ist.gcount()));
				byte *encrypted = algorithm(IV, block_size, key);
				memcpy(IV, encrypted, block_size);
				for (size_t i = 0; i < block_size; i++)
					encrypted[i] ^= block[i];
				ost.write(encrypted, static_cast<streamsize>(block_size));
				delete[] encrypted;
			}
		}

		delete[] block;
		delete[] IV;
		return;
	}

	static void decrypt(
		istream& ist, ostream& ost,
		const size_t block_size, const vector<byte>& key, const byte *init_vector,
		byte *(*algorithm)(const byte* block, const size_t block_size, const vector<byte>& key),
		void(*padding)(byte *block, const size_t block_size, const size_t filled_blocks)
		)
	{
		byte *block = new byte[block_size + 1];
		block[block_size] = '\0';

		byte *IV = new byte[block_size + 1];
		memcpy(IV, init_vector, block_size);
		IV[block_size] = '\0';

		while (ist) {
			ist.read(block, block_size);
			if (ist.gcount()) {
				byte *decrypted = algorithm(IV, block_size, key);
				memcpy(IV, decrypted, block_size);
				for (size_t i = 0; i < block_size; i++)
					decrypted[i] ^= block[i];
				ost.write(decrypted, block_size);
				delete[] decrypted;
			}
		}

		delete[] block;
		delete[] IV;
		return;
	}
};


class CBC : OperationMode
{
public:
	static void encrypt(
		istream& ist, ostream& ost,
		const size_t block_size, const vector<byte>& key, const byte *init_vector,
		byte *(*algorithm)(const byte* block, const size_t block_size, const vector<byte>& key),
		void(*padding)(byte *block, const size_t block_size, const size_t filled_blocks)
		)
	{
		byte *block = new byte[block_size + 1];
		block[block_size] = '\0';

		byte *IV = new byte[block_size + 1];
		memcpy(IV, init_vector, block_size);
		IV[block_size] = '\0';

		while (ist) {
			ist.read(block, static_cast<streamsize>(block_size));
			if (ist.gcount()) {
				if (ist.gcount() != static_cast<streamsize>(block_size))
					padding(block, block_size, static_cast<size_t>(ist.gcount()));
				for (size_t i = 0; i < block_size; i++)
					IV[i] ^= block[i];
				byte *encrypted = algorithm(IV, block_size, key);
				memcpy(IV, encrypted, block_size);
				ost.write(encrypted, static_cast<streamsize>(block_size));
				delete[] encrypted;
			}
		}

		delete[] block;
		delete[] IV;
		return;
	}

	static void decrypt(
		istream& ist, ostream& ost,
		const size_t block_size, const vector<byte>& key, const byte *init_vector,
		byte *(*algorithm)(const byte* block, const size_t block_size, const vector<byte>& key),
		void(*padding)(byte *block, const size_t block_size, const size_t filled_blocks)
		)
	{
		byte *block = new byte[block_size + 1];
		block[block_size] = '\0';

		byte *IV = new byte[block_size + 1];
		memcpy(IV, init_vector, block_size);
		IV[block_size] = '\0';

		while (ist) {
			ist.read(block, block_size);
			if (ist.gcount()) {
				byte *decrypted = algorithm(block, block_size, key);
				for (size_t i = 0; i < block_size; i++)
					decrypted[i] ^= IV[i];
				memcpy(IV, block, block_size);
				ost.write(decrypted, block_size);
				delete[] decrypted;
			}
		}

		delete[] block;
		delete[] IV;
		return;
	}
};