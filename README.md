# Octopus

Octopus is a modern C++ library for embedding digital signal processing as a language inside your software. It transcends single domains (e.g. audio, video, math, etc.), combining signals of different domains and clocks in the same processing graph. Liboctopus makes heavy use of C++ operator overloading where it makes sense (arithmetic and mono-stereo operations).

Other features of Octopus include:

 - A syntax as close to DSP mathematics as possible
 - Other signal types besides real numbers (booleans, bitmaps or even your own classes)
 - Different clocks with different sample rates in the same graph
 - Extensibility: write your own custom signals
 
***Octopus is written in c++17. Make sure to add the -std=c++1z flag to your compiler!***

## Examples

```
#include <cmath>
#include <iostream>
#include <octopus/Octopus.hpp>

using namespace octo;

// Create the sine signal used above
class Sine : public Signal<float>
{
public:
	// Take a clock in the constructor and pass it to the base class
	Sine(Clock& clock, float frequency = 0.0f) :
	     Signal<float>(clock),
	     frequency(clock, frequency)
	{

	}
    
	// Create a Value signal for the frequency to this signal
	// Values can be set to a constant float value, or refer to another signal
	Value<float> frequency;
	
	// Utility macro necessary for each signal
	GENERATE_MOVE(Sine)
    
private:
	// This function will be called whenever a new sample is needed
	void generateSample(float& out) final override
	{
		out = std::sin(phase * 6.28318530717959);

		// Increment and wrap the phase
		phase += this->getClock().delta() * frequency();
		while (phase >= 1.0)
			phase -= 1.0;
	}
	
	// This function is called when the clock on the signal changes
	void clockChanged(octo::Clock& clock) final override
	{
		frequency.setClock(clock);
	}

	long double phase = 0;
};

int main(int argc, char* argv[])
{
	// Create a clock that represents the audio clock running at 44100 Hertz
	InvariableClock audio(44100);

	// Create a sine oscillator that outputs floats and uses the audio clock
	Sine oscillator(audio);

	// Set the frequency of the oscillator to 440 + (100 * an lfo at 0.5 Hertz)
	// In other words, set the oscillator to vibrate between 340 and 540 once per 2 seconds
	oscillator.frequency = 440.0f + 100.0f * Sine(audio, 0.5f);

	do
	{
	  // Output the current sample of the oscillator (= z^0)
	  std::cout << oscillator() << std::endl;

	// Move the audio clock to its next frame each iteration, up to its hundredth
	} while (audio.tick() < 100);
	
	return 0;
}
```


## Installation

A [Homebrew](http://brew.sh) and [LinuxBrew](http://linuxbrew.sh) formula exists for macOS and Linux respectively.

```
brew tap dsperados/tap
brew install octopus
```

Octopus can also be built manually, by running the CMake script in the root.

```
mkdir build
cd build
cmake ..
make
sudo make install
```

This library is written in c++17. Make sure you have the **latest version** of your compiler (on macOS this would be **Xcode 7** or higher), and add the **-std=c++1z** flag to your compiler!

## Documentation

A Doxygen generated documentation can be found [here](http://api.dsperados.com/octopus).

## Platforms

Octopus should work with any compiler on any platform that supports modern C++ (17). It contains no platform-dependent code.

## License

The entire Bear Suite, including BearAudio is licensed under GPLv3. Commercial licensing is available upon request.
