#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct color{
	unsigned char r;
	unsigned char g;
	unsigned char b;
} color;
typedef struct point{
	int x;
	int y;
}point;
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
color getpixel(unsigned char *buffer,int firstpixel,int width,int height,int x,int y,char *is_scanned)
{
	//*is_scanned=0;
	color res;
	//add error checking later?
	//pixels start from the bottom left or smth
	y = height - (y + 1);
	int rowsize = (width%4) ? width+(4-width%4) : width;//some magick because rows are padded
	int bytetoread = (3 * ((rowsize * y)+x))+firstpixel;
	res.r = buffer[bytetoread+2];
	res.g = buffer[bytetoread+1];
	res.b = buffer[bytetoread];

	*is_scanned = 1;
	return res;
}
//gets the bounderies as that makes mapping a bigger image to a smaller image easier
//and it has other uses for later
point getbounds(char is_large,int width,int height)
{
	point bounds;
	width -=1;
	height -=1;
	int baseborder = (is_large) ? 175 : 79;
	if (width <= baseborder  && height <= baseborder)
	{
		bounds.x=width;
		bounds.y=height;
	}
	else
	{
		int baseborderX = (width>baseborder) ? baseborder : width;
		int baseborderY = (height>baseborder) ? baseborder : height;
		
		if (width>=height)
		{
			bounds.x = baseborderX;
			bounds.y = (int)(baseborderY*height/width);
		}
		else
		{
			bounds.x = (int)(baseborderX*width/height);
			bounds.y = baseborderY;
		}
	}
	return bounds;
}
void draw(unsigned char *buffer,int firstpixel,int width,int height,char is_large)
{
	point bounds = getbounds(is_large,width,height);
	color pixel;
	char is_scanned;
	for (int x1=0,y1=0,x2,y2;y1<=bounds.y;y1++)
	{
		y2 = (int)(height*y1/bounds.y);
		for(x1=0;x1<=bounds.x;x1++)
		{
			x2 = (int)(width*x1/bounds.x);
			pixel = getpixel(buffer,firstpixel,width,height,x2,(height-1)-y2,&is_scanned);
			printf("draw color %d %d %d 255 0 0\n",pixel.r,pixel.g,pixel.b);
			printf("draw rect %d %d 1 1 0 0\n",x1,y1);
			if(!((y1*bounds.x+x1)%20)) printf("drawflush display1\n");
		}
	}
}
int main()
{
	char is_large=0;//canvas size
	unsigned char tempbytes[4]={0};
	//char temp[16]={0};//
	FILE* fp = fopen("cup.bmp","rb");
	int filesize=0;
	if(fp == NULL)
	{
		fprintf(stderr,"cannot open file.\n");
		return -1;
	}
	unsigned char headbuffer[6]={0};

	fread(headbuffer,sizeof(headbuffer[0]),6,fp);
	if(headbuffer[0]!='B'&&headbuffer[1]!='M')
	{
		fprintf(stderr,"invalid file format.\n");
		fclose(fp);
		return -1;
	}
	strncpy(tempbytes,headbuffer+2,4);//skips the first 2 bytes then reads 4 bytes from buffer then puts them into "a"
	filesize = bytetoint(tempbytes,4);
	unsigned char *buffer = malloc(sizeof(char) * filesize);
	fread(buffer,sizeof(buffer[0]),(filesize-6),fp);
	//from here the code "byte_addres=byte_addres-6"

	strncpy(tempbytes,buffer+4,4);
	int firstpixel = bytetoint(tempbytes,4)-6;
	//the starting address of the pixels ^
	strncpy(tempbytes,buffer+12,4);//the width of the image
	int width = bytetoint(tempbytes,4);
	strncpy(tempbytes,buffer+16,4);//the height of the image
	int height = bytetoint(tempbytes,4);

	//color test = getpixel(buffer,firstpixel,width,height,20,104,&temp[8]);
	//printf("%d %d %d %d\n",test.r,test.g,test.b,temp[8]);
	
	//point bounds = getbounds(is_large,width,height);
	//printf("%d %d\n",bounds.x,bounds.y);
	
	draw(buffer,firstpixel,width,height,is_large);

	free(buffer);
	buffer = NULL;
	fclose(fp);
	return 0;
}
