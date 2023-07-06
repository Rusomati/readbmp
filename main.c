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
void getpixel(unsigned char *buffer,int firstpixel,int width,int height,int x,int y)
{
	y = height - y;//optimisable?
	int rowsize=width+(width%4);//some magick because rows are padded
	//pixels start from the bottom left or smth
	int bytetoread = (3 * ((rowsize * y)+x))+firstpixel;
	for(int i=0;i<rowsize*height;i++)
	{
	bytetoread = (3*i)+firstpixel;
	//printf("index:\t\t%d\n",bytetoread-6);//?
	printf("\033[48;2;%d;%d;%dm  \033[0m",buffer[bytetoread-4],buffer[bytetoread-5],buffer[bytetoread-6]);
	if(!(i%rowsize))printf("\n");
	}
}
int main()
{
	unsigned char tempbytes[4]={0};
	FILE* fp = fopen("0083.bmp","rb");
	int filesize=0;
	if(fp == NULL)
	{
		fprintf(stderr,"cannot open file");
		return -1;
	}
	unsigned char headbuffer[6]={0};

	fread(headbuffer,sizeof(headbuffer[0]),6,fp);
	strncpy(tempbytes,headbuffer+2,4);//skips the first 2 bytes then reads 4 bytes from buffer then puts them into "a"
	filesize = bytetoint(tempbytes,4);
	unsigned char *buffer = malloc(sizeof(char) * filesize);
	fread(buffer,sizeof(buffer[0]),(filesize-6),fp);
	//from here the code "byte_addres=byte_addres-6"

	strncpy(tempbytes,buffer+4,4);
	int firstpixel = bytetoint(tempbytes,4);
	//the starting address of the pixels
	//should i add "-6" at the end?
	//can i optimise this mess?
	strncpy(tempbytes,buffer+12,4);//the width of the image
	int width = bytetoint(tempbytes,4);
	strncpy(tempbytes,buffer+16,4);//the height of the image
	int height = bytetoint(tempbytes,4);
	//printf("filesize:\t%d\n",filesize);//?
	getpixel(buffer,firstpixel,width,height,0,0);//-6 here? can i mix the first 2 arguments then add -6?

	free(buffer);
	buffer = NULL;
	fclose(fp);
	return 0;
}
