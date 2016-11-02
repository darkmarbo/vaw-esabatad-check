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
	if not os.path.isdir(spk_1):
		continue;
		
	list_spk_1 = os.listdir(spk_1);
	list_spk_2 = os.listdir(spk_2);
	for wav_1 in list_spk_1:
		if wav_1 not in list_spk_2:
			print ("%s not in %s"%(wav_1, spk_2))
			continue;
		temp_1 = "%s\\%s"%(spk_1,wav_1)
		temp_2 = "%s\\%s"%(spk_2,wav_1)
		if not os.path.isdir(temp_1):
			continue;
		
		wav_name_1 = os.listdir(temp_1);
		wav_name_2 = os.listdir(temp_2);
		for name_1 in wav_name_1:
			name_1_tmp = name_1[-10:]
			for name_2_tmp in wav_name_2:
				if name_2_tmp[-10:] == name_1_tmp:
					name_1 = "%s\\%s"%(temp_1,name_1)
					name_2 = "%s\\%s"%(temp_2,name_2_tmp)
					if name_1[-4:] == ".wav" or name_1[-4:] == ".WAV":
						print("start...:%s\t%s"%(name_1, name_2));
						cmd="wav_check.exe 1 %s %s %s "%(name_1, name_2, sys.argv[3])
						os.system(cmd);
						break;
					else:
						print ("%s!!wav"%(wav_1))
						
		
		
		
		
		
		
		
