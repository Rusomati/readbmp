#include <stdio.h>
#include <string.h>
#include <stdlib.h>
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
	int filesize=0;
	if(fp == NULL)
	{
		fprintf(stderr,"cannot open file");
		return -1;
	}
	unsigned char headbuffer[6]={0};

	fread(headbuffer,sizeof(headbuffer[0]),6,fp);
	//for(int i=0;i<6;i++)printf("%d\n",headbuffer[i]);
	strncpy(a,headbuffer+2,4);//skips the first 2 bytes then reads 4 bytes from buffer then puts them into "a"
	filesize = bytetoint(a,4);
	unsigned char *buffer = malloc(sizeof(char) * filesize);
	fread(buffer,sizeof(buffer[0]),(filesize-6),fp);
	//for(int i=0;i<6;i++)printf("%d\n",buffer[i]);
	//from here the code "byte_addres=byte_addres-6"

	strncpy(a,buffer+4,4);//gets the starting address of the pizels
	printf("%d\n",bytetoint(a,4));
	strncpy(a,buffer+12,4);//gets the width of the image
	printf("%d\n",bytetoint(a,4));
	strncpy(a,buffer+16,4);//gets the height of the image//wrong according to the wiki
	printf("%d\n",bytetoint(a,4));

	free(buffer);
	buffer = NULL;
	fclose(fp);
	return 0;
}
