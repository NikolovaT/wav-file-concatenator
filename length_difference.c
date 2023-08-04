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
