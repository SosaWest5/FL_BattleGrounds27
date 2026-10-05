#pragma once
#include "PluginProcessor.h"
#include "Battle.h"
#include <juce_gui_extra/juce_gui_extra.h>
class BattleEditor final:public juce::AudioProcessorEditor,private juce::Timer{
public:
 explicit BattleEditor(BattleProcessor&);~BattleEditor()override{stopTimer();processor.stop();}
 void paint(juce::Graphics&)override;void resized()override;bool keyPressed(const juce::KeyPress&)override;bool keyStateChanged(bool)override;void focusLost(FocusChangeType)override;void mouseDown(const juce::MouseEvent&)override;
 void startBattle();void advanceTo(double t){game.update(t);} // deterministic native smoke/preview entry
private:
 BattleProcessor& processor;battle::Battle game;bool menu=true;std::array<bool,4> held{{false,false,false,false}};int lastCombo=0;
 juce::ComboBox opponent,difficulty;std::array<juce::TextButton,4> picks;
 juce::TextButton start{"START BATTLE"},back{"CHOOSE MC"},pause{"PAUSE"};juce::ToggleButton vocals{"Vocals"};juce::Slider volume;
 void timerCallback()override;void syncVisibility();void press(int);void stage(juce::Graphics&,juce::Rectangle<float>,bool);void rapper(juce::Graphics&,float,float,float,int,bool,double);void drawGame(juce::Graphics&);
 JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(BattleEditor)
};
