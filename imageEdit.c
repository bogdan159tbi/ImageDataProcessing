#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "bmp_header.h"
#define MAX_FILE 90
#define BMP_FILEHEADER 14
#define BMP_INFOHEADER 40



typedef struct
{
unsigned char *bgr;
}pixel;


void black_white(pixel**map,signed int height,signed int width,FILE *out,bmp_infoheader *info_header,bmp_fileheader *file_header)
{

//calcularea mediei dintre pixeli
for(int i=0;i<height;i++)
{
	for(int j=0;j<width;j++)
	{
	int sum=map[i][j].bgr[0]+map[i][j].bgr[1]+map[i][j].bgr[2];
	sum=sum/3;
	map[i][j].bgr[1]=sum;
	map[i][j].bgr[0]=sum;
	map[i][j].bgr[2]=sum;
	
	}
}

//scrierea
	fwrite(file_header,sizeof(bmp_fileheader),1,out);
	fwrite(info_header,sizeof(bmp_infoheader),1,out);
for(int i=0;i<height;i++)
	{
	for(int j=0;j<width;j++)
			{
			
			fwrite(&map[i][j].bgr[0],sizeof(unsigned char),1,out);
			fwrite(&map[i][j].bgr[1],sizeof(unsigned char),1,out);
			fwrite(&map[i][j].bgr[2],sizeof(unsigned char),1,out);
		    }
		unsigned char ignorePadd=0;
		for(int k=0;k<width%4;k++)
			fwrite(&ignorePadd,sizeof(unsigned char),1,out);
		
	}

}

void no_crop(pixel **map,signed int height,signed int width,FILE *out,bmp_fileheader *file_header,bmp_infoheader *info_header)
{
if(height==width)
{
	fwrite(file_header,sizeof(bmp_fileheader),1,out);
	fwrite(info_header,sizeof(bmp_infoheader),1,out);
	for(int i=0;i<height;i++)
	{
	for(int j=0;j<width;j++)
			{
			fwrite(&map[i][j].bgr[0],sizeof(unsigned char),1,out);
			fwrite(&map[i][j].bgr[1],sizeof(unsigned char),1,out);
			fwrite(&map[i][j].bgr[2],sizeof(unsigned char),1,out);
		    }
		unsigned char ignorePadd=0;
		for(int k=0;k<width%4;k++)
			fwrite(&ignorePadd,sizeof(unsigned char),1,out);
		
	}
}
if(height<width)
{
int difference=width-height;
int padding=width%4;
info_header->height=width;

	fwrite(file_header,sizeof(bmp_fileheader),1,out);
	fwrite(info_header,sizeof(bmp_infoheader),1,out);
	for(int i=0;i<difference/2;i++){
	for(int j=0;j<width;j++)
	{	unsigned pix_alb=255;
		fwrite(&pix_alb,sizeof(unsigned char),1,out);
		fwrite(&pix_alb,sizeof(unsigned char),1,out);	
		fwrite(&pix_alb,sizeof(unsigned char),1,out);
	}
	unsigned char ignorePadd=0;
	for(int k=0;k<width%4;k++)
		fwrite(&ignorePadd,sizeof(unsigned char),1,out);
}

	for(int i=0;i<height;i++)
	{
	for(int j=0;j<width;j++)
			{
			fwrite(&map[i][j].bgr[0],sizeof(unsigned char),1,out);
			fwrite(&map[i][j].bgr[1],sizeof(unsigned char),1,out);
			fwrite(&map[i][j].bgr[2],sizeof(unsigned char),1,out);
		    }
		unsigned char ignorePadd=0;
		for(int k=0;k<width%4;k++)
			fwrite(&ignorePadd,sizeof(unsigned char),1,out);
		
	}
	for(int i=0;i<difference/2;i++)
	{
	for(int j=0;j<width;j++)
	{	unsigned pix_alb=255;
		fwrite(&pix_alb,sizeof(unsigned char),1,out);
		fwrite(&pix_alb,sizeof(unsigned char),1,out);	
		fwrite(&pix_alb,sizeof(unsigned char),1,out);
	}
	unsigned char ignorePadd=0;
	for(int k=0;k<width%4;k++)
			fwrite(&ignorePadd,sizeof(unsigned char),1,out);
}

}
if(height>width)
{
int difference=height-width;
int padding=width%4;
info_header->width=height;
	fwrite(file_header,sizeof(bmp_fileheader),1,out);
	fwrite(info_header,sizeof(bmp_infoheader),1,out);
	for(int i=0;i<height;i++){
	for(int j=0;j<difference/2;j++)
	{	unsigned pix_alb=255;
		fwrite(&pix_alb,sizeof(unsigned char),1,out);
		fwrite(&pix_alb,sizeof(unsigned char),1,out);	
		fwrite(&pix_alb,sizeof(unsigned char),1,out);
	}
	for(int j=0;j<width;j++)
			{
			fwrite(&map[i][j].bgr[0],sizeof(unsigned char),1,out);
			fwrite(&map[i][j].bgr[1],sizeof(unsigned char),1,out);
			fwrite(&map[i][j].bgr[2],sizeof(unsigned char),1,out);
		    }
	for(int j=0;j<difference/2;j++)
	{	unsigned pix_alb=255;
		fwrite(&pix_alb,sizeof(unsigned char),1,out);
		fwrite(&pix_alb,sizeof(unsigned char),1,out);	
		fwrite(&pix_alb,sizeof(unsigned char),1,out);
	}
	unsigned char ignorePadd=0;
	for(int k=0;k<height%4;k++)
			fwrite(&ignorePadd,sizeof(unsigned char),1,out);
	
}

}

}

int ok(int x,int y,int height,int width)
{
	if(x<0 || x>=height ||y<0 ||y>=width)
		return 0;
	return 1;
}

unsigned  char maxim_red(pixel **map,int height,int width,int pooling,int pozX,int pozY)
{
unsigned char max_red=0;
	int i;
		for(i=pozX-pooling/2;i<=pozX+pooling/2;i++)
		  for(int j=pozY-pooling/2;j<=pozY+pooling/2;j++)
			{
				if((ok(i,j,height,width))&& (max_red<map[i][j].bgr[2]))
					max_red=map[i][j].bgr[2];
			}
	return max_red;
}
unsigned  char minim_red(pixel **map,int height,int width,int pooling,int pozX,int pozY)
{
unsigned char minim_red=255;
	int i;

		for(i=pozX-pooling/2;i<=pozX+pooling/2;i++)
			for(int j=pozY-pooling/2;j<=pozY+pooling/2;j++)
			{
				if((ok(i,j,height,width))&& (minim_red>map[i][j].bgr[2]))
					minim_red=map[i][j].bgr[2];
			}

	return minim_red;
	}
unsigned  char maxim_green(pixel **map,int height,int width,int pooling,int pozX,int pozY)
{
unsigned char max_green=0;
	int i=pozX;

		for(i=pozX-pooling/2;i<=pozX+pooling/2;i++)
			for(int j=pozY-pooling/2;j<=pozY+pooling/2;j++)
		 	{if(ok(i,j,height,width))
			{
				if(max_green<map[i][j].bgr[1])
					max_green=map[i][j].bgr[1];
			}
		}
	return max_green;
}
unsigned  char minim_green(pixel **map,int height,int width,int pooling,int pozX,int pozY)
{
unsigned char minim_green=255;
	int i=pozX;

		for(i=pozX-pooling/2;i<=pozX+pooling/2;i++)
			for(int j=pozY-pooling/2;j<=pozY+pooling/2;j++)
		 	{if(ok(i,j,height,width))
			{
				if(minim_green>map[i][j].bgr[1])
					minim_green=map[i][j].bgr[1];
			}
		}
	return minim_green;
}
unsigned char maxim_blue(pixel **map,int height,int width,int pooling,int pozX,int pozY)
{
unsigned char max_blue=0;

		for(int i=pozX-pooling/2;i<=pozX+pooling/2;i++)
			for(int j=pozY-pooling/2;j<=pozY+pooling/2;j++)
			if(ok(i,j,height,width))
			{
				if(max_blue<map[i][j].bgr[0])
					max_blue=map[i][j].bgr[0];
			}

	return max_blue;
}
unsigned char minim_blue(pixel **map,int height,int width,int pooling,int pozX,int pozY)
{
unsigned char minim_blue=255;

		for(int i=pozX-pooling/2;i<=pozX+pooling/2;i++)
			for(int j=pozY-pooling/2;j<=pozY+pooling/2;j++)
			if(ok(i,j,height,width))
			{
				if(minim_blue>map[i][j].bgr[0])
					minim_blue=map[i][j].bgr[0];
			}

	return minim_blue;
}

void pooling(pixel **map,int height,int width,bmp_fileheader *file_header,bmp_infoheader *info_header,int pooling,int ok,FILE *out)
{

for(int i=0;i<height;i++)
	for(int j=0;j<width;j++)
	{
		if(ok)
		{
			map[i][j].bgr[0]=maxim_blue(map,height,width,pooling,i,j);
			map[i][j].bgr[1]=maxim_green(map,height,width,pooling,i,j);
			map[i][j].bgr[2]=maxim_red(map,height,width,pooling,i,j);
		}
		else
		{
			map[i][j].bgr[0]=minim_blue(map,height,width,pooling,i,j);
			map[i][j].bgr[1]=minim_green(map,height,width,pooling,i,j);
			map[i][j].bgr[2]=minim_red(map,height,width,pooling,i,j);
		}
	}

fwrite(file_header,sizeof(bmp_fileheader),1,out);
	fwrite(info_header,sizeof(bmp_infoheader),1,out);
	for(int i=0;i<height;i++)
	{
	for(int j=0;j<width;j++)
			{
			fwrite(&map[i][j].bgr[0],sizeof(unsigned char),1,out);
			fwrite(&map[i][j].bgr[1],sizeof(unsigned char),1,out);
			fwrite(&map[i][j].bgr[2],sizeof(unsigned char),1,out);
		    }
		unsigned char ignorePadd=0;
		for(int k=0;k<width%4;k++)
			fwrite(&ignorePadd,sizeof(unsigned char),1,out);
		
	}
}

typedef struct 
{
	int x,y;
}coord;
void swap(unsigned char *a,unsigned char *b)
{ 
	unsigned char tmp=*a;
	a=b;
	*b=tmp;
}
int cluster(FILE *out,pixel **map,bmp_fileheader *file_header,bmp_infoheader *info_header,int height,int width,int threshold_nr)
{

	int zona=1;
	int directie_X[5];
	directie_X[0]=-1;
	directie_X[1]=0;
	directie_X[2]=1;
	directie_X[3]=0;
	int directie_Y[5];
	directie_Y[0]=0;
	directie_Y[1]=1;
	directie_Y[2]=0;
	directie_Y[3]=-1;
	int reds=0,greens=0,blues=0;
	coord *coada=calloc(height*width,sizeof(coord));
	
	if(!coada)
		return 1;
	int modified[height][width];
		for(int i=0;i<height;i++)
		for(int j=0;j<width;j++)
			modified[i][j]=0;

	int nextX,nextY;
	int pr,ul;
	for(int i=height-1;i>=0;i--)
		for(int j=0;j<width;j++)
	{
		if(!modified[i][j])
		{
		modified[i][j]=zona;
		pr=0;
		ul=0;
		coada[pr].x=i;
		coada[pr].y=j;
		reds=map[i][j].bgr[2];
		blues=map[i][j].bgr[0];
		greens=map[i][j].bgr[1];
		
		while(pr<=ul)
			{		
			int xx=coada[pr].x;
			int yy=coada[pr].y;
			for(int directie=0;directie<4;directie++)
				{
				 nextX=xx+directie_X[directie];
				 nextY=yy+directie_Y[directie];	
				 
				if((modified[nextX][nextY]==0) && ok(nextX,nextY,height,width)) {
				//if(conditie_task5(matrice_red[i][j],matrice_green[i][j],matrice_blue[i][j],matrice_red[nextX][nextY],matrice_green[nextX][nextY],matrice_blue[nextX][nextY],threshold_nr))
					unsigned char newred,newblue,newgreen;
					newred=map[nextX][nextY].bgr[2];
					newblue=map[nextX][nextY].bgr[0];
					newgreen=map[nextX][nextY].bgr[1];
					int sum=0;
					sum=abs(newred-map[i][j].bgr[2])+abs(newblue-map[i][j].bgr[0])+abs(newgreen-map[i][j].bgr[1]);
					if(sum<=threshold_nr){
					ul++;
					coada[ul].x=nextX;
					coada[ul].y=nextY;
					modified[nextX][nextY]=zona;
					reds=reds+map[nextX][nextY].bgr[2];
					blues=blues+map[nextX][nextY].bgr[0];
					greens=greens+map[nextX][nextY].bgr[1];
											}
						
				}
				
				}
					pr++;
			}//end while
		
		reds=reds/(ul+1);
		greens=greens/(ul+1);
		blues=blues/(ul+1);
		for(int k=0;k<=ul;k++){
			int xi=coada[k].x;
			int yi=coada[k].y;
			
			map[xi][yi].bgr[0]=blues;
			map[xi][yi].bgr[1]=greens;
			map[xi][yi].bgr[2]=reds;
			}
		
		}//endif
	}

fwrite(file_header,sizeof(bmp_fileheader),1,out);
fwrite(info_header,sizeof(bmp_infoheader),1,out);
for(int i=0;i<height;i++)
	{
	for(int j=0;j<width;j++)
			{
			fwrite(&map[i][j].bgr[0],sizeof(unsigned char),1,out);
			fwrite(&map[i][j].bgr[1],sizeof(unsigned char),1,out);
			fwrite(&map[i][j].bgr[2],sizeof(unsigned char),1,out);
		    }
		unsigned char ignorePadd=0;
		for(int k=0;k<width%4;k++)
			fwrite(&ignorePadd,sizeof(unsigned char),1,out);
	}

free(coada);
return 0;
}

int main()
{
 FILE *which_image;
 char file_name[]="input.txt";
 which_image=fopen(file_name,"rt");
 if(!which_image)
	{
		printf(" input couldn't open file");
		return -1;
	}	
 
 char *test_number=malloc(15);
 if(!test_number)
  {
  	printf("nu s a citit testul\n");
  	return 1;
  }
  fgets(test_number,15,which_image);

 char *filter_test=malloc(35);
 if(!filter_test)
 {
 	free(test_number);
 	return 1;
 }
 fgets(filter_test,35,which_image);

 char *pooling_test=malloc(35);
 if(!pooling_test)
 {
 	free(filter_test);
 	free(test_number);
 	return 1;
 }
	  
 fgets(pooling_test,35,which_image);

 char *cluster_test=malloc(35);
 if(!cluster_test)
 {
 	free(filter_test);
 	free(test_number);
 	free(pooling_test);
 	return 1;
 }
 fgets(cluster_test,35,which_image);

 fclose(which_image);
 printf("%s\n",cluster_test);

FILE* imageData;
char *input=malloc(35);
input[0]='\0';
strcat(input,"input/images/");
strcat(input,test_number);
input[strlen(input)-1]='\0';

imageData=fopen(input,"rb");
bmp_fileheader *file_header=calloc(1,sizeof(bmp_fileheader));
 if(!file_header)
 {	printf("nu s a alocat header\n");
 	return 1;
 }
fread(file_header,sizeof(bmp_fileheader),1,imageData);

bmp_infoheader *info_header=calloc(1,sizeof(bmp_infoheader));
 if(!info_header)
 {
 	free(file_header);
 	printf("nu s a alocat info\n");
 	return 1;
 }
fread(info_header,sizeof(bmp_infoheader),1,imageData);
int imagePadding=info_header->width%4;
fseek(imageData,file_header->imageDataOffset,SEEK_SET);
	
signed int height=info_header->height;
signed int width=info_header->width;


pixel **map=calloc(height,sizeof(pixel*));


for(int i=0;i<info_header->height;i++)
	{
	map[i]=calloc(info_header->width,sizeof(pixel));
	for(int j=0;j<info_header->width;j++){
			map[i][j].bgr=calloc(3,sizeof(unsigned char));
			fread(&map[i][j].bgr[0],sizeof(unsigned char),1,imageData);
			fread(&map[i][j].bgr[1],sizeof(unsigned char),1,imageData);
			fread(&map[i][j].bgr[2],sizeof(unsigned char),1,imageData);
			}
			unsigned char ignorePadd=0;
		for(int k=0;k<imagePadding;k++)
			fread(&ignorePadd,sizeof(unsigned char),1,imageData);
		
	}

free(input);
	
//pentru task1

FILE *out;
char *output=malloc(30);
output[0]='\0';
strcat(output,test_number);

output[5]='\0';
strcat(output,"_black_white.bmp");

out=fopen(output,"wt");

pixel **b=calloc(height,sizeof(pixel*));


for(int i=0;i<info_header->height;i++)
	{
	b[i]=calloc(info_header->width,sizeof(pixel));
	for(int j=0;j<info_header->width;j++){
			b[i][j].bgr=calloc(3,sizeof(unsigned char));
			}
			unsigned char ignorePadd=0;
		for(int k=0;k<imagePadding;k++)
			fread(&ignorePadd,sizeof(unsigned char),1,imageData);	
	}
for(int i=0;i<height;i++)
	for(int j=0;j<width;j++)
	{
		b[i][j].bgr[0]=map[i][j].bgr[0];
		b[i][j].bgr[1]=map[i][j].bgr[1];
		b[i][j].bgr[2]=map[i][j].bgr[2];
	}

black_white(b,height,width,out,info_header,file_header);
fclose(out);
/*
output[0]='\0';
strcat(output,test_number);
output[5]='\0';
strcat(output,"_nocrop.bmp");
out=fopen(output,"wb");

	int old=height;
	int oldw=width;
	info_header->height=height;
	no_crop(map,height,width,out,file_header,info_header);
	info_header->height=old;
	info_header->width=oldw;
fclose(out);

pooling_test[strlen(pooling_test)-1]='\0';

FILE *pool=fopen(pooling_test,"rt");
char okMax=0;
int clusteringNr=0;
if(!pool){
printf("nu e \n");
return -1;
}
fscanf(pool,"%c",&okMax);
fscanf(pool,"%d",&clusteringNr);
fclose(pool);
output[0]='\0';
strcat(output,test_number);
output[5]='\0';
strcat(output,"_pooling.bmp");

 out=fopen(output,"wb");
 int ok;
 if(!out)
 {
 	printf("nu s sa deschis\n");
 	return 1;
 }

 if(okMax=='M')
 	ok=1;
 else ok=0;


pooling(map,height,width,file_header,info_header,clusteringNr,ok,out);
fclose(out);
*/
output[0]='\0';
strcat(output,test_number);
output[5]='\0';
strcat(output,"_clustered.bmp");

FILE *clust;
/*
clust=fopen(cluster_test,"rt");

int threshold_nr=100;
if(!clust)
{	printf(" a nu s a deschis\n");


}
//fscanf(clust,"%d",&threshold_nr);
fclose(clust);
*/
int threshold_nr=100;
out=fopen(output,"wb");
if(!out){
printf("nu s a deshc out cluster");
return 1;
}

cluster(out,map,file_header,info_header,height,width,threshold_nr);
fclose(out);

for(int i=0;i<info_header->height;i++)
	for(int j=0;j<width;j++)
		{free(map[i][j].bgr);
		free(b[i][j].bgr);
		}

for(int i=0;i<info_header->height;i++)
	{
	free(map[i]);
	free(b[i]);
	}
free(b);
free(map);
free(output);
 free(cluster_test);
 free(pooling_test);
 free(filter_test);
 free(test_number);
 free(info_header);
 free(file_header);
	return 0;
 }
 
