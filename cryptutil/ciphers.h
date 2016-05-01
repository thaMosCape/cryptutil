#pragma once
#include "cryptutil.h"
#include <vector>


byte* transposition(byte* block, size_t block_size, std::vector<byte>& key) {
	byte* result;
	result = static_cast<byte*>(calloc(block_size+1, sizeof(byte)));
	result[block_size] = '\0';

	for (size_t i = 0; i < block_size; i++)
		result[key[i]] = block[i];

	return result;
}