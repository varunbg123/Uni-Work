/*
 * Question Generator
 * Provides a method that generates simple math questions, provides the answer
 * and four options.
 *
 * Varun Gonsalves, Kaif Kutchwala, Emmanuel Leo
 *
 * Date: 07/05/2023
 *
 */

#include "QuestionGenerator.h"

#include <stdio.h>
#include <stdlib.h>

#include "Timer/Timer.h"

// Function to add two numbers
float add(float a, float b) {
    return a + b;
}

// Function to subtract two numbers
float subtract(float a, float b) {
    return a - b;
}

// Function to multiply two numbers
float multiply(float a, float b) {
    return a * b;
}

// Function to divide two numbers
float divide(float a, float b) {
    return a / b;
}

// Function to swap the values in two variables
// Exchange the values stored in the memory locations
// pointed to by the two pointers.
void swap(int* a, int* b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

// Array of math operations using function pointers.
// All functions have the same signature.
Operation operations[4] = {&add, &subtract, &multiply, &divide};

// Character Array that includes the symbol for each math operation
// symbols are in same order as operation function pointers.
char* operation_symbols[4] = {"+", "-", "x", "/"};

// Count number of operations stored.
int num_operations = sizeof(operation_symbols) / sizeof(operation_symbols[0]);

// Generates a random number between min_value and max_value inclusive
unsigned int generateRandomNumber(unsigned int min_value, unsigned int max_value) {
    int range;
    unsigned int random_num;
    // seed the random number generator with the current time
    srand(Timer_getValue());

    // Generate a random number within the range
    range = max_value - min_value + 1;
    random_num = (rand() % range) + min_value;

    return random_num;
}

// Function used to shuffle an array.
void shuffle(int arr[], int size) {
    int i, j;
    // seed the random number generator with the current time
    srand(Timer_getValue());
    // For loop to run through and change the positions of values in array
    for (i = size - 1; i > 0; i--) {
        // generate a random index between 0 and i
        j = rand() % (i + 1);
        // swap the elements at indices i and j
        swap(&arr[i], &arr[j]);
    }
}

// Question Generator
// Generates a question based on specified difficulty.
// Returns a QuestionResult that contains the question text,
// the answer, and an array of 4 options including the answer.
QuestionResult QuestionGenerator_generateQuestion(unsigned int difficulty) {
    // Declare result variable of type  QuestionResult.
    QuestionResult result;
    // Declare question_text to be 30 characters long
    char question_text[30];
    char* operation_symbol;

    // setting default min, max range values for number1 and number2
    int min_value = 1;
    // For higher difficulties use larger numbers
    // Done by changing the max value of both numbers
    int max_value_1 = difficulty != 1 ? 100 : 10;
    int max_value_2 = difficulty == 3 ? 100 : 10;

    // Generate random operation and 2 numbers
    int operation_id = generateRandomNumber(0, num_operations - 1);
    int number1 = generateRandomNumber(min_value, max_value_1);
    int number2 = generateRandomNumber(min_value, max_value_2);

    // if the operation is a substract, check if
    // the second value is greater than the first, if so swap
    // so that number1 is always greater than num2
    if (operation_id == 1) {
        if (number2 > number1)
            swap(&number1, &number2);
        if (number1 == number2) {  // If both numbers are equal
            // Add a random amount to number1 such that it is more than number2
            // but less than max_value_1
            // This is to prevent overflow when calculating the options
            int random_number = generateRandomNumber(min_value, max_value_1 - number1);
            number1 += random_number;
        }
    }

    // If the operation is a divide then we first multiply number1 and number2
    // and swap number1 to be the result.
    // This is done so that the question reads What is {number1*number2}/{number2}
    // Ensures that result of division is a whole number
    if (operation_id == 3) {
        int number3 = number1 * number2;
        swap(&number3, &number1);
    }

    // The operation is called here using number1 and number2 to get answer
    result.answer = operations[operation_id](number1, number2);
    // Create 4 options that are close to the answer
    result.options[0] = result.answer;
    result.options[1] = result.answer + 1;
    result.options[2] = result.answer - 1;
    result.options[3] = result.answer + 2;
    // Shuffle the options array to randomise the array
    shuffle(result.options, 4);

    // Generate the question_text
    // The symbols are using the same index as the operation and this can be extracted
    operation_symbol = operation_symbols[operation_id];
    // Create the sentence and store in question_text
    sprintf(question_text, "What is %u%s%u?", number1, operation_symbol, number2);
    // Sentence is written to result which is returned
    strcpy(result.text, question_text);

    return result;
}
