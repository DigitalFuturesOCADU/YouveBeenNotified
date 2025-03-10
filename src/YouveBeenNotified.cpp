// YouveBeenNotified.cpp
// Implementation for the base animation classes

#include "YouveBeenNotified.h"

//======================================================================
// KeyframeAnimation Implementation
//======================================================================

KeyframeAnimation::KeyframeAnimation(const String& name) : name(name) {
    // Initialize with empty keyframe list
}

void KeyframeAnimation::addKeyFrame(float value, unsigned long time) {
    Keyframe newFrame = {value, time};
    keyframes.push_back(newFrame);
}

bool KeyframeAnimation::setKeyFrameValue(int index, float newValue) {
    if (index < 0 || index >= keyframes.size()) {
        return false;
    }
    keyframes[index].value = newValue;
    return true;
}

bool KeyframeAnimation::setKeyFrameTime(int index, unsigned long newTime) {
    if (index < 0 || index >= keyframes.size()) {
        return false;
    }
    keyframes[index].time = newTime;
    return true;
}

int KeyframeAnimation::getKeyframeCount() const {
    return keyframes.size();
}

const String& KeyframeAnimation::getName() const {
    return name;
}

float KeyframeAnimation::getKeyFrameValue(int index) const {
    if (index < 0 || index >= keyframes.size()) {
        return 0.0;
    }
    return keyframes[index].value;
}

unsigned long KeyframeAnimation::getKeyFrameTime(int index) const {
    if (index < 0 || index >= keyframes.size()) {
        return 0;
    }
    return keyframes[index].time;
}

//======================================================================
// BaseNotifier Implementation
//======================================================================

BaseNotifier::BaseNotifier() :
    currentAnimation(nullptr),
    targetAnimation(nullptr),
    currentMode(PLAY_ONCE),
    currentState(IDLE),
    globalSpeed(1.0),
    startTime(0),
    pauseTime(0),
    totalPausedTime(0),
    currentKeyframeIndex(0),
    nextKeyframeIndex(0),
    currentValue(0.0),
    isReversing(false),
    isPlayingBackward(false),
    valueScale(1.0),
    valueOffset(0.0),
    minValue(-INFINITY),
    maxValue(INFINITY),
    isBlending(false),
    blendStartTime(0),
    blendDuration(0),
    startValue(0.0)
{
}

void BaseNotifier::addAnimation(const KeyframeAnimation& animation) {
    // Only add if it has keyframes and isn't already in our list
    if (animation.getKeyframeCount() == 0) {
        return;
    }
    
    // Check for duplicate name
    for (const auto& anim : animations) {
        if (anim.getName() == animation.getName()) {
            return; // Skip duplicates
        }
    }
    
    animations.push_back(animation);
}

void BaseNotifier::playAnimation(const KeyframeAnimation& animation, PlayMode mode) {
    if (animation.getKeyframeCount() < 1) {
        return;
    }
    
    // Stop any current animation
    stop();
    
    // Set up new animation
    for (const auto& anim : animations) {
        if (anim.getName() == animation.getName()) {
            currentAnimation = const_cast<KeyframeAnimation*>(&anim);
            break;
        }
    }
    
    // If not in our collection, use the provided one directly
    if (currentAnimation == nullptr) {
        addAnimation(animation);
        for (auto& anim : animations) {
            if (anim.getName() == animation.getName()) {
                currentAnimation = &anim;
                break;
            }
        }
    }
    
    // Initialize playback state
    currentMode = mode;
    isReversing = false;
    isPlayingBackward = false;
    currentKeyframeIndex = 0;
    nextKeyframeIndex = 1;
    
    if (currentAnimation->getKeyframeCount() > 1) {
        nextKeyframeIndex = 1;
    } else {
        nextKeyframeIndex = 0;
    }
    
    // Start timing
    startTime = millis();
    totalPausedTime = 0;
    
    // Initial value
    currentValue = currentAnimation->getKeyFrameValue(0);
    
    // Set state
    currentState = PLAYING;
}

bool BaseNotifier::playAnimation(const String& name, PlayMode mode) {
    // Find animation by name
    for (auto& anim : animations) {
        if (anim.getName() == name) {
            playAnimation(anim, mode);
            return true;
        }
    }
    return false;
}

void BaseNotifier::crossfadeTo(const KeyframeAnimation& animation, unsigned long blendTime, PlayMode mode) {
    if (animation.getKeyframeCount() < 1 || currentState == IDLE) {
        // If no animation is playing, just start the new one
        playAnimation(animation, mode);
        return;
    }
    
    // Save current value for blending
    startValue = currentValue;
    
    // Find the target animation in our collection
    targetAnimation = nullptr;
    for (auto& anim : animations) {
        if (anim.getName() == animation.getName()) {
            targetAnimation = &anim;
            break;
        }
    }
    
    // If not in our collection, add it
    if (targetAnimation == nullptr) {
        addAnimation(animation);
        for (auto& anim : animations) {
            if (anim.getName() == animation.getName()) {
                targetAnimation = &anim;
                break;
            }
        }
    }
    
    // Set up blending
    isBlending = true;
    blendStartTime = millis();
    blendDuration = blendTime;
}

bool BaseNotifier::crossfadeTo(const String& name, unsigned long blendTime, PlayMode mode) {
    // Find animation by name
    for (auto& anim : animations) {
        if (anim.getName() == name) {
            crossfadeTo(anim, blendTime, mode);
            return true;
        }
    }
    return false;
}

void BaseNotifier::update() {
    if (currentState == IDLE || currentState == COMPLETED) {
        return;
    }
    
    if (currentState == PAUSED) {
        // When paused, just track time
        totalPausedTime = millis() - pauseTime;
        return;
    }
    
    if (isBlending && targetAnimation != nullptr) {
        // Handle blending between animations
        unsigned long currentTime = millis();
        unsigned long elapsedTime = currentTime - blendStartTime;
        
        if (elapsedTime >= blendDuration) {
            // Blend complete, switch to target animation
            isBlending = false;
            playAnimation(*targetAnimation, currentMode);
            targetAnimation = nullptr;
        } else {
            // Calculate blend factor (0.0 to 1.0)
            float t = static_cast<float>(elapsedTime) / blendDuration;
            
            // Calculate blended value
            currentValue = interpolateValue(
                startValue, 
                targetAnimation->getKeyFrameValue(0), 
                t
            );
        }
        return;
    }
    
    // Regular animation update
    calculateCurrentValue();
}

float BaseNotifier::interpolateValue(float startVal, float endVal, float t) {
    // Linear interpolation
    return startVal + (endVal - startVal) * t;
}

float BaseNotifier::calculateCurrentValue() {
    if (currentAnimation == nullptr || currentAnimation->getKeyframeCount() < 1) {
        return currentValue;
    }
    
    // Get elapsed time, accounting for pauses
    unsigned long currentTime = millis();
    unsigned long effectiveTime = currentTime - startTime - totalPausedTime;
    
    // Scale by global speed
    if (globalSpeed != 0) {
        effectiveTime = static_cast<unsigned long>(effectiveTime / globalSpeed);
    }
    
    // Handle single keyframe case
    if (currentAnimation->getKeyframeCount() == 1) {
        currentValue = currentAnimation->getKeyFrameValue(0);
        return currentValue;
    }
    
    // Find current keyframe pair based on time
    unsigned long animationDuration = currentAnimation->getKeyFrameTime(
        currentAnimation->getKeyframeCount() - 1
    );
    
    // Handle animation completion
    if (effectiveTime >= animationDuration) {
        if (currentMode == PLAY_ONCE) {
            // Animation complete
            currentValue = currentAnimation->getKeyFrameValue(
                currentAnimation->getKeyframeCount() - 1
            );
            currentState = COMPLETED;
            return currentValue;
        } else if (currentMode == PLAY_LOOP) {
            // Loop back to start
            startTime = currentTime - totalPausedTime;
            effectiveTime = 0;
            currentKeyframeIndex = 0;
            nextKeyframeIndex = 1;
        } else if (currentMode == PLAY_BOOMERANG) {
            if (!isReversing) {
                // Switch to reverse playback
                isReversing = true;
                startTime = currentTime - totalPausedTime;
                effectiveTime = 0;
                currentKeyframeIndex = currentAnimation->getKeyframeCount() - 1;
                nextKeyframeIndex = currentAnimation->getKeyframeCount() - 2;
            } else {
                // Switch to forward playback
                isReversing = false;
                startTime = currentTime - totalPausedTime;
                effectiveTime = 0;
                currentKeyframeIndex = 0;
                nextKeyframeIndex = 1;
            }
        }
    }
    
    // Find current position in animation
    bool keyframeUpdated = false;
    
    if (!isReversing) {
        // Forward playback
        while (nextKeyframeIndex < currentAnimation->getKeyframeCount() && 
               effectiveTime >= currentAnimation->getKeyFrameTime(nextKeyframeIndex)) {
            currentKeyframeIndex = nextKeyframeIndex;
            nextKeyframeIndex++;
            keyframeUpdated = true;
        }
    } else {
        // Reverse playback
        while (nextKeyframeIndex >= 0 && 
               effectiveTime >= (animationDuration - currentAnimation->getKeyFrameTime(nextKeyframeIndex))) {
            currentKeyframeIndex = nextKeyframeIndex;
            nextKeyframeIndex--;
            keyframeUpdated = true;
        }
    }
    
    // Interpolate between keyframes
    if (!isReversing) {
        if (nextKeyframeIndex < currentAnimation->getKeyframeCount()) {
            float startVal = currentAnimation->getKeyFrameValue(currentKeyframeIndex);
            float endVal = currentAnimation->getKeyFrameValue(nextKeyframeIndex);
            
            unsigned long startTime = currentAnimation->getKeyFrameTime(currentKeyframeIndex);
            unsigned long endTime = currentAnimation->getKeyFrameTime(nextKeyframeIndex);
            unsigned long segmentDuration = endTime - startTime;
            
            if (segmentDuration > 0) {
                float t = static_cast<float>(effectiveTime - startTime) / segmentDuration;
                t = constrain(t, 0.0f, 1.0f);
                currentValue = interpolateValue(startVal, endVal, t);
            } else {
                currentValue = startVal;
            }
        }
    } else {
        // Reverse playback interpolation
        if (nextKeyframeIndex >= 0) {
            float startVal = currentAnimation->getKeyFrameValue(currentKeyframeIndex);
            float endVal = currentAnimation->getKeyFrameValue(nextKeyframeIndex);
            
            unsigned long startTime = animationDuration - currentAnimation->getKeyFrameTime(currentKeyframeIndex);
            unsigned long endTime = animationDuration - currentAnimation->getKeyFrameTime(nextKeyframeIndex);
            unsigned long segmentDuration = endTime - startTime;
            
            if (segmentDuration > 0) {
                float t = static_cast<float>(effectiveTime - startTime) / segmentDuration;
                t = constrain(t, 0.0f, 1.0f);
                currentValue = interpolateValue(startVal, endVal, t);
            } else {
                currentValue = startVal;
            }
        }
    }
    
    return currentValue;
}

void BaseNotifier::setValueScale(float scale) {
    valueScale = scale;
}

void BaseNotifier::setValueOffset(float offset) {
    valueOffset = offset;
}

void BaseNotifier::setValueRange(float min, float max) {
    minValue = min;
    maxValue = max;
}

void BaseNotifier::pause() {
    if (currentState == PLAYING) {
        currentState = PAUSED;
        pauseTime = millis();
    }
}

void BaseNotifier::resume() {
    if (currentState == PAUSED) {
        totalPausedTime += (millis() - pauseTime);
        currentState = PLAYING;
    }
}

void BaseNotifier::stop() {
    currentState = IDLE;
    currentAnimation = nullptr;
    targetAnimation = nullptr;
    isBlending = false;
}

float BaseNotifier::getValue() const {
    // Apply scale and offset
    float adjustedValue = currentValue * valueScale + valueOffset;
    
    // Constrain to range
    return constrain(adjustedValue, minValue, maxValue);
}

void BaseNotifier::setGlobalSpeed(float speed) {
    // Prevent division by zero
    if (speed == 0) {
        speed = 0.001;
    }
    
    // If changing speed of a running animation, adjust timing
    if (currentState == PLAYING && globalSpeed != speed) {
        // Calculate current progress and adjust start time
        unsigned long currentTime = millis();
        unsigned long effectiveTime = currentTime - startTime - totalPausedTime;
        unsigned long newEffectiveTime = static_cast<unsigned long>(effectiveTime * (globalSpeed / speed));
        
        startTime = currentTime - totalPausedTime - newEffectiveTime;
    }
    
    globalSpeed = speed;
}

float BaseNotifier::getGlobalSpeed() const {
    return globalSpeed;
}

String BaseNotifier::getCurrentAnimationName() const {
    if (currentAnimation != nullptr) {
        return currentAnimation->getName();
    }
    return "";
}

bool BaseNotifier::isPlaying() const {
    return currentState == PLAYING;
}

bool BaseNotifier::isPaused() const {
    return currentState == PAUSED;
}

bool BaseNotifier::isCompleted() const {
    return currentState == COMPLETED;
}

bool BaseNotifier::isBlendingAnimations() const {
    return isBlending;
}

AnimationState BaseNotifier::getState() const {
    return currentState;
}

//======================================================================
// ServoNotifier Implementation
//======================================================================

ServoNotifier::ServoNotifier(Servo& servoRef, int minAngle, int maxAngle) 
    : minAngle(minAngle), maxAngle(maxAngle) {
    servo = &servoRef;
}

void ServoNotifier::update() {
    // Update animation calculations
    BaseNotifier::update();
    
    // Don't update hardware here - let user call servo.write with getValue()
}

LEDNotifier::LEDNotifier(int pin, LEDMode mode) 
    : pin(pin), mode(mode), threshold(0.5f) {
}

void LEDNotifier::begin() {
    pinMode(pin, OUTPUT);
    digitalWrite(pin, LOW);
}

void LEDNotifier::setMode(LEDMode newMode) {
    mode = newMode;
}

void LEDNotifier::setThreshold(float newThreshold) {
    threshold = constrain(newThreshold, 0.0f, 1.0f);
}

void LEDNotifier::update() {
    // First update animation values
    BaseNotifier::update();
    
    // Apply value to the LED
    if (mode == ANALOG) {
        // For analog (PWM) mode
        int pwmValue = constrain(round(getValue()), 0, 255);
        analogWrite(pin, pwmValue);
    } else {
        // For digital (ON/OFF) mode - use threshold
        if (getValue() >= threshold) {
            digitalWrite(pin, HIGH);
        } else {
            digitalWrite(pin, LOW);
        }
    }
}

//======================================================================
// RGBKeyframeAnimation Implementation
//======================================================================

RGBKeyframeAnimation::RGBKeyframeAnimation(const String& name) : name(name) {
    // Initialize with empty keyframe list
}

void RGBKeyframeAnimation::addKeyFrame(byte r, byte g, byte b, unsigned long time) {
    RGBKeyframe newFrame = {r, g, b, time};
    keyframes.push_back(newFrame);
}

bool RGBKeyframeAnimation::setKeyFrameColor(int index, byte r, byte g, byte b) {
    if (index < 0 || index >= keyframes.size()) {
        return false;
    }
    keyframes[index].red = r;
    keyframes[index].green = g;
    keyframes[index].blue = b;
    return true;
}

bool RGBKeyframeAnimation::setKeyFrameTime(int index, unsigned long newTime) {
    if (index < 0 || index >= keyframes.size()) {
        return false;
    }
    keyframes[index].time = newTime;
    return true;
}

int RGBKeyframeAnimation::getKeyframeCount() const {
    return keyframes.size();
}

const String& RGBKeyframeAnimation::getName() const {
    return name;
}

void RGBKeyframeAnimation::getKeyFrameColor(int index, byte& r, byte& g, byte& b) const {
    if (index < 0 || index >= keyframes.size()) {
        r = g = b = 0;
        return;
    }
    r = keyframes[index].red;
    g = keyframes[index].green;
    b = keyframes[index].blue;
}

unsigned long RGBKeyframeAnimation::getKeyFrameTime(int index) const {
    if (index < 0 || index >= keyframes.size()) {
        return 0;
    }
    return keyframes[index].time;
}

//======================================================================
// RGBLEDNotifier Implementation
//======================================================================

RGBLEDNotifier::RGBLEDNotifier() :
    currentAnimation(nullptr),
    currentMode(PLAY_ONCE),
    currentState(IDLE),
    globalSpeed(1.0),
    startTime(0),
    pauseTime(0),
    totalPausedTime(0),
    currentKeyframeIndex(0),
    nextKeyframeIndex(0),
    currentRed(0),
    currentGreen(0),
    currentBlue(0),
    isReversing(false),
    isBlending(false),
    blendStartTime(0),
    blendDuration(0),
    startRed(0),
    startGreen(0),
    startBlue(0)
{
    // Initialize with no animations
}

void RGBLEDNotifier::addAnimation(const RGBKeyframeAnimation& animation) {
    // Only add if it has keyframes and isn't already in our list
    if (animation.getKeyframeCount() == 0) {
        return;
    }
    
    // Check for duplicate name
    for (const auto& anim : animations) {
        if (anim.getName() == animation.getName()) {
            return; // Skip duplicates
        }
    }
    
    animations.push_back(animation);
}

void RGBLEDNotifier::playAnimation(const RGBKeyframeAnimation& animation, PlayMode mode) {
    if (animation.getKeyframeCount() < 1) {
        return;
    }
    
    // Stop any current animation
    stop();
    
    // Set up new animation
    for (const auto& anim : animations) {
        if (anim.getName() == animation.getName()) {
            currentAnimation = const_cast<RGBKeyframeAnimation*>(&anim);
            break;
        }
    }
    
    // If not in our collection, use the provided one directly
    if (currentAnimation == nullptr) {
        addAnimation(animation);
        for (auto& anim : animations) {
            if (anim.getName() == animation.getName()) {
                currentAnimation = &anim;
                break;
            }
        }
    }
    
    // Initialize playback state
    currentMode = mode;
    isReversing = false;
    currentKeyframeIndex = 0;
    nextKeyframeIndex = 1;
    
    if (currentAnimation->getKeyframeCount() > 1) {
        nextKeyframeIndex = 1;
    } else {
        nextKeyframeIndex = 0;
    }
    
    // Start timing
    startTime = millis();
    totalPausedTime = 0;
    
    // Initial color
    currentAnimation->getKeyFrameColor(0, currentRed, currentGreen, currentBlue);
    
    // Set state
    currentState = PLAYING;
}

bool RGBLEDNotifier::playAnimation(const String& name, PlayMode mode) {
    // Find animation by name
    for (auto& anim : animations) {
        if (anim.getName() == name) {
            playAnimation(anim, mode);
            return true;
        }
    }
    return false;
}

void RGBLEDNotifier::crossfadeTo(const RGBKeyframeAnimation& animation, unsigned long blendTime, PlayMode mode) {
    if (animation.getKeyframeCount() < 1 || currentState == IDLE) {
        // If no animation is playing, just start the new one
        playAnimation(animation, mode);
        return;
    }
    
    // Save current color for blending
    startRed = currentRed;
    startGreen = currentGreen;
    startBlue = currentBlue;
    
    // Find the target animation in our collection
    for (auto& anim : animations) {
        if (anim.getName() == animation.getName()) {
            targetAnimation = &anim;
            break;
        }
    }
    
    // If not in our collection, add it
    if (targetAnimation == nullptr) {
        addAnimation(animation);
        for (auto& anim : animations) {
            if (anim.getName() == animation.getName()) {
                targetAnimation = &anim;
                break;
            }
        }
    }
    
    // Set up blending
    isBlending = true;
    blendStartTime = millis();
    blendDuration = blendTime;
}

bool RGBLEDNotifier::crossfadeTo(const String& name, unsigned long blendTime, PlayMode mode) {
    // Find animation by name
    for (auto& anim : animations) {
        if (anim.getName() == name) {
            crossfadeTo(anim, blendTime, mode);
            return true;
        }
    }
    return false;
}

void RGBLEDNotifier::update() {
    if (currentState == IDLE || currentState == COMPLETED) {
        return;
    }
    
    if (currentState == PAUSED) {
        // When paused, just track time
        totalPausedTime = millis() - pauseTime;
        return;
    }
    
    if (isBlending && targetAnimation != nullptr) {
        // Handle blending between animations
        unsigned long currentTime = millis();
        unsigned long elapsedTime = currentTime - blendStartTime;
        
        if (elapsedTime >= blendDuration) {
            // Blend complete, switch to target animation
            isBlending = false;
            playAnimation(*targetAnimation, currentMode);
            targetAnimation = nullptr;
        } else {
            // Calculate blend factor (0.0 to 1.0)
            float t = static_cast<float>(elapsedTime) / blendDuration;
            
            // Get target color
            byte targetRed, targetGreen, targetBlue;
            targetAnimation->getKeyFrameColor(0, targetRed, targetGreen, targetBlue);
            
            // Calculate blended color
            interpolateColor(
                startRed, startGreen, startBlue,
                targetRed, targetGreen, targetBlue,
                t, currentRed, currentGreen, currentBlue
            );
        }
        return;
    }
    
    // Regular animation update - just calculate color values
    calculateCurrentColor();
}

void RGBLEDNotifier::interpolateColor(
    byte startR, byte startG, byte startB,
    byte endR, byte endG, byte endB,
    float t, byte& outR, byte& outG, byte& outB)
{
    // Linear interpolation for each channel
    outR = startR + (endR - startR) * t;
    outG = startG + (endG - startG) * t;
    outB = startB + (endB - startB) * t;
}

void RGBLEDNotifier::calculateCurrentColor() {
    if (currentAnimation == nullptr || currentAnimation->getKeyframeCount() < 1) {
        return;
    }
    
    // Get elapsed time, accounting for pauses
    unsigned long currentTime = millis();
    unsigned long effectiveTime = currentTime - startTime - totalPausedTime;
    
    // Scale by global speed
    if (globalSpeed != 0) {
        effectiveTime = static_cast<unsigned long>(effectiveTime / globalSpeed);
    }
    
    // Handle single keyframe case
    if (currentAnimation->getKeyframeCount() == 1) {
        currentAnimation->getKeyFrameColor(0, currentRed, currentGreen, currentBlue);
        return;
    }
    
    // Find current keyframe pair based on time
    unsigned long animationDuration = currentAnimation->getKeyFrameTime(
        currentAnimation->getKeyframeCount() - 1
    );
    
    // Handle animation completion
    if (effectiveTime >= animationDuration) {
        if (currentMode == PLAY_ONCE) {
            // Animation complete
            currentAnimation->getKeyFrameColor(
                currentAnimation->getKeyframeCount() - 1,
                currentRed, currentGreen, currentBlue
            );
            currentState = COMPLETED;
            return;
        } else if (currentMode == PLAY_LOOP) {
            // Loop back to start
            startTime = currentTime - totalPausedTime;
            effectiveTime = 0;
            currentKeyframeIndex = 0;
            nextKeyframeIndex = 1;
        } else if (currentMode == PLAY_BOOMERANG) {
            if (!isReversing) {
                // Switch to reverse playback
                isReversing = true;
                startTime = currentTime - totalPausedTime;
                effectiveTime = 0;
                currentKeyframeIndex = currentAnimation->getKeyframeCount() - 1;
                nextKeyframeIndex = currentAnimation->getKeyframeCount() - 2;
            } else {
                // Switch to forward playback
                isReversing = false;
                startTime = currentTime - totalPausedTime;
                effectiveTime = 0;
                currentKeyframeIndex = 0;
                nextKeyframeIndex = 1;
            }
        }
    }
    
    // Find current position in animation
    bool keyframeUpdated = false;
    
    if (!isReversing) {
        // Forward playback
        while (nextKeyframeIndex < currentAnimation->getKeyframeCount() && 
               effectiveTime >= currentAnimation->getKeyFrameTime(nextKeyframeIndex)) {
            currentKeyframeIndex = nextKeyframeIndex;
            nextKeyframeIndex++;
            keyframeUpdated = true;
        }
    } else {
        // Reverse playback
        while (nextKeyframeIndex >= 0 && 
               effectiveTime >= (animationDuration - currentAnimation->getKeyFrameTime(nextKeyframeIndex))) {
            currentKeyframeIndex = nextKeyframeIndex;
            nextKeyframeIndex--;
            keyframeUpdated = true;
        }
    }
    
    // Interpolate between keyframes
    if (!isReversing) {
        if (nextKeyframeIndex < currentAnimation->getKeyframeCount()) {
            byte startR, startG, startB;
            byte endR, endG, endB;
            
            currentAnimation->getKeyFrameColor(currentKeyframeIndex, startR, startG, startB);
            currentAnimation->getKeyFrameColor(nextKeyframeIndex, endR, endG, endB);
            
            unsigned long startTime = currentAnimation->getKeyFrameTime(currentKeyframeIndex);
            unsigned long endTime = currentAnimation->getKeyFrameTime(nextKeyframeIndex);
            unsigned long segmentDuration = endTime - startTime;
            
            if (segmentDuration > 0) {
                float t = static_cast<float>(effectiveTime - startTime) / segmentDuration;
                t = constrain(t, 0.0f, 1.0f);
                interpolateColor(startR, startG, startB, endR, endG, endB, t, 
                                 currentRed, currentGreen, currentBlue);
            } else {
                currentRed = startR;
                currentGreen = startG;
                currentBlue = startB;
            }
        }
    } else {
        // Reverse playback interpolation
        if (nextKeyframeIndex >= 0) {
            byte startR, startG, startB;
            byte endR, endG, endB;
            
            currentAnimation->getKeyFrameColor(currentKeyframeIndex, startR, startG, startB);
            currentAnimation->getKeyFrameColor(nextKeyframeIndex, endR, endG, endB);
            
            unsigned long startTime = animationDuration - currentAnimation->getKeyFrameTime(currentKeyframeIndex);
            unsigned long endTime = animationDuration - currentAnimation->getKeyFrameTime(nextKeyframeIndex);
            unsigned long segmentDuration = endTime - startTime;
            
            if (segmentDuration > 0) {
                float t = static_cast<float>(effectiveTime - startTime) / segmentDuration;
                t = constrain(t, 0.0f, 1.0f);
                interpolateColor(startR, startG, startB, endR, endG, endB, t, 
                                 currentRed, currentGreen, currentBlue);
            } else {
                currentRed = startR;
                currentGreen = startG;
                currentBlue = startB;
            }
        }
    }
}

void RGBLEDNotifier::pause() {
    if (currentState == PLAYING) {
        currentState = PAUSED;
        pauseTime = millis();
    }
}

void RGBLEDNotifier::resume() {
    if (currentState == PAUSED) {
        totalPausedTime += (millis() - pauseTime);
        currentState = PLAYING;
    }
}

void RGBLEDNotifier::stop() {
    currentState = IDLE;
    currentAnimation = nullptr;
}

void RGBLEDNotifier::setGlobalSpeed(float speed) {
    // Prevent division by zero
    if (speed == 0) {
        speed = 0.001;
    }
    
    // If changing speed of a running animation, adjust timing
    if (currentState == PLAYING && globalSpeed != speed) {
        // Calculate current progress and adjust start time
        unsigned long currentTime = millis();
        unsigned long effectiveTime = currentTime - startTime - totalPausedTime;
        unsigned long newEffectiveTime = static_cast<unsigned long>(effectiveTime * (globalSpeed / speed));
        
        startTime = currentTime - totalPausedTime - newEffectiveTime;
    }
    
    globalSpeed = speed;
}

float RGBLEDNotifier::getGlobalSpeed() const {
    return globalSpeed;
}

void RGBLEDNotifier::getRGB(byte& r, byte& g, byte& b) const {
    r = currentRed;
    g = currentGreen;
    b = currentBlue;
}

byte RGBLEDNotifier::getRed() const {
    return currentRed;
}

byte RGBLEDNotifier::getGreen() const {
    return currentGreen;
}

byte RGBLEDNotifier::getBlue() const {
    return currentBlue;
}

uint32_t RGBLEDNotifier::getColor() const {
    // Pack RGB values into a single 32-bit color value
    // Format: 0x00RRGGBB (same format as NeoPixel library's Color function)
    return ((uint32_t)currentRed << 16) | ((uint32_t)currentGreen << 8) | currentBlue;
}

String RGBLEDNotifier::getCurrentAnimationName() const {
    if (currentAnimation != nullptr) {
        return currentAnimation->getName();
    }
    return "";
}

bool RGBLEDNotifier::isPlaying() const {
    return currentState == PLAYING;
}

bool RGBLEDNotifier::isPaused() const {
    return currentState == PAUSED;
}

bool RGBLEDNotifier::isCompleted() const {
    return currentState == COMPLETED;
}

bool RGBLEDNotifier::isBlendingAnimations() const {
    return isBlending;
}

AnimationState RGBLEDNotifier::getState() const {
    return currentState;
}
