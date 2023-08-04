#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <math.h>
#include <stdint.h>
#include <libgen.h>
#include "wav.h"
#include "generate_silence.c"
#include "length_difference.c"
#include "print_messages.c"

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

    print_messages(silentMode, 1);

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

    print_messages(silentMode, 2);
    if(silentMode==0){
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

    print_messages(silentMode, 3);

    // Copy the audio data from the fist input file to the output file
    char buffer[4096];
    size_t bytes_read;
    while((bytes_read = fread(buffer, 1, sizeof(buffer), input1)) > 0) {
        fwrite(buffer, 1, bytes_read, output);
    }

    print_messages(silentMode, 4);

    // Generate a buffer of five seconds of silence
    int buffer_size = header1.SampleRate * header1.NumChannels * (header1.BitsPerSample / 8);
    char *silence_buffer = malloc(buffer_size * 5);
    generate_silence(silence_buffer, header1.SampleRate, header1.NumChannels, 5);

    // Write the buffer of silence to the output file
    fwrite(silence_buffer, 1, buffer_size * 5, output);

    print_messages(silentMode, 5);

    // Copy the audio data from the second input file to the output file
    while ((bytes_read = fread(buffer, 1, sizeof(buffer), input2)) > 0) {
        fwrite(buffer, 1, bytes_read, output);
    }

    print_messages(silentMode, 6);

    // Close the files
    fclose(input1);
    fclose(input2);
    fclose(output);

    // Free the silence buffer
    free(silence_buffer);

    return 0;
}
