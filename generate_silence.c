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
