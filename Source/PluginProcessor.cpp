#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "BinaryData.h"
#include <juce_audio_formats/juce_audio_formats.h>
namespace{constexpr double pi=3.141592653589793;double noise(uint32_t n){n^=n>>16;n*=0x7feb352d;n^=n>>15;n*=0x846ca68b;n^=n>>16;return double(n&65535)/32767.5-1.;}}
BattleProcessor::BattleProcessor():AudioProcessor(BusesProperties().withInput("Input",juce::AudioChannelSet::stereo(),true).withOutput("Output",juce::AudioChannelSet::stereo(),true)){
 juce::WavAudioFormat format;for(int mc=0;mc<4;++mc)for(int bar=0;bar<6;++bar){auto name="mc"+juce::String(mc)+"_"+juce::String(bar)+"_wav";int size=0;auto* bytes=BinaryData::getNamedResource(name.toRawUTF8(),size);
 if(bytes&&size>0){std::unique_ptr<juce::AudioFormatReader> reader(format.createReaderFor(new juce::MemoryInputStream(bytes,size,false),true));if(reader){auto& b=clips[size_t(mc*6+bar)];b.setSize(1,int(reader->lengthInSamples));reader->read(&b,0,b.getNumSamples(),0,true,false);}}}
}
void BattleProcessor::prepareToPlay(double r,int){rate=r;cheerSamples=0;}
bool BattleProcessor::isBusesLayoutSupported(const BusesLayout& b)const{auto o=b.getMainOutputChannelSet();return(o==juce::AudioChannelSet::mono()||o==juce::AudioChannelSet::stereo())&&b.getMainInputChannelSet()==o;}
void BattleProcessor::begin(){frozen=0;epoch=juce::Time::getMillisecondCounterHiRes();paused=false;running=true;vocalGain=1;}
double BattleProcessor::battleTime()const{if(!running.load()||paused.load())return frozen.load();return juce::jlimit(0.,30.,(juce::Time::getMillisecondCounterHiRes()-epoch.load())*.001);}
void BattleProcessor::pause(bool b){if(b){frozen=battleTime();paused=true;}else{epoch=juce::Time::getMillisecondCounterHiRes()-frozen.load()*1000.;paused=false;}}
void BattleProcessor::stop(){frozen=battleTime();running=false;cheers=0;}
void BattleProcessor::processBlock(juce::AudioBuffer<float>& b,juce::MidiBuffer& midi){juce::ScopedNoDenormals guard;midi.clear();if(!running.load()||paused.load())return;double start=battleTime();if(start>=30)return;
 if(cheers.exchange(0)>0)cheerSamples=int(rate*.15);float level=volume.load();bool speak=vocals.load();int h=home.load(),a=away.load();float humanGain=vocalGain.load();
 for(int i=0;i<b.getNumSamples();++i){double t=start+i/rate;if(t>=30)break;double beat=std::fmod(t,.5),hat=std::fmod(t,.25);int beatIndex=int(t*2);uint32_t sample=uint32_t(t*rate);double n=noise(sample);
 double kick=std::sin(2*pi*(62*beat-38*beat*beat))*std::exp(-beat*24)*.28;
 double snare=beatIndex%2?n*std::exp(-beat*32)*.13:0;double hats=n*std::exp(-hat*75)*.035;double frequencies[]={55,55,65.406,49};double bass=std::sin(2*pi*frequencies[(beatIndex/4)%4]*t)*.065*(1-std::min(1.,beat*1.3));
 double value=kick+snare+hats+bass;int phrase=std::min(11,int(t/2.5)),side=(phrase/2)%2,bar=(phrase/4)*2+phrase%2,mc=side?a:h;
 if(speak){const auto& clip=clips[size_t(mc*6+bar)];double pos=std::fmod(t,2.5)*22050.;int index=int(pos);if(index+1<clip.getNumSamples()){auto* x=clip.getReadPointer(0);double voice=x[index]+(x[index+1]-x[index])*(pos-index);value+=voice*.65*(side?1.:humanGain);}}
 if(cheerSamples>0){value+=n*.055*double(cheerSamples)/(rate*.15);--cheerSamples;}
 float out=float(value)*level;for(int c=0;c<b.getNumChannels();++c)b.addSample(c,i,out);
 }
}
juce::AudioProcessorEditor* BattleProcessor::createEditor(){return new BattleEditor(*this);}
void BattleProcessor::getStateInformation(juce::MemoryBlock& b){juce::XmlElement x("FLBattleGrounds27");x.setAttribute("home",home.load());x.setAttribute("away",away.load());x.setAttribute("difficulty",difficulty.load());x.setAttribute("volume",double(volume.load()));x.setAttribute("vocals",vocals.load());copyXmlToBinary(x,b);}
void BattleProcessor::setStateInformation(const void* d,int size){auto x=getXmlFromBinary(d,size);if(x&&x->hasTagName("FLBattleGrounds27")){home=juce::jlimit(0,3,x->getIntAttribute("home",0));away=juce::jlimit(0,3,x->getIntAttribute("away",1));difficulty=juce::jlimit(0,2,x->getIntAttribute("difficulty",1));volume=float(juce::jlimit(0.,1.,x->getDoubleAttribute("volume",.65)));vocals=x->getBoolAttribute("vocals",true);}}
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter(){return new BattleProcessor();}
