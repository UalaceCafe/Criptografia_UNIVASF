#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../lib/aes.h"

const char* help_message = "Usage: aes --[enc|dec]=<filename> --out=<filename> --key=<128-bit key>\n Note: keys with length different than 128 bits will be truncated or padded with garbage";

typedef enum { ENCRYPT, DECRYPT } cmd_mode;
typedef struct {
	cmd_mode mode;
	char in[256];
	char out[256];
	char key[17];
} cmd_args;

typedef struct {
	uint8_t* buffer;
	size_t unpadded_size;
	size_t padded_size;
} data_buffer;

void parse_args(int argc, char *argv[], cmd_args* args);
void read_input_file(const char* filename, data_buffer* buffer);
uint8_t* get_key(const char* key);

int main(int argc, char* argv[]) {
	cmd_args args = { 0 };
	parse_args(argc, argv, &args);

	printf("[\n Mode: %s\n", args.mode == ENCRYPT ? "encrypt" : "decrypt");
	printf(" Input file: %s\n", args.in);
	printf(" Output file: %s\n", args.out);
	printf(" Key: %s\n]\n\n", args.key);

	data_buffer buffer = { 0 };
	read_input_file(args.in, &buffer);

	struct AES_ctx ctx;
	uint8_t* key = get_key(args.key);
	AES_init_ctx(&ctx, key);

	FILE* file = fopen(args.out, "wb");
	if (file == NULL) {
		puts("Error: cannot open output file");
		exit(EXIT_FAILURE);
	}

	if (args.mode == ENCRYPT) {
		for (size_t i = 0; i < buffer.padded_size; i += 16) {
			AES_ECB_encrypt(&ctx, buffer.buffer + i);
		}

		if (fwrite(buffer.buffer, 1, buffer.padded_size, file) != buffer.padded_size) {
			puts("Error: cannot write to output file");
			exit(EXIT_FAILURE);
		}
	} else {
		for (size_t i = 0; i < buffer.padded_size; i += 16) {
			AES_ECB_decrypt(&ctx, buffer.buffer + i);
		}

		if (fwrite(buffer.buffer, 1, buffer.unpadded_size, file) != buffer.unpadded_size) {
			puts("Error: cannot write to output file");
			exit(EXIT_FAILURE);
		}
	}

	free(buffer.buffer);
	free(key);

	return EXIT_SUCCESS;
}

void parse_args(int argc, char *argv[], cmd_args* args) {
	if (argc < 4) {
		puts("Error: missing arguments");
		puts(help_message);
		exit(EXIT_FAILURE);
	}

	for (int i = 1; i < argc; i++) {
        if (strncmp(argv[i], "--enc=", 6) == 0) {
            args->mode = ENCRYPT;
            strncpy(args->in, argv[i] + 6, sizeof(args->in) - 1);
        } else if (strncmp(argv[i], "--dec=", 6) == 0) {
            args->mode = DECRYPT;
            strncpy(args->in, argv[i] + 6, sizeof(args->in) - 1);
        } else if (strncmp(argv[i], "--out=", 6) == 0) {
            strncpy(args->out, argv[i] + 6, sizeof(args->out) - 1);
        } else if (strncmp(argv[i], "--key=", 6) == 0) {
            strncpy(args->key, argv[i] + 6, sizeof(args->key) - 1);
			if (strlen(args->key) != 16) {
                puts("Warning: keys length is different than 128 bits and will be truncated or padded with garbage\n");
			}
        } else {
			puts("Error: invalid argument");
            puts(help_message);
            exit(EXIT_FAILURE);
        }
    }

    if (args->out[0] == '\0' || args->key[0] == '\0') {
        puts("Error: output file and key must be specified");
        puts(help_message);
		exit(EXIT_FAILURE);
    }
}

void read_input_file(const char* filename, data_buffer* buffer) {
	FILE* file = fopen(filename, "rb");
	if (file == NULL) {
		puts("Error: cannot open input file");
		exit(EXIT_FAILURE);
	}

	fseek(file, 0, SEEK_END);
	buffer->unpadded_size = ftell(file);
	printf("Input file size (unpadded): %zu bytes\n", buffer->unpadded_size);
	rewind(file);

	if (buffer->unpadded_size % 16 != 0) {
		buffer->padded_size = (buffer->unpadded_size / 16 + 1) * 16;
	} else {
		buffer->padded_size = buffer->unpadded_size;
	}

	buffer->buffer = malloc(buffer->padded_size);
	if (buffer->buffer == NULL) {
		puts("Error: cannot allocate memory");
		exit(EXIT_FAILURE);
	}

	if (fread(buffer->buffer, 1, buffer->unpadded_size, file) != buffer->unpadded_size) {
		puts("Error: cannot read input file");
		exit(EXIT_FAILURE);
	}

	size_t diff = buffer->padded_size - buffer->unpadded_size;
	if (diff) {
		printf("Warning: padding input file with %zu '%02llx' bytes\n", diff, diff);
		printf("Input file size (padded): %zu bytes\n", buffer->padded_size);
		memset(buffer->buffer + buffer->unpadded_size, diff, diff);
	}

	fclose(file);
}

uint8_t* get_key(const char* key) {
	uint8_t* result = malloc(16);
	if (result == NULL) {
		perror("Error: cannot allocate memory");
		exit(EXIT_FAILURE);
	}

	for (int i = 0; i < 16; i++) {
		result[i] = key[i];
	}

	return result;
}
