#define _CRT_SECURE_NO_WARNINGS // For work in Visual Studio 

// Need it for scanf, printf, operations with strings, error detection and Windows
#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <Windows.h>

// Also for error detection
extern int errno;

// Constants 
#define DISCOUNT_MIN_AGE 18
#define DISCOUNT_MAX_AGE 25
#define DISCOUNT_PERCENTAGE 0.25f
#define MAX_SALES 10
#define SIZE 5
#define CSV_FILE "data.csv"

#define MENU_OPTION_BUY_CARS 'a'
#define MENU_OPTION_VIEW_CARS_STOCK 'b'
#define MENU_OPTION_VIEW_SALES_DATA 'c'
#define MENU_OPTION_EXIT 'x'

#define TRUE 1
#define FALSE 0

#define bool unsigned char

unsigned short carsAvailable = 100; 
unsigned short carsAvailableConst = 100; // I use it to know initial quantity of carsAvailable
unsigned short numberOfSales = 0;
char carModel[MAX_SALES][201];
unsigned short carAmountPerSale[MAX_SALES];
unsigned short typeOfCarPerSale[MAX_SALES];
bool discountGivenPerSale[MAX_SALES];
float fullTotalPrice[MAX_SALES];
float discount[MAX_SALES];
char customerNames[MAX_SALES][201];
char customerName[200]; // Using when sales is proccesing 

float carPrices[] = { 10000.0f, 15000.0f, 20000.0f, 25000.0f, 30000.0f }; 
char carTypes[][10] = { "CHEVROLET", "AUDI", "FORD", "MERCEDES", "TESLA" };
int remainingAmountOfCarModel[] = { 25, 15, 20, 10, 30 };
int remainingAmountOfCarModelConst[] = { 25, 15, 20, 10, 30 }; // I use it to know initial quantity of remainingAmountOfCarModel


// Will hold the file after opening
FILE* file;

// Here I'm creating a file
void createfile() {

    // If file does not exist, then we need to create file
    if ((file = fopen(CSV_FILE, "r")) != NULL) {

        fclose(file);

    }

    else {

        file = fopen(CSV_FILE, "w");
        fclose(file);

    }

}

// Here I'm reading a file 
void readDataFromFile() {

    FILE* file = fopen(CSV_FILE, "r");

    carsAvailable = carsAvailableConst;

    // 
    for (int i = 0; i < SIZE; i++) {

        remainingAmountOfCarModel[i] = remainingAmountOfCarModelConst[i];

    }

    // This counting amount of lines were read from the file
    int lineCounter = 0;

    int count = 0;
    char c;

    for (c = getc(file); c != EOF; c = getc(file))

        if (c == '\n')

            count = count + 1;

    fclose(file);

    // Here I'm reading data from file 
    FILE* fileRead = fopen(CSV_FILE, "r");

    while (lineCounter < count) {

        fscanf(file, "%[^,],%hd,%f,%f,%[^,],\n", carModel[lineCounter], &carAmountPerSale[lineCounter], &fullTotalPrice[lineCounter], &discount[lineCounter], customerNames[lineCounter]);

        // Here I'm updating remaining amount of cars
        for (int i = 0; i < SIZE; i++) {

            if (strcmp(carModel[lineCounter], carTypes[i]) == 0) {

                remainingAmountOfCarModel[i] -= carAmountPerSale[lineCounter];
                carsAvailable -= carAmountPerSale[lineCounter];

            }

        }

        lineCounter++;

    }

    numberOfSales = lineCounter;

    fclose(fileRead);

}

// Here I'm writing data to file
void writeDataToFile(char carModel[], unsigned carQuantity, float carTotalPrice, float discount, char name[]) {

    // Here I additing data to existing file 
    FILE* file = fopen(CSV_FILE, "a");

    if (file) {

        fprintf(file, "%s,%d,%f,%f,%s,\n", carModel, carQuantity, carTotalPrice, discount, name);

    }

    else {

        // Here I creating and writting file
        fclose(file);
        FILE* file = fopen(CSV_FILE, "w");
        fprintf(file, "%s,%d,%f,%f,%s,\n", carModel, carQuantity, carTotalPrice, discount, name);

    }

    fclose(file);

}

// I did this so that the buffer was cleared and there were no problems with validation
void clearBuffer() {

    char c;

    do {

        c = getchar();

    } while (c != '\n' && c != EOF);

}

// Function exist only for one reason - clear the screen
void clearScreen() {

    system("cls");

}

// I use this function where I need pause code and give user chance to write char
void pauseProgram(char userChoice) {

    if (userChoice == MENU_OPTION_EXIT) {

        printf("\n\nPress Enter to Exit...");

    }

    else {

        printf("\n\nPress Enter to return to the Menu...");

    }

    if (userChoice != '_') {

        getchar();

    }

    getchar();

}

// This is a simple scanf for user input
char getCharFromConsole(char message[201]) {

    char userInput;

    printf(message);
    scanf("\n%c", &userInput);

    return userInput;

}

// The same as previous, but for unsigned short
unsigned short getUnsignedShortFromConsole(char message[201]) {

    unsigned short userInput;

    printf(message);
    scanf("%hd", &userInput);

    return userInput;

}

// The same as previous, but for string
void getStringFromConsole(char message[201], char* str) {

    printf(message);
    scanf("\n%[^\n]s", str);

}

// Function was made for swaping two numbers 
void swapUnsignedShort(unsigned short* a, unsigned short* b) {

    unsigned short temp;

    temp = *a;
    *a = *b;
    *b = temp;

}

// This is for menu Buying Cars (user can choose between different models of cars)
void menu_showCarTypes() {

    int numberOfCars = sizeof(carPrices) / sizeof(float);

    printf("\nCar Types:\n");

    for (int i = 0; i < numberOfCars; i++) {

        printf("%d - %s\n", i, carTypes[i]);

    }

}

// Function for activating discount
float menu_applyDiscount(float currentPrice) {

    return currentPrice * (1 - DISCOUNT_PERCENTAGE);

}

// Function for determining if user need to have discount
bool menu_checkIfDiscountIsNeeded(unsigned short userAge) {

    if (userAge >= DISCOUNT_MIN_AGE && userAge <= DISCOUNT_MAX_AGE) {

        return TRUE;

    }

    else {

        while (TRUE) {

            clearBuffer();
            char nusCardResponse = getCharFromConsole("\nDo you have an NUS card ? Answer 'y' or 'n' : ");

            if (nusCardResponse == 'y') {

                return TRUE;

            }

            if (nusCardResponse == 'n') {

                return FALSE;

            }
        }
    }
}

// The function prints whether the user have or have not the discount
void menu_printDiscountOutcome(bool giveDiscount) {

    switch (giveDiscount) {

    case TRUE:

        printf("\nYou get a discount!");
        break;

    case FALSE:

        printf("\nNo discount given.");
        break;

    }

}

// Function for greeting of our Car Sales Market
void menu_greetCustomer() {

    printf("Welcome to the Car Sales Market!\n\n");

}

// Function for menu where user can choose his/her option
void menu_showMenu() {

    printf("Menu:\n");
    printf("%c. Buy Cars\n", MENU_OPTION_BUY_CARS);
    printf("%c. View Cars Stock\n", MENU_OPTION_VIEW_CARS_STOCK);
    printf("%c. View Sales Data\n", MENU_OPTION_VIEW_SALES_DATA);
    printf("%c. Exit\n\n", MENU_OPTION_EXIT);

}

// Function if user choose Buy Cars 
int menu_buyCars() {

    if (carsAvailable > 0) {

        printf("Buy Cars:\n\n");
        printf("There are %hd cars available.\n\n", carsAvailable);

    }

    else {

        printf("\nSorry, there are no more cars available.");
        return 0;

    }

    unsigned short carsNeeded = 0, userAge = 0, carType;
    bool giveDiscount = FALSE;
    float totalPrice = 0;

    clearBuffer();
    getStringFromConsole("What is your name? Name: ", &customerName);

    clearBuffer();
    carsNeeded = getUnsignedShortFromConsole("\nHow many cars do you need? Amount: ");

    // Validation
    while (carsNeeded == 0) {

        clearBuffer();
        carsNeeded = getUnsignedShortFromConsole("\nSorry, you can't buy zero cars. Write real amount of cars what you want to buy: ");

    }

    // Validation
    while (carsAvailable < carsNeeded) {

        clearBuffer();
        carsNeeded = getUnsignedShortFromConsole("\nSorry, look at the number of cars that are left and write the amount of cars that you want to buy:  ");

        // Validation
        while (carsNeeded == 0) {

            clearBuffer();
            carsNeeded = getUnsignedShortFromConsole("\nSorry, you can't buy zero cars. Write real amount of cars what you want to buy: ");

        }

    }

    carAmountPerSale[numberOfSales] = carsNeeded;

    menu_showCarTypes();
    clearBuffer();

    carType = getUnsignedShortFromConsole("\nWhat type of car do you need? Choose from above: ");

    // Validation
    while (carType >= 5) {

        clearBuffer();
        carType = getUnsignedShortFromConsole("\nYou need to choose car from the list: ");

    }

    // Check if we have needed amount of cars
    if (remainingAmountOfCarModel[carType] >= carsNeeded) {

        remainingAmountOfCarModel[carType] -= carsNeeded;

    }

    else {

        printf("\nUnfortunately we have run out of cars of this model\n");
        return 0;

    }

    typeOfCarPerSale[numberOfSales] = carType;

    totalPrice = carsNeeded * carPrices[carType];

    carsAvailable -= carsNeeded;
    clearBuffer();

    userAge = getUnsignedShortFromConsole("\nHow old are you? Age: ");

    // Validation
    while (userAge <= 15 || userAge >= 100) {

        clearBuffer();
        userAge = getUnsignedShortFromConsole("\nPlease write the right age to buy a car: ");

    }

    giveDiscount = menu_checkIfDiscountIsNeeded(userAge);

    if (giveDiscount == TRUE) {

        totalPrice = menu_applyDiscount(totalPrice);

    }

    discountGivenPerSale[numberOfSales] = giveDiscount;

    menu_printDiscountOutcome(giveDiscount);

    printf("\n\nThank you.\n");
    printf("You have bought %hd cars.\n", carsNeeded);
    printf("Total cost is %f GBP.\n", totalPrice);
    printf("\nThere are %hd cars remaining.", carsAvailable);

    fullTotalPrice[numberOfSales] = totalPrice;

    numberOfSales++;

    // Here I'm writing data which user choose to data 
    writeDataToFile(carTypes[carType], carsNeeded, carsNeeded * carPrices[carType], carsNeeded * carPrices[carType] - totalPrice, customerName);

    return 1;

}

// I made Selection Sort for Car Stock which takes the smallest number in the array and moves it to the needed position in the array
void SelectionSortCarStock() {

    // Loop will go through our array until penultimate value
    for (int i = 0; i < SIZE - 1; i++) {

        int position = i;

        // This loop find index from unsorted portion of the array
        for (int j = i + 1; j < SIZE; j++)

            if (remainingAmountOfCarModel[j] > remainingAmountOfCarModel[position]) position = j;

        if (position != i) {

            // Swapping remaining amount of car model
            unsigned temp = remainingAmountOfCarModel[i];
            remainingAmountOfCarModel[i] = remainingAmountOfCarModel[position];
            remainingAmountOfCarModel[position] = temp;

            // Swapping remaining amount of car model in array that we didn't touch
            unsigned temp_const = remainingAmountOfCarModelConst[i];
            remainingAmountOfCarModelConst[i] = remainingAmountOfCarModelConst[position];
            remainingAmountOfCarModelConst[position] = temp_const;

            // Swapping car types
            char temp_model[100];
            strcpy(temp_model, carTypes[i]);
            strcpy(carTypes[i], carTypes[position]);
            strcpy(carTypes[position], temp_model);

            // Swapping car prices
            float temp_price = carPrices[i];
            carPrices[i] = carPrices[position];
            carPrices[position] = temp_price;

        }

    }

}

// Bubble Sort for View Sales Data
void BubbleSortViewSalesData() {

    for (int i = 0; i < numberOfSales; i++) {

        for (int j = 0; j < numberOfSales - 1 - i; j++) {

            // Here I'm swapping data if total price is higher compared with previous sale
            if (fullTotalPrice[j] < fullTotalPrice[j + 1]) {

                // Swaping Car Model
                char temp_model[100];
                strcpy(temp_model, carModel[j]);
                strcpy(carModel[j], carModel[j + 1]);
                strcpy(carModel[j + 1], temp_model);

                // Swaping number of car
                unsigned short temp_carAmountPerSale;
                temp_carAmountPerSale = carAmountPerSale[j];
                carAmountPerSale[j] = carAmountPerSale[j + 1];
                carAmountPerSale[j + 1] = temp_carAmountPerSale;

                // Swaping total price
                float temp_fullTotalPrice;
                temp_fullTotalPrice = fullTotalPrice[j];
                fullTotalPrice[j] = fullTotalPrice[j + 1];
                fullTotalPrice[j + 1] = temp_fullTotalPrice;

                // Swaping discount value
                float tempdiscount;
                tempdiscount = discount[j];
                discount[j] = discount[j + 1];
                discount[j + 1] = tempdiscount;

                // Swaping customer name
                char temp_customerNames[100];
                strcpy(temp_customerNames, customerNames[j]);
                strcpy(customerNames[j], customerNames[j + 1]);
                strcpy(customerNames[j + 1], temp_customerNames);

            }

        }

    }

}

// View Stock function 
void menu_viewCarsStock() {

    SelectionSortCarStock();

    for (int i = 0; i < sizeof(carTypes) / sizeof(carTypes[0]); i++) {

        printf("%s - %d - %f\n", carTypes[i], remainingAmountOfCarModel[i], carPrices[i]);

    }

}

// View Sales information function 
void menu_viewSalesData() {

    BubbleSortViewSalesData();

    printf("All Sales Data:\n\n");

    // Print sales information of each user
    for (int i = 0; i < numberOfSales; i++) {

        printf("Sale Index: %d | Type of Car: %s | Number of Cars: %hd | Sale Amount: %f | Discount Given: %f | Customer Name: %s\n\n", i + 1, carModel[i], carAmountPerSale[i], fullTotalPrice[i], discount[i], customerNames[i]);

    }

}

// Exiting function to say thanks to customer for using my programm
void menu_exit() {

    printf("Thank you for using this Car Sales Market. Bye-bye!");

}

// And here I have main function where all function is used and thanks to this function programme works 
void main() {

    createfile();
    char userChoice;

    do {

        readDataFromFile();
        clearScreen();

        menu_greetCustomer();
        menu_showMenu();

        userChoice = getCharFromConsole("Please choose one: ");

        clearScreen();

        switch (userChoice) {

        case MENU_OPTION_BUY_CARS:

            menu_buyCars();

            if (menu_buyCars == 0) {

                continue;

            }

            break;

        case MENU_OPTION_VIEW_CARS_STOCK:

            menu_viewCarsStock();
            break;

        case MENU_OPTION_VIEW_SALES_DATA:

            menu_viewSalesData();
            break;

        case MENU_OPTION_EXIT:

            menu_exit();
            break;

        }

        pauseProgram(userChoice);

    } while (userChoice != MENU_OPTION_EXIT);

    clearScreen();

    printf("Have a good day!\n");

}