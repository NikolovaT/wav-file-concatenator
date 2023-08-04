//----------------------------------------------------------------------------------------------
//DESCRIPTION:
// Prints logs showing the programs progress
//
//PARAMETERS:
// int silentMode
//
//
//RETURN VALUE:
// return concatenated: the full path and defaut name for the output file
//----------------------------------------------------------------------------------------------

void print_messages(int silentMode, int message){
    if(silentMode==0){
        switch(message){
            case 1:
                printf("\nFiles successfully opened");
                break;
            case 2:
                printf("\nBoth files are valid WAV files with the same audio format.");

                break;
            case 3:
                printf("\nGenerate the header of the output file: DONE");
                break;
            case 4:
                printf("\nTransfer audio data from the first input file: DONE");
                break;
            case 5:
                printf("\nAdd five second of silence after the audio data from the first file: DONE");
                break;
            case 6:
                printf("\nTransfer audio data from the second input file: DONE");
                break;
        }
    }
}

