#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <math.h>
#include <stdint.h>
#include <libgen.h>
#include "wav.h"

//----------------------------------------------------------------------------------------------
//DESCRIPTION:
// Generate a buffer of silence to be inserted between the sound data from the two input files
//
//PARAMETERS:
// char *buffer: gets silence_buffer that points to the buffer
// int sample_rate: gets header1.SampleRate the sample rate of the first file
// int num_channels: gets header1.NumChannels the number of channels of the first file
// int duration: gets the duration of the silence in seconds
//
//RETURN VALUE:
// No return value
//----------------------------------------------------------------------------------------------

void generate_silence(char *buffer, int sample_rate, int num_channels, int duration) {
    int num_samples = sample_rate * num_channels * duration;
    for (int i = 0; i < num_samples; i++) {
        buffer[i] = 0;
    }
}

//----------------------------------------------------------------------------------------------
//DESCRIPTION:
// Compares the length difference between the sound data from the two input files
// and displays it in the chosen numerical system
//
//PARAMETERS:
// int first_length: gets header1.Subchunk2Size the lenght of the sound data of the fist input file
// int second_length: gets header2.Subchunk2Size the lenght of the sound data of the second input file
// char *numSystem: gets numSystem that points to the chosen numerical system
//
//RETURN VALUE:
// No return value
//----------------------------------------------------------------------------------------------

void length_difference(int first_length, int second_length, char *numSystem) {
    int diff = abs(first_length - second_length)/8;

    //For dec or Default
    if (!numSystem || memcmp(numSystem, "dec", 3) == 0 || memcmp(numSystem, "hex", 3) != 0 && memcmp(numSystem, "oct", 3) != 0) {
        if (first_length < second_length) {
            printf("\nThe sound content of input file No 2 has %d (in dec) bytes more sound data than input file No 1", diff);
        } else {
            printf("\nThe sound content of input file No 1 has %d (in dec) bytes more sound data than input file No 2", diff);
        }
    }
    // For hex
    else if(memcmp(numSystem, "hex", 3) == 0) {
        long quotient, remainder;
        int i, j = 0;
        char hexadecimalnum[100];
        quotient = diff;

        if (first_length < second_length) {
            printf("\nThe sound content of input file No 2 has ");
        } else {
            printf("\nThe sound content of input file No 1 has ");
        }

        while (quotient != 0) {
            remainder = quotient % 16;
            if (remainder < 10) {
                hexadecimalnum[j++] = 48 + remainder;
            } else {
                hexadecimalnum[j++] = 55 + remainder;
            }
            quotient = quotient / 16;
        }

        for (i = j-1; i >= 0; i--) {
            printf("%c", hexadecimalnum[i]);
        }

        if (first_length < second_length) {
            printf(" (in hex) bytes more sound data than input file No 1");
        } else {
            printf(" (in hex) bytes more sound data than input file No 2");
        }
    }
    // For oct
    else if(memcmp(numSystem, "oct", 3) == 0) {
        long remainder, quotient,octalnum=0;
        int octalNumber[100], i = 1, j;
        quotient = diff;

        while (quotient != 0) {
            octalNumber[i++] = quotient % 8;
            quotient = quotient / 8;
        }

        for (j = i - 1; j > 0; j--) {
            octalnum = octalnum*10 + octalNumber[j];
        }

        if (first_length < second_length) {
            printf("\nThe sound content of input file No 2 has %d (in oct) bytes more sound data than input file No 1", octalnum);
        } else {
            printf("\nThe sound content of input file No 1 has %d (in oct) bytes more sound data than input file No 2", octalnum);
        }
    }
}

//----------------------------------------------------------------------------------------------
//DESCRIPTION:]
// Makes the default path to the output file the same as the path to the first file
// Creates default name for the output file that is (input file name 1)-(input file name 2)-
// concatenated.wav
//
//PARAMETERS:
// char* first_file: gets input_file1 that points to the full path of the first file
// char* second_file: gets input_file2 that points to the full path of the second file
//
//RETURN VALUE:
// return concatenated: the full path and defaut name for the output file
//----------------------------------------------------------------------------------------------


char* default_filename(char* first_file,  char* second_file) {
    // Get the fist file without extension and the second file's name
    first_file[strlen(first_file)-4] = '\0';
    second_file[strlen(second_file)-4] = '\0';
    char* first_filename = first_file;
    char* second_filename = strrchr(second_file, '\\');

    // If the paths end with a '/' the file is in the directory
    if (second_filename == NULL) {
        second_filename = second_file;
    } else {
        second_filename++;
    }

    size_t first_len = strlen(first_filename);
    size_t second_len = strlen(second_filename);

    // Allocate memory for the concatenated file name with malloc
    size_t concat_len = first_len + second_len + 20;  // 20 extra characters for (-concatenated.wav)
    char* concatenated = (char*)malloc((concat_len + 1) * sizeof(char));

    // Concatenate the file names
    snprintf(concatenated, concat_len + 1, "%s-%s-concatenated.wav", first_filename, second_filename);

    return concatenated;
}

int main(int argc, char *argv[]) {
    char *input_file1 = NULL;
    char *input_file2 = NULL;
    char *output_file = NULL;
    int silentMode = 0;
    char *numSystem = NULL;

    // Get input
    int opt;
    while ((opt = getopt(argc, argv, "f:b:r:n:s::")) != -1) {
        switch (opt) {
        case 'f':
            input_file1 = optarg;
            break;
        case 'b':
            input_file2 = optarg;
            break;
        case 'r':
            output_file = optarg;
            break;
        case 's':
            silentMode = 1;
            break;
        case 'n':
            numSystem = optarg;
            break;
        default:
            fprintf(stderr, "Usage: %s -f path to input file on the front -b path to input file on the back -r path to the output file (optional, if no path specified the defaut name is firstFile-secondFile-concatenated) -s silent mode (optional) -n num sysytem hex or oct with dec a s default (optional) \n", argv[0]);
            exit(EXIT_FAILURE);
        }
    }

    // Check if input is valid
    if (input_file1 == NULL || input_file2 == NULL) {
        fprintf(stderr, "Usage: %s -f path to input file on the front -b path to input file on the back -r path to the output file (optional, if no path specified the defaut name is firstFile-secondFile-concatenated) -s silent mode (optional) -n num sysytem hex or oct with dec a s default (optional) \n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // OPen files
    FILE *input1 = fopen(input_file1, "rb");
    if (input1 == NULL) {
        perror("\nError opening input file No. 1");
        exit(EXIT_FAILURE);
    }

    FILE *input2 = fopen(input_file2, "rb");
    if (input2 == NULL) {
        perror("\nError opening input file No. 2");
        exit(EXIT_FAILURE);
    }

    if(output_file==NULL){
        output_file = default_filename(input_file1, input_file2); //Create the default name for output file
    }

    FILE *output = fopen(output_file, "wb");
    if (output == NULL) {
        perror("\nError opening output file");
        exit(EXIT_FAILURE);
    }

    if (silentMode == 0) {
        printf("\nFiles successfully opened");
    }

    // Use construct from wav.h
    wav_header_t header1, header2, header_out;

    // Read the headers of the input files
    fread(&header1, sizeof(wav_header_t), 1, input1);
    fread(&header2, sizeof(wav_header_t), 1, input2);

    // Verify that the input files are valid WAV files
    if (memcmp(header1.ChunkID, "RIFF", 4) != 0 || memcmp(header1.Format, "WAVE", 4) != 0 ||
            memcmp(header2.ChunkID, "RIFF", 4) != 0 || memcmp(header2.Format, "WAVE", 4) != 0) {
        fprintf(stderr, "\nError: Input files are not valid WAV files\n");
        exit(EXIT_FAILURE);
    }

    // Verify that the input files have the same format
    if (header1.AudioFormat != header2.AudioFormat || header1.NumChannels != header2.NumChannels ||
            header1.SampleRate != header2.SampleRate) {
        fprintf(stderr, "\nError: Input files have different audio formats\n");
        exit(EXIT_FAILURE);
    }

    if (silentMode == 0) {
        printf("\nBoth files are valid WAV files with the same audio format.");
        length_difference(header1.Subchunk2Size, header2.Subchunk2Size, numSystem);   // Display length difference
    }


    // Copy the header of the first input file to the output file
    memcpy(&header_out, &header1, sizeof(wav_header_t));

    // Update the header of the output file to reflect the total length of the audio data
    header_out.ChunkSize = sizeof(wav_header_t) + header1.Subchunk2Size + header2.Subchunk2Size +
                           5 * header1.SampleRate * header1.NumChannels * (header1.BitsPerSample / 8);
    header_out.Subchunk2Size = header1.Subchunk2Size + header2.Subchunk2Size +
                               5 * header1.SampleRate * header1.NumChannels * (header1.BitsPerSample / 8);

    // Write the header of the output file
    fwrite(&header_out, sizeof(wav_header_t), 1, output);


    if (silentMode == 0) {
        printf("\nGenerate the header of the output file: DONE");
    }

    // Copy the audio data from the fist input file to the output file
    char buffer[4096];
    size_t bytes_read;
    while((bytes_read = fread(buffer, 1, sizeof(buffer), input1)) > 0) {
        fwrite(buffer, 1, bytes_read, output);
    }

    if (silentMode == 0) {
        printf("\nTransfer audio data from the first input file: DONE");
    }

    // Generate a buffer of five seconds of silence
    int buffer_size = header1.SampleRate * header1.NumChannels * (header1.BitsPerSample / 8);
    char *silence_buffer = malloc(buffer_size * 5);
    generate_silence(silence_buffer, header1.SampleRate, header1.NumChannels, 5);

    // Write the buffer of silence to the output file
    fwrite(silence_buffer, 1, buffer_size * 5, output);

    if (silentMode == 0) {
        printf("\nAdd five second of silence after the audio data from the first file: DONE");
    }

    // Copy the audio data from the second input file to the output file
    while ((bytes_read = fread(buffer, 1, sizeof(buffer), input2)) > 0) {
        fwrite(buffer, 1, bytes_read, output);
    }

    if (silentMode == 0) {
        printf("\nTransfer audio data from the second input file: DONE");
    }

    // Close the files
    fclose(input1);
    fclose(input2);
    fclose(output);

    // Free the silence buffer
    free(silence_buffer);

    return 0;
}
