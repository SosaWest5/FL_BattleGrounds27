#pragma once
#include <array>
#include <vector>
#include <string>
#include <cmath>
#include <algorithm>
#include <random>
namespace battle {
struct MC {const char* name;int overall,flow,humor,attack;unsigned colour;const char* style;std::array<const char*,6> bars;};
inline const std::array<MC,4> mcs{{
 {"Peace-maker",88,87,84,92,0x59dfbe,"CALM VOICE. PETTY INTENT.",{{
 "Peace-maker? I make peace with your career being dead.",
 "Your barber took your hairline and your label took your bread.",
 "You dress like unpaid rent with a chain from aisle nine.",
 "Your hardest fucking bar was a password you couldn't find.",
 "You call yourself a boss, but your mom still pays the Wi-Fi.",
 "Your whole crew said you're fire. They were praying you would retire."}}},
 {"Sunlyt",94,97,82,94,0xa8a0ff,"TECHNICAL FLOW. COLD PUNCHLINES.",{{
 "I'm Sunlyt. You're a porch light with a past-due bill.",
 "Your bars got no signal. Even airplane mode has skill.",
 "You rap in circles. Google Maps keeps saying turn around.",
 "Your album went double cardboard. Nobody heard a sound.",
 "I put your ego in a box and made the postage cheap.",
 "Your flow's a bedtime story. Even your ad-libs fell asleep."}}},
 {"Plenty",90,89,98,84,0xffca69,"CROWD COMEDIAN. NONSTOP ROASTS.",{{
 "You got plenty of excuses and a shortage of talent.",
 "Your chain turned your neck green. That's a salad, not a pendant.",
 "Your fit looks like a couch that somebody left outside.",
 "Your mixtape's so damn trash, the garbage truck declined.",
 "You flex a rented whip. The payment's due at seven.",
 "Your fans are all your cousins. Two unfollowed. Now it's eleven."}}},
 {"BASHH",92,93,76,98,0xff6389,"HEAVY DELIVERY. ZERO MERCY.",{{
 "BASHH in the building. Your tough-guy act's a rental.",
 "You bark like a pit bull, then fold like a pretzel.",
 "Your crew's all yes-men. They clap when you sneeze.",
 "You couldn't move a crowd with a fire drill, please.",
 "You got a big mouth and a bargain-bin flow.",
 "I left your pride backstage. Even lost and found said no."}}}
}};
struct Note {double at;int lane,side;bool judged=false;};
struct Battle {
 int home=0,away=1,difficulty=1;double time=0;bool finished=false;std::array<int,2> points{{0,0}},combo{{0,0}},best{{0,0}},hits{{0,0}},misses{{0,0}};
 std::vector<Note> notes;std::string feedback="GET READY";double feedbackUntil=1;int flashLane=-1;double flashUntil=0;std::mt19937 rng{27};
 int activeSide()const{return std::min(5,int(time/5))%2;} int activeMC()const{return activeSide()==0?home:away;} int barIndex()const{int phrase=std::min(11,int(time/2.5));return (phrase/4)*2+phrase%2;}
 double crowd()const{int total=points[0]+points[1];return total?double(points[0])/total:.5;}
 double random(){return std::uniform_real_distribution<double>(0,1)(rng);}
 void start(int h,int a,int d,unsigned seed=27){*this=Battle{};home=std::clamp(h,0,3);away=std::clamp(a,0,3);difficulty=std::clamp(d,0,2);rng.seed(seed);
  // Identical note density on both sides: skill ratings affect AI accuracy, not opportunity count.
  for(int turn=0;turn<6;++turn){int side=turn%2,mc=side?away:home;for(int n=0;n<8;++n){double at=turn*5+.5+n*.5;notes.push_back({at,(n*3+turn/2+mc)%4,side,false});}
   if(difficulty>=1)for(int n:{2,5})notes.push_back({turn*5+.75+n*.5,(n+mc+turn/2)%4,side,false});
   if(difficulty==2)for(int n:{0,3,6})notes.push_back({turn*5+.75+n*.5,(n+1+mc)%4,side,false});}
  std::sort(notes.begin(),notes.end(),[](auto a,auto b){return a.at<b.at;});
 }
 void award(Note& n,int quality){n.judged=true;int s=n.side;if(quality<=0){++misses[size_t(s)];combo[size_t(s)]=0;if(s==0){feedback="MISS / STREAK LOST";feedbackUntil=time+.55;}return;}
  ++hits[size_t(s)];++combo[size_t(s)];best[size_t(s)]=std::max(best[size_t(s)],combo[size_t(s)]);int multiplier=1+std::min(3,combo[size_t(s)]/8);int mc=s==0?home:away;
  int punch=(mcs[size_t(mc)].humor+mcs[size_t(mc)].attack)/20;points[size_t(s)]+=(quality+punch)*multiplier;
  if(s==0){feedback=quality==100?"PERFECT FLOW":quality==75?"CLEAN BAR":"ON BEAT";feedbackUntil=time+.6;flashLane=n.lane;flashUntil=time+.16;}
 }
 void update(double t){if(finished)return;time=std::clamp(t,time,30.);for(auto& n:notes){if(n.judged)continue;if(n.side==1&&time>=n.at){double skill=mcs[size_t(away)].flow/100.;double probability=std::clamp(.72+(skill-.87)*.7+difficulty*.045,.65,.90);if(random()>probability)award(n,0);else award(n,random()<.52?100:75);}else if(n.side==0&&time>n.at+.12)award(n,0);}
  if(time>=30){for(auto& n:notes)if(!n.judged)award(n,0);finished=true;feedback=points[0]==points[1]?"DRAW / RUN IT BACK":points[0]>points[1]?"YOU OWN THE CROWD":"OPPONENT TAKES THE CROWD";feedbackUntil=99;}
 }
 bool press(int lane,double t){update(t);if(finished||activeSide()!=0||lane<0||lane>3)return false;Note* closest=nullptr;double error=.121;for(auto& n:notes)if(!n.judged&&n.side==0&&n.lane==lane){double e=std::abs(n.at-time);if(e<error){error=e;closest=&n;}}
  if(closest){award(*closest,error<=.045?100:error<=.085?75:45);return true;}
  combo[0]=0;points[0]=std::max(0,points[0]-25);feedback="OFF BEAT / -25";feedbackUntil=time+.5;return false;
 }
};
}
