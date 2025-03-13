# Arduino R4 WiFi Matrix Graphics Library

## Table of Contents
- [Setup](#setup)
- [Matrix Methods](#matrix-methods)
  - [begin()](#begin)
  - [end()](#end)
  - [width()](#width)
  - [height()](#height)
- [Drawing Control Methods](#drawing-control-methods)
  - [beginDraw()](#begindraw)
  - [endDraw()](#enddraw)
  - [background()](#background)
  - [clear()](#clear)
  - [set()](#set)
- [Style Control Methods](#style-control-methods)
  - [fill()](#fill)
  - [noFill()](#nofill)
  - [stroke()](#stroke)
  - [noStroke()](#nostroke)
- [Shape Drawing Methods](#shape-drawing-methods)
  - [point()](#point)
  - [line()](#line)
  - [rect()](#rect)
  - [circle()](#circle)
  - [ellipse()](#ellipse)
- [Text Methods](#text-methods)
  - [text()](#text)
  - [textFont()](#textfont)
  - [textFontWidth()](#textfontwidth)
  - [textFontHeight()](#textfontheight)
  - [beginText()](#begintext)
  - [print(), println()](#print-println)
  - [endText()](#endtext)
  - [textScrollSpeed()](#textscrollspeed)
- [Example Sketches](#example-sketches)
  - [Scrolling Text](#scrolling-text)
  - [Drawing Shapes](#drawing-shapes)
  - [Sine Wave Animation](#sine-wave-animation)

This documentation covers the Arduino Graphics Library as used with the Arduino R4 WiFi's built-in 12×8 single-color LED matrix.

## Setup

```cpp
// Include ArduinoGraphics BEFORE Arduino_LED_Matrix
#include "ArduinoGraphics.h"
#include "Arduino_LED_Matrix.h"

// Create the matrix instance
ArduinoLEDMatrix matrix;
```

## Matrix Methods

### `begin()`

#### Description

Initializes the R4 WiFi's LED matrix.

#### Syntax

```cpp
matrix.begin()
```

#### Parameters

None

#### Returns

Boolean: true for success, false on failure.

#### Example

```cpp
if (!matrix.begin()) {
  Serial.println("Failed to initialize the LED matrix!");
  while (1);
}
```

### `end()`

#### Description

Stops the LED matrix.

#### Syntax

```cpp
matrix.end()
```

#### Parameters

None

#### Returns

Nothing

#### Example

```cpp
matrix.end();
```

### `width()`

#### Description

Returns the pixel width of the LED matrix (12 pixels).

#### Syntax

```cpp
matrix.width()
```

#### Parameters

None

#### Returns

Returns 12 (the width of the R4 WiFi LED matrix).

#### Example

```cpp
int w = matrix.width(); // w will be 12
```

### `height()`

#### Description

Returns the pixel height of the LED matrix (8 pixels).

#### Syntax

```cpp
matrix.height()
```

#### Parameters

None

#### Returns

Returns 8 (the height of the R4 WiFi LED matrix).

#### Example

```cpp
int h = matrix.height(); // h will be 8
```

## Drawing Control Methods

### `beginDraw()`

#### Description

Begins a drawing operation on the LED matrix.

#### Syntax

```cpp
matrix.beginDraw()
```

#### Parameters

None

#### Returns

Nothing

#### Example

```cpp
matrix.beginDraw();
matrix.point(0, 0); // Draw a point at position 0,0
matrix.endDraw();
```

### `endDraw()`

#### Description

Ends a drawing operation and displays the changes to the LED matrix.

#### Syntax

```cpp
matrix.endDraw()
```

#### Parameters

None

#### Returns

Nothing

#### Example

```cpp
matrix.beginDraw();
matrix.point(5, 3); // Draw a point at position 5,3
matrix.endDraw();
```

### `background()`

#### Description

Set the background color. For the single color matrix, this determines if background pixels are on or off.

#### Syntax

```cpp
matrix.background(r, g, b)
matrix.background(hexColor)
```

#### Parameters

- r, g, b: Color values (0-255). For the single-color matrix, use 0 for off, 255 for on.
- hexColor: 24-bit RGB color in hexadecimal format (0xRRGGBB or 0xFFFFFF for white)

#### Returns

Nothing

#### Example

```cpp
// Set the background to "off"
matrix.background(0, 0, 0);

// Set the background to "on" (using hex color)
matrix.background(0xFFFFFF);
```

### `clear()`

#### Description

Clears the LED matrix (sets all LEDs to the background state).

#### Syntax

```cpp
matrix.clear()
matrix.clear(x, y)
```

#### Parameters

- x: x position of the pixel to clear
- y: y position of the pixel to clear

#### Returns

Nothing

#### Example

```cpp
// Clear the entire matrix
matrix.beginDraw();
matrix.clear();
matrix.endDraw();

// Clear just one pixel
matrix.beginDraw();
matrix.clear(2, 3); // Reset LED at position 2,3 to background state
matrix.endDraw();
```

### `set()`

#### Description

Sets the state of a specific LED directly.

#### Syntax

```cpp
matrix.set(x, y, r, g, b)
matrix.set(x, y, hexColor)
```

#### Parameters

- x: x position of the pixel (0-11)
- y: y position of the pixel (0-7)
- r, g, b: Color values (0-255). For the single-color matrix, use 0 for off, 255 for on.
- hexColor: 24-bit RGB color in hexadecimal format (0xRRGGBB or 0xFFFFFF for white)

#### Returns

Nothing

#### Example

```cpp
matrix.beginDraw();
// Turn on LED at position 3,2 (using RGB)
matrix.set(3, 2, 255, 255, 255);
// Turn on LED at position 4,2 (using hex)
matrix.set(4, 2, 0xFFFFFF);
matrix.endDraw();
```

## Style Control Methods

### `fill()`

#### Description

Sets the fill state for drawing operations (on/off).

#### Syntax

```cpp
matrix.fill(r, g, b)
matrix.fill(hexColor)
```

#### Parameters

- r, g, b: Color values (0-255). For the single-color matrix, use 0 for off, 255 for on.
- hexColor: 24-bit RGB color in hexadecimal format (0xRRGGBB or 0xFFFFFF for white)

#### Returns

Nothing

#### Example

```cpp
matrix.beginDraw();
// Set fill to "on" using hex color
matrix.fill(0xFFFFFF);
matrix.rect(2, 2, 8, 4); // Draw a filled rectangle
matrix.endDraw();
```

### `noFill()`

#### Description

Disables filling for drawing operations.

#### Syntax

```cpp
matrix.noFill()
```

#### Parameters

None

#### Returns

Nothing

#### Example

```cpp
matrix.beginDraw();
matrix.stroke(0xFFFFFF); // Set stroke to "on"
matrix.noFill(); // Disable filling
matrix.rect(2, 2, 8, 4); // Draw rectangle outline only
matrix.endDraw();
```

### `stroke()`

#### Description

Sets the stroke state for drawing operations (on/off).

#### Syntax

```cpp
matrix.stroke(r, g, b)
matrix.stroke(hexColor)
```

#### Parameters

- r, g, b: Color values (0-255). For the single-color matrix, use 0 for off, 255 for on.
- hexColor: 24-bit RGB color in hexadecimal format (0xRRGGBB or 0xFFFFFFFF for white)

#### Returns

Nothing

#### Example

```cpp
matrix.beginDraw();
// Set stroke to "on" using hex color
matrix.stroke(0xFFFFFFFF);
matrix.noFill();
matrix.rect(1, 1, 10, 6); // Draw rectangle outline
matrix.endDraw();
```

### `noStroke()`

#### Description

Disables stroke for drawing operations.

#### Syntax

```cpp
matrix.noStroke()
```

#### Parameters

None

#### Returns

Nothing

#### Example

```cpp
matrix.beginDraw();
matrix.noStroke(); // Disable stroke
matrix.fill(0xFFFFFF); // Set fill to "on"
matrix.rect(3, 2, 6, 4); // Draw filled rectangle with no outline
matrix.endDraw();
```

## Shape Drawing Methods

### `point()`

#### Description

Draws a single point (turns on a single LED).

#### Syntax

```cpp
matrix.point(x, y)
```

#### Parameters

- x: x position of the point (0-11)
- y: y position of the point (0-7)

#### Returns

Nothing

#### Example

```cpp
matrix.beginDraw();
matrix.stroke(0xFFFFFF); // Set the point color to "on"
matrix.point(6, 4); // Turn on the LED at position 6,4
matrix.endDraw();
```

### `line()`

#### Description

Draws a line between two points.

#### Syntax

```cpp
matrix.line(x1, y1, x2, y2)
```

#### Parameters

- x1: x position of the starting point of the line
- y1: y position of the starting point of the line
- x2: x position of the end point of the line
- y2: y position of the end point of the line

#### Returns

Nothing

#### Example

```cpp
matrix.beginDraw();
matrix.stroke(0xFFFFFFFF); // Set stroke to "on"
matrix.line(0, 0, 11, 7); // Draw diagonal line across matrix
matrix.endDraw();
```

### `rect()`

#### Description

Draws a rectangle with optional fill.

#### Syntax

```cpp
matrix.rect(x, y, width, height)
```

#### Parameters

- x: x position of the rectangle
- y: y position of the rectangle
- width: width of the rectangle
- height: height of the rectangle

#### Returns

Nothing

#### Example

```cpp
matrix.beginDraw();
matrix.stroke(0xFFFFFFFF); // Set stroke to "on"
matrix.noFill();
matrix.rect(2, 1, 8, 6); // Draw rectangle outline
matrix.endDraw();
```

### `circle()`

#### Description

Draws a circle with optional fill.

#### Syntax

```cpp
matrix.circle(x, y, diameter)
```

#### Parameters

- x: x center position of the circle
- y: y center position of the circle
- diameter: diameter of the circle

#### Returns

Nothing

#### Example

```cpp
matrix.beginDraw();
matrix.stroke(0xFFFFFFFF); // Set stroke to "on"
matrix.fill(0xFFFFFFFF); // Set fill to "on"
matrix.circle(5, 3, 5); // Draw a circle in the center of matrix
matrix.endDraw();
```

### `ellipse()`

#### Description

Draws an ellipse with optional fill.

#### Syntax

```cpp
matrix.ellipse(x, y, width, height)
```

#### Parameters

- x: x center position of the ellipse
- y: y center position of the ellipse
- width: width of the ellipse
- height: height of the ellipse

#### Returns

Nothing

#### Example

```cpp
matrix.beginDraw();
matrix.stroke(0xFFFFFFFF); // Set stroke to "on"
matrix.noFill();
matrix.ellipse(5, 3, 8, 4); // Draw an ellipse outline
matrix.endDraw();
```

## Text Methods

### `text()`

#### Description

Draws text on the LED matrix.

#### Syntax

```cpp
matrix.text(string)
matrix.text(string, x, y)
```

#### Parameters

- string: string to draw
- x: x position for the start of the text
- y: y position for the start of the text

#### Returns

Nothing

#### Example

```cpp
matrix.beginDraw();
matrix.stroke(0xFFFFFFFF); // Set text color to "on"
matrix.text("Hi", 0, 1); // Display "Hi" starting at position 0,1
matrix.endDraw();
```

### `textFont()`

#### Description

Sets the font used for text. The library includes Font_4x6 and Font_5x7 built in.

#### Syntax

```cpp
matrix.textFont(font)
```

#### Parameters

- font: font to set (Font_4x6 or Font_5x7)

#### Returns

Nothing

#### Example

```cpp
// ArduinoGraphics.h already includes Font_4x6 and Font_5x7
matrix.beginDraw();
matrix.stroke(0xFFFFFFFF); // Set text color to "on"
matrix.textFont(Font_4x6);
matrix.text("Hi", 0, 1);
matrix.endDraw();
```

### `textFontWidth()`

#### Description

Returns the width, in pixels, of the current font.

#### Syntax

```cpp
matrix.textFontWidth()
```

#### Parameters

None

#### Returns

Width of the font in pixels

#### Example

```cpp
matrix.textFont(Font_4x6);
int w = matrix.textFontWidth(); // Will be 4 for Font_4x6
```

### `textFontHeight()`

#### Description

Returns the height, in pixels, of the current font.

#### Syntax

```cpp
matrix.textFontHeight()
```

#### Parameters

None

#### Returns

Height of the font in pixels

#### Example

```cpp
matrix.textFont(Font_4x6);
int h = matrix.textFontHeight(); // Will be 6 for Font_4x6
```

### `beginText()`

#### Description

Start the process of displaying and optionally scrolling text.

#### Syntax

```cpp
matrix.beginText()
matrix.beginText(x, y)
matrix.beginText(x, y, hexColor)
```

#### Parameters

- x: x position of the text
- y: y position of the text
- hexColor: 24-bit RGB color in hexadecimal format (0xRRGGBB or 0xFFFFFF for white)

#### Returns

Nothing

#### Example

```cpp
matrix.beginText(0, 0);
matrix.print("Hi");
matrix.endText();
```

### `print(), println()`

#### Description

Adds text to be displayed after beginText() is called. Similar to Serial.print() and Serial.println().

#### Syntax

```cpp
matrix.print(text)
matrix.println(text)
```

#### Parameters

- text: Text to display or scroll

#### Returns

Nothing

#### Example

```cpp
matrix.beginText(0, 1);
matrix.print("Hello");
matrix.endText();
```

### `endText()`

#### Description

End the process of displaying and optionally scrolling text.

#### Syntax

```cpp
matrix.endText()
matrix.endText(scrollDirection)
```

#### Parameters

- scrollDirection: (optional) the direction to scroll. Valid options are NO_SCROLL, SCROLL_LEFT, SCROLL_RIGHT, SCROLL_UP, SCROLL_DOWN

#### Returns

Nothing

#### Example

```cpp
// Scrolling text example
matrix.beginText(0, 0);
matrix.print("Hello Arduino!");
matrix.endText(SCROLL_LEFT);
```

### `textScrollSpeed()`

#### Description

Sets the text scrolling speed, the speed controls the delay in milliseconds between scrolling each pixel.

#### Syntax

```cpp
matrix.textScrollSpeed(speed)
```

#### Parameters

- speed: scroll speed in milliseconds

#### Returns

Nothing

#### Example

```cpp
matrix.beginText(0, 0);
matrix.textScrollSpeed(100); // Scroll speed of 100ms
matrix.print("Arduino R4!");
matrix.endText(SCROLL_LEFT);
```

## Example Sketches

### Scrolling Text

```cpp
// Text Animation Example
// Include ArduinoGraphics BEFORE Arduino_LED_Matrix
#include "ArduinoGraphics.h"
#include "Arduino_LED_Matrix.h"

// Create the matrix instance
ArduinoLEDMatrix matrix;

void setup() {
  // Initialize the LED matrix
  matrix.begin();

  // Display static text first
  matrix.beginDraw();
  matrix.stroke(0xFFFFFFFF);
  matrix.textFont(Font_4x6);
  matrix.text("Hi!", 1, 1);
  matrix.endDraw();
  
  delay(2000); // Display static text for 2 seconds
}

void loop() {
  // Make scrolling text
  matrix.beginDraw();
  
  // Set text color (using hex code)
  matrix.stroke(0xFFFFFFFF);
  
  // Set scrolling speed (milliseconds between updates)
  matrix.textScrollSpeed(100);
  
  // Add the scrolling text
  matrix.textFont(Font_4x6); // Smaller font fits better
  matrix.beginText(0, 1, 0xFFFFFF);
  matrix.print("  Hello Arduino R4!  ");
  matrix.endText(SCROLL_LEFT);
  
  matrix.endDraw();
}
```

### Drawing Shapes

```cpp
// Drawing Shapes Example
// Include ArduinoGraphics BEFORE Arduino_LED_Matrix
#include "ArduinoGraphics.h"
#include "Arduino_LED_Matrix.h"

// Create the matrix instance
ArduinoLEDMatrix matrix;

void setup() {
  // Initialize the LED matrix
  matrix.begin();
}

void loop() {
  // Draw a rectangle
  matrix.beginDraw();
  matrix.clear();
  matrix.stroke(0xFFFFFFFF); // White outline
  matrix.noFill();
  matrix.rect(1, 1, 10, 6);
  matrix.endDraw();
  delay(1000);
  
  // Draw a filled rectangle
  matrix.beginDraw();
  matrix.clear();
  matrix.stroke(0xFFFFFFFF);
  matrix.fill(0xFFFFFFFF);
  matrix.rect(2, 2, 8, 4);
  matrix.endDraw();
  delay(1000);
  
  // Draw a circle
  matrix.beginDraw();
  matrix.clear();
  matrix.stroke(0xFFFFFFFF);
  matrix.noFill();
  matrix.circle(6, 4, 6);
  matrix.endDraw();
  delay(1000);
  
  // Draw a filled circle
  matrix.beginDraw();
  matrix.clear();
  matrix.stroke(0xFFFFFFFF);
  matrix.fill(0xFFFFFFFF);
  matrix.circle(6, 4, 4);
  matrix.endDraw();
  delay(1000);
  
  // Draw lines
  matrix.beginDraw();
  matrix.clear();
  matrix.stroke(0xFFFFFFFF);
  matrix.line(0, 0, 11, 7); // Diagonal line
  matrix.line(0, 7, 11, 0); // Crossed diagonal
  matrix.endDraw();
  delay(1000);
  
  // Draw points pattern
  matrix.beginDraw();
  matrix.clear();
  matrix.stroke(0xFFFFFFFF);
  for (int x = 1; x < 11; x += 2) {
    for (int y = 1; y < 7; y += 2) {
      matrix.point(x, y);
    }
  }
  matrix.endDraw();
  delay(1000);
}
```

### Sine Wave Animation

```cpp
// Sine Wave Animation Example
// Include ArduinoGraphics BEFORE Arduino_LED_Matrix
#include "ArduinoGraphics.h"
#include "Arduino_LED_Matrix.h"

// Create the matrix instance
ArduinoLEDMatrix matrix;

// Variables for sine wave animation
float phase = 0.0;
const float phaseIncrement = 0.2; // How fast the wave moves

void setup() {
  // Initialize the LED matrix
  matrix.begin();
}

void loop() {
  // Clear the display
  matrix.beginDraw();
  matrix.clear();
  
  // Set the drawing color
  matrix.stroke(0xFFFFFFFF);
  
  // Draw the sine wave
  for (int x = 0; x < 12; x++) {
    // Calculate y position using sine function
    // Map sine output (-1 to 1) to LED matrix coordinates
    float sinValue = sin(x * 0.5 + phase);
    int y = (int)(3.5 + 3.0 * sinValue); // Center at y=3.5, amplitude = 3
    
    // Make sure y stays within bounds of display (0-7)
    y = constrain(y, 0, 7);
    
    // Draw the point
    matrix.point(x, y);
  }
  
  // Update the phase for animation
  phase += phaseIncrement;
  if (phase > 2 * PI) {
    phase -= 2 * PI; // Keep phase within reasonable range
  }
  
  // Show the frame
  matrix.endDraw();
  
  // Control animation speed
  delay(100);
}
```