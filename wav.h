typedef struct {
    char ChunkID[4];  // should be "RIFF"
    int32_t ChunkSize;  // size of the entire file in bytes minus 8 bytes for the previous two fields
    char Format[4];  // should be "WAVE"
    char Subchunk1ID[4];  // should be "fmt "
    int32_t Subchunk1Size;  // size of the rest of the Subchunk (16 for PCM)
    int16_t AudioFormat;  // PCM = 1 if no compression
    int16_t NumChannels;  // Mono = 1, Stereo = 2, etc.
    int32_t SampleRate;  // 8000, 44100, etc.
    int32_t ByteRate;  // == SampleRate * NumChannels * BitsPerSample/8
    int16_t BlockAlign;  // == NumChannels * BitsPerSample/8 The number of bytes for one sample including all channels
    int16_t BitsPerSample;  // 8 bits = 8, 16 bits = 16, etc.
    char Subchunk2ID[4];  // should be "data"
    int32_t Subchunk2Size;  // == NumSamples * NumChannels * BitsPerSample/8 This is the number of bytes in the data
} wav_header_t;
