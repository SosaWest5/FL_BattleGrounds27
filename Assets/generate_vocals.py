# Build-time asset regeneration only; the finished plugin embeds the WAV files.
import subprocess, pathlib, re, wave
root=pathlib.Path(__file__).resolve().parent
text=(root.parent/'Source/Battle.h').read_text()
bars=re.findall(r'^ "(.*)"[,}]',text,re.M)
assert len(bars)==24,len(bars)
for i,bar in enumerate(bars):
 mc,line=divmod(i,6)
 raw=root/'raw.wav';out=root/f'mc{mc}_{line}.wav'
 subprocess.run(['espeak-ng','-v','en-us','-s',str([225,245,230,220][mc]),'-p',str([42,55,65,30][mc]),'-w',str(raw),bar],check=True)
 with wave.open(str(raw)) as w:duration=w.getnframes()/w.getframerate()
 speed=max(.5,min(2.,duration/2.23))
 subprocess.run(['ffmpeg','-y','-loglevel','error','-i',str(raw),'-af',f'atempo={speed},apad,atrim=duration=2.35,afade=t=out:st=2.25:d=0.10','-ar','22050','-ac','1',str(out)],check=True)
raw.unlink()
print('Generated 24 original synthetic vocal clips')
