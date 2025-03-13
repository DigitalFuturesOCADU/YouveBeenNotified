# Arduino R4 WiFi Matrix Graphics Library

## Table of Contents
- [Setup](#setup)
- [Methods](#methods)
  - [begin()](#begin)
  - [end()](#end)
  - [width()](#width)
  - [height()](#height)
  - [beginDraw()](#begindraw)
  - [endDraw()](#enddraw)
  - [clear()](#clear)
  - [fill()](#fill)
  - [noFill()](#nofill)
  - [stroke()](#stroke)
  - [noStroke()](#nostroke)
  - [line()](#line)
  - [point()](#point)
  - [rect()](#rect)
  - [circle()](#circle)
  - [ellipse()](#ellipse)
  - [text()](#text)
  - [textFont()](#textfont)
  - [textFontWidth()](#textfontwidth)
  - [textFontHeight()](#textfontheight)
  - [set()](#set)
  - [beginText()](#begintext)
  - [endText()](#endtext)
  - [textScrollSpeed()](#textscrollspeed)
- [Inverted Drawing Methods](#inverted-drawing-methods)
  - [Inverted Display Setup](#inverted-display-setup)
  - [Inverted Drawing](#inverted-drawing)
  - [Inverted Shapes](#inverted-shapes)
  - [Inverted Canvas Clearing](#inverted-canvas-clearing)
  - [Complete Inverted Drawing Example](#complete-inverted-drawing-example)
- [Complete Example](#complete-example)

This documentation covers the Arduino Graphics Library as used with the Arduino R4 WiFi's built-in 12×8 single-color LED matrix.

## Setup

```cpp
#include "Arduino_LED_Matrix.h"
#include "ArduinoGraphics.h"

ArduinoLEDMatrix matrix;
```

## Methods

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

1 for success, 0 on failure.

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
matrix.set(0, 0, 1); // Turn on LED at position 0,0
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
matrix.set(5, 3, 1); // Turn on LED at position 5,3
matrix.endDraw();
```

### `clear()`

#### Description

Clears the LED matrix (turns all LEDs off).

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
matrix.clear(2, 3); // Turn off LED at position 2,3
matrix.endDraw();
```

### `fill()`

#### Description

Sets the fill state for drawing operations (on/off).

#### Syntax

```cpp
matrix.fill(state)
```

#### Parameters

- state: 1 for on, 0 for off

#### Returns

Nothing

#### Example

```cpp
matrix.beginDraw();
matrix.clear();
matrix.fill(1); // Set fill to "on"
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
matrix.clear();
matrix.stroke(1); // Set stroke to "on"
matrix.noFill(); // Disable filling
matrix.rect(2, 2, 8, 4); // Draw rectangle outline only
matrix.endDraw();
```

### `stroke()`

#### Description

Sets the stroke state for drawing operations (on/off).

#### Syntax

```cpp
matrix.stroke(state)
```

#### Parameters

- state: 1 for on, 0 for off

#### Returns

Nothing

#### Example

```cpp
matrix.beginDraw();
matrix.clear();
matrix.stroke(1); // Set stroke to "on"
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
matrix.clear();
matrix.noStroke(); // Disable stroke
matrix.fill(1); // Set fill to "on"
matrix.rect(3, 2, 6, 4); // Draw filled rectangle with no outline
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
matrix.clear();
matrix.stroke(1); // Set stroke to "on"
matrix.line(0, 0, 11, 7); // Draw diagonal line across matrix
matrix.endDraw();
```

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
matrix.clear();
matrix.point(6, 4); // Turn on the LED at position 6,4
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
matrix.clear();
matrix.stroke(1); // Set stroke to "on"
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
matrix.clear();
matrix.stroke(1); // Set stroke to "on"
matrix.fill(1); // Set fill to "on"
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
matrix.clear();
matrix.stroke(1); // Set stroke to "on"
matrix.fill(0); // No fill
matrix.ellipse(5, 3, 8, 4); // Draw an ellipse outline
matrix.endDraw();
```

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
matrix.clear();
matrix.text("Hi", 0, 1); // Display "Hi" starting at position 0,1
matrix.endDraw();
```

### `textFont()`

#### Description

Sets the font used for text. The library includes Font_4x6 and Font_5x7 built in, but Font_4x6 is recommended for the 12x8 matrix.

#### Syntax

```cpp
matrix.textFont(font)
```

#### Parameters

- font: font to set (typically Font_4x6 for the R4 WiFi matrix)

#### Returns

Nothing

#### Example

```cpp
#include "Font_4x6.h"

matrix.beginDraw();
matrix.clear();
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
int h = matrix.textFontHeight(); // Will be 6 for Font_4x6
```

### `set()`

#### Description

Turns a specific LED on or off.

#### Syntax

```cpp
matrix.set(x, y, state)
```

#### Parameters

- x: x position of the pixel (0-11)
- y: y position of the pixel (0-7)
- state: 1 for on, 0 for off

#### Returns

Nothing

#### Example

```cpp
matrix.beginDraw();
matrix.clear();
matrix.set(3, 2, 1); // Turn on LED at position 3,2
matrix.set(4, 2, 1); // Turn on LED at position 4,2
matrix.set(5, 2, 1); // Turn on LED at position 5,2
matrix.endDraw();
```

### `beginText()`

#### Description

Start the process of displaying and optionally scrolling text.

#### Syntax

```cpp
matrix.beginText()
matrix.beginText(x, y)
```

#### Parameters

- x: x position of the text
- y: y position of the text

#### Returns

Nothing

#### Example

```cpp
matrix.beginText(0, 0);
matrix.print("Hi");
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

- scrollDirection: (optional) the direction to scroll, defaults to NO_SCROLL if not provided. Valid options are NO_SCROLL, SCROLL_LEFT, SCROLL_RIGHT, SCROLL_UP, SCROLL_DOWN

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

## Inverted Drawing Methods

The standard drawing approach turns on specific LEDs against a dark background. Inverted drawing does the opposite - all LEDs are turned on by default, and your shapes or text appear by turning specific LEDs off.

### Inverted Display Setup

#### Description

Initializes the matrix with all LEDs turned on (inverted display).

#### Example

```cpp
void setupInvertedDisplay() {
  matrix.beginDraw();
  // Turn on all LEDs
  for (int x = 0; x < matrix.width(); x++) {
    for (int y = 0; y < matrix.height(); y++) {
      matrix.set(x, y, 1);
    }
  }
  matrix.endDraw();
}
```

### Inverted Drawing

#### Description

When drawing in inverted mode, you'll use the standard drawing functions but set the stroke and fill to 0 (off) instead of 1 (on).

#### Example - Inverted Text

```cpp
void drawInvertedText(const char* text, int x, int y) {
  // Save current state
  matrix.beginDraw();
  
  // Set the "off" state for drawing text
  // In inverted mode, we "erase" pixels to show text
  matrix.stroke(0);
  
  // Draw the text - it will appear as "off" pixels
  matrix.text(text, x, y);
  
  matrix.endDraw();
}
```

### Inverted Shapes

#### Description

Draw shapes by turning off LEDs against an illuminated background.

#### Example - Inverted Rectangle

```cpp
void drawInvertedRect(int x, int y, int width, int height) {
  matrix.beginDraw();
  
  // Fill and stroke with "off" pixels
  matrix.stroke(0);
  matrix.fill(0);
  
  // Draw a rectangle - it will appear as "off" pixels
  matrix.rect(x, y, width, height);
  
  matrix.endDraw();
}
```

### Inverted Canvas Clearing

#### Description

Resets the display to the inverted state (all LEDs on).

#### Example

```cpp
void clearInverted() {
  matrix.beginDraw();
  
  // Turn all LEDs on
  for (int x = 0; x < matrix.width(); x++) {
    for (int y = 0; y < matrix.height(); y++) {
      matrix.set(x, y, 1);
    }
  }
  
  matrix.endDraw();
}
```

### Complete Inverted Drawing Example

```cpp
#include "Arduino_LED_Matrix.h"
#include "ArduinoGraphics.h"
#include "Font_4x6.h"

ArduinoLEDMatrix matrix;

void setup() {
  Serial.begin(9600);
  
  // Initialize the matrix
  if (!matrix.begin()) {
    Serial.println("Failed to initialize the matrix!");
    while (1);
  }
  
  // Set up inverted display (all LEDs on)
  invertedSetup();
  
  // Draw inverted smiley face
  drawInvertedSmiley();
  
  delay(2000);
  
  // Reset to inverted state
  invertedSetup();
  
  // Show inverted scrolling text
  matrix.beginText(0, 1);
  matrix.textFont(Font_4x6);
  matrix.stroke(0); // Text appears by turning LEDs off
  matrix.textScrollSpeed(150);
  matrix.print("Inverted!");
  matrix.endText(SCROLL_LEFT);
}

void invertedSetup() {
  matrix.beginDraw();
  // Turn all LEDs on
  for (int x = 0; x < matrix.width(); x++) {
    for (int y = 0; y < matrix.height(); y++) {
      matrix.set(x, y, 1);
    }
  }
  matrix.endDraw();
}

void drawInvertedSmiley() {
  matrix.beginDraw();
  
  // Eyes - turn these LEDs off
  matrix.set(3, 2, 0);
  matrix.set(8, 2, 0);
  
  // Mouth - turn these LEDs off
  matrix.set(2, 5, 0);
  matrix.set(3, 6, 0);
  matrix.set(4, 6, 0);
  matrix.set(5, 6, 0);
  matrix.set(6, 6, 0);
  matrix.set(7, 6, 0);
  matrix.set(8, 6, 0);
  matrix.set(9, 5, 0);
  
  matrix.endDraw();
}

void loop() {
  // Matrix will continue to scroll inverted text
}
```

## Complete Example

```cpp
#include "Arduino_LED_Matrix.h"
#include "ArduinoGraphics.h"
#include "Font_4x6.h"

ArduinoLEDMatrix matrix;

void setup() {
  Serial.begin(9600);
  
  // Initialize the LED Matrix
  if (!matrix.begin()) {
    Serial.println("Failed to initialize the matrix!");
    while (1);
  }
  
  // Draw a smiley face
  matrix.beginDraw();
  matrix.clear();
  
  // Eyes
  matrix.set(3, 2, 1);
  matrix.set(8, 2, 1);
  
  // Mouth
  matrix.set(2, 5, 1);
  matrix.set(3, 6, 1);
  matrix.set(4, 6, 1);
  matrix.set(5, 6, 1);
  matrix.set(6, 6, 1);
  matrix.set(7, 6, 1);
  matrix.set(8, 6, 1);
  matrix.set(9, 5, 1);
  
  matrix.endDraw();
  
  delay(2000);
  
  // Scrolling text
  matrix.beginText(0, 1);
  matrix.textFont(Font_4x6);
  matrix.textScrollSpeed(150);
  matrix.print("Arduino R4 WiFi!");
  matrix.endText(SCROLL_LEFT);
}

void loop() {
  // Matrix will continue to scroll text
}
```
