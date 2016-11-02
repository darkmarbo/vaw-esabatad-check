import sys
import os
import string

if len(sys.argv) < 4:
	print("usage: %s data_dir channel_num out_log\n"%(sys.argv[0]));
	sys.exit(0)

gl_ch="CHANNEL"
gl_wv="WAVE"
gl_sp="SPEAKER"
gl_ss="SESSION"

data=sys.argv[1];
ch_num=int(sys.argv[2]);
log_name=sys.argv[3];


for ch in range(0,ch_num):
	ch_dir="%s\\%s%d"%(data,gl_ch,ch);
	if not os.path.isdir(ch_dir):
		print("%s not is dir!\n"%(ch_dir));
		continue;

##########################   CHANNEL  
for ch_fs in range(0,ch_num-1):
	ch_fs_dir="%s\\%s%d\\%s"%(data, gl_ch, ch_fs,gl_wv);
	if not os.path.isdir(ch_fs_dir):
		print("%s not is dir!\n"%(ch_fs_dir));
		continue;
	
	for ch_ls in range(ch_fs+1, ch_num):
		ch_ls_dir="%s\\%s%d\\%s"%(data, gl_ch, ch_ls, gl_wv);
		if not os.path.isdir(ch_ls_dir):
			print("%s not is dir!\n"%(ch_ls_dir));
			continue;
		#print ("check:\t%s -> %s\n"%(ch_fs_dir,ch_ls_dir));
		
		##############################  SPEAKER  
		sp_name_list = os.listdir(ch_fs_dir);
		for sp_name in sp_name_list:
			sp_dir_1 = "%s\\%s"%(ch_fs_dir,sp_name);
			sp_dir_2 = "%s\\%s"%(ch_ls_dir,sp_name);
			if not os.path.isdir(sp_dir_1) or not os.path.isdir(sp_dir_2):
				print("%s not is dir!\n"%(sp_dir_1));
				continue;
				
			#print("test:\t%s\t%s\n"%(sp_dir_1, sp_dir_2));	
			
			#######################   SESSION 
			if "SPEAKER0720" ==  sp_name:
				continue;
				
			ss_name_list = os.listdir(sp_dir_1);
			for ss_name in ss_name_list:
				ss_dir_1 = "%s\\%s"%(sp_dir_1,ss_name);
				ss_dir_2 = "%s\\%s"%(sp_dir_2,ss_name);
				if not os.path.isdir(ss_dir_1) or not os.path.isdir(ss_dir_2):
					print("%s not is dir!\n"%(ss_dir_1));
					continue;
				
				#print("test:\t%s\t%s\n"%(ss_dir_1, ss_dir_2));
			#######################   WAV 
				wv_name_list = os.listdir(ss_dir_1);
				wv_name_list_2 = os.listdir(ss_dir_2);
				for wv_name in wv_name_list:
					nm_tmp = wv_name[-8:];
					wv_1 = "%s\\%s"%(ss_dir_1, wv_name);
					for wv_name_2 in wv_name_list_2:
						if nm_tmp == wv_name_2[-8:]:
							wv_2 = "%s\\%s"%(ss_dir_2, wv_name_2);
							if wv_1[-4:] == ".wav" or wv_1[-4:] == ".WAV":
								print("test:\t%s\t%s\n"%(wv_1, wv_2));
								cmd="wav_check.exe 1 %s %s %s "%(wv_1, wv_2, log_name)
								os.system(cmd);
								break;
								
								
								
								
								
								
								
								
								
								
								
								
								
								
								
								
								
								
								
								
								
								
								
								
								
								
								
								
					
