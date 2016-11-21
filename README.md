# Octopus

Octopus is a modern C++ library for embedding digital signal processing as a language inside your software. It transcends single domains (e.g. audio, video, math, etc.), combining signals of different domains and clocks in the same processing graph. Liboctopus makes heavy use of C++ operator overloading where it makes sense (arithmetic and mono-stereo operations).

Other features of Octopus include:

 - A syntax as close to DSP mathematics as possible
 - Other signal types besides real numbers (booleans, bitmaps or even your own classes)
 - Different clocks with different sample rates in the same graph
 - Extensibility: write your own custom signals

## Examples

```
// Create a clock that represents the audio clock running at 44100 Hertz
InvariableClock audio(44100);

// Create a sine oscillator that outputs floats and uses the audio clock
Sine<float> oscillator(audio);

// Set the frequency of the oscillator to 440 + (100 * an lfo at 0.5 Hertz)
// In other words, set the oscillator to vibrate between 340 and 540 once per 2 seconds
oscillator.frequency = 440 + 100 * Sine<float>(audio, 0.5);

do
{
  // Output the current sample of the oscillator (= z^0)
  cout << oscillator() << endl;
  
// Move the audio clock to its next frame each iteration, up to its hundredth
} while (audio.tick() < 100);
```
```
// Create the sine signal used above
class Sine : public Signal<float>
{
public:
	// Take a clock in the constructor and pass it to the base class
	MySignal(Clock& clock) :
		Signal<float>(clock),
		frequency(clock)
	{

	}
    
	// Create a Value signal for the frequency to this signal
	// Values can be set to a constant float value, or refer to another signal
	Value<float> frequency;
	
	// Utility macro necessary for each signal
	GENERATE_MOVE(MySignal)
    
private:
	// This function will be called whenever a new sample is needed
	void generateSample(T& out) final override
	{
		out = std::sin(phase * 6.28318530717959);

		// Increment and wrap the phase
		phase += this->getClock().delta() * frequency();
		while (phase >= 1.0)
			phase -= 1.0;
	}

	long double phase = 0;
};
```

## Platforms

Octopus should work with any compiler on any platform that supports modern C++ (17). It contains no platform-dependent code.
