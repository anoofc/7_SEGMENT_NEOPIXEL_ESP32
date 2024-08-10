#define DEBUG       0           // Set to 1 to enable debug output
#define LEDPIN      25          // Pin number for the LED strip
#define DIGITS      3           // Number of digits in the display
#define NUM_PIXELS  7*DIGITS    // Number of pixels in the strip

// Define the macros
#define CLEAR strip.clear(); strip.show()       // Clear the display
#define DELAY delay(10)                         // Delay for 10ms

// Define the colors
#define RED         255,   0,   0
#define GREEN         0, 255,   0
#define BLUE          0,   0, 255
#define YELLOW      255, 255,   0
#define ORANGE      255, 125,   0
#define PURPLE      255,   0, 255
#define CYAN          0, 255, 255
#define WHITE       255, 255, 255
#define BLACK         0,   0,   0

// Include the necessary libraries
#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

// Create an instance of the Adafruit_NeoPixel class
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_PIXELS, LEDPIN, NEO_GRB + NEO_KHZ800);

int count = 9999;  // Initialize the count variable to 0

// Define the binary representation of the numbers 0 to 9 to display on the 7-segment display
const uint8_t numbers[10][7] = {
    {1,1,1,1,1,1,0}, // Zero
    {0,1,1,0,0,0,0}, // One
    {1,1,0,1,1,0,1}, // Two
    {1,1,1,1,0,0,1}, // Three
    {0,1,1,0,0,1,1}, // Four
    {1,0,1,1,0,1,1}, // Five
    {1,0,1,1,1,1,1}, // Six
    {1,1,1,0,0,0,0}, // Seven
    {1,1,1,1,1,1,1}, // Eight
    {1,1,1,1,0,1,1}  // Nine
};

// Function to display the number
// number is the number to display
// color is the color to display the number
// Check the number[] array to see which LEDs to turn on
void displayNumber(const uint8_t* number, uint32_t color, uint32_t color2, uint8_t digit) {
    for (int i = (0); i < (7); i++) {
        if (number[i] == 1) {
            strip.setPixelColor(i+(7*digit), color);
        } else {
            strip.setPixelColor(i+(7*digit) , color2);
        }
        strip.show();
        // if (DEBUG) { Serial.print(number[i]); }     // Print the number binary array to the serial monitor
    }
    // if (DEBUG){Serial.println();}   // Print a newline to the serial monitor
}


// Function to slice the four digit number into individual digits
// Update the number on the display
// If debug is enabled, print the count and the individual digits to the serial monitor
// Call anywhere to update the number on the display

void numberUpdate(uint16_t number) {
    uint8_t digit0 = number % 10;               // Get the last digit of the number
    uint8_t digit1 = (number / 10) % 10;        // Get the second last digit of the number
    uint8_t digit2 = (number / 100) % 10;       // Get the third last digit of the number
    uint8_t digit3 = (number / 1000) % 10;      // Get the fourth last digit of the number
    displayNumber(numbers[digit0], strip.Color(RED), strip.Color(BLACK), 3);        // Display the last digit
    displayNumber(numbers[digit1], strip.Color(RED), strip.Color(BLACK), 2);        // Display the second last digit
    displayNumber(numbers[digit2], strip.Color(RED), strip.Color(BLACK), 1);        // Display the third last digit
    displayNumber(numbers[digit3], strip.Color(RED), strip.Color(BLACK), 0);        // Display the fourth last digit
    if (DEBUG) { Serial.println ("Digit 3: " + String(digit3) + "\t Digit 2: " + String(digit2) + "\t Digit 1: " + String (digit1) + "\t Digit 0: " + String (digit0)); }
}


// Function to process the serial input
// Check if the input data is a number between 0 and 9999 and call the numberUpdate function to update the display
// Print an error message to the serial monitor if the input is invalid

void processData(String inputData) {
    // Check if the input data is a number
    if (inputData.toInt() != 0 || inputData.equals("0")) {            // Check if the input data is a number
        int data = inputData.toInt();                                 // Convert the input data to an integer
        if (data >= 0 && data <= 9999) { numberUpdate(data); }        // Check if the input data is between 0 and 9999 and call the numberUpdate function to update the display
        else { if (DEBUG) { Serial.println("Invalid input"); }}       // Otherwise Print an error message to the serial monitor if the input is invalid
    } else { if (DEBUG) { Serial.println("Invalid input");}}          // Print an error message to the serial monitor if the input is not a number
}


// Function to read the serial input
// Check if there is data available on the serial port
// Read the data and call the processData function

void readSerial(){
    if (Serial.available() > 0) {
        String inputData = Serial.readStringUntil('\n');
        inputData.trim();
        if (DEBUG){ Serial.print("Received: "); Serial.println(inputData); }
        CLEAR;                                      // Clear the display
        processData(inputData);                     // Call the processData function to process the input data
    }
}


// Function to count down from 9999 to 0
// Update the number on the display
// If the count reaches 0, reset it to 9999
// For testing purposes
void countDown(){
    if (count > 0) { numberUpdate(count); count--; DELAY; } 
    else { count = 9999;}
}

void setup() {
    Serial.begin(9600);     // Start the serial monitor
    strip.begin();          // Initialize the strip

}

// Display each number in the loop
void loop() {

    readSerial();   // Read the serial input  and display the number
    // countDown();    // Count down from 9999 to 0 for testing purposes
    
}