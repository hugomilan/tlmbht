/*
 * TLMBHT - Transmission-line Modeling Method applied to BioHeat Transfer Problems.
 * 
 * Copyright (C) 2015 to 2016 by Cornell University. All Rights Reserved.
 * 
 * Written by Hugo Fernando Maia Milan.
 * 
 * Free for educational, research and non-profit purposes.
 * Refer to the license file for details.
 */

/* 
 * File:   libstringtlmbht.c
 * Author: Hugo Fernando Maia Milan
 * Email:  hugofernando@gmail.com
 *
 * Created on June 24, 2016.
 *
 *
 *
 * Revision history:
 * 
 * Date: 
 * Who: 
 * Description: 
 *
 */

/*
 * Description of this function:
 * functions that manipulate strings
 *
 */

#include "libstringtlmbht.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdint.h>

/*
 * compareCaseInsensitive: compare the two inputs to verify if they are equal,
 * without considering the case of the words.
 * Returns 0 if they are equal.
 * 
 * Note that this function does not remove blank spaces before the inputs.
 * It should be done before getting here.
 * However, it ignores blank spaces after the word comparison until '=' or ';'
 * is found, or until the end of the length of the input.
 * All the text after '=' or ';' is ignored.
 */
unsigned int compareCaseInsensitive(char *input, char *wordToCompare) {
    // DEBUG: got inside and what was the input
    // printf("inside compareCaseInsensitive %s\n", input);

    // if the length of input (which can be a phrase) is less than the length
    // of wordToCompare then, by definition, they are not the same word.
    if (strlen(input) < strlen(wordToCompare)) {
        // DEBUG: Identify where did the return come from
        // printf("Returned 1");
        return 1;
    }

    // compare letter by letter. If one of the letters are not equal, then, we get into the if
    for (int i = 0; i < strlen(wordToCompare); i++) {
        if ((input[i] != wordToCompare[i]) && (input[i] != toupper(wordToCompare[i]))) {
            // DEBUG: Identify where did the return come from
            // printf("Returned 2");
            return 2;
        }
    }

    // now that I confirmed that the first characters are equal. I have to confirm
    // that the following characters are blank spaces until I find '=' or ';'

    char *pEqual, *pSemiColon;
    int length;
    pEqual = strchr(input, '=');
    pSemiColon = strchr(input, ';');

    if (pEqual == NULL) {
        // there is no '=' character nor ';' character
        if (pSemiColon == NULL) {
            length = strlen(input);

            // there is no '=' character but there is ';' character
        } else {
            length = strlen(input) - strlen(pSemiColon);
        }

        // there is no ';' character but there is '=' character
    } else if (pSemiColon == NULL) {
        length = strlen(input) - strlen(pEqual);

        // there is both '=' and ';' characters
    } else if (strlen(pEqual) > strlen(pSemiColon)) {
        length = strlen(input) - strlen(pEqual);
    } else {
        length = strlen(input) - strlen(pSemiColon);
    }


    for (int i = strlen(wordToCompare); i < length; i++) {
        if ((!isspace(input[i])) && input[i] != '\0' && input[i] != EOF) {
            // DEBUG: Identify where did the return come from
            // printf("Returned 3");
            return 3;
        }

    }
    // DEBUG: Identify where did the return come from
    // printf("Returned 0");
    return 0;
}

/*
 * getUsefulContent: remove comments (followed by '//') and remove blank spaces
 * at the begining and end of the string
 */
unsigned int getUsefulContent(char * input) {

    char * pCommentChar;

    // DEBUG: print the quantity of character and input
    // printf("Input line (%zu) %s:", strlen(input), input);

    // that's the end of file or just a new line character
    // Obs.: I don't expect to get EOF here because that's the condition to quit
    // the while-loop
    if (*input == EOF || *input == '\n') {
        // DEBUG: print the quantity of characters and the input
        //        printf("Only new line (%zu) %s:", strlen(input), input);
        return 9998;
    }


    // now I will look for comments

    // checking where the comments (if any) in this line start
    if ((pCommentChar = strchr(input, '/')) != NULL) // true if it found any (the first) '/'
    {
        // if that's true, the second character is / and, then, I have // which means
        // commentaries
        if (*(pCommentChar + 1) == '/') {
            input[(strlen(input) - strlen(pCommentChar))] = '\0';
            // DEBUG: print the quantity of characters and the input after removing comments
            // printf("Without comments (%zu) %s\n:", strlen(input), input);    

            // if the size is zero, then there were just comments on this line
            if (strlen(input) == 0) return 9998;
        }
    }

    removeBlankSpacesBeforeAndAfter(input);

    // if after we removed what is not important, the length is zero, then we just
    // don't need to process this line.
    if (strlen(input) < 1) return 9998;

    return 0;
}

/*
 * removeBlankSpacesBefore: Remove blank spaces at the begin of the input until
 * it finds a non-blank space.
 */
void removeBlankSpacesBefore(char * input) {
    while (isspace(input[0]) && *input != '\n') {
        for (int i = 0; i < strlen(input); i++) {
            input[i] = input[i + 1];
        }
        // DEBUG: Check that got inside the remove blanks before
        // printf("Inside (%zu): %s\n", strlen(input), input);
    }
}

/*
 * removeBlankSpacesAfter: Remove blank spaces at the end of the input until
 * it finds a non-blank space.
 */
void removeBlankSpacesAfter(char * input) {
    int i = strlen(input) - 1;
    while (isspace(input[i])) {
        input[i] = '\0'; // this is the end-of-line character
        i--;
    }
    // DEBUG: Check that got inside the remove blanks
    // printf("Inside (%zu): [%s]\n", strlen(input), input);
}

/*
 * removeBlankSpacesBeforeAndAfter: Remove blank spaces at the begin of the input until
 * it finds a non-blank space.
 * Then, remove blank spaces at the end of the input until
 * it finds a non-blank space.
 */
void removeBlankSpacesBeforeAndAfter(char * input) {
    // now I will remove the blank spaces before the other characters
    removeBlankSpacesBefore(input);
    // now I will remove the blank spaces after the other characters
    removeBlankSpacesAfter(input);
}

/*
 * getBetweenEqualAndSemicolon: get the text (or value) after '=' and before ';'
 */
unsigned int getBetweenEqualAndSemicolon(char * input) {
    char *pEqual, *pSemiColon;
    int i, ic = 0;

    // DEBUG: inside the function
    // printf("Inside getBetweenEqualAndSemicolon %s\n", input);
    if ((pEqual = strchr(input, '=')) == NULL) // true if it did not found '='
    {
        return 3668;
    }

    if ((pSemiColon = strchr(input, ';')) == NULL) // true if it did not found ';'
    {
        return 3669;
    }


    for (i = (strlen(input) - strlen(pEqual) + 1);
            i < (strlen(input) - strlen(pSemiColon)); i++) {
        input[ic++] = input[i];
    }
    input[ic++] = '\0';

    // DEBUG: print what the value this code gave for the input
    // printf("Value (%d) = %s\n",strlen(input), input);
    return 0;
}

/*
 * getBetweenBrackets: get text between []
 */
unsigned int getBetweenBrackets(char * input) {
    char *pStartBrackets, *pEndBrackets;
    int i, ic = 0;

    // DEBUG: inside the function
    // printf("Inside getBetweenBrackets %s\n", input);
    pStartBrackets = strchr(input, '[');
    pEndBrackets = strchr(input, ']');

    if (pStartBrackets == NULL && pEndBrackets == NULL) {
        return 9999; // this is not an error. We just did not find any brackets
    } else if (pStartBrackets == NULL && pEndBrackets != NULL) {
        return 7152;
    } else if (pStartBrackets != NULL && pEndBrackets == NULL) {
        return 7153;
    }


    for (i = (strlen(input) - strlen(pStartBrackets) + 1);
            i < (strlen(input) - strlen(pEndBrackets)); i++) {
        input[ic++] = input[i];
    }
    input[ic++] = '\0';

    // DEBUG: print what the value this code gave for the input
    // printf("Value (%d) = %s\n",strlen(input), input);
    return 0;
}

/*
 * readVectorInputs: read numbers that are allocated as line vectors
 */
unsigned int readVectorInputs(char * input, int * quantity, int ** numberStored) {
    unsigned int errorTLMnumber, withBrackets = 1;

    // get the text between equal and the semicolon
    if ((errorTLMnumber = getBetweenEqualAndSemicolon(input)) != 0)
        return errorTLMnumber;

    // get the text within the brackets, if any
    if ((errorTLMnumber = getBetweenBrackets(input)) != 0)
        if (errorTLMnumber != 9999) // when it is 9999, we did not find brackets
            return errorTLMnumber;

    if (errorTLMnumber == 9999) withBrackets = 0;
    // remove the excessive blank spaces at the beginning and at the end
    removeBlankSpacesBeforeAndAfter(input);

    // get how many blank spaces are between the numbers. More specifically, gets
    // the quantity of numbers separations.
    if ((errorTLMnumber = getQuantityOfBlankSpaces(input, quantity)) != 0)
        return errorTLMnumber;
    // DEBUG: testing how many blank spaces did it find
    // printf("We found %04d blank spaces.\n", *quantity);
    (*quantity)++;

    if (withBrackets == 0 && *quantity > 1) return 7154;

    // DEBUG: testing how many numbers did it find
    // printf("We found %04d numbers.\n", *quantity);

    // now I now exactly how many numbers I shall get
    *numberStored = (int*) malloc(sizeof (int)*(*quantity));

    if (withBrackets == 0 || *quantity == 1) {// I just have one number
        sscanf(input, "%d", numberStored[0]);
    } else {
        int j = 0;
        sscanf(input, "%d", numberStored[j]);
        j++;
        for (int i = 1; i < strlen(input); i++) {
            if (isspace(input[i])) {
                if (!isspace(input[i - 1])) {
                    sscanf(input + i, "%d", numberStored[0] + j);
                    j++;
                }

                // DEBUG: shows where spaces were found
                // printf("Position with space %d\n",i);
            }
        }

    }
    return 0;
}

/*
 * getQuantityOfBlankSpaces: get the quantity of blank spaces between the number.
 * double blank spaces are not counted. Example: '1 2 3  4   5' is read as 4 blank
 * spaces. The blank spaces between 3 and 4, 4 and 5, are counted as one, each one.
 */
unsigned int getQuantityOfBlankSpaces(char *input, int * quantity) {

    for (int i = 0; i < strlen(input); i++) {
        if (isspace(input[i])) {
            if (!isspace(input[i - 1]))
                (*quantity)++;
            // DEBUG: shows where spaces were found
            // printf("Position with space %d\n",i);
        }
    }

    //DEBUG: testing how many number did it find
    //    printf("We found %04d blank spaces.\n", *quantity);

    return 0;
}

/*
 * testStartBrackets: test the sintax for starting the brackets
 */
unsigned int testStartBrackets(char * input) {

    if (input[0] != '{') {
        return 1358;
    } // the first character is the brackets beginning

    // if there is more than 2 character, we shall investigate what it is
    // 2 characters: one is the bracket and the other is the end of line.
    if (strlen(input) > 2) {
        ++input; // remove the bracket
        while (isspace(*input) && input[0] != '\n') // remove blank lines
        {
            ++input;
        }

        // if there is something left, then it is likely that there is a syntax mistake
        if (strlen(input) > 1) {
            return 6742;
        }

    }

    return 0;
}

/*
 * testEndBrackets: Tests the end of bracket
 */
unsigned int testEndBrackets(char * input) {

    // remove the bracket
    input++;
    // if there is something left, then it is likely that there is a syntax mistake
    if (strlen(input) > 1) {
        return 6743;
    }

    return 0;
}