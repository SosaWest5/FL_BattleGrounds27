#pragma once
#include <juce_audio_processors/juce_audio_processors.h>
#include <atomic>
#include <array>
class BattleProcessor final:public juce::AudioProcessor {
public:
 BattleProcessor();const juce::String getName()const override{return "FL_BattleGrounds27";}
 void prepareToPlay(double,int)override;void releaseResources()override{}
 bool isBusesLayoutSupported(const BusesLayout&)const override;void processBlock(juce::AudioBuffer<float>&,juce::MidiBuffer&)override;
 juce::AudioProcessorEditor* createEditor()override;bool hasEditor()const override{return true;}
 bool acceptsMidi()const override{return false;}bool producesMidi()const override{return false;}bool isMidiEffect()const override{return false;}
 double getTailLengthSeconds()const override{return 0;}int getNumPrograms()override{return 1;}int getCurrentProgram()override{return 0;}void setCurrentProgram(int)override{}
 const juce::String getProgramName(int)override{return {};}void changeProgramName(int,const juce::String&)override{}
 void getStateInformation(juce::MemoryBlock&)override;void setStateInformation(const void*,int)override;
 bool hasAllVoices()const{for(const auto& c:clips)if(c.getNumSamples()==0)return false;return true;}
 void begin();void pause(bool);void stop();double battleTime()const;bool isPaused()const{return paused.load();}void cue(){cheers.fetch_add(1);}
 std::atomic<int> home{0},away{1},difficulty{1};std::atomic<float> volume{.65f},vocalGain{1.f};std::atomic<bool> vocals{true};
private:
 std::atomic<bool> running{false},paused{false};std::atomic<double> epoch{0},frozen{0};std::atomic<unsigned> cheers{0};
 double rate=44100;int cheerSamples=0;std::array<juce::AudioBuffer<float>,24> clips;
 JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(BattleProcessor)
};
