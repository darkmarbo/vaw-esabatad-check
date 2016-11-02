import sys
import os
import string

def check_dir(dir_name,value_len,value_del,out_log):
	list_file = os.listdir(dir_name);
	for line in list_file:
		path_tmp = "%s\\%s"%(dir_name,line);
		if os.path.isdir(path_tmp):
			check_dir(path_tmp,value_len,value_del,out_log);
		else:
			if path_tmp[-4:] == ".wav" or path_tmp[-4:] == ".WAV":
				print(path_tmp);
				cmd="wav_check.exe 2 %s %d %d %s "%(path_tmp, value_len, value_del,out_log);
				os.system(cmd);
	return 0;

if len(sys.argv) < 2:
    print("usage: %s wav_dir \n"%(sys.argv[0]));
    sys.exit(0)

#fp_log=open("check_wav_eq.log","w");
#fp_log.write("continuous  0 value \n");
#fp_log.close()

dir_root=sys.argv[1]
value_len=int(sys.argv[2])
value_del=int(sys.argv[3])
out_log=sys.argv[4]

check_dir(dir_root,value_len,value_del,out_log);


