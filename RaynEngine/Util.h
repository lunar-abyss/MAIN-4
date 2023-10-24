#pragma once

#define _CRT_SECURE_NO_WARNINGS

#include "Types.h"
#include <direct.h>
#include <stdlib.h>
#include <stdio.h>

i32 strlen(u8* s) {
	i32 length = 0;
	for (length; s[length] != 0; length++);
	return length;
}

u8* concat(u8* s1, u8* s2) {
	u8* res = malloc(strlen(s1) + strlen(s2) + 1);

	i32 i = 0;
	for (i; i < strlen(s1); i++)
		res[i] = s1[i];

	i32 j = 0;
	for (j; j < strlen(s2); j++)
		res[i + j] = s2[j];

	res[i + j] = 0;
	return res;
}

u8* FileReadBytes(u8* filepath, i32 length) {
	FILE* fptr = fopen(filepath, "r");

	u8* buffer = malloc(length);
	i32 i = 0;

	while (true) {
		i32 c = fgetc(fptr);
		if (c == -1 && i >= length)
			break;

		buffer[i] = (u8)c;

		i++;
	}

	fclose(fptr);

	return buffer;
}