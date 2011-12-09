//======================
// The main effect file
//======================


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Configuration
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

// Chromatic adaptation: (makes all tones more neutral)
#define EADAPTATION

// The target white point (0-10, higher = more blue)
// If default is too blue, try 4, if it's too red, try 6
#define WHITEPOINT 5

// Saturation controls: (Uncomment the #define to enable them)
// 0 is grayscale, 1 is normal, higher values = more saturated
//#define SATURATION 1.0

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// End of configuration
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


// Include all used shaders
#include "shaders/naturalcolors.h"

// Set up the wanted color functions
#define Pass1 NaturalColors

// Include the color pipeline, always needed
#include "include/pipeline.h"
