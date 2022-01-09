#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <getopt.h>

static size_t fileSize(FILE* const file)
{
    fseek(file, 0, SEEK_END);

    size_t size = ftell(file);

    fseek(file, 0, SEEK_SET);

    return size;
}

int main(const int argc, const char** argv)
{
    if (argc != 3)
    {
        fputs("Usage: input, output\n", stderr);

        return -1;
    }

    const char* const inputFileName = argv[1];
    const char* const outputFileName = argv[2];

    fprintf(stderr, "File: %s\n", inputFileName);

    FILE* input = fopen(inputFileName, "r");

    if (input == NULL)
    {
        fprintf(stderr, "Failed to open file %s\n", inputFileName);

        return -1;
    }

    size_t inputSize = fileSize(input);
    uint8_t* inputBuffer = malloc(inputSize);

    if (fread(inputBuffer, inputSize, 1, input) == 0)
    {
        fputs("Failed to read from input file\n", stderr);

        return -1;
    }

    fputs("File Data: ", stderr);
    fwrite(inputBuffer, inputSize, 1, stderr);
    fputc('\n', stderr);

    FILE* output = fopen(outputFileName, "w");

    if (output == NULL)
    {
        fprintf(stderr, "Failed to open file %s\n", outputFileName);

        return -1;
    }

    fprintf(output, "#pragma once\n\n");
    fprintf(output, "static unsigned char data[%lu] = {\n", inputSize);
    fputc('\t', output);

    for (int i = 0; i < inputSize - 1; i++)
    {
        uint8_t byte = inputBuffer[i];

        if ((i + 1) % 10 == 0)
        {
            fprintf(output, "0x%x, \n\t", byte);
        }
        else
        {
            fprintf(output, "0x%x, ", byte);
        }
    }
    
    fprintf(output, "0x%x", inputBuffer[inputSize - 1]);

    fputs("\n}", output);

    free(inputBuffer);
    fclose(output);
    fclose(input);

    return 0;
}