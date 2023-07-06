#include <stdio.h>
#include <string.h>
//gets 256^x
int getprefix(int power)
{
	int res = 1;
	if (power == 0)return 1;
	for (int i=0;i<power;i++)
	{
		res*=256;
	}
	return res;
}
//converts an array of bytes to an integer//i bet that an easier way exists but whatever
int bytetoint(unsigned char *bytes,int length)
{
	int res = 0;
	for (int i=(length-1);i>=0;i--)
	{
		res += (bytes[i]*getprefix(i));
	}
	return res;
}
int main()
{
	unsigned char a[128]={0};//for testing
	FILE* fp = fopen("0083.bmp","rb");
	if(fp == NULL)
	{
		fprintf(stderr,"cannot open file");
		return -1;
	}
	unsigned char buffer[14]={0};

	fread(buffer,sizeof(buffer[0]),14,fp);
	for(int i=0;i<14;i++)printf("%d\n",buffer[i]);
	strncpy(a,buffer+2,4);//skips the first 2 bytes then reads 4 bytes from buffer then puts them into "a"
	printf("%d\n",bytetoint(a,4));

	fclose(fp);
	return 0;
}
