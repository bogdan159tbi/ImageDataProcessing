#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "bmp_header.h"
#define MAX_FILE 90
#define BMP_FILEHEADER 14
#define BMP_INFOHEADER 40

/*
struct Queue
{
	int front,size,rear;
	unsigned capacity;
	unsigned char *array;
};

struct Queue* create(unsigned capacity)
{
struct Queue* queue=(struct Queue*)calloc(1,sizeof(struct Queue));
if(!queue)
{
	printf("nu s a putut aloca queue");
	return -1;
}
queue->capacity=capacity;
queue->front=queue->size=0;
queue->rear=capacity-1;
queue->array=(unsigned char*)calloc(queue->capacity,sizeof(unsigned char));
if(!queue->array)
{
	free(queue);
	printf("nu s a putut aloca array ul");
	return -1;
}
return queue;
}

int empty_queue(struct Queue* queue)
{
	if(!queue->size)
		return 1;
	return 0;
}
int full_queue(struct Queue* queue)
{
	return (queue->size == queue->capacity);
}
void add_queue(struct Queue* queue,unsigned char elem)
{
	queue->rear=(queue->rear + 1)%queue->capacity;//sa schimb forumula asta
	queue->array[queue->rear]=elem;
	queue->size=queue->size+1;
}
void remove_queue(struct Queue* queue)
{
	unsigned char elem=queue->array[queue->front];
	queue->front=(queue->front + 1)%queue->capacity; //sa schimb formula asta
	queue->size=queue->size-1;
}
int front_queue(struct Queue* queue)
{
	if(!empty_queue(queue))
		return queue->array[queue->front];
}
*/


bmp_fileheader *read_header(unsigned char file_header[14])
{	
	bmp_fileheader *f_header;
	f_header=malloc(14);
	f_header->fileMarker1=file_header[0];
	f_header->fileMarker2=file_header[1];
	f_header->bfSize=0;

	unsigned int size; 
	size=file_header[2];
	size=size|(file_header[3]<<8);
	size=size|(file_header[4]<<16);
	size=size|(file_header[5]<<24);
	f_header->bfSize=size;
	//10
	unsigned int offset;
	offset=file_header[10];
	offset=offset|(file_header[11]<<8);
	offset=offset|(file_header[12]<<16);
	offset=offset|(file_header[13]<<24);
	f_header->imageDataOffset=offset;

	return f_header;

}

bmp_infoheader *read_info(unsigned char f_info[40])
{	
	bmp_infoheader *file_info;
	file_info=malloc(40);
	unsigned int dib=0;
	dib=f_info[0];
	dib=dib|(f_info[1]<<8);
	dib=dib|(f_info[2]<<16);
	dib=dib|(f_info[3]<<24);
	file_info->biSize=dib;

	signed int width=0,height=0;
	width=f_info[4];
	width=width|(f_info[5]<<8);
	width=width|(f_info[6]<<16);
	width=width|(f_info[7]<<24);
	file_info->width=width;

	height=f_info[8];
	height=height|(f_info[9]<<8);
	height=height|(f_info[10]<<16);
	height=height|(f_info[11]<<24);
	file_info->height=height;
	//211
	unsigned int size_img=0;
	size_img=f_info[20];
	size_img=size_img|(f_info[21]<<8);
	size_img=size_img|(f_info[22]<<16);
	size_img=size_img|(f_info[23]<<24);
	file_info->biSizeImage=size_img;

	return  file_info;
}
int find_padding(unsigned char width)
{	
	//return (4-((width*3)%4));
	return width%4;
}

void black_white(FILE* output,unsigned char *img_data,unsigned char *file_info,int padding,int size,unsigned int width)
{
	for(int i=0;i<size;i+=3){
		unsigned char p1,p2,p3;
		//fac suma pixelilor pentru a afla media
		p1=img_data[i];
		p2=img_data[i+1];
		p3=img_data[i+2];
		int avg=(p1+p2+p3)/3;
		img_data[i]=avg;
		img_data[i+1]=avg;
		img_data[i+2]=avg;
		//ignore padding
		if(!i%width)
		i+=padding;
	}
	fwrite(file_info,sizeof(unsigned char),54,output);
	fwrite(img_data,sizeof(unsigned char),size,output);

}
void afisare_img(FILE* output,unsigned char *img_data,unsigned char file_info[54],int size)
{
	fwrite(file_info,sizeof(unsigned char),54,output);
	fwrite(img_data,sizeof(unsigned char),size,output);
}

void add_white(unsigned char *img_data,unsigned char *file_info,int size,int width,int height,FILE *out)
{	

	unsigned char **matrice;
	int height_mat;
	int width_mat;
	if(height==width)
	{
		fwrite(file_info,sizeof(unsigned char),54,out);
		fwrite(img_data,sizeof(unsigned char),size,out);
	}

	if(height<width){
	int dif=width-height;

	int padding=find_padding(width);
	int width_bytes=padding+width*3;
	//daca avem deja multiplu de 4 octeti pe fiecare linie
	if(padding%4==0)
		padding=0;
	height_mat=width;
	matrice=malloc(height_mat*sizeof(unsigned char*));

	for(int i=0;i<height_mat;i++)
		matrice[i]=malloc(width_bytes);

	for(int a=0;a<dif/2;a++){
		for(int b=0;b<width*3;b++)
			matrice[a][b]=255;
		for(int pad=0;pad<padding;pad++)
			matrice[a][3*width+pad]=0;
	}
			for (int a =0; a < height; a++) {
	   		 for (int b = 0; b < width*3; b++) 
	            matrice[a+dif/2][b]=img_data[a*(width*3 + padding)+b];
	    	for (int pad = 0; pad < padding; pad++) 
	       		 matrice[a+dif/2][3*width + pad] = 0;
											}
						
	for(int a=height+dif/2;a<height+dif;a++)
		{for(int  b=0;b<width*3;b++)
			matrice[a][b]=255;
		for(int pad=0;pad<padding;pad++)
			matrice[a][3*width+pad]=0;
		}

	memcpy(file_info+22,&height_mat,sizeof(unsigned int));
	fwrite(file_info,1,54,out);
	
	for (int a = 0; a < height_mat; a++) {
	    for (int b = 0; b < width*3; b++) 
	    	fwrite(&matrice[a][b],1,1,out);
	    for (int pad = 0; pad < padding; pad++) {
	        fwrite(&matrice[a][3*width + pad],1,1,out);
	    }
	   
	}
	 
 	for(int i=0;i<height_mat;i++)
		free(matrice[i]);

	free(matrice);
	}//end if height<width;
	if(height>width)
	{	
		int dif=height-width;
		width_mat=height;
		int padding=find_padding(width);
		
		if(padding%4==0)
			padding=0;
		int new_padding=find_padding(width_mat);
		if(new_padding%4==0)
			new_padding=0;
		int width_bytes=width_mat*3+new_padding;
		matrice=malloc(height*sizeof(unsigned char*));
		for(int i=0;i<height;i++)
			matrice[i]=malloc(width_bytes);
		// fac toata imaginea alba initial dupa care adaug
		// pixelii imaginii initiale
		for(int a=0;a<height;a++){

		for(int b=0;b<width_mat*3;b++)
			matrice[a][b]=255;
		for(int pad=0;pad<new_padding;pad++)
			matrice[a][pad+width_mat*3]=0;
								}

								
		for (int a =0; a < height; a++) 
	   		 for (int b = 0; b < width*3; b++) 
	            matrice[a][b+3*(dif/2)]=img_data[a*(width*3 + padding)+b];
									
		
	memcpy(file_info+18,&width_mat,sizeof(unsigned int));
	fwrite(file_info,1,54,out);
	
	for (int a = 0; a < height; a++) {
	    for (int b = 0; b < width_mat*3; b++) 
	    	fwrite(&matrice[a][b],1,1,out);
	    for (int pad = 0; pad < new_padding; pad++) {
	        fwrite(&matrice[a][3*width_mat+pad],1,1,out);
	    }
	}

	for(int i=0;i<height;i++)
			free(matrice[i]);
		free(matrice);	
	}
}
typedef struct 
{
	unsigned char red,green,blue;
}pixel;

int ok(int x,int y,int height,int width)
{
	if(x<0 || x>=height ||y<0 ||y>=width)
		return 0;
	return 1;
}

unsigned  char maxim_red(unsigned char **matrice_red,int height,int width,int pooling,int pozX,int pozY)
{
unsigned char max_red=0;
	int i;

		for(i=pozX-pooling/2;i<=pozX+pooling/2;i++)
			for(int j=pozY-pooling/2;j<=pozY+pooling/2;j++)
			{
				if((ok(i,j,height,width))&& (max_red<matrice_red[i][j]))
					max_red=matrice_red[i][j];
			}

	return max_red;
}
unsigned  char minim_red(unsigned char **matrice_red,int height,int width,int pooling,int pozX,int pozY)
{
unsigned char minim_red=255;
	int i;

		for(i=pozX-pooling/2;i<=pozX+pooling/2;i++)
			for(int j=pozY-pooling/2;j<=pozY+pooling/2;j++)
			{
				if((ok(i,j,height,width))&& (minim_red>matrice_red[i][j]))
					minim_red=matrice_red[i][j];
			}

	return minim_red;
	}
unsigned  char maxim_green(unsigned char **matrice_green,int height,int width,int pooling,int pozX,int pozY)
{
unsigned char max_green=0;
	int i=pozX;

		for(i=pozX-pooling/2;i<=pozX+pooling/2;i++)
			for(int j=pozY-pooling/2;j<=pozY+pooling/2;j++)
		 	{if(ok(i,j,height,width))
			{
				if(max_green<matrice_green[i][j])
					max_green=matrice_green[i][j];
			}
		}
	return max_green;
}
unsigned  char minim_green(unsigned char **matrice_green,int height,int width,int pooling,int pozX,int pozY)
{
unsigned char minim_green=255;
	int i=pozX;

		for(i=pozX-pooling/2;i<=pozX+pooling/2;i++)
			for(int j=pozY-pooling/2;j<=pozY+pooling/2;j++)
		 	{if(ok(i,j,height,width))
			{
				if(minim_green>matrice_green[i][j])
					minim_green=matrice_green[i][j];
			}
		}
	return minim_green;
}
unsigned char maxim_blue(unsigned char **matrice_blue,int height,int width,int pooling,int pozX,int pozY)
{
unsigned char max_blue=0;

		for(int i=pozX-pooling/2;i<=pozX+pooling/2;i++)
			for(int j=pozY-pooling/2;j<=pozY+pooling/2;j++)
			if(ok(i,j,height,width))
			{
				if(max_blue<matrice_blue[i][j])
					max_blue=matrice_blue[i][j];
			}

	return max_blue;
}
unsigned char minim_blue(unsigned char **matrice_blue,int height,int width,int pooling,int pozX,int pozY)
{
unsigned char minim_blue=255;

		for(int i=pozX-pooling/2;i<=pozX+pooling/2;i++)
			for(int j=pozY-pooling/2;j<=pozY+pooling/2;j++)
			if(ok(i,j,height,width))
			{
				if(minim_blue>matrice_blue[i][j])
					minim_blue=matrice_blue[i][j];
			}

	return minim_blue;
}
void swap(unsigned char *a,unsigned char *b)
{
	unsigned char tmp;
	tmp=*a;
	*a=*b;
	*b=tmp;

}
void pool(unsigned char *img_data,int height,int width,unsigned char *file_info,FILE *out,int pooling,int ok)
{	
	unsigned char **matrice_red,**matrice_blue,**matrice_green,**red_copy,**blue_copy,**green_copy;
	matrice_red=malloc(height*sizeof(unsigned char*));
	red_copy=malloc(height*sizeof(unsigned char*));
	matrice_green=malloc(height*sizeof(unsigned char*));
	green_copy=malloc(height*sizeof(unsigned char*));
	matrice_blue=malloc(height*sizeof(unsigned char*));
	blue_copy=malloc(height*sizeof(unsigned char*));
		for(int i=0;i<height;i++)
			{matrice_red[i]=malloc(width*sizeof(unsigned char));
			red_copy[i]=malloc(width*sizeof(unsigned char));

			matrice_blue[i]=malloc(width*sizeof(unsigned char));
			blue_copy[i]=malloc(width*sizeof(unsigned char));
	
			matrice_green[i]=malloc(width*sizeof(unsigned char));
			green_copy[i]=malloc(width*sizeof(unsigned char));
			}
	int padding=find_padding(width);
	if(padding%4==0)
		padding=0;
	int col=0;
	for (int a =0; a < height; a++) {
	   col=0;
	   for (int b = 2; b < width*3; b+=3) 
	      {matrice_blue[a][col]=img_data[a*(width*3 + padding)+b];
	   	   blue_copy[a][col]=matrice_blue[a][col];
	   	   col++;
	      }
	   col=0;
	   for(int b=1;b<width*3;b+=3)
		  {matrice_green[a][col]=img_data[a*(width*3 + padding)+b];
	   	   green_copy[a][col]=matrice_green[a][col];
	   	   col++;
	   	  }
	   col=0;
	   for(int b=0;b<width*3;b+=3){
	   	  matrice_red[a][col]=img_data[a*(width*3 + padding)+b];
	   	  red_copy[a][col]=matrice_red[a][col];
	   	  col++;
	   }
	}

	if(ok){
		for (int a =height-1; a >=0; a--) 
	   	  for (int b = width-1; b >=0; b--) {
			matrice_red[a][b]=maxim_red(red_copy,height,width,pooling,a,b);
			matrice_green[a][b]=maxim_green(green_copy,height,width,pooling,a,b);
			matrice_blue[a][b]=maxim_blue(blue_copy,height,width,pooling,a,b);
					}
	}
	else
	{
		for (int a =height-1; a >=0; a--) 
	   	  for (int b = width-1; b >=0; b--) {
			matrice_red[a][b]=minim_red(red_copy,height,width,pooling,a,b);
			matrice_green[a][b]=minim_green(green_copy,height,width,pooling,a,b);
			matrice_blue[a][b]=minim_blue(blue_copy,height,width,pooling,a,b);
					}
	}
	 fwrite(file_info,1,54,out);
	 for (int a =0; a < height; a++){ 
	   for (int b = 0; b < width; b++) {
			fwrite(&matrice_red[a][b],1,1,out);	
			fwrite(&matrice_green[a][b],1,1,out);
			fwrite(&matrice_blue[a][b],1,1,out);		
		}
		for (int pad = 0; pad < padding; pad++) 
	       {unsigned char pad=0;
		   fwrite(&pad,1,1,out);
	   	   }
	   	   
		}

	//fclose(out);
	for(int i=0;i<height;i++)
		{	
		free(matrice_red[i]);
		free(red_copy[i]);
		free(blue_copy[i]);
		free(green_copy[i]);
		free(matrice_green[i]);
		free(matrice_blue[i]);
		}
	free(red_copy);
	free(blue_copy);
	free(green_copy);
	free(matrice_red);
	free(matrice_green);
	free(matrice_blue);
}
unsigned char sum(unsigned char **matrice,int dim,int **filter_matrix,int pozX,int pozY,int height,int width)
{
	int i,j;
	int sum=0;
	int lin_filtru=0;
	int col_filtru;
	for(i=pozX-dim/2;i<=pozX+dim/2;i++){
		col_filtru=0;
		for(j=pozY-dim/2;j<=pozY+dim/2;j++)
		{
			if(ok(i,j,height,width))
			{
				sum+=filter_matrix[lin_filtru][col_filtru]*(matrice[i][j]);
			}
			col_filtru++;
		}
		lin_filtru++;
	}
 if(sum<0)
 	return 0;
 if(sum>255)
 	return 255;
 return sum;
}

void convolution(FILE *out,unsigned char *img_data,int **filter,int width,int height,int filter_dim,unsigned char *file_info)
{
	unsigned char **matrice_red,**matrice_blue,**matrice_green,**red_copy,**blue_copy,**green_copy;
	matrice_red=malloc(height*sizeof(unsigned char*));
	red_copy=malloc(height*sizeof(unsigned char*));
	matrice_green=malloc(height*sizeof(unsigned char*));
	green_copy=malloc(height*sizeof(unsigned char*));
	matrice_blue=malloc(height*sizeof(unsigned char*));
	blue_copy=malloc(height*sizeof(unsigned char*));
		for(int i=0;i<height;i++)
			{matrice_red[i]=malloc(width*sizeof(unsigned char));
			red_copy[i]=malloc(width*sizeof(unsigned char));

			matrice_blue[i]=malloc(width*sizeof(unsigned char));
			blue_copy[i]=malloc(width*sizeof(unsigned char));
	
			matrice_green[i]=malloc(width*sizeof(unsigned char));
			green_copy[i]=malloc(width*sizeof(unsigned char));
			}
	int padding=find_padding(width);
	if(padding%4==0)
		padding=0;
	int col=0;
	for (int a =0; a < height; a++) {
	   col=0;
	   for (int b = 2; b < width*3; b+=3) 
	      {matrice_blue[a][col]=img_data[a*(width*3 + padding)+b];
	   	   blue_copy[a][col]=matrice_blue[a][col];
	   	   col++;
	      }
	   col=0;
	   for(int b=1;b<width*3;b+=3)
		  {matrice_green[a][col]=img_data[a*(width*3 + padding)+b];
	   	   green_copy[a][col]=matrice_green[a][col];
	   	   col++;
	   	  }
	   col=0;
	   for(int b=0;b<width*3;b+=3){
	   	  matrice_red[a][col]=img_data[a*(width*3 + padding)+b];
	   	  red_copy[a][col]=matrice_red[a][col];
	   	  col++;
	   }
	}
	for (int a =0; a <height; a++) 
	   	 for (int b = 0; b <width; b++)
	   	 {
	   	 	matrice_red[a][b]=sum(red_copy,filter_dim,filter,a,b,height,width);
	   	 	matrice_green[a][b]=sum(green_copy,filter_dim,filter,a,b,height,width);
	   	 	matrice_blue[a][b]=sum(blue_copy,filter_dim,filter,a,b,height,width);
	   	 }

	 fwrite(file_info,1,54,out);
	 for (int a =0; a < height; a++){ 
	   for (int b = 0; b < width; b++) {
			fwrite(&matrice_red[a][b],1,1,out);	
			fwrite(&matrice_green[a][b],1,1,out);
			fwrite(&matrice_blue[a][b],1,1,out);		
		}
		for (int pad = 0; pad < padding; pad++) 
	       {unsigned char pad=0;
		   fwrite(&pad,1,1,out);
	   	   }
	   	   
		}
	fclose(out);

	for(int i=0;i<height;i++)
		{	
		free(matrice_red[i]);
		free(red_copy[i]);
		free(blue_copy[i]);
		free(green_copy[i]);
		free(matrice_green[i]);
		free(matrice_blue[i]);
		}
	free(red_copy);
	free(blue_copy);
	free(green_copy);
	free(matrice_red);
	free(matrice_green);
	free(matrice_blue);

}

int conditie_task5(unsigned char pix1R,unsigned char pix1G,unsigned char pix1B,unsigned char pix2R,unsigned char pix2G,unsigned char pix2B,int threshold_nr)
{
	if(pix1R<pix2R)
		swap(&pix1R,&pix2R);
	if(pix1G<pix2G)
		swap(&pix1G,&pix2G);
	if(pix1B<pix2B)
		swap(&pix1B,&pix2B);

	if(pix1R-pix2R+pix1B-pix2B+pix1G-pix2G<=threshold_nr)
		return 1;

	return 0;
}
int okXY(int x,int y,int height,int width)
{
	if(x>=height || y>=width || y<0 || x<0)
		return 0;
	return 1;
}

void algFill(int threshold_nr,int height,int width,int x,int y,int zona,int ***modified,unsigned char **matrice_red,unsigned char **matrice_green,unsigned char **matrice_blue)
{
	(*modified)[x][y]=zona;
	int directie_X[4]={-1,0,1,0};
	int directie_Y[4]={0,1,0,-1};
	for(int i=0;i<4;i++)
	{
		int nouX=x+directie_X[i];
		int nouY=y+directie_Y[i];
		if(okXY(nouX,nouY,height,width))
		if(conditie_task5(matrice_red[x][y],matrice_green[x][y],matrice_blue[x][y],matrice_red[nouX][nouY],matrice_green[nouX][nouY],matrice_blue[nouX][nouY],threshold_nr))
		{
		(*modified)[nouX][nouY]=zona;
		algFill(threshold_nr,height,width,nouX,nouY,zona,modified,matrice_red,matrice_green,matrice_blue);
		}
	}
}

struct coord
{
	int first,last;
};
int cluster_task(FILE *out,unsigned char *img_data,unsigned char *file_info,int latime,int height,int threshold_nr)
{
	unsigned char **matrice_red,**matrice_blue,**matrice_green;
	matrice_red=malloc(height*sizeof(unsigned char*));
	matrice_blue=malloc(height*sizeof(unsigned char*));
	matrice_green=malloc(height*sizeof(unsigned char*));
	for(int i=0;i<height;i++)
		{matrice_red[i]=malloc(latime*sizeof(unsigned char));
		matrice_blue[i]=malloc(latime*sizeof(unsigned char));
		matrice_green[i]=malloc(latime*sizeof(unsigned char));
			}
	int padding=find_padding(latime);
	if(padding%4==0)
		padding=0;
	int col=0;
	for (int a =0; a < height; a++) {
	   col=0;
	   for (int b = 2; b < latime*3; b+=3) 
	      {matrice_blue[a][col]=img_data[a*(latime*3 + padding)+b];
	   	   col++;
	      }
	   col=0;
	   for(int b=1;b<latime*3;b+=3)
		  {matrice_green[a][col]=img_data[a*(latime*3 + padding)+b];
	   	   col++;
	   	  }
	   col=0;
	   for(int b=0;b<latime*3;b+=3){
	   	  matrice_red[a][col]=img_data[a*(latime*3 + padding)+b];
	   	  col++;
	   }
	}

	int **modified=malloc(height*sizeof(int*));
	if(!modified)
		return -1;

	for(int i=0;i<height;i++)
	modified[i]=malloc(latime*sizeof(int));
		
	int zona=0;
	for(int i=0;i<height;i++)
		for(int j=0;j<latime;j++)
			if(!modified[i][j])
			{
				zona++;
				algFill(threshold_nr,height,latime,i,j,zona,&modified,matrice_red,matrice_green,matrice_blue);

			}


	 fwrite(file_info,1,54,out);
	 for (int a =0; a < height; a++){ 
	   for (int b = 0; b < latime; b++) {
			fwrite(&matrice_red[a][b],1,1,out);	
			fwrite(&matrice_green[a][b],1,1,out);
			fwrite(&matrice_blue[a][b],1,1,out);		
		}
		for (int pad = 0; pad < padding; pad++) 
	       {unsigned char pad=0;
		   fwrite(&pad,1,1,out);
	   	   }
	   	   
		}

	for(int i=0;i<height;i++)
		{	
		free(matrice_red[i]);
		free(modified[i]);
		free(matrice_green[i]);
		free(matrice_blue[i]);
		}
	free(modified);
	free(matrice_red);
	free(matrice_green);
	free(matrice_blue);

}


int main(int argc, char const *argv[])
{
	char file_name[]="input.txt";
	FILE *in;
	char binary_file[MAX_FILE]="",filter_name[MAX_FILE],pooling[MAX_FILE],threshold_name[MAX_FILE];
	in=fopen(file_name,"rt");
	if(!in)
	{
		printf(" input couldn't open file");
		return -1;
	}
	
	fgets(binary_file,MAX_FILE,in);
	char binary_file_copie[MAX_FILE]="";
	strcat(binary_file_copie,"input/images/");
	strcat(binary_file_copie,binary_file);
	binary_file_copie[strlen(binary_file_copie)-1]='\0';

	fgets(filter_name,MAX_FILE,in);
	filter_name[strlen(filter_name)-1]='\0';

	fgets(pooling,MAX_FILE,in);
	pooling[strlen(pooling)-1]='\0';

	fgets(threshold_name,MAX_FILE,in);
	threshold_name[strlen(threshold_name)-5]='\0';
	char which_test[1];
	strcpy(which_test,threshold_name+strlen(threshold_name)-1);
	threshold_name[strlen(threshold_name)-1]='\0';
	strcat(threshold_name,which_test);
	strcat(threshold_name,".txt");
	printf("%s\n",threshold_name);

	fclose(in);

	FILE *image;
	image=fopen(binary_file_copie,"rb");
	if(!image)
	{
		printf("binary file couldn t open image");
		return -1;
	}
	
	char test_name[6];
	strcpy(file_name,binary_file);

	printf("%s\n",file_name);
	file_name[5]='\0';
	char output_task1[150]="",output_task2[150]="",output_task3[150]="",output_task4[150]="",output_task5[150]="";
	
	strcat(output_task1,file_name);
	strcat(output_task2,file_name);
	strcat(output_task3,file_name);
	strcat(output_task4,file_name);
	strcat(output_task5,file_name);

	strcat(output_task1,"_black_white.bmp");
	strcat(output_task2,"_nocrop.bmp");
	strcat(output_task3,"_pooling.bmp");
	strcat(output_task4,"_filter.bmp");
	strcat(output_task5,"_clustered.bmp");

	unsigned char file_info[54],copie_file_info[54];
	bmp_infoheader *f_info;
	f_info=malloc(54);
	fread(file_info,sizeof(unsigned char),54,image);
	unsigned int width,height,copie_width,copie_height;
	for(int i=0;i<54;i++)
		copie_file_info[i]=file_info[i];

	memcpy(&width,file_info+18,sizeof(unsigned int));
	memcpy(&height,file_info+22,sizeof(unsigned int));
	int pad_initial=find_padding(width);
	if(pad_initial%4==0)
		pad_initial=0;
	int size=height*(width*3+pad_initial);
	unsigned char *img_data=malloc(size);
	unsigned char *copie_img_data=malloc(size);
	fread(img_data,sizeof(unsigned char),size,image);
	for(int i=0;i<size;i++)
		copie_img_data[i]=img_data[i];
	fclose(image);
	
	FILE *pool_nr;
	pool_nr=fopen(pooling,"rt");
	if(!pool_nr)
	{
		printf("couldn t open pooling file\n");
		return -1;
	}	
	int pool_filter=0;
	char ok_max=0;
	fscanf(pool_nr,"%c",&ok_max);
	if(!ok_max)
		return -1;
	fscanf(pool_nr,"%d",&pool_filter);
	if(!pool_filter)
		return -1;
	fclose(pool_nr);
	//am terminat citirea
	FILE* out;
	out=fopen(output_task1,"wb");
	if(!out)
	{
		printf("nu s a deschis out");
		return -1;
	}
	black_white(out,copie_img_data,copie_file_info,pad_initial,size,width);
	fclose(out);
	
	out=fopen(output_task2,"wb");
	if(!out)
	{
		printf("nu s a deschis out");
		return -1;
	}
	for(int i=0;i<54;i++)
		copie_file_info[i]=file_info[i];
	for(int i=0;i<size;i++)
		copie_img_data[i]=img_data[i];
	add_white(copie_img_data,copie_file_info,size,width,height,out);
	fclose(out);


	out=fopen(output_task3,"wb");
	int ok;
	if(ok_max=='M')
		ok=1;
	else ok =0;
	copie_width=width;
	copie_height=height;
	pool(img_data,height,width,file_info,out,pool_filter,ok);
	fclose(out);

	FILE *filter;
	filter=fopen(filter_name,"rt");
	int dim_filter;
	fscanf(filter,"%d",&dim_filter);
	int **filter_matrix=malloc(sizeof(int*)*dim_filter);
	if(!filter_matrix)
		return -1;
	for(int i=0;i<dim_filter;i++)
		{filter_matrix[i]=malloc(sizeof(int)*dim_filter);
			if(!filter_matrix[i])
			{
				for(int j=0;j<i;j++)
					free(filter_matrix[j]);
				free(filter_matrix);
			}
		}	
		for(int i=0;i<dim_filter;i++)
			for(int j=0;j<dim_filter;j++)
				fscanf(filter,"%d",&filter_matrix[i][j]);
		fclose(filter);
	out=fopen(output_task4,"wb");
	convolution(out,img_data,filter_matrix,width,height,dim_filter,file_info);
	fclose(out);
	
	FILE *cluster;
	cluster=fopen(threshold_name,"rt");	
	if(!cluster)
	{
		printf("nu s a deschis cluster");
		return -1;
	}
	int threshold_nr;
	fscanf(cluster,"%d",&threshold_nr);
	fclose(cluster);


	out=fopen(output_task5,"wb");
	if(!out){printf("nu s a deschis out de la task5 \n");
		return -1;
	}
	
	//cluster_task(out,img_data,file_info,copie_width,height,threshold_nr);
	fclose(out);

	for(int i=0;i<dim_filter;i++)
		free(filter_matrix[i]);

	free(filter_matrix);

	free(img_data);
	free(copie_img_data);

	free(f_info);		
		return 0;
}

