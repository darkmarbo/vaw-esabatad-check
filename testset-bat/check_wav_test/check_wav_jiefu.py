import sys
import os
import string


def check_dir(dir_name,value,out_file):
	list_file = os.listdir(dir_name);
	for line in list_file:
		path_tmp = "%s\\%s"%(dir_name,line);
		if os.path.isdir(path_tmp):
			check_dir(path_tmp,value,out_file);
		else:
			if path_tmp[-4:] == ".wav" or path_tmp[-4:] == ".WAV":
				print(path_tmp);
				cmd="wav_check.exe 3 \"%s\" %d %s "%(path_tmp, value, out_file);
				#print(cmd)
				os.system(cmd);
	return 0;

if len(sys.argv) < 4:
    print("usage: %s wav_dir 32000 out.log \n"%(sys.argv[0]));
    sys.exit(0)

#fp_log=open("check_wav_jiefu.log","w");
#fp_log.write("jiefu :\n");
#fp_log.close()

dir_root=sys.argv[1];
value=int(sys.argv[2])
out_log=sys.argv[3]
check_dir(dir_root,value,out_log);
