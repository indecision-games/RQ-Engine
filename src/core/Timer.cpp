#include "Timer.hpp"

using namespace RQEngine;
//
// Basic Timer
//
Timer::Timer(){
    clock = sf::Clock();
    runTime = sf::microseconds(0);

    started = false;
    paused = false;
}

void Timer::start(){
    started = true;
    paused = false;

    clock.restart();
    runTime = sf::microseconds(0);
}

void Timer::stop(){
    started = false;
    paused = false;
    runTime = sf::microseconds(0);
}

void Timer::pause(){
    if(started && !paused){
        paused = true;
        runTime += clock.restart();
    }
}

void Timer::unpause(){
    if(started && paused){
        paused = false;
        clock.restart();
    }
}

uint64_t Timer::getElapsedTime(){
    sf::Time time;
    if(started){
        if(paused){
            time = runTime;
        }else{
            time = clock.getElapsedTime() + runTime;
        }
    }
    return time.asMicroseconds();
}

bool Timer::isStarted(){
    return started;
}

bool Timer::isPaused(){
    return paused && started;
}
//
// FPS Limiter and Timestep
//
FpsTimer::FpsTimer(int maxFramesPerSecond){
    maxFPS = maxFramesPerSecond;
    currentFPS = maxFramesPerSecond;
    frameCount = 0;
    clock = sf::Clock();
    startTime = sf::microseconds(0);
}

void FpsTimer::start(){
    frameCount++;
    previousFrameTime = startTime;
    startTime = clock.getElapsedTime();
}

void FpsTimer::update(){
    if(frameCount == SAMPLE_FRAME){
        sf::Time currentTime = clock.getElapsedTime();
        frameCount = 0;
        currentFPS = (float)SAMPLE_FRAME / (currentTime - fpsCounterTime).asSeconds();
        fpsCounterTime = currentTime;
    }
}

void FpsTimer::limit(){
    sf::Time currentTime = clock.getElapsedTime();
    if(frameCount == SAMPLE_FRAME){
        frameCount = 0;
        currentFPS = (float)SAMPLE_FRAME / (currentTime - fpsCounterTime).asSeconds();
        fpsCounterTime = currentTime;
    }
    float frameTime = (currentTime - startTime).asSeconds();
    float maxFrameTime = 1.0f/(float)maxFPS;
    if (maxFrameTime > frameTime){
        sf::Time delay = sf::seconds(maxFrameTime - frameTime);
        sf::sleep(delay);
    }
}

void FpsTimer::setMaxFps(int maxFramesPerSecond){
    maxFPS = maxFramesPerSecond;
}

float FpsTimer::getCurrentFPS(){
    return currentFPS;
}

float FpsTimer::getDeltaTime(){
    return (startTime - previousFrameTime).asSeconds();
}

float FpsTimer::getMaxDeltaTime(){
    return 1.0f / (float)maxFPS;
}
