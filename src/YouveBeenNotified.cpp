// YouveBeenNotified.cpp
// Implementation for the animation classes

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
// ServoNotifier Implementation
//======================================================================

ServoNotifier::ServoNotifier(int minAngle, int maxAngle) 
    : servo(nullptr), 
      minAngle(minAngle), 
      maxAngle(maxAngle),
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
      startValue(0.0) {
}

void ServoNotifier::addAnimation(const KeyframeAnimation& animation) {
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

void ServoNotifier::playAnimation(const KeyframeAnimation& animation, PlayMode mode) {
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

bool ServoNotifier::playAnimation(const String& name, PlayMode mode) {
    // Find animation by name
    for (auto& anim : animations) {
        if (anim.getName() == name) {
            playAnimation(anim, mode);
            return true;
        }
    }
    return false;
}

void ServoNotifier::crossfadeTo(const KeyframeAnimation& animation, unsigned long blendTime, PlayMode mode) {
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

bool ServoNotifier::crossfadeTo(const String& name, unsigned long blendTime, PlayMode mode) {
    // Find animation by name
    for (auto& anim : animations) {
        if (anim.getName() == name) {
            crossfadeTo(anim, blendTime, mode);
            return true;
        }
    }
    return false;
}

float ServoNotifier::interpolateValue(float startVal, float endVal, float t) {
    // Linear interpolation
    return startVal + (endVal - startVal) * t;
}

float ServoNotifier::calculateCurrentValue() {
    if (currentAnimation == nullptr || currentAnimation->getKeyframeCount() < 1) {
        return currentValue;
    }
    
    // Get elapsed time, accounting for pauses
    unsigned long currentTime = millis();
    unsigned long effectiveTime = currentTime - startTime - totalPausedTime;
    
    // Handle single keyframe case
    if (currentAnimation->getKeyframeCount() == 1) {
        currentValue = currentAnimation->getKeyFrameValue(0);
        return currentValue;
    }
    
    // Get animation duration (scaled by globalSpeed)
    unsigned long animationDuration = currentAnimation->getKeyFrameTime(
        currentAnimation->getKeyframeCount() - 1
    ) / globalSpeed;
    
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
               effectiveTime >= (currentAnimation->getKeyFrameTime(nextKeyframeIndex) / globalSpeed)) {
            currentKeyframeIndex = nextKeyframeIndex;
            nextKeyframeIndex++;
            keyframeUpdated = true;
        }
    } else {
        // Reverse playback
        unsigned long scaledAnimationDuration = animationDuration; // Already scaled above
        while (nextKeyframeIndex >= 0 && 
               effectiveTime >= (scaledAnimationDuration - 
                               (currentAnimation->getKeyFrameTime(nextKeyframeIndex) / globalSpeed))) {
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
            
            unsigned long startTimeVal = currentAnimation->getKeyFrameTime(currentKeyframeIndex) / globalSpeed;
            unsigned long endTimeVal = currentAnimation->getKeyFrameTime(nextKeyframeIndex) / globalSpeed;
            unsigned long segmentDuration = endTimeVal - startTimeVal;
            
            if (segmentDuration > 0) {
                float t = static_cast<float>(effectiveTime - startTimeVal) / segmentDuration;
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
            
            unsigned long animDuration = animationDuration; // Already scaled
            unsigned long startTimeVal = animDuration - 
                                      (currentAnimation->getKeyFrameTime(currentKeyframeIndex) / globalSpeed);
            unsigned long endTimeVal = animDuration - 
                                     (currentAnimation->getKeyFrameTime(nextKeyframeIndex) / globalSpeed);
            unsigned long segmentDuration = endTimeVal - startTimeVal;
            
            if (segmentDuration > 0) {
                float t = static_cast<float>(effectiveTime - startTimeVal) / segmentDuration;
                t = constrain(t, 0.0f, 1.0f);
                currentValue = interpolateValue(startVal, endVal, t);
            } else {
                currentValue = startVal;
            }
        }
    }
    
    return currentValue;
}

void ServoNotifier::update() {
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
    
    // Don't update hardware here - let user call servo.write with getValue()
}

void ServoNotifier::setValueScale(float scale) {
    valueScale = scale;
}

void ServoNotifier::setValueOffset(float offset) {
    valueOffset = offset;
}

void ServoNotifier::setValueRange(float min, float max) {
    minValue = min;
    maxValue = max;
}

void ServoNotifier::pause() {
    if (currentState == PLAYING) {
        currentState = PAUSED;
        pauseTime = millis();
    }
}

void ServoNotifier::resume() {
    if (currentState == PAUSED) {
        totalPausedTime += (millis() - pauseTime);
        currentState = PLAYING;
    }
}

void ServoNotifier::stop() {
    currentState = IDLE;
    currentAnimation = nullptr;
    targetAnimation = nullptr;
    isBlending = false;
}

int ServoNotifier::getValue() const {
    // Apply scale and offset
    float adjustedValue = currentValue * valueScale + valueOffset;
    
    // Constrain to range and round to nearest integer
    return round(constrain(adjustedValue, minValue, maxValue));
}

bool ServoNotifier::hasChanged() {
    static int lastValue = -1;
    int currentIntValue = getValue();
    
    bool changed = (currentIntValue != lastValue);
    lastValue = currentIntValue;
    
    return changed;
}

void ServoNotifier::setGlobalSpeed(float speed) {
    // Prevent division by zero
    if (speed == 0) {
        speed = 0.001;
    }
    
    // Simply update the speed factor - no timing adjustments needed
    globalSpeed = speed;
}

float ServoNotifier::getGlobalSpeed() const {
    return globalSpeed;
}

String ServoNotifier::getCurrentAnimationName() const {
    if (currentAnimation != nullptr) {
        return currentAnimation->getName();
    }
    return "";
}

bool ServoNotifier::isPlaying() const {
    return currentState == PLAYING;
}

bool ServoNotifier::isPaused() const {
    return currentState == PAUSED;
}

bool ServoNotifier::isCompleted() const {
    return currentState == COMPLETED;
}

bool ServoNotifier::isBlendingAnimations() const {
    return isBlending;
}

AnimationState ServoNotifier::getState() const {
    return currentState;
}

bool ServoNotifier::completed() const {
    return isCompleted();
}

unsigned long ServoNotifier::timeToNextKey() const {
    if (currentState != PLAYING || currentAnimation == nullptr) {
        return 0;
    }
    
    if (currentAnimation->getKeyframeCount() <= 1 || 
        nextKeyframeIndex >= currentAnimation->getKeyframeCount() ||
        nextKeyframeIndex < 0) {
        return 0;
    }
    
    // Get elapsed time, accounting for pauses
    unsigned long currentTime = millis();
    unsigned long effectiveTime = currentTime - startTime - totalPausedTime;
    
    // Get the time of the next keyframe (scaled by globalSpeed)
    unsigned long nextKeyTime;
    
    if (!isReversing) {
        nextKeyTime = currentAnimation->getKeyFrameTime(nextKeyframeIndex) / globalSpeed;
    } else {
        // For reverse playback
        unsigned long animationDuration = currentAnimation->getKeyFrameTime(
            currentAnimation->getKeyframeCount() - 1
        ) / globalSpeed;
        nextKeyTime = animationDuration - 
                    (currentAnimation->getKeyFrameTime(nextKeyframeIndex) / globalSpeed);
    }
    
    // If we're already past the next keyframe time, return 0
    if (effectiveTime >= nextKeyTime) {
        return 0;
    }
    
    // Calculate time remaining to next keyframe
    return nextKeyTime - effectiveTime;
}

unsigned long ServoNotifier::timeRemaining() const {
    if (currentState != PLAYING || currentAnimation == nullptr) {
        return 0;
    }
    
    if (currentAnimation->getKeyframeCount() < 1) {
        return 0;
    }
    
    // For looping animations, there's no true "end"
    if (currentMode == PLAY_LOOP || currentMode == PLAY_BOOMERANG) {
        // Return time to complete current cycle
        unsigned long currentTime = millis();
        unsigned long effectiveTime = currentTime - startTime - totalPausedTime;
        
        unsigned long animationDuration = currentAnimation->getKeyFrameTime(
            currentAnimation->getKeyframeCount() - 1
        ) / globalSpeed;
        
        if (isReversing && currentMode == PLAY_BOOMERANG) {
            // For boomerang in reverse direction, calculate time to get back to the start
            return animationDuration - (effectiveTime % animationDuration);
        } else {
            // For regular loop or boomerang in forward direction
            return animationDuration - (effectiveTime % animationDuration);
        }
    }
    
    // For PLAY_ONCE mode
    unsigned long currentTime = millis();
    unsigned long effectiveTime = currentTime - startTime - totalPausedTime;
    
    unsigned long animationDuration = currentAnimation->getKeyFrameTime(
        currentAnimation->getKeyframeCount() - 1
    ) * globalSpeed;
    
    // If we're already past the end, return 0
    if (effectiveTime >= animationDuration) {
        return 0;
    }
    
    // Calculate time remaining to the end
    return animationDuration - effectiveTime;
}

//======================================================================
// LEDNotifier Implementation
//======================================================================

LEDNotifier::LEDNotifier(int pin, LEDMode mode) 
    : pin(pin), 
      mode(mode), 
      threshold(0.5f),
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
      startValue(0.0) {
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

void LEDNotifier::addAnimation(const KeyframeAnimation& animation) {
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

void LEDNotifier::playAnimation(const KeyframeAnimation& animation, PlayMode mode) {
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

bool LEDNotifier::playAnimation(const String& name, PlayMode mode) {
    // Find animation by name
    for (auto& anim : animations) {
        if (anim.getName() == name) {
            playAnimation(anim, mode);
            return true;
        }
    }
    return false;
}

void LEDNotifier::crossfadeTo(const KeyframeAnimation& animation, unsigned long blendTime, PlayMode mode) {
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

bool LEDNotifier::crossfadeTo(const String& name, unsigned long blendTime, PlayMode mode) {
    // Find animation by name
    for (auto& anim : animations) {
        if (anim.getName() == name) {
            crossfadeTo(anim, blendTime, mode);
            return true;
        }
    }
    return false;
}

float LEDNotifier::interpolateValue(float startVal, float endVal, float t) {
    // Linear interpolation
    return startVal + (endVal - startVal) * t;
}

float LEDNotifier::calculateCurrentValue() {
    if (currentAnimation == nullptr || currentAnimation->getKeyframeCount() < 1) {
        return currentValue;
    }
    
    // Get elapsed time, accounting for pauses
    unsigned long currentTime = millis();
    unsigned long effectiveTime = currentTime - startTime - totalPausedTime;
    
    // Handle single keyframe case
    if (currentAnimation->getKeyframeCount() == 1) {
        currentValue = currentAnimation->getKeyFrameValue(0);
        return currentValue;
    }
    
    // Get animation duration (scaled by globalSpeed)
    unsigned long animationDuration = currentAnimation->getKeyFrameTime(
        currentAnimation->getKeyframeCount() - 1
    ) / globalSpeed;
    
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
               effectiveTime >= (currentAnimation->getKeyFrameTime(nextKeyframeIndex) / globalSpeed)) {
            currentKeyframeIndex = nextKeyframeIndex;
            nextKeyframeIndex++;
            keyframeUpdated = true;
        }
    } else {
        // Reverse playback
        unsigned long scaledAnimationDuration = animationDuration; // Already scaled above
        while (nextKeyframeIndex >= 0 && 
               effectiveTime >= (scaledAnimationDuration - 
                               (currentAnimation->getKeyFrameTime(nextKeyframeIndex) / globalSpeed))) {
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
            
            unsigned long startTimeVal = currentAnimation->getKeyFrameTime(currentKeyframeIndex) / globalSpeed;
            unsigned long endTimeVal = currentAnimation->getKeyFrameTime(nextKeyframeIndex) / globalSpeed;
            unsigned long segmentDuration = endTimeVal - startTimeVal;
            
            if (segmentDuration > 0) {
                float t = static_cast<float>(effectiveTime - startTimeVal) / segmentDuration;
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
            
            unsigned long animDuration = animationDuration; // Already scaled
            unsigned long startTimeVal = animDuration - 
                                       (currentAnimation->getKeyFrameTime(currentKeyframeIndex) / globalSpeed);
            unsigned long endTimeVal = animDuration - 
                                     (currentAnimation->getKeyFrameTime(nextKeyframeIndex) / globalSpeed);
            unsigned long segmentDuration = endTimeVal - startTimeVal;
            
            if (segmentDuration > 0) {
                float t = static_cast<float>(effectiveTime - startTimeVal) / segmentDuration;
                t = constrain(t, 0.0f, 1.0f);
                currentValue = interpolateValue(startVal, endVal, t);
            } else {
                currentValue = startVal;
            }
        }
    }
    
    return currentValue;
}

void LEDNotifier::update() {
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
    
    // First update animation values
    calculateCurrentValue();
    
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

void LEDNotifier::setValueScale(float scale) {
    valueScale = scale;
}

void LEDNotifier::setValueOffset(float offset) {
    valueOffset = offset;
}

void LEDNotifier::setValueRange(float min, float max) {
    minValue = min;
    maxValue = max;
}

void LEDNotifier::pause() {
    if (currentState == PLAYING) {
        currentState = PAUSED;
        pauseTime = millis();
    }
}

void LEDNotifier::resume() {
    if (currentState == PAUSED) {
        totalPausedTime += (millis() - pauseTime);
        currentState = PLAYING;
    }
}

void LEDNotifier::stop() {
    currentState = IDLE;
    currentAnimation = nullptr;
    targetAnimation = nullptr;
    isBlending = false;
}

int LEDNotifier::getValue() const {
    // Apply scale and offset
    float adjustedValue = currentValue * valueScale + valueOffset;
    
    // Constrain to range and round to nearest integer
    return round(constrain(adjustedValue, minValue, maxValue));
}

bool LEDNotifier::hasChanged() {
    static int lastValue = -1;
    int currentIntValue = getValue();
    
    bool changed = (currentIntValue != lastValue);
    lastValue = currentIntValue;
    
    return changed;
}

void LEDNotifier::setGlobalSpeed(float speed) {
    // Prevent division by zero
    if (speed == 0) {
        speed = 0.001;
    }
    
    // Simply update the speed factor - no timing adjustments needed
    globalSpeed = speed;
}

float LEDNotifier::getGlobalSpeed() const {
    return globalSpeed;
}

String LEDNotifier::getCurrentAnimationName() const {
    if (currentAnimation != nullptr) {
        return currentAnimation->getName();
    }
    return "";
}

bool LEDNotifier::isPlaying() const {
    return currentState == PLAYING;
}

bool LEDNotifier::isPaused() const {
    return currentState == PAUSED;
}

bool LEDNotifier::isCompleted() const {
    return currentState == COMPLETED;
}

bool LEDNotifier::isBlendingAnimations() const {
    return isBlending;
}

AnimationState LEDNotifier::getState() const {
    return currentState;
}

bool LEDNotifier::completed() const {
    return isCompleted();
}

unsigned long LEDNotifier::timeToNextKey() const {
    if (currentState != PLAYING || currentAnimation == nullptr) {
        return 0;
    }
    
    if (currentAnimation->getKeyframeCount() <= 1 || 
        nextKeyframeIndex >= currentAnimation->getKeyframeCount() ||
        nextKeyframeIndex < 0) {
        return 0;
    }
    
    // Get elapsed time, accounting for pauses
    unsigned long currentTime = millis();
    unsigned long effectiveTime = currentTime - startTime - totalPausedTime;
    
    // Get the time of the next keyframe (scaled by globalSpeed)
    unsigned long nextKeyTime;
    
    if (!isReversing) {
        nextKeyTime = currentAnimation->getKeyFrameTime(nextKeyframeIndex) / globalSpeed;
    } else {
        // For reverse playback
        unsigned long animationDuration = currentAnimation->getKeyFrameTime(
            currentAnimation->getKeyframeCount() - 1
        ) * globalSpeed;
        nextKeyTime = animationDuration - 
                     (currentAnimation->getKeyFrameTime(nextKeyframeIndex) / globalSpeed);
    }
    
    // If we're already past the next keyframe time, return 0
    if (effectiveTime >= nextKeyTime) {
        return 0;
    }
    
    // Calculate time remaining to next keyframe
    return nextKeyTime - effectiveTime;
}

unsigned long LEDNotifier::timeRemaining() const {
    if (currentState != PLAYING || currentAnimation == nullptr) {
        return 0;
    }
    
    if (currentAnimation->getKeyframeCount() < 1) {
        return 0;
    }
    
    // For looping animations, there's no true "end"
    if (currentMode == PLAY_LOOP || currentMode == PLAY_BOOMERANG) {
        // Return time to complete current cycle
        unsigned long currentTime = millis();
        unsigned long effectiveTime = currentTime - startTime - totalPausedTime;
        
        unsigned long animationDuration = currentAnimation->getKeyFrameTime(
            currentAnimation->getKeyframeCount() - 1
        ) * globalSpeed;
        
        if (isReversing && currentMode == PLAY_BOOMERANG) {
            // For boomerang in reverse direction, calculate time to get back to the start
            return animationDuration - (effectiveTime % animationDuration);
        } else {
            // For regular loop or boomerang in forward direction
            return animationDuration - (effectiveTime % animationDuration);
        }
    }
    
    // For PLAY_ONCE mode
    unsigned long currentTime = millis();
    unsigned long effectiveTime = currentTime - startTime - totalPausedTime;
    
    unsigned long animationDuration = currentAnimation->getKeyFrameTime(
        currentAnimation->getKeyframeCount() - 1
    ) * globalSpeed;
    
    // If we're already past the end, return 0
    if (effectiveTime >= animationDuration) {
        return 0;
    }
    
    // Calculate time remaining to the end
    return animationDuration - effectiveTime;
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