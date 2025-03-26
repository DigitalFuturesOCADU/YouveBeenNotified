## Small update in 1.1.1

This update gives a bit more flexibility for how the base ServoNotifier objects can be created. This does not break any of the previous examples. It simply adds more ways of doing the contstructor. In an earlier version of the code, the notifier was explicity linked to the Servo object, so it required it to be passed into the notifier. That is the method that is used in the examples. However, because the notifer just returns a number, it doesn't actually need to be linked to the Servo object and in the current code the Servo object doesn't actually do anything

### Method shown in examples (Still works)

```cpp
#include <Servo.h>
#include "YouveBeenNotified.h"

// Create servo and notifier objects
Servo myServo;
ServoNotifier notifier(myServo);
```

### Alternative Method (Also works)
```cpp
#include <Servo.h>
#include "YouveBeenNotified.h"

// Create servo and notifier objects
Servo myServo;
ServoNotifier notifier;
```
**Everything else in the code remains the same**