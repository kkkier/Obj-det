
#include <stdlib.h>
#include <qstring.h>


// printf() in color
// 
// This document describes how to produce output from printf in color. There only three steps to using printf on color.
// 
//    1. Tell the terminal that it should print in color
//    2. Write text of desired length
//    3. Reset the console to the normal color.
// 
// Using printf in color is actually way easier that one would think. The first part is to tell the terminal that it should print in color; tell it what attributes, foreground and background colors to use. This is usually done with escape sequences similar to the following: ^[[1;33;40m. This escape sequence would set the foreground to bright red and the background to black. It should also be mentioned that the first two characters are actually one. '^[' is created by CTRL+V, ESC. How do you tell the console this information? With printf. More on the colours and their number ordering later.
// 
// Using printf to make this colors is really easy, check this out:
// 
// 	printf("\033[1;36m");     // set colours 
// 	printf("Hello World");    // print 
// 	printf("\033[0m");        // reset colours
// 
// The above code would output the phrase "Hello World" in bright cyan. Notice the \033, this is the octal code for ^[ the start of our escape sequence, \x1b could also be used if one chooses to use hexidecimal over octal. After writing our phrase the colours are reset using \033[0m.
// 
// Truly that is all there is it to it. The only thing now is to learn the color code escape sequences. Some examples follow and a key is provided below that.
// 
//   printf("\x1b[1;31;40m");	// Bright red on black
//   printf("\x1b[3;33;45m");	// Blinking yellow on magenta
//   printf("\x1b[1;30;47m");	// Bright black (grey) on dim white
// 
//   Style           Foreground      Background
//   1st Digit       2nd Digit       3rd Digit
//   0 - Reset       30 - Black      40 - Black
//   1 - FG Bright   31 - Red        41 - Red
//   2 - Unknown     32 - Green      42 - Green
//   3 - Unknown     33 - Yellow     43 - Yellow
//   4 - Underline   34 - Blue       44 - Blue
//   5 - BG Bright   35 - Magenta    45 - Magenta
//   6 - Unknown     36 - Cyan       46 - Cyan
//   7 - Reverse     37 - White      47 - White


int __level = 5;

static bool checkLevel(int level)
{
	if (level<__level) return true; else return false; 
}

void log(const char* text, int level=5)
{
	if (checkLevel(level)) printf("%s", text);
}

void log(const QString& text, int level=5)
{
	log(text.latin1(),level);
}

void logLine(const QString& text, int level=5)
{
	log(text, level);
	printf("\n");
}


