import sys
import os
import string

if len(sys.argv) < 4:
	print("usage: %s wav_dir_1 wav_dir_2 out_log\n"%(sys.argv[0]));
	sys.exit(0)


list_1 = os.listdir(sys.argv[1]);
list_2 = os.listdir(sys.argv[2]);

for dir_1 in list_1:
	if dir_1 not in list_2:
		print ("%s not in %s \n"%(dir_1,sys.argv[2]))
		continue;

		

	spk_1 = "%s\\%s"%(sys.argv[1], dir_1)
	spk_2 = "%s\\%s"%(sys.argv[2], dir_1)
	
	list_spk_1 = os.listdir(spk_1);
	list_spk_2 = os.listdir(spk_2);
	for wav_1 in list_spk_1:
		if wav_1 not in list_spk_2:
			print ("%s not in %s"%(wav_1, spk_2))
			continue;
		if wav_1[-4:] == ".wav" or wav_1[-4:] == ".WAV":
			name_1 = "%s\\%s"%(spk_1,wav_1)
			name_2 = "%s\\%s"%(spk_2,wav_1)
			print("start...:%s\t%s"%(name_1, name_2));
			cmd="wav_check.exe 1 %s %s %s "%(name_1, name_2, sys.argv[3])
			os.system(cmd);
		else:
			print ("%s!!"%(wav_1))
			continue;
		
		
		
		
		
		
		
