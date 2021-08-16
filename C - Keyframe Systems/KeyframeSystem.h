#ifndef KEYFRAMESYSTEM_H
#define KEYFRAMESYSTEM_H

#include <iostream>
#include <vector>
#include "glm\glm.hpp"
#include "MathHelper.h"

template <typename K>
struct Keyframe
{
	K keyValue;
	int startFrame;

	Keyframe(K _keyValue, int _startFrame)
	{
		keyValue = _keyValue;
		startFrame = _startFrame;
	}
};

template <typename T>
class KeyframeSystem
{
public:
	KeyframeSystem();
	KeyframeSystem(int totalFrameLength);

	~KeyframeSystem() {}

	void setPaused(bool _paused);
	void setPlaysForward(bool _playsForward);
	void setLoops(bool _loops);
	void setTotalNumFrames(int newFrameLength);
	
	void jumpToNextKeyframe();
	void jumpToPreviousKeyframe();
	void jumpToStart();
	void jumpToEnd();

	void output() const;
	void addKey(Keyframe<T> key);
	T update(float deltaTime);

private:
	std::vector<Keyframe<T>> keys;
	int currentKeyframe;
	int nextKeyframe;
	int currentFrameNumber;
	int totalNumFrames;
	bool paused;
	bool loops;
	bool forward; //If it is playing forward, false means its playing in reverse
	float t;
};


/*
	Template implementations
*/
template<typename T>
KeyframeSystem<T>::KeyframeSystem()
{
	currentKeyframe = 0;
	nextKeyframe = 1;
	paused = false;
	loops = true;
	forward = true;
	currentFrameNumber = 0;
	t = 0.0f;
	totalNumFrames = 0;
}

template<typename T>
KeyframeSystem<T>::KeyframeSystem(int numFramesTotal)
{
	currentKeyframe = 0;
	nextKeyframe = 1;
	paused = false;
	loops = true;
	forward = true;
	currentFrameNumber = 0;
	t = 0.0f;
	totalNumFrames = numFramesTotal;
}

template<typename T>
void KeyframeSystem<T>::setPaused(bool _paused)  {
	paused = _paused;
}

template<typename T>
void KeyframeSystem<T>::setLoops(bool _loops) {
	loops = _loops;
}

template<typename T>
void KeyframeSystem<T>::setPlaysForward(bool _playsForward) {
	forward = _playsForward;
}

template<typename T>
void KeyframeSystem<T>::setTotalNumFrames(int newLength) {
	totalNumFrames = newLength;
}

template<typename T>
void KeyframeSystem<T>::addKey(Keyframe<T> key) {
	keys.push_back(key);
}

template<typename T>
void KeyframeSystem<T>::jumpToNextKeyframe()
{
	if (currentKeyframe < keys.size() - 1)
		currentKeyframe++;
	else
		currentKeyframe = 0;

	if (nextKeyframe < keys.size() - 1)
		nextKeyframe++;
	else
		nextKeyframe = 0;

	currentFrameNumber = keys[currentKeyframe].startFrame;
	t = 0;
}

template<typename T>
void KeyframeSystem<T>::jumpToPreviousKeyframe()
{
	if (currentKeyframe > 0)
		currentKeyframe--;
	else
		currentKeyframe = keys.size() - 1;

	if (nextKeyframe > 0)
		nextKeyframe--;
	else
		currentKeyframe = keys.size() - 1;

	currentFrameNumber = keys[currentKeyframe].startFrame;
	t = 0;
}

template<typename T>
void KeyframeSystem<T>::jumpToStart()
{
	currentKeyframe = 0;
	nextKeyframe = 1;
	currentFrameNumber = 0;
	t = 0;
}

template<typename T>
void KeyframeSystem<T>::jumpToEnd()
{
	currentKeyframe = keys.size() - 1;
	nextKeyframe = 0;
	currentFrameNumber = keys[currentKeyframe].startFrame;
	t = 0;
}

template<typename T>
void KeyframeSystem<T>::output() const
{
	std::cout << "NUM KEYS: " << keys.size() << std::endl;
	std::cout << "CURRENT KEY: " << currentKeyframe << std::endl;
	std::cout << "NEXT KEY: " << nextKeyframe << std::endl;
	std::cout << "CURRENT FRAME: " << currentFrameNumber << std::endl;
	std::cout << "TOTAL NUM FRAMES: " << totalNumFrames << std::endl;
	std::cout << "T: " << t << endl;
	std::cout << "--------------------------" << std::endl;
}

template<typename T>
T KeyframeSystem<T>::update(float deltaTime)
{
	if (!paused)
	{
		if (forward)
			currentFrameNumber++;
		else
			currentFrameNumber--;

		if (nextKeyframe < keys.size() - 1)
			t = float(currentFrameNumber - keys[currentKeyframe].startFrame) / float(keys[nextKeyframe].startFrame - keys[currentKeyframe].startFrame);
		else
			t = float(currentFrameNumber - keys[currentKeyframe].startFrame) / float(totalNumFrames - keys[currentKeyframe].startFrame);

		T p0 = keys[currentKeyframe].keyValue;
		T p1 = keys[nextKeyframe].keyValue;

		if (forward)
		{
			if (t > 1.0f)
			{
				if (currentKeyframe < keys.size() - 1)
					currentKeyframe++;

				if (nextKeyframe < keys.size() - 1)
					nextKeyframe++;
			}
		}
		else
		{
			if (t < 0.0f)
			{
				if (currentKeyframe > 0)
					currentKeyframe--;

				if (nextKeyframe > 0)
					nextKeyframe--;
			}
		}

		if (forward && currentFrameNumber > totalNumFrames)
		{
			if (loops)
			{
				currentFrameNumber = 0;
				currentKeyframe = 0;
				nextKeyframe = 1;
				t = 0;
			}
		}
		else if (!forward && currentFrameNumber < 0)
		{
			if (loops)
			{
				currentFrameNumber = totalNumFrames + 1;
				currentKeyframe = keys.size() - 2;
				nextKeyframe = keys.size() - 1;
			}
			else
				return MathHelper::LERP(keys[0].keyValue, keys[1].keyValue, 0.0f);
		}

		return MathHelper::LERP<T>(p0, p1, t);
	}
	else
	{
		if (forward)
			return MathHelper::LERP<T>(keys[currentKeyframe].keyValue, keys[nextKeyframe].keyValue, t);
		else
			return MathHelper::LERP<T>(keys[nextKeyframe].keyValue, keys[currentKeyframe].keyValue, t);
	}
}

#endif
