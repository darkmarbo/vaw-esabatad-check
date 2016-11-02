## 检测这两个目录下的对应文件（两个通道）是否是复制而来的。
#check_wav.py E:\SamsungS5 E:\iPhone6 D:\check_wav_new.log
#check_wav.py E:\King-ASR-256\data\Channel0\Wave E:\King-ASR-256\data\Channel1\Wave 256_chl.log 
#check_wav.py E:\King-ASR-383\DATA 4 check_wav_new.log

### 连续200ms的相同幅度采样点 不考虑首尾的500ms，
#check_wav_eq.py E:\desktop 200 500 D:\check_eq.log 


## 语音截幅
check_wav_jiefu.py test2 32000 jiefu.log 
pause