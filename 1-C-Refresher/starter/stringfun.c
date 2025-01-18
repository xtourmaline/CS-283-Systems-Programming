#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>


#define BUFFER_SZ 50

//prototypes
void usage(char *);
void print_buff(char *, int);
int  setup_buff(char *, char *, int);

//prototypes for functions to handle required functionality
int  count_words(char *, int , int);
void reverse(char*, int, int);
void word_print(char*, int, int);
void replace(char*, int, int, char*, char*);
//add additional prototypes here

int setup_buff(char *buff, char *user_str, int len){
    //TODO: #4:  Implement the setup buff as per the directions
    int count = 0;
    bool adjacentSpace = true; //starts off true to prevent beginning white spaces
    bool nullTermReach = false;

    memset(buff, '.', len);
    
    while (count != len) {
        char currChar = *(user_str++);

        if (currChar == '\0') { // checks whether we get to a null terminator
            nullTermReach = true;
            break;
        }

        // if we reach a white space, we check whether or not that there is an adjacent white space already, if there is, we go next loop cycle. if not, we add the space and continue.
        if (currChar == '\t' || currChar == '\n' || currChar == ' ') {
            if (adjacentSpace) {
                continue;
            }

            *(buff + count) = ' ';
            adjacentSpace = true;
        } else {
            *(buff + count) = currChar;
            adjacentSpace = false;
        }

        count++;
    }

    // if we dont reach the null terminator, we know that there is an overflow
    if (!nullTermReach && *(user_str) != '\0') {
        return -1;
    }

    if (adjacentSpace) {
        count--;
        *(buff + count) = '.';
    }

    return count;
}

void print_buff(char *buff, int len){
    printf("Buffer:  [");
    for (int i=0; i<len; i++){
        putchar(*(buff+i));
    }
    putchar(']');
    putchar('\n');
}

void usage(char *exename){
    printf("usage: %s [-h|c|r|w|x] \"string\" [other args]\n", exename);

}

int count_words(char *buff, int, int str_len){
    int count = 1;

    for (int i = 0; i < str_len; ++i) {// counts up every time we see a space
        if (*(buff + i) == ' ') {
            count++;
        }
    }

    return count;
}

void reverse(char* buff, int, int str_len) {
    char temp;

    for (int i = 0; i < (str_len / 2); ++i) { // swaps the element at beginning and end and moved in 1 by 1 till it gets to the middle
        temp = *(buff + i);
        *(buff + i) = *(buff + (str_len - 1 - i));
        *(buff + (str_len - 1 - i)) = temp;
    }
}

void word_print(char* buff, int, int str_len) {
    printf("Word Print\n");
    printf("----------\n");

    int count = 1;
    int charCount = 0;
    bool startWord = true; // this is so we dont repeat certain prints

    for (int i = 0; i < str_len; ++i) {
        if (startWord) {
            printf("%d. ", count);
            startWord = false;
        }
        
        if (*(buff + i) != ' ') { // this is to print each character
            charCount++;
            putchar(*(buff + i));
            continue;
        }

        printf("(%d)\n", charCount); // gives the character count once the word is completed

        count++;
        charCount = 0;
        startWord = true; // to go onto the next word
    }

    printf("(%d)\n", charCount); // we need this because in the last iteration, line 120 does not go off
    printf("\n");
    printf("Number of words returned: %d\n", count);
}

void replace(char* buff, int len, int str_len, char* prev, char* next) {
    bool matching = false; // if we are currently processing a possible match

    // helpful information about the substring 'prev' in the buffer
    int begin = -1, end = -1;
    int length = 0;

    for (int i = 0; i < str_len; ++i) {
        if (!matching) {
            if (*prev == *(buff + i)) {
                matching = true;
                begin = i;
                ++length;
            }
        }
        else {
            if (*(prev + length) == *(buff + i)) {
                ++length;
                // check ahead for if we've finished searching for our match.
                // this handles the corner case of 'prev' being at the end of
                // the string, which would normally force the loop to end prematurely
                if (*(prev + length) == '\0') {
                    end = i;
                    break;
                }
            }
            // if our matching failed midway, we completely reset and disregard all old info
            else {
                begin = -1;
                end = -1;
                length = 0;

                matching = false;
            }
        }
    }

    // these length variables are later used to cut off the excess
    int left_length = begin; // from 0 to head of match
    int right_length = str_len - (end + 1); // from str_len to tail of match, +1 to match past the last char
    int next_length = 0;

    for (char *c = next; *c != '\0'; ++c) {
        ++next_length;
    }

    // variables to hold the left and right sides of the buffer
    // after excluding the 'prev' substring. we use these to join
    // the strings again in the buffer.
    char* left = malloc(sizeof(char) * len);
    char* right = malloc(sizeof(char) * len);

    if (left == NULL || right == NULL) {
        printf("error: failed to replace() (substring malloc failure)\n");
        exit(-2);
    }

    // copy everything from before the substring match, and after it
    //
    // e.g. for "hello foo bar", assuming a match on foo:
    // left => "hello "
    // right => " bar"
    memcpy(left, buff, left_length);
    memcpy(right, buff + end + 1, right_length);
    
    // clear out the entire buffer to dots
    memset(buff, '.', len);

    // clamp values to ensure they always stay below 50.
    // we do not perform this on left_length, since it
    // already fit in the buffer once.
    //
    // e.g. for a replacement where left_length = 49, next_length = 4, right_length = 10,
    // this would effectively clamp next_length = 1, and subsequently clamp right_length = 0.
    if (left_length + next_length > len) {
        next_length -= (left_length + next_length) - len;
    }
    if (left_length + next_length + right_length > len) {
        right_length -= (left_length + next_length + right_length) - len;
    }

    memcpy(buff, left, left_length);
    memcpy(buff + left_length, next, next_length);
    memcpy(buff + left_length + next_length, right, right_length);

    //printf("'%s' '%s'\n", left, right);

    free(left);
    free(right);
}

/*
void replace(char* buff, int len, int str_len, char* word, char* newWord) {
    int newWordLen = 0;
    int wordLen = 0;
    
    int match = 0; //keeps track of how many letters are match so far of the word
    int matchStart = -1;

    int diff = 0;

    for (int i = 0; newWord[i] != '\0'; ++i) {
        newWordLen++;
    }

    for (int i = 0; word[i] != '\0'; ++i) {
        wordLen++;
    }

    int new_str_len = str_len - wordLen + newWordLen;
    if (new_str_len > 50) {
        //printf("New word is too big for buffer.\n");
        //exit(1);

        diff = new_str_len - str_len;
    } // checks if new string len will be over 50

    for (int i = 0; i < str_len; ++i) { // trying to find if we have a match in the string, if we do, we will be able to find where the match starts
        if (*(buff + i) == *(word + match)) {
            match++;

            if (match == wordLen) {
                break; // breaks out of the loop once we find a match
            } else if (match == 1) {
                matchStart = i;
            }
        } else {
            match = 0;
            matchStart = -1;
        }
    }

    if (matchStart == -1) {
        printf("Could not find word in string provided.\n");
        exit(3);
    }

    int j = str_len;

    // how we put in the new word will depend on if the word makes the new string length shorter or longer
    if (new_str_len > str_len) {
        // copies starting from the end of the original string to the position of the new string
        for (int i = new_str_len - diff; i > matchStart; --i) {
            *(buff + i) = *(buff + j - diff);
            j--;
        }

        j = 0;
        
        // now that we have enough space to put in the new word without rewriting anything, we copy in the new word.
        for (int i = matchStart; i < matchStart + newWordLen; ++i) {
            *(buff + i) = *(newWord + j);
            j++;
        }
    } else {
        j = 0;
        bool firstSpace = false;

        // put in the new word first
        for (int i = matchStart; i < matchStart + newWordLen; ++i) {
            *(buff + i) = *(newWord + j);
            j++;
        }

        j = matchStart + j;

        // then we shift everything to the right
        for (int i = j; i < str_len + (str_len - new_str_len + 1); ++i) {

            if (*(buff + i) == ' ' || i > str_len) {
                firstSpace = true;
            }

            if (!firstSpace) {
                continue;
            }

            *(buff + j) = *(buff + i);
            j++;
        }

        // if we have leftover spaces in the buffer, we add the period back (this usually happens if the buff was full)
        for (int i = new_str_len; i < len; i++) {
            *(buff + i) = '.';
        }
    }
}
*/

//ADD OTHER HELPER FUNCTIONS HERE FOR OTHER REQUIRED PROGRAM OPTIONS

int main(int argc, char *argv[]){

    char *buff;             //placehoder for the internal buffer
    char *input_string;     //holds the string provided by the user on cmd line
    char opt;               //used to capture user option from cmd line
    int  rc;                //used for return codes
    int  user_str_len;      //length of user supplied string

    //TODO:  #1. WHY IS THIS SAFE, aka what if arv[1] does not exist?
    //      This is safe because for the second part of the or statement to be evaluted, the first part of the or statement would need to be false which means argc is greater than or equal to 2, ensuring that you can always access argv[1].
    if ((argc < 2) || (*argv[1] != '-')){
        usage(argv[0]);
        exit(1);
    }

    opt = (char)*(argv[1]+1);   //get the option flag

    //handle the help flag and then exit normally
    if (opt == 'h'){
        usage(argv[0]);
        exit(0);
    }

    //WE NOW WILL HANDLE THE REQUIRED OPERATIONS

    //TODO:  #2 Document the purpose of the if statement below
    //      This is to exit if we do not have enough arguments when we call the command (less than three arguments in argc). Then it will call usage which will print out the correct way to call the command and exit with a 1.
    if (argc < 3){
        usage(argv[0]);
        exit(1);
    }

    input_string = argv[2]; //capture the user input string

    //TODO:  #3 Allocate space for the buffer using malloc and
    //          handle error if malloc fails by exiting with a 
    //          return code of 2, as per requirements.
    // CODE GOES HERE FOR #3
    buff = malloc(sizeof(char)*BUFFER_SZ);

    if (buff == NULL) {
        exit(2);
    }

    user_str_len = setup_buff(buff, input_string, BUFFER_SZ);     //see todos
    if (user_str_len < 0){
        printf("Error setting up buffer, error = %d\n", user_str_len);
        exit(3);
    }

    switch (opt){
        case 'c':
            rc = count_words(buff, BUFFER_SZ, user_str_len);  //you need to implement
            if (rc < 0){
                printf("Error counting words, rc = %d\n", rc);
                exit(3);
            }
            printf("Word Count: %d\n", rc);
            break;
        case 'r':
            reverse(buff, BUFFER_SZ, user_str_len);
            break;
        case 'w':
            word_print(buff, BUFFER_SZ, user_str_len);
            break;
        //TODO:  #5 Implement the other cases for 'r' and 'w' by extending
        //       the case statement options
        case 'x':
            if (argc != 5) {
                usage(argv[0]);
                exit(1);
            }

            replace(buff, BUFFER_SZ, user_str_len, argv[3], argv[4]);
            break;
        default:
            usage(argv[0]);
            exit(1);
    }

    //TODO:  #6 Dont forget to free your buffer before exiting
    print_buff(buff,BUFFER_SZ);

    free(buff);
    exit(0);
}

//TODO:  #7  Notice all of the helper functions provided in the 
//          starter take both the buffer as well as the length.  Why
//          do you think providing both the pointer and the length
//          is a good practice, after all we know from main() that 
//          the buff variable will have exactly 50 bytes?
//  
//          In general, it's always good to pass both the size of a memory buffer
//          when passing the buffer. This is to help prevent buffer overflows
//          or segmentation faults. It can be useful to provide a concept of modularity,
//          where our functions aren't directly dependent on any values outside of their
//          own context (parameters + local vars).