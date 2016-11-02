
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <math.h>
#include <vector>
#include <complex> 
#include <bitset> 
#include "wav_check.h" 

using namespace std;

const int CHECK_LEN = 200;  // 检测长度等于200ms   区间内采样点基本为恒定值
const double MIX_ERR = 0.1;  // 两个语音，相同位置采样点的差值范围
const double DL_0 = 0.00000001; // 判断是否为0的阈值
int JIEFU = 32000;


typedef struct _wavhead
{
	char            riff[4];            //"RIFF"
	unsigned long   filelong;           // +8 = File size
	char            wav[8];             //"WAVEfmt "
	unsigned long   t1;
	short           tag;
	short           channels;
	unsigned long   samplerate;
	unsigned long   typesps;
	unsigned short  psbytes;
	unsigned short  psbits;
	char            data[4];
	unsigned long   sumbytes;
}WAVEHEAD;



int main(int argc, char *argv[])
{
	if (argc < 3)
	{
		printf("usage: %s 1 wav_name_1	wav_name_2			log.txt \n 检查两个通道的语音是否是复制的！\n) ", argv[0]);
		printf("usage: %s 2 wav_name	check_len(200ms)	del_time(500ms) log.txt \n 检测这个语音连续200ms为同一值\n");
		printf("usage: %s 3 wav_name	32000				log.txt\n 有截幅语音\n");
		return 0;
	}


	int ret = 0;
	if (atoi(argv[1]) == 1)
	{
		FILE *fp_log = fopen(argv[4], "a+");
		if (!fp_log)
		{
			printf("open log %s err!\n",argv[4]);
			return 0;
		}
		ret = check_n(argv[2], argv[3]);
		if (ret == -10)
		{
			fprintf(fp_log, "%s\t%s\n", argv[2], argv[3]);
		}
		else if (ret < 0)
		{
			printf("read file err!\n");
			return 0;
		}
		fclose(fp_log);
	}
	else if( atoi(argv[1]) == 2 )
	{
		FILE *fp_log_eq = fopen(argv[5], "a+");
		if (!fp_log_eq)
		{
			printf("open log %s err!\n",argv[5]);
			return 0;
		}
		int check_len = atoi(argv[3]);
		int del_time = atoi(argv[4]);
		ret = check_eq(argv[2], check_len, del_time);
		if (ret == -10)
		{
			fprintf(fp_log_eq, "%s\n", argv[2]);
		}
		fclose(fp_log_eq);
	}
	else if (atoi(argv[1]) == 3)
	{
		FILE *fp_log_jiefu = fopen(argv[4], "a+");
		int jiefu_value = atoi(argv[3]);
		if (jiefu_value > 100)
		{
			JIEFU = jiefu_value;
		}

		ret = check_jiefu(argv[2]);
		if (ret == -10)
		{
			fprintf(fp_log_jiefu, "%s\n", argv[2]);
		}
		fclose(fp_log_jiefu);
	}

	return 0;
}
 
int check_jiefu(const char *file1)
{
	int sampleRate_1 = 0;
	int ret = 0;


	int len_wav_1 = ReadFileLength(file1, &sampleRate_1);
	//printf("wav_1_sample=%d\n", sampleRate_1);

	double time_wav_1 = double(len_wav_1) / double(sampleRate_1);
	//printf("wav_1=%fs\n", time_wav_1);

	short *data_wav1 = new short[len_wav_1];
	ret = ReadFile(file1, data_wav1, 0, len_wav_1);
	if (ret < 0)
	{
		printf("ReadFile:%s error!\n", file1);
		return 0;
	}
	
	short max = 0;
	short mix = 0;
	for (int ii = 0; ii < len_wav_1; ii++)
	{
		if (data_wav1[ii] > max) max = data_wav1[ii];
		if (data_wav1[ii] < mix) mix = data_wav1[ii];

	}
	//printf("%d\t%d\n", max, mix);	
	if (max > JIEFU || mix <0 -JIEFU)
	{
		return -10;
	}

	delete[] data_wav1;

	return 0;
}


// 检测 连续 chk_len 时间内，采样点取值相同。
int check_eq(const char *file1, int chk_len, int del_time)
{
	int sampleRate_1 = 0;
	int ret = 0;


	int len_wav_1 = ReadFileLength(file1, &sampleRate_1);
	//printf("wav_1_sample=%d\n", sampleRate_1);

	double time_wav_1 = double(len_wav_1) / double(sampleRate_1);
	//printf("wav_1=%fs\n", time_wav_1);

	short *data_wav1 = new short[len_wav_1];
	ret = ReadFile(file1, data_wav1, 0, len_wav_1);
	if (ret < 0)
	{
		printf("ReadFile:%s error!\n", file1);
		return 0;
	}

	int len_del = del_time * sampleRate_1 / 1000;
	if (len_wav_1 > 2*len_del)
	{
		len_wav_1 -= 2*len_del;
	}
	else
	{
		delete[] data_wav1;
		return 0;
	}
	data_wav1 += len_del;

	// 选取 默认 200ms 比对长度 
	int check_len = chk_len*sampleRate_1 / 1000;
	short *tmp = data_wav1;

	for (int ii = 0; ii < len_wav_1 - check_len; ii += 80)
	{
		ret = check_eq_sub(data_wav1 + ii, check_len - 1);
		if (ret == -10)
		{
			return -10;
		}
	}

	data_wav1 -= len_del;
	delete[] data_wav1;

	return 0;
}
/*
int check_eq(const char *file1, int chk_len)
{
	int sampleRate_1 = 0;
	int ret = 0;


	int len_wav_1 = ReadFileLength(file1, &sampleRate_1);
	//printf("wav_1_sample=%d\n", sampleRate_1);

	double time_wav_1 = double(len_wav_1) / double(sampleRate_1);
	//printf("wav_1=%fs\n", time_wav_1);

	short *data_wav1 = new short[len_wav_1];
	ret = ReadFile(file1, data_wav1, 0, len_wav_1);
	if (ret < 0)
	{
		printf("ReadFile:%s error!\n", file1);
		return 0;
	}

	// 选取 默认 200ms 比对长度 
	int check_len = chk_len*sampleRate_1 / 1000;
	short *tmp = data_wav1;

	for (int ii = 0; ii < len_wav_1 - check_len; ii += 30)
	{
		ret = check_eq_sub(data_wav1+ii, check_len-1);
		if (ret == -10)
		{
			return -10;
		}
	}


	delete[] data_wav1;
	
	return 0;
}
*/

/*
	连续一段时间内 为 同一个值 
*/
int check_eq_sub(const short *data, int size)
{
	short value = data[0];
	
	for (int ii = 0; ii < size; ii++)
	{
		if (value != data[ii])
		{
			return 0;
		}
	}

	return -10;
}


/*
// 判断整个数组，最大和最小值得差在一个范围内。

int check_eq_sub(const short *data, int size)
{
	int count = 0;
	short max = 0;
	short mix = 10000;
	for (int ii = 0; ii < size; ii++)
	{
		if (max < data[ii])
		{
			max = data[ii];
		}
		if (mix > data[ii])
		{
			mix = data[ii];
		}

	}

	if (abs(max - mix) > 21)
	{
		return -1;
	}

	return -10;
}
*/

 int check_n(const char *file1, const char *file2)
{
		int sampleRate_1 = 0;
		int sampleRate_2 = 0;
		int ret = 0;


		int len_wav_1 = ReadFileLength(file1, &sampleRate_1);
		int len_wav_2 = ReadFileLength(file2, &sampleRate_2);
		//printf("wav_1_sample=%d\twav_2_sample=%d\n", sampleRate_1,sampleRate_2);
		if (sampleRate_1 != sampleRate_2 )
		{
			printf("sampleRate not equel!\n");
			return -1;
		}

		double time_wav_1 = double(len_wav_1) / double(sampleRate_1);
		double time_wav_2 = double(len_wav_2) / double(sampleRate_2);
		//printf("wav_1=%fs\twav_2=%fs\n", time_wav_1, time_wav_2);

		short *data_wav1 = new short[len_wav_1];
		ret = ReadFile(file1, data_wav1, 0, len_wav_1);
		if (ret < 0)
		{
			printf("ReadFile:%s error!\n", file1);
			return 0;
		}

		short *data_wav2 = new short[len_wav_2];
		ret = ReadFile(file2, data_wav2, 0, len_wav_2);
		if (ret < 0)
		{
			printf("ReadFile:%s error!\n", file2);
			return 0;
		}

		int check_len = len_wav_1;
		if (len_wav_1 > len_wav_2)
		{
				check_len = len_wav_2; 
		}

		double err_1 = 1;
		for (int count_ii = 0; count_ii < check_len; count_ii ++)
		{
			if (data_wav1[count_ii] > 180 && data_wav2[count_ii]> 10)
			{
				err_1 = abs(double(data_wav2[count_ii]) / double(data_wav1[count_ii]));
				break;
			}
		}
		
		
		int count_err = 0;
		for (int ii = 1; ii < check_len; ii++)
		{
			double err_now = double(data_wav2[ii]) / double(data_wav1[ii]);
			if (abs(err_now - err_1)/err_1 < MIX_ERR)
			{
				count_err++;
			}
		}

		double result_v = double(count_err) / double(check_len - 1);
		printf("c1=%d\tc2=%d\tresult_v = %.4f\n", count_err, check_len, result_v);
		if (result_v > 0.5)
		{
			//printf("");
			return -10;
		}
		
		

		delete[] data_wav1;
		delete[] data_wav2;

		return 0;
}



float findLocalMaximum(float* score, int length)
{
	float maxiScore = 0;
	float maxiPos = 0;
	for (int i = 0; i<length; i++)
	{
		if (score[i]>maxiScore)
		{
			maxiScore = score[i];
			maxiPos = i;

		}
	}
	return maxiPos;
}

float findLocalMiximum(float* score, int length)
{
	float mixiScore = 100000000.0;
	float mixiPos = 0;
	for (int i = 0; i<length; i++)
	{
		if (score[i]<mixiScore)
		{
			mixiScore = score[i];
			mixiPos = i;

		}
	}
	return mixiPos;
}



// 从bias处开始读取 halfWindow 个short， 如果不够，返回-1。
int ReadFile(const char *wfile, short* allbuf, int bias, int halfWindow)
{
	bool oflag = false;
	FILE *fp = NULL;
	WAVEHEAD head;
	int SAMFREQ = -1;
	int sample_count = 0, channel_num = 0, readflag = 0;
	int numSample = 0; //读数据长度
	try
	{

		
			fp = fopen(wfile, "rb");
			if (fp == NULL) {
				return -1;
			}
			oflag = true;
			fseek(fp, 0, SEEK_END);
			sample_count = ftell(fp) - sizeof(WAVEHEAD);
			fseek(fp, 0, SEEK_SET);
			fread(&head, 1, sizeof(WAVEHEAD), fp);
			//data
			if (head.data[0] != 'd'&&head.data[1] != 'a'&&head.data[2] != 't'&&head.data[3] != 'a')
			{
				fclose(fp);
				return -1;
			}
			//RIFF
			if (head.riff[0] != 'R'&&head.riff[1] != 'I'&&head.riff[2] != 'F'&&head.riff[3] != 'F')
			{
				fclose(fp);
				return -1;
			}
			//"WAVEfmt "
			if (head.wav[0] != 'W'&&head.wav[1] != 'A'&&head.wav[2] != 'V'&&head.wav[3] != 'E'&&head.wav[4] != 'f'&&head.wav[5] != 'm'&&head.wav[6] != 't'&&head.wav[7] != ' ')
			{
				fclose(fp);
				return -1;
			}
			//定位数据
			fseek(fp, (long)(head.t1 - 16) - 4, SEEK_CUR);
			fread(&head.sumbytes, 1, sizeof(long), fp);
			//得到字节数
			sample_count = head.sumbytes;
			if (head.samplerate>48000 || head.samplerate<0)
			{
				fclose(fp);
				exit(-1);
			}
			SAMFREQ = head.samplerate;
			channel_num = head.channels;
		


		//得到样本数（n个通道样本数和，且为16bit）
		sample_count /= sizeof(short);
		if (sample_count % channel_num != 0) {
			fclose(fp);
			return -2;
		}
		// 分配空间读取数据
		// 从bias的开始读取 halfWindow 个short， 如果不够，返回-1。
		//printf("bias=%d\tsample_count=%d\thalfWindow=%d\n", bias, sample_count, halfWindow);
		if (bias + halfWindow <= sample_count)
		{
			numSample = halfWindow;
		}
		else
		{
			return -10;
		}
		//allbuf = (short*)malloc(numSample * sizeof(short));
		fseek(fp, bias*sizeof(short), SEEK_CUR);
		fread(allbuf, sizeof(short), numSample, fp);

		fclose(fp);
		oflag = false;
	}
	catch (...)
	{
		if (oflag)
			fclose(fp);

		if (allbuf)free(allbuf);
		allbuf = NULL;
		return -1;

	}
	return 0;
}



int ReadFileLength(const char *wfile, int* sampleRate)
{
	bool oflag = false;
	FILE *fp = NULL;
	WAVEHEAD head;
	int SAMFREQ = -1;
	int sample_count = 0, channel_num = 0, readflag = 0;
	int numSample = 0;//读数据长度
	try
	{
		//判断声音文件
		if (strstr(wfile, ".wav") || strstr(wfile, ".WAV")) 
		{
			fp = fopen(wfile, "rb");
			if (fp == NULL) {
				return -1;
			}
			oflag = true;
			fseek(fp, 0, SEEK_END);
			sample_count = ftell(fp) - sizeof(WAVEHEAD);
			fseek(fp, 0, SEEK_SET);
			fread(&head, 1, sizeof(WAVEHEAD), fp);
			//data
			if (head.data[0] != 'd'&&head.data[1] != 'a'&&head.data[2] != 't'&&head.data[3] != 'a')
			{
				fclose(fp);
				return -1;
			}
			//RIFF
			if (head.riff[0] != 'R'&&head.riff[1] != 'I'&&head.riff[2] != 'F'&&head.riff[3] != 'F')
			{
				fclose(fp);
				return -1;
			}
			//"WAVEfmt "
			if (head.wav[0] != 'W'&&head.wav[1] != 'A'&&head.wav[2] != 'V'&&head.wav[3] != 'E'&&head.wav[4] != 'f'&&head.wav[5] != 'm'&&head.wav[6] != 't'&&head.wav[7] != ' ')
			{
				fclose(fp);
				return -1;
			}
			//定位数据
			fseek(fp, (long)(head.t1 - 16) - 4, SEEK_CUR);
			fread(&head.sumbytes, 1, sizeof(long), fp);
			//得到字节数
			sample_count = head.sumbytes;
			if (head.samplerate>48000 || head.samplerate<0)
			{
				fclose(fp);
				exit(-1);
			}
			SAMFREQ = head.samplerate;
			channel_num = head.channels;

			*sampleRate = SAMFREQ;
		}
		else
		{
			printf("file name *.wav or *.WAV!\n");
			return -2;
		}

		//得到样本数（n个通道样本数和，且为16bit）
		sample_count /= sizeof(short);
		if (sample_count % channel_num != 0) {
			fclose(fp);
			return -2;
		}
		/*//分配空间读取数据
		if (bias+MAX<sample_count)
		{
		numSample = MAX;
		}
		else
		{
		numSample = sample_count-bias;
		}
		allbuf = (short*)malloc(numSample * sizeof(short));
		fread(allbuf, sizeof(short), numSample,fp+bias);
		fclose(fp);
		oflag=false;*/

		fclose(fp);
		return sample_count;
	}
	catch (...)
	{
		if (oflag)
			fclose(fp);

		/*if(allbuf)free(allbuf);
		allbuf=NULL;*/
		return -1;

	}

	fclose(fp);
	return 0;
}

