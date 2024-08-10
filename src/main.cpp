#define DEBUG       1           // Set to 1 to enable debug output
#define LEDPIN      25          // Pin number for the LED strip
#define DIGITS      1           // Number of digits in the display
#define NUM_PIXELS  7*DIGITS    // Number of pixels in the strip

// Define some macros to make the code more readable
#define CLEAR strip.clear(); strip.show()
#define DELAY delay(1000)

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

int count = 0;  // Initialize the count variable to 0

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
void displayNumber(const uint8_t* number, uint32_t color, uint32_t color2) {
    for (int i = 0; i < 7; i++) {
        if (number[i] == 1) {
            strip.setPixelColor(i, color);
        } else {
            strip.setPixelColor(i, color2);
        }
        strip.show();
        if (DEBUG) { Serial.print(number[i]); }     // Print the number binary array to the serial monitor
    }
    Serial.println();   // Print a newline to the serial monitor
}

// Function to process the serial input
void processData(char inputData) {
    uint8_t data = (uint8_t)(inputData - '0');                               // Convert the input data to an integer
    if (data >= 0 && data <= 9) {                                            // Check if the input data is between 0 and 9
        displayNumber(numbers[data], strip.Color(RED), strip.Color(BLACK));  // Display the number
    } else { if (DEBUG) { Serial.println("Invalid input"); }}                // Print an error message to the serial monitor if the input is invalid
}


// Function to read the serial input
// Check if there is data available on the serial port
// Read the data and call the processData function

void readSerial(){
    if (Serial.available() > 0) {
        char inputData = Serial.read();
        if (DEBUG){ Serial.print("Received: "); Serial.println(inputData); }
        CLEAR;
        processData(inputData);
    }
}

// Function to count down from 9 to 0
// Check if the count is less than 10 and display the number at that index in the numbers array with the color red 
// Increment the count variable and reset it to 0 if it reaches 10

void countDown(){
    if (count < 10) {   
        displayNumber(numbers[count], strip.Color(RED), strip.Color(BLACK));
        count++;
        if (count == 10) {
            count = 0;
        }
        DELAY;              // Delay for 1 second
    }
}

void setup() {
    Serial.begin(9600);     // Start the serial monitor
    strip.begin();          // Initialize the strip

}

// Display each number in the loop
void loop() {

    readSerial();   // Read the serial input  and display the number
    countDown();    // Count down from 9 to 0
}