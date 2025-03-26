// YouveBeenNotified.h
// A library for keyframe-based servo and LED animations
// For Arduino

#ifndef YOUVEBEENNOTIFIED_H
#define YOUVEBEENNOTIFIED_H

#include <Arduino.h>
#include <vector>
#include <Servo.h>

// Playback modes
enum PlayMode {
    PLAY_ONCE,
    PLAY_LOOP,
    PLAY_BOOMERANG
};

// Shorthand constants
static const PlayMode ONCE = PLAY_ONCE;
static const PlayMode LOOP = PLAY_LOOP;
static const PlayMode BOOMERANG = PLAY_BOOMERANG;

// Animation state
enum AnimationState {
    IDLE,
    PLAYING,
    PAUSED,
    COMPLETED
};

// LED operation mode
enum LEDMode {
    ANALOG,   // PWM output (0-255)
    DIGITAL   // On/Off (0=OFF, 1=ON)
};

// ----------------------------------------------------------------
// KeyframeAnimation Class
// Stores a sequence of value/time keyframes
// ----------------------------------------------------------------
class KeyframeAnimation {
private:
    String name;
    struct Keyframe {
        float value;
        unsigned long time;
    };
    std::vector<Keyframe> keyframes;

public:
    // Constructor with optional name
    KeyframeAnimation(const String& name = "");
    
    // Add a keyframe with value and time to reach it
    void addKeyFrame(float value, unsigned long time);
    
    // Dynamically modify keyframe values
    bool setKeyFrameValue(int index, float newValue);
    bool setKeyFrameTime(int index, unsigned long newTime);
    
    // Utility methods
    int getKeyframeCount() const;
    const String& getName() const;
    float getKeyFrameValue(int index) const;
    unsigned long getKeyFrameTime(int index) const;
};

// ----------------------------------------------------------------
// ServoNotifier Class
// Controls servo movements using animations
// ----------------------------------------------------------------
class ServoNotifier {
private:
    Servo* servo;
    int minAngle;
    int maxAngle;
    
    std::vector<KeyframeAnimation> animations;
    KeyframeAnimation* currentAnimation;
    KeyframeAnimation* targetAnimation;    // For blending
    PlayMode currentMode;
    AnimationState currentState;
    float globalSpeed;
    
    // Timing and state tracking
    unsigned long startTime;
    unsigned long pauseTime;
    unsigned long totalPausedTime;
    int currentKeyframeIndex;
    int nextKeyframeIndex;
    float currentValue;
    bool isReversing;
    bool isPlayingBackward;
    
    // Value adjustment properties
    float valueScale;     // Multiplier for final value
    float valueOffset;    // Added to final value
    float minValue;       // Output clamping minimum
    float maxValue;       // Output clamping maximum
    
    // Blend control
    bool isBlending;
    unsigned long blendStartTime;
    unsigned long blendDuration;
    float startValue;     // Value at blend start
    
    // Internal methods
    float interpolateValue(float startVal, float endVal, float t);
    float calculateCurrentValue();

public:
    // Constructor
    

    ServoNotifier(int minAngle = 0, int maxAngle = 180);
    
    // The existing constructor remains unchanged
    ServoNotifier(Servo& servo, int minAngle = 0, int maxAngle = 180);
    
    // Animation management
    void addAnimation(const KeyframeAnimation& animation);
    
    // Play animation by reference
    void playAnimation(const KeyframeAnimation& animation, PlayMode mode = PLAY_ONCE);
    
    // Play animation by name
    bool playAnimation(const String& name, PlayMode mode = PLAY_ONCE);
    
    // Enhanced transition methods
    void crossfadeTo(const KeyframeAnimation& animation, unsigned long blendTime, PlayMode mode = PLAY_ONCE);
    bool crossfadeTo(const String& name, unsigned long blendTime, PlayMode mode = PLAY_ONCE);
    
    // Value adjustment methods
    void setValueScale(float scale);
    void setValueOffset(float offset);
    void setValueRange(float min, float max);
    
    // Playback control
    void pause();
    void resume();
    void stop();
    
    // Update animation state and calculate new value
    void update();
    
    // Get the current interpolated and adjusted value as an integer
    int getValue() const;
    
    // Check if the value has changed since last getValue() call
    bool hasChanged();
    
    // Speed control
    void setGlobalSpeed(float speed);
    float getGlobalSpeed() const;
    
    // Status methods
    String getCurrentAnimationName() const;
    bool isPlaying() const;
    bool isPaused() const;
    bool isCompleted() const;
    bool completed() const;  // Alias for isCompleted
    bool isBlendingAnimations() const;
    AnimationState getState() const;
    
    // Timing methods
    unsigned long timeToNextKey() const;
    unsigned long timeRemaining() const;
};

// ----------------------------------------------------------------
// LEDNotifier Class
// Controls LED animations
// ----------------------------------------------------------------
class LEDNotifier {
private:
    int pin;
    LEDMode mode;
    float threshold;  // Threshold for digital mode (0.0-1.0)
    
    std::vector<KeyframeAnimation> animations;
    KeyframeAnimation* currentAnimation;
    KeyframeAnimation* targetAnimation;    // For blending
    PlayMode currentMode;
    AnimationState currentState;
    float globalSpeed;
    
    // Timing and state tracking
    unsigned long startTime;
    unsigned long pauseTime;
    unsigned long totalPausedTime;
    int currentKeyframeIndex;
    int nextKeyframeIndex;
    float currentValue;
    bool isReversing;
    bool isPlayingBackward;
    
    // Value adjustment properties
    float valueScale;     // Multiplier for final value
    float valueOffset;    // Added to final value
    float minValue;       // Output clamping minimum
    float maxValue;       // Output clamping maximum
    
    // Blend control
    bool isBlending;
    unsigned long blendStartTime;
    unsigned long blendDuration;
    float startValue;     // Value at blend start
    
    // Internal methods
    float interpolateValue(float startVal, float endVal, float t);
    float calculateCurrentValue();
    
public:
    LEDNotifier(int pin, LEDMode mode = ANALOG);
    void begin();
    
    // Animation management
    void addAnimation(const KeyframeAnimation& animation);
    
    // Play animation by reference
    void playAnimation(const KeyframeAnimation& animation, PlayMode mode = PLAY_ONCE);
    
    // Play animation by name
    bool playAnimation(const String& name, PlayMode mode = PLAY_ONCE);
    
    // Enhanced transition methods
    void crossfadeTo(const KeyframeAnimation& animation, unsigned long blendTime, PlayMode mode = PLAY_ONCE);
    bool crossfadeTo(const String& name, unsigned long blendTime, PlayMode mode = PLAY_ONCE);
    
    // Set mode after construction
    void setMode(LEDMode newMode);
    
    // Set threshold for digital mode
    void setThreshold(float newThreshold);
    
    // Value adjustment methods
    void setValueScale(float scale);
    void setValueOffset(float offset);
    void setValueRange(float min, float max);
    
    // Playback control
    void pause();
    void resume();
    void stop();
    
    // Update and apply value to LED directly
    void update();
    
    // Get the current interpolated and adjusted value as an integer
    int getValue() const;
    
    // Check if the value has changed since last getValue() call
    bool hasChanged();
    
    // Speed control
    void setGlobalSpeed(float speed);
    float getGlobalSpeed() const;
    
    // Status methods
    String getCurrentAnimationName() const;
    bool isPlaying() const;
    bool isPaused() const;
    bool isCompleted() const;
    bool completed() const;  // Alias for isCompleted
    bool isBlendingAnimations() const;
    AnimationState getState() const;
    
    // Timing methods
    unsigned long timeToNextKey() const;
    unsigned long timeRemaining() const;
};

// ----------------------------------------------------------------
// RGBKeyframeAnimation Class
// Stores a sequence of RGB color keyframes
// ----------------------------------------------------------------
class RGBKeyframeAnimation {
private:
    String name;
    struct RGBKeyframe {
        byte red;
        byte green;
        byte blue;
        unsigned long time;
    };
    std::vector<RGBKeyframe> keyframes;

public:
    RGBKeyframeAnimation(const String& name = "");
    
    // Add a color keyframe
    void addKeyFrame(byte r, byte g, byte b, unsigned long time);
    
    // Modify keyframes
    bool setKeyFrameColor(int index, byte r, byte g, byte b);
    bool setKeyFrameTime(int index, unsigned long newTime);
    
    // Utility methods
    int getKeyframeCount() const;
    const String& getName() const;
    void getKeyFrameColor(int index, byte& r, byte& g, byte& b) const;
    unsigned long getKeyFrameTime(int index) const;
};

#endif // YOUVEBEENNOTIFIED_H