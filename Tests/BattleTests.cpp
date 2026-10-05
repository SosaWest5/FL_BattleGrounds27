#include "../Source/Battle.h"
#include <iostream>
#include <stdexcept>
void check(bool b,const char* s){if(!b)throw std::runtime_error(s);}
int main(){using namespace battle;Battle b;b.start(0,1,1);check(b.notes.size()==60,"normal chart");check(b.crowd()==.5,"neutral crowd");
 auto first=b.notes[0];check(b.press(first.lane,first.at),"perfect hit");check(b.points[0]>100&&b.combo[0]==1,"points and streak");int p=b.points[0];b.press(first.lane,first.at);check(b.points[0]==p-25,"duplicate tap penalty");
 b.update(5);check(b.activeSide()==1,"alternating turn");p=b.points[0];check(!b.press(0,5.1)&&b.points[0]==p,"opponent keys ignored");b.update(30);check(b.finished,"30 second end");check(b.hits[1]+b.misses[1]==30,"all AI notes scored");
 for(int h=0;h<4;++h)for(int a=0;a<4;++a)for(int d=0;d<3;++d){b.start(h,a,d);auto chart=b.notes;for(auto n:chart){if(n.side==0)b.press(n.lane,n.at);else b.update(n.at);}b.update(30);check(b.finished&&b.misses[0]==0,"perfect chart");check(b.hits[0]==int(chart.size()/2),"all player notes");check(b.points[0]>b.points[1],"perfect play wins");check(b.crowd()>=0&&b.crowd()<=1,"crowd bounds");check(b.barIndex()==5,"final verse");}
 b.start(3,2,0);b.update(30);check(b.points[0]==0&&b.misses[0]==24,"idle misses");std::cout<<"Battle timing, scoring and 48 matchup checks passed\n";
}
