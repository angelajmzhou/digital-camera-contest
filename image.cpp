#include <vector>
#include <algorithm>
#include <cstdio>
#include <cstring>
#include <cassert>
#include <fstream>
#include <iostream>
#include <cmath>
#include <string>
#define HEIGHT 2460
#define WIDTH 3360

using namespace std;

/*
	kernel/convolution matrix/mask: small matrix for blurring/sharpening/edge detectione etc
	accomplished via convolution: 
*/
//PROBLEM: brightness too high...


int pic[2460][3360];  // Height x Width
void OB();
void WB();
void medianFilter(int);
void colormatrix();
void gamma(double);
void edge_enhance(int threshold);
void rgb2bmp(char *);
void saturationEqualization(int contrast);
void CFA_to_krkb();
void bilateralSmoothing(int kernel_size, float spatial_sigma, float intensity_sigma);
vector<vector<float> > generate2DGaussianKernel(int kernel_size, float sigma);


int r[2460][3360];
int g[2460][3360];
int b[2460][3360];
int e[2460][3360];
int kr[2460][3360];
int kb[2460][3360];
int new_r[2460][3360];
int new_g[2460][3360];
int new_b[2460][3360];
int new_e[2460][3360];
int h[2460][3360];
double s[2460][3360];
double v[2460][3360];


unsigned short int gamma_value[1024] = {
	0,1,3,5,8,9,11,12,13,14,16,17,18,20,21,22,24,25,26,28,
	29,30,32,33,34,36,37,38,39,41,42,43,45,46,47,49,50,51,53,54,
	55,57,58,59,61,62,63,64,66,67,68,70,71,72,74,75,76,78,79,80,
	82,83,84,85,87,87,88,89,89,90,91,91,92,93,93,94,95,95,96,97,
	97,98,99,99,100,101,101,102,103,103,104,105,105,106,107,107,108,108,109,110,
	110,111,112,112,113,114,114,115,116,116,117,118,118,119,120,120,121,122,122,123,
	124,124,125,126,126,127,128,128,129,129,130,130,130,130,131,131,131,132,132,132,
	133,133,133,134,134,134,135,135,135,136,136,136,137,137,137,138,138,138,139,139,
	139,140,140,140,141,141,141,142,142,142,143,143,143,144,144,144,145,145,145,146,
	146,146,147,147,147,148,148,148,149,149,149,150,150,150,151,151,151,152,152,152,
	153,153,153,154,154,154,154,155,155,155,156,156,156,157,157,157,158,158,158,159,
	159,159,160,160,160,161,161,161,162,162,162,163,163,163,164,164,164,165,165,165,
	166,166,166,167,167,167,168,168,168,169,169,169,170,170,170,171,171,171,171,171,
	172,172,172,172,172,172,173,173,173,173,173,173,174,174,174,174,174,174,175,175,
	175,175,175,175,176,176,176,176,176,176,176,177,177,177,177,177,177,178,178,178, 
	178,178,178,179,179,179,179,179,179,180,180,180,180,180,180,181,181,181,181,181,
	181,182,182,182,182,182,182,183,183,183,183,183,183,184,184,184,184,184,184,185,
	185,185,185,185,185,186,186,186,186,186,186,187,187,187,187,187,187,188,188,188,
	188,188,188,189,189,189,189,189,189,189,190,190,190,190,190,190,191,191,191,191,
	191,191,192,192,192,192,192,192,193,193,193,193,193,193,194,194,194,194,194,194,
	195,195,195,195,195,195,196,196,196,196,196,196,197,197,197,197,197,197,198,198,
	198,198,198,198,199,199,199,199,199,199,200,200,200,200,200,200,201,201,201,201,
	201,201,201,202,202,202,202,202,202,203,203,203,203,203,203,204,204,204,204,204,
	204,205,205,205,205,205,205,206,206,206,206,206,206,207,207,207,207,207,207,208,
	208,208,208,208,208,209,209,209,209,209,209,210,210,210,210,210,210,211,211,211,
	211,211,211,212,212,212,212,212,212,213,213,213,213,213,213,213,213,213,213,214,
	214,214,214,214,214,214,214,214,214,214,214,215,215,215,215,215,215,215,215,215,
	215,215,215,216,216,216,216,216,216,216,216,216,216,216,216,217,217,217,217,217,
	217,217,217,217,217,217,217,217,218,218,218,218,218,218,218,218,218,218,218,218,
	219,219,219,219,219,219,219,219,219,219,219,219,220,220,220,220,220,220,220,220,
	220,220,220,220,221,221,221,221,221,221,221,221,221,221,221,221,222,222,222,222,
	222,222,222,222,222,222,222,222,223,223,223,223,223,223,223,223,223,223,223,223,
	223,224,224,224,224,224,224,224,224,224,224,224,224,225,225,225,225,225,225,225,
	225,225,225,225,225,226,226,226,226,226,226,226,226,226,226,226,226,227,227,227,
	227,227,227,227,227,227,227,227,227,228,228,228,228,228,228,228,228,228,228,228,
	228,229,229,229,229,229,229,229,229,229,229,229,229,229,230,230,230,230,230,230,
	230,230,230,230,230,230,231,231,231,231,231,231,231,231,231,231,231,231,232,232,
	232,232,232,232,232,232,232,232,232,232,233,233,233,233,233,233,233,233,233,233,
	233,233,234,234,234,234,234,234,234,234,234,234,234,234,235,235,235,235,235,235,
	235,235,235,235,235,235,235,236,236,236,236,236,236,236,236,236,236,236,236,237,
	237,237,237,237,237,237,237,237,237,237,237,238,238,238,238,238,238,238,238,238,
	238,238,238,239,239,239,239,239,239,239,239,239,239,239,239,240,240,240,240,240,
	240,240,240,240,240,240,240,241,241,241,241,241,241,241,241,241,241,241,241,241,
	242,242,242,242,242,242,242,242,242,242,242,242,243,243,243,243,243,243,243,243,
	243,243,243,243,244,244,244,244,244,244,244,244,244,244,244,244,245,245,245,245,
	245,245,245,245,245,245,245,245,246,246,246,246,246,246,246,246,246,246,246,246,
	247,247,247,247,247,247,247,247,247,247,247,247,247,248,248,248,248,248,248,248,
	248,248,248,248,248,249,249,249,249,249,249,249,249,249,249,249,249,250,250,250,
	250,250,250,250,250,250,250,250,250,251,251,251,251,251,251,251,251,251,251,251,
	251,252,252,252,252,252,252,252,252,252,252,252,252,253,253,253,253,253,253,253,
	253,253,253,253,253,254,254,254,254,254,254,254,254,254,254,254,254,255,255,255,
	255,255,255,255
};


int main(int argc,char *argv[])
{

	for(int k=1;k<argc;k++)
	{
		FILE *fp;
		fp = fopen(argv[k],"rb");	// �}��  
		fseek(fp,862144,SEEK_SET);
		
		for(int i=0; i<2460; i++)
		{
			for(int j=0; j<3360; j++)
			{
				pic[i][j] = 0;
				fread(pic[i]+j,1,1,fp);
				
				pic[i][j] <<= 8;
				fread(pic[i]+j,1,1,fp);
			}
		}
		fclose(fp);  // ���� 
		
		OB();
		WB();
		CFA_to_krkb();
		colormatrix();
		gamma(1.04);  // 1.06
		saturationEqualization(135);
		medianFilter(5);
		bilateralSmoothing(3, 1.5, 11.5);
		edge_enhance(90);
		rgb2bmp(argv[k]);
	}

	return 0;
	
}  // main end


void OB()
{
	int ob_sum = 0;
	int ob_avg = 0;
	int count = 0;
	int i,j;
	
    //here, we calculate optical black for each color, including "emerald"
	
	//here: Emerald
    //add up the edge values so we can take the avg
	for(i = 0; i <2460 ; i = i+2)
	{
		for(int j=0 ; j<5; j = j+2)
		{
			ob_sum += pic[i][j];
			count++;
		}
		for(j = 3294 ; j<3343; j = j+2)
		{
			ob_sum = ob_sum + pic[i][j];
			count++;
		}
	}
    
    //get the average "edge" (ie black) value
    ob_avg = (count != 0) ? ob_sum / count : 0;

    /*iterate over all the non-edge pixels, subtracting the OB balance if it's greater
     and setting it to 0 if it's less than (unlikely)*/
	for(i = 0; i <2460 ; i = i+2)
		for(int j = 6 ; j<3293; j = j+2)
		{
			if(pic[i][j] > ob_avg)
				pic[i][j] = pic[i][j] - ob_avg;
			else
				pic[i][j]=0;
		}
	
	ob_sum = 0;
	count = 0;

	//OB for R region
	for(i = 0; i <2460 ; i = i+2)
	{
		for(j = 1 ; j<5; j = j+2)
		{
			ob_sum = ob_sum + pic[i][j];
			count++;
		}
		for(j = 3293 ; j<3343; j = j+2)
		{
			ob_sum = ob_sum + pic[i][j];
			count++;
		}
	}
	 ob_avg = (count != 0) ? ob_sum / count : 0;	
	
	for(i = 0; i <2460 ; i = i+2)
		for(int j = 5 ; j<3293; j = j+2)
		{
			if(pic[i][j] > ob_avg)
				pic[i][j] = pic[i][j] - ob_avg;
			else
				pic[i][j]=0;
		}
	
	//OB for B region
	ob_sum = 0;
	count = 0;
	
	for(i = 1; i <2460 ; i = i+2)
	{
		for(int j = 0 ; j<5; j = j+2)
		{
			ob_sum = ob_sum + pic[i][j];
			count++;
		}
		for(j = 3294 ; j<3343; j = j+2)
		{
			ob_sum = ob_sum + pic[i][j];
			count++;
		}
	}
	ob_avg = (count != 0) ? ob_sum / count : 0;
	for(i = 1; i <2460 ; i = i+2)
		for(int j = 6 ; j<3293; j = j+2)
		{
			if(pic[i][j] > ob_avg)
				pic[i][j] = pic[i][j] - ob_avg;
			else
				pic[i][j]=0;
		}

	//OB for G region
	ob_sum = 0;
	count = 0;
	for(i = 1; i <2460 ; i = i+2)
	{
		for(int j = 1 ; j<5; j = j+2)
		{
			ob_sum = ob_sum + pic[i][j];
			count++;
		}
		for(j = 3293 ; j<3343; j = j+2)
		{
			ob_sum = ob_sum + pic[i][j];
			count++;
		}
	}
	ob_avg = (count != 0) ? ob_sum / count : 0;
	for(i = 1; i <2460 ; i = i+2)
		for(int j = 5 ; j<3293; j = j+2)
        {
			if(pic[i][j] > ob_avg)
				pic[i][j] = pic[i][j] - ob_avg;
			else
				pic[i][j]=0;
		}
		
	
	printf("Optical Black ---OK!\n");

}

void WB()
{
	int i,j;

	double r_hist[16384],g_hist[16384],b_hist[16384];//16384=2^14
	double r_accu[16384],g_accu[16384],b_accu[16384];
	//cumulative frequency distribution
    
    //each channel has 14 bits max (ie 255 is max, thus 14 bits needed to store it) Rest of 32b int reserved.
	//channels being RGBE
	for(i=0;i<16384;i++)
	{//initialization histogram to store requency of each intensity level (0-16363 for rgb channels)
	 r_hist[i]=0;
	 g_hist[i]=0;
	 b_hist[i]=0;
	}

	for(i=0;i<2460;i+=2){
		  for(j=5;j<3293;j+=2){
			//increment by 1 at the frequency at that pixel.	`
			  r_hist[pic[i][j]]++;//R
			  b_hist[pic[i+1][j+1]]++;//B
			  g_hist[pic[i+1][j]]++;//G
//			  g_hist[pic[i][j+1]/2]++;//E
//			  r_hist[(pic[i][j+1]/3)]++;//E
//			  b_hist[(pic[i][j+1]/2)]++;//E


		  }
	  }
	  //histogram accumulate
	  r_accu[0]=r_hist[0];
	  g_accu[0]=g_hist[0];
	  b_accu[0]=b_hist[0];
	  for(i=1;i<16384;i++){//
		  r_accu[i]=r_hist[i]+r_accu[i-1];
		  g_accu[i]=g_hist[i]+g_accu[i-1];
		  b_accu[i]=b_hist[i]+b_accu[i-1];
	  }
	  //normalization
	  for(i=0;i<16384;i++){
		//total number of pixels * channels
	      r_hist[i]=((float)r_accu[i]/1997568)*16384;//2066400=3360*2460/4, 16384=2^14
		  g_hist[i]=((float)g_accu[i]/1997568)*16384;//2066400=3360*2460/4, 16384=2^14
		  b_hist[i]=((float)b_accu[i]/1997568)*16384;//2066400=3360*2460/4, 16384=2^14

		  //normalize, then multiply by the maximum to get it in proportion to the maximum. 
		  //"stretch" to get value 0-16384

		  // normalize here for total count
		  //1997568 comes from 3264*2448/4
		  //from the dimensions of the final image used divided by the proportion of each pixel's data each color represents
	  }

	  //ie: there are so many counts-- how many are <= those counts? 0-1 
	  //this is such that max value --255-- is 1 when normalized
	  //ie biggest and under == all summed together == 1 when normalized
	  //top ?? percent?

	   /*White Point Detection*/
	  float r_equal,g_equal,b_equal;
	  float Y_hist,Cr_hist,Cb_hist;
      float Y_bright=0.0,Cr_bright=192.0,Cb_bright=192.0;//probably white point
	  double Y_avg_hist=0.0,Cr_avg_hist1=0.0,Cb_avg_hist1=0.0,Cr_avg_hist2=0.0,Cb_avg_hist2=0.0;
	  unsigned int counterCb1=0,counterCb2=0,counterCr1=0,counterCr2=0;
	  
	  //do for every pixel
	  for(i=0;i<2460;i+=2)
	  {
		  for(j=5;j<3293;j+=2)
		  {
			  r_equal=r_hist[pic[i][j]];//r histogram equalization
			  b_equal=b_hist[pic[i+1][j+1]];//b histogram equalization
			  g_equal=g_hist[pic[i+1][j]];//g histogram equalization

			  //This is the color space conversion for the matrix. 

			  Y_hist=(0.299*r_equal)+(0.587*g_equal)+(0.114*b_equal);
			  Cr_hist=(0.500*r_equal)-(0.4187*g_equal)-(0.0813*b_equal);
			  Cb_hist=(-0.1687*r_equal)-(0.3313*g_equal)+(0.500*b_equal);
              
			  if ( Y_hist>=12000 && (Cr_hist<=192 && Cr_hist>=-192) && (Cb_hist<=192 && Cb_hist>=-192))
			  {
				  if (Y_hist>Y_bright)//the bigger the better
					   Y_bright=Y_hist;
				  if (fabs(Cr_hist)<fabs(Cr_bright))//the smaller the better
					  Cr_bright=Cr_hist;
				  if (fabs(Cb_hist)<fabs(Cb_bright))//the smaller the better
					  Cb_bright=Cb_hist;

				  Y_avg_hist+=Y_hist;
				  //for calculating avg Y_hist value

				  if (Cr_hist>0)
				  {
				     Cr_avg_hist1+=Cr_hist;
					 counterCr1++;
				  }
				  else
				  {
                     Cr_avg_hist2+=Cr_hist;
					 counterCr2++;
				  }

				  if (Cb_hist>0)
				  {
				     Cb_avg_hist1+=Cb_hist;
					 counterCb1++;
				  }
				  else
				  {
                     Cb_avg_hist2+=Cb_hist;
					 counterCb2++;
				  }
			  }	
			  	  
		  }
	  }
	  
	if (counterCr1+counterCr2+counterCb1+counterCb2==0)
	{
		//if total = 0, abnormal
		printf("================error message============\n");
		printf("error in varsha awb count awb white point\n");
		printf("=========================================\n");
		  //Y_bright=11000;
		  //Cr_bright=-400;
		  //Cb_bright=-400;
	}
		else
	{
	     Y_avg_hist/=(counterCr1+counterCr2+counterCb1+counterCb2);
	     Cr_avg_hist1/=counterCr1;
		 Cr_avg_hist2/=counterCr2;
	     Cb_avg_hist1/=counterCb1;
		 Cb_avg_hist2/=counterCb2;
	  }
      //select the minimum and maximum between
	  //brightest of histtogram in Y Cr Cb
	  //average of histogram in Y Cr Cb
	  //as Y_low,Cr_low,Cb_low Y_up,Cr_up,Cb_up
	  float Y_low,Cr_low,Cb_low, Y_up,Cr_up,Cb_up;

	  if (Y_bright>Y_avg_hist)
	  {
          Y_up=Y_bright;
		  Y_low=Y_avg_hist;
	  }else
	  {
		  Y_low=Y_bright;
		  Y_up=Y_avg_hist; 
	  }

	  if (Cr_bright>Cr_avg_hist1){
          Cr_up=Cr_bright;
		  Cr_low=Cr_avg_hist2;
	  }else if(Cr_bright<Cr_avg_hist2)
	  {
		  Cr_low=Cr_bright;
		  Cr_up=Cr_avg_hist1; 
	  }else
	  {
		  Cr_up=Cr_avg_hist1;
		  Cr_low=Cr_avg_hist2;
	  }

	  if (Cb_bright>Cb_avg_hist1)
	  {
          Cb_up=Cb_bright;
		  Cb_low=Cb_avg_hist2;
	  }else if(Cb_bright<Cb_avg_hist2)
	  {
		  Cb_low=Cb_bright;
		  Cb_up=Cb_avg_hist1; 
	  }else
	  {
		  Cb_up=Cb_avg_hist1;
		  Cb_low=Cb_avg_hist2;
	  }

	  /*find the reference white point*/
	  long int R_white=0,G_white=0,B_white=0;
	  double R_avg=0,G_avg=0,B_avg=0;
	  float R_hist_avg=0,G_hist_avg=0,B_hist_avg=0;
      unsigned int counter=0;

	  for(i=0;i<2460;i+=2){
		  for(j=5;j<3293;j+=2){
			  //calculate the original R G B
			  R_avg+=pic[i][j];//r
			  B_avg+=pic[i+1][j+1];//b
              G_avg+=pic[i+1][j];//g
 
			  //calculate the R G B after histogram equalization
			  r_equal=r_hist[pic[i][j]];//r histogram equalization
			  b_equal=b_hist[pic[i+1][j+1]];//b histogram equalization
			  g_equal=g_hist[pic[i+1][j]];//g histogram equalization
			  R_hist_avg+=r_equal;
              G_hist_avg+=g_equal;
			  B_hist_avg+=b_equal;

			  Y_hist=(0.299*r_equal)+(0.587*g_equal)+(0.114*b_equal); 
			  Cr_hist=(0.500*r_equal)-(0.4187*g_equal)-(0.0813*b_equal);
			  Cb_hist=(-0.1687*r_equal)-(0.3313*g_equal)+(0.500*b_equal);

			  //find the pixel which Y_up<=Y_hist>=Y_low and Cr_up<=Cr_hist>=Cr_low and Cb_up<=Cb_hist>=Cb_low
			  //for one pixel-- the "white reference"

			  //choose average to be a white point.
			  if ( (Y_hist>=Y_low && Y_hist<=Y_up) && (Cr_hist>=Cr_low && Cr_hist<=Cr_up) && (Cb_hist>=Cb_low && Cb_hist<=Cb_up)){
			      R_white+=pic[i][j];//r
				  B_white+=pic[i+1][j+1];//b
				  G_white+=pic[i+1][j];//g
				  counter++;
			  }

		  }
	  }
	  
	  if (counter!=0){//find out the reference white point
            R_avg/=2066400;
			G_avg/=2066400;
			B_avg/=2066400;
			
			R_hist_avg/=2066400;
            G_hist_avg/=2066400;
			B_hist_avg/=2066400;

		    R_white/=counter;
			G_white/=counter;
		    B_white/=counter;

      /*----White Balance Adjustment----*/
			//calculate the scale factor from reference white point
			float R_scale,G_scale,B_scale;
			R_scale=((0.299*R_white)+(0.587*G_white)+(0.114*B_white))/R_white;//Y_white/R_white
			G_scale=((0.299*R_white)+(0.587*G_white)+(0.114*B_white))/G_white;//Y_white/G_white
            B_scale=((0.299*R_white)+(0.587*G_white)+(0.114*B_white))/B_white;//Y_white/B_white				
			//calculate the scale factor from gray world assumption 
            float R_GWA,G_GWA,B_GWA;
			R_GWA=((0.299*R_avg)+(0.587*G_avg)+(0.114*B_avg))/R_avg;//Y_avg/R_avg
            G_GWA=((0.299*R_avg)+(0.587*G_avg)+(0.114*B_avg))/G_avg;//Y_avg/G_avg
			B_GWA=((0.299*R_avg)+(0.587*G_avg)+(0.114*B_avg))/B_avg;//Y_avg/B_avg
			
            //calculate the color cast
			float R_factor,G_factor,B_factor;
			if (B_hist_avg+192>=G_hist_avg && B_hist_avg>R_hist_avg){//blusih
                R_factor=R_scale;
			    G_factor=G_scale;
                B_factor=B_GWA;
				printf("bluish\n");
			}else if (G_hist_avg+192>R_hist_avg && R_hist_avg>B_hist_avg){//greenish
				R_factor=R_scale;
				G_factor=G_GWA;
                B_factor=B_scale;
				printf("greenish\n");
			}else if(R_hist_avg>G_hist_avg && G_hist_avg>B_hist_avg){//reddish
				R_factor=R_GWA;
				G_factor=R_scale;
				B_factor=B_scale;
				printf("redish\n");
			}else{
				R_factor=R_scale;
				G_factor=G_scale;
				B_factor=B_scale;   
				printf("normal\n");
			}
			//white balance gain factor
/*			cout<<R_factor<<endl;
			cout<<G_factor<<endl;
			cout<<B_factor<<endl;
*/          if(!(R_factor==1 && G_factor==1 && B_factor==1)){
				for(i=0;i<2460;i+=2){
					for(j=0;j<3360;j+=2){
						//calculate the original R G B
						if (pic[i][j+1]*R_factor<0)
                           pic[i][j+1]=0;
						else
						   pic[i][j+1]*=R_factor;//r
						if (pic[i+1][j]*B_factor<0)
							pic[i+1][j]=0;
						else
						    pic[i+1][j]*=B_factor;//b
						if (pic[i+1][j+1]*G_factor<0)
							pic[i+1][j+1]=0;
						else
						    pic[i+1][j+1]*=G_factor;//g
					}
				}
            }
			//at end, convert the RGB. 
			//each *by a factor: R*R factor,
	  }else{//not find out the reference white point
		  printf("@_@ not do the varsha white balance... @_@\n");
	  }
   

	printf("White balance ---OK!\n");

}


void colormatrix()
{
 //color correction matrix: on sony srf report
 // [R']=[ 1+sa+sb   -sa       -sb     ][R]
 // [G']=[ -sc       1+sc+sd   -sd     ][G]
 // [B']=[ -se       -sf       1+se+sf ][B]  
 // 	 
	//convert to YUV space to further enhance the colors. 
	unsigned short int i,j;
	double temp_r,temp_g,temp_b;
	//double sa=0.265625,sb=0.17;
	//double sc=0.21,sd=0.21;
	//double se=0.19,sf=0.19;
	
	for(i=0;i<2460;i++){
		for(j=0;j<3360;j++){
			//sets temp_color to curr color
			temp_r=r[i][j];
			temp_g=g[i][j];
			temp_b=b[i][j];

		
			
			//r[i][j]=(1.0+sa+sb)*temp_r - (sa)*temp_g - (sb)*temp_b;
			r[i][j]=(double)((1.2665625)*temp_r+(-0.28125)*temp_g+(0.0)*temp_b);//paul

            //r[i][j]=(double)((6.3329)*temp_r+(-2.7900)*temp_g+(1.5002)*temp_b);       
			if (r[i][j]>16360)//1022.5*16
				r[i][j]=16360;
			else if (r[i][j]<0)
				r[i][j]=0;
			
			//g[i][j]=(1.0+sc+sd)*temp_g - (sc)*temp_r   - (sd)*temp_b;
			g[i][j]=(double)((-0.1875)*temp_r+(1.203125)*temp_g+(-0.046875)*temp_b);//paul
            //g[i][j]=(double)((0.1362)*temp_r+(2.6597)*temp_g+(0.0751)*temp_b);
			if (g[i][j]>16360)//1022.5*16
				g[i][j]=16360;
	        else if (g[i][j]<0)
				g[i][j]=0;
			
			//b[i][j]=(1.0+se+sf)*temp_b - (se)*temp_r - (sf)*temp_g ;
			b[i][j]=(double)((-0.078125)*temp_r+(-0.296875)*temp_g+(1.359375)*temp_b);//paul
            //b[i][j]=(double)((1.2656)*temp_r+(-1.5919)*temp_g+(4.7861)*temp_b);
			if (b[i][j]>16360)//1022.5*16
				b[i][j]=16360;
			else if (b[i][j]<0)
				b[i][j]=0;
		}
	}
	printf("colormatrix ---OK!\n");
}

void medianFilter(int kernelSize) {
    int edge = kernelSize / 2;
	int x, y;
    // Loop over each pixel in the image (excluding the borders)
	 for(y = 1 + edge; y <2460-edge ; y++){
		for(x = 5+edge ; x<3293-edge; x++)
   		 {
            vector<int> neighbors;
			neighbors.reserve(kernelSize*kernelSize);
            // Collect all neighbors of the current pixel defined by the kernel size
            for (int ky = -edge; ky <= edge; ky++) {
                for (int kx = -edge; kx <= edge; kx++) {
                    neighbors.push_back(pic[y + ky][x + kx]);
                }
            }

            // Sort the intensities to find the median
            sort(neighbors.begin(), neighbors.end());

            // Set the median value as the new intensity for the central pixel
            pic[y][x] = neighbors[neighbors.size() / 2];
        }
	}
	printf("Median filter ---OK!\n");
}

void gamma(double gamma_cur)
{
	//delinearizes sensor data
    int i,j;
	for(i=0;i<2460;i++)
	{
		for(j=0;j<3360;j++)
		{
			if (pow(r[i][j],gamma_cur)>16383){
				r[i][j]=16383;
			}else{
			    r[i][j]=pow(r[i][j],gamma_cur);
			}

			if (pow(g[i][j],gamma_cur)>16383){
				g[i][j]=16383;
			}else{
				g[i][j]=pow(g[i][j],gamma_cur);
			}

			if (pow(b[i][j],gamma_cur)>16383){
				b[i][j]=16383;
			}else{
				b[i][j]=pow(b[i][j],gamma_cur);
			}


			r[i][j]=gamma_value[r[i][j]/16];//16=2^14/2^10
			g[i][j]=gamma_value[g[i][j]/16];
			b[i][j]=gamma_value[b[i][j]/16];  
		}
	}
	
	printf("gamma ---OK!\n");


}

void rgb2bmp(char *name)
{

	FILE *fp;
	int zero=0;
	char filename[100];
	strcpy(filename,name);
	strcat(filename,".bmp");

	fp=fopen(filename,"wb");

	fwrite(&"BM",1,2,fp);	
	int temp=54+3264*2448*3;
	fwrite(&temp,4,1,fp);
	
	for(int bmpcount=0;bmpcount<4;bmpcount++)//reserved
		fwrite(&zero,1,1,fp);

	temp=54;//start address of datas
	fwrite(&temp,4,1,fp);
	temp=40;//size of [0E-35]
	fwrite(&temp,1,1,fp);
	fwrite(&zero,1,1,fp);
	fwrite(&zero,1,1,fp);
	fwrite(&zero,1,1,fp);
	int width=3264;//width
	fwrite(&width,4,1,fp);
	int height=2448;//height
	fwrite(&height,4,1,fp);
	temp=1;//color planes
	fwrite(&temp,2,1,fp);
	temp=24;//bits/pixel
	fwrite(&temp,2,1,fp);
	fwrite(&zero,4,1,fp);//Compression format
	
	temp=23970816;//data size->3264*2448*3
	fwrite(&temp,4,1,fp);
	fwrite(&zero,4,1,fp);//width pixels/meter
	fwrite(&zero,4,1,fp);//height pixels/meter
	fwrite(&zero,2,1,fp);//color tables
	fwrite(&zero,1,1,fp);//
	fwrite(&zero,1,1,fp);
	fwrite(&zero,4,1,fp);//color indexes
//body
	for(int bmpi=2453;bmpi>=6;bmpi--)
		for(int bmpj=17;bmpj<3281;bmpj++)
		{
		    fwrite(&(b[bmpi][bmpj]),1,1,fp);
			fwrite(&(g[bmpi][bmpj]),1,1,fp);
			fwrite(&(r[bmpi][bmpj]),1,1,fp);
		}
	fclose(fp);
	printf("%s output finished.\n",name);
}


void edge_enhance(int threshold)
{
	int i, diff;
	for(i=0;i<2460;i++)
		for(int j=5;j<3293;j++)
		{
			if((i==0)||(i==2459)||(j==5)||(j==3292))
			{
				pic[i][j] = r[i][j];
				//pic = r (red) for the very edge
			}
			else
			{
				//look at the 4 direct neighbors and subtract their avg from 2*the current value. 
				//if it's all the same color, will be r[i][j]
				//if colors are different: red is more red if even one neighbor is less than center
				//red is less red if neighors are redder than center. 
				diff = 2 * r[i][j] - (r[i-1][j] + r[i][j-1] +  r[i][j+1] + r[i+1][j])/4;
				pic[i][j] = abs(r[i][i] - diff) > threshold ? diff : r[i][j];
			}

		}
		for(i=1;i<2459;i++){
			for(int j=6;j<3292;j++)
			{
				if(pic[i][j]>255){
					pic[i][j] = 255;
				}
				else if(pic[i][j]<0){
					pic[i][j] = 0;      
				}

				r[i][j] = pic[i][j];

		}
	}

	for(i=0;i<2460;i++)
		for(int j=5;j<3293;j++)
		{
			if((i==0)||(i==2459)||(j==5)||(j==3292))
			{
				pic[i][j] = g[i][j];
			}
			else
			{
				diff = 2 * g[i][j] - (g[i-1][j] + g[i][j-1] +  g[i][j+1] + g[i+1][j])/4;
				pic[i][j] = abs(g[i][i] - diff) > threshold ? diff : g[i][j];
			}

		}
		for(i=1;i<2459;i++){
			for(int j=6;j<3292;j++)
			{
				if(pic[i][j]>255){
					pic[i][j] = 255;
				}
				else if(pic[i][j]<0){
					pic[i][j] = 0;      
				}

				g[i][j] = pic[i][j];

		}
	}


	for( i=0;i<2460;i++)
		for(int j=5;j<3293;j++)
		{
			if((i==0)||(i==2459)||(j==5)||(j==3292))
			{
				pic[i][j] = b[i][j];
			}
			else
			{
				diff = 2 * b[i][j] - (b[i-1][j] + b[i][j-1] +  b[i][j+1] + b[i+1][j])/4;
				pic[i][j] = abs(b[i][i] - diff) > threshold ? diff : b[i][j];
			}

		}
		for(i=1;i<2459;i++){
			for(int j=6;j<3292;j++)
			{
				if(pic[i][j]>255){
					pic[i][j] = 255;
				}
				else if(pic[i][j]<0){
					pic[i][j] = 0;      
				}

				b[i][j] = pic[i][j];

		}
	}
	printf("Edge Enhancement ---OK!\n");
}


void CFA_to_krkb(){
	
	printf("start get color interpolation\n");
	
	//init color => bilinear 
	//get three channel RGB
	
	int i,j;
	for(i=1;i<2459;i=i+1){//height
		for(j=1;j<3359;j=j+1){//width
			r[i][j] = 0;
			g[i][j] = 0;
			b[i][j] = 0;
			if(i%2==0){//even height
				if(j%2==0){//g=> r b
				
					r[i][j] = (pic[i][j-1] + pic[i][j+1])/2;
					g[i][j] = pic[i][j];
					b[i][j] = (pic[i-1][j] + pic[i+1][j])/2;
				}else{//r=>g b
				
					r[i][j] = pic[i][j];
					g[i][j] = (pic[i][j-1] + pic[i][j+1] + pic[i-1][j] + pic[i+1][j])/4;
					b[i][j] = (pic[i-1][j-1] + pic[i+1][j-1] + pic[i-1][j+1] + pic[i+1][j+1])/4;
				}
			}else{//odd height
				if(j%2==0){//b=>g r
				
					r[i][j] = (pic[i-1][j-1] + pic[i+1][j-1] + pic[i-1][j+1] + pic[i+1][j+1])/4;
					g[i][j] = (pic[i][j-1] + pic[i][j+1] + pic[i-1][j] + pic[i+1][j])/4;
					b[i][j] = pic[i][j];
				}else{//g=> r b
					
					r[i][j] = (pic[i-1][j] + pic[i+1][j])/2;
					g[i][j] = pic[i][j];
					b[i][j] = (pic[i][j-1] + pic[i][j+1])/2;
				}
			}
		}
	}
	

	//change to kr kb
	//kr = g - r
	//kb = g - b
	
	for(i=0;i<2459;i=i+1){//width
		for(j=0;j<3359;j=j+1){//height
			kr[i][j] = g[i][j]-r[i][j];
			kb[i][j] = g[i][j]-b[i][j];
		}
	}
	//horizontal and vertical edge-filters
	//threshold don't know so use another method
	int dhg,dvg,dhb,dvb,dhr,dvr;
	for(i=3;i<2458;i=i+1){//height
		for(j=3;j<3358;j=j+1){//width
			if(i%2==0){//even height
				if(j%2==0){//g=> kr kb
					kb[i][j] = (kb[i+1][j]+kb[i-1][j])/2;
					kr[i][j] = (kr[i][j+1]+kr[i][j-1])/2;
				}else{//r=>g kb
					dhg = abs(g[i][j+1] - g[i][j-1]) + abs(2*(kr[i][j])-kr[i][j-2]-kr[i][j+2]);
					dvg = abs(g[i+1][j] - g[i-1][j]) + abs(2*(kr[i][j])-kr[i-2][j]-kr[i+2][j]);
					dhb = abs(kb[i-1][j+1] - kb[i+1][j-1]) + abs(2*(kr[i][j])-kr[i+2][j-2]-kr[i-2][j+2]);
					dvb = abs(kb[i+1][j+1] - kb[i-1][j-1]) + abs(2*(kr[i][j])-kr[i-2][j-2]-kr[i+2][j+2]);
					if(dhg<dvg){
						g[i][j] = (g[i][j+1] + g[i][j-1])/2;
					}else{
						if(dhg>dvg){
							g[i][j] = (g[i+1][j] + g[i-1][j])/2;
						}else{//==
							g[i][j] = (g[i][j+1] + g[i][j-1] + g[i][j+1] + g[i][j-1])/4;
						}
					}
					
					if(dhb<dvb){
						kb[i][j] = (kb[i-1][j+1] + kb[i+1][j-1])/2;
					}else{
						if(dhb>dvb){
							kb[i][j] = (kb[i+1][j+1] + kb[i-1][j-1])/2;
						}else{//==
							kb[i][j] = (kb[i-1][j+1] + kb[i+1][j-1] + kb[i+1][j+1] + kb[i-1][j-1])/4;
						}
					}
				}
			}else{//odd height
				if(j%2==0){//b=>g kr
					dhg = abs(g[i][j+1] - g[i][j-1]) + abs(2*(kb[i][j])-kb[i][j-2]-kb[i][j+2]);
					dvg = abs(g[i+1][j] - g[i-1][j]) + abs(2*(kb[i][j])-kb[i-2][j]-kb[i+2][j]);
					dhr = abs(kr[i-1][j+1] - kr[i+1][j-1]) + abs(2*(kb[i][j])-kb[i+2][j-2]-kb[i-2][j+2]);
					dvr = abs(kr[i+1][j+1] - kr[i-1][j-1]) + abs(2*(kb[i][j])-kb[i-2][j-2]-kb[i+2][j+2]);
					if(dhg<dvg){
						g[i][j] = (g[i][j+1] + g[i][j-1])/2;
					}else{
						if(dhg>dvg){
							g[i][j] = (g[i+1][j] + g[i-1][j])/2;
						}else{//==
							g[i][j] = (g[i][j+1] + g[i][j-1] + g[i][j+1] + g[i][j-1])/4;
						}
					}
					
					if(dhr<dvr){
						kr[i][j] = (kr[i-1][j+1] + kr[i+1][j-1])/2;
					}else{
						if(dhr>dvr){
							kr[i][j] = (kr[i+1][j+1] + kr[i-1][j-1])/2;
						}else{//==
							kr[i][j] = (kr[i-1][j+1] + kr[i+1][j-1] + kr[i+1][j+1] + kr[i-1][j-1])/4;
						}
					}
				}else{//g=> kr kb
					kb[i][j] = (kb[i][j+1]+kb[i][j-1])/2;
					kr[i][j] = (kr[i+1][j]+kr[i-1][j])/2;
				}
			}
		}
	}
	//back rgb domain
	for(i=0;i<2459;i=i+1){//width
		for(j=0;j<3359;j=j+1){//height
			r[i][j] = g[i][j]-kr[i][j];
			b[i][j] = g[i][j]-kb[i][j];
		}
	}
	//ends here
	printf("Finish color interpolation\n");
}


void saturationEqualization(int contrast) //try to understand this fcn? 
//HSV conversion, use to create saturation histogram. 
//https://citeseerx.ist.psu.edu/document?repid=rep1&type=pdf&doi=f3beb91bdd820f1ba554338160016ce3c502ba80
{
	double themin, themax, delta;
	int i,j;
	const double epsilon = 0.0001; // or a suitable small number

	
	for(i=0; i<2460; i++)  // Height
	{
		for(j=0; j<3360; j++)  // Width
		{
			//   Calculate HSV from RGB
			//   Hue is in degrees:
			//order: "hue" "saturation" "brightness"

			//normalize Saturation and Vibrance (ranged 0-1)
			double r1 = r[i][j]/255.0;
			double g1 = g[i][j]/255.0;
			double b1 = b[i][j]/255.0;
			
			themin = (r1<g1 ? r1 : g1) < b1 ? (r1<g1 ? r1 : g1) : b1; //min color value
			themax = (r1>g1 ? r1 : g1) > b1 ? (r1>g1 ? r1 : g1) : b1; //max color value

			delta  = themax - themin;
			v[i][j] = themax;
			s[i][j] = (themax)? delta/themax : 0;
		
			//if(!(j%50) && !(i%50))printf("r: %lf g: %lf b: %lf v:%lf s:%lf\n delta: %lf max: %lf min: %lf \n", r1, g1, b1, v[i][j], s[i][j], delta, themax, themin);

			//https://www.rapidtables.com/convert/color/rgb-to-hsl.html
			//this is not the problem
			if (delta < epsilon){
				h[i][j] = 0; //ie grayscale; all color in equal amounts. 
				//if(!(j%50) && !(i%20))(printf("grayscale\n"));
				}			// possible cases: R G b or R g B or r G B
				//two comparisons to handle when two colors have equal components
			else if (fabs(themax - r1) < epsilon){// && (themax - g[i][j]) > epsilon) {
					h[i][j]= 60 * fmod(((g1 - b1)/delta),6);
					//if(!(j%50) && !(i%20))(printf("red dominant\n"));
				}
			else if (fabs(themax - g1) < epsilon){// && (themax - b[i][j]) > epsilon){
					h[i][j]= 60*(2 + ((b1 - r1)/delta));
					//if(!(j%50) && !(i%20))(printf("green dominant\n"));
				}
			else if (fabs(themax - b1) < epsilon){// && (themax - r[i][j]) > epsilon){
					h[i][j]= 60*(4 + (r1 - g1) / delta);
					//(!(j%50) && !(i%20))(printf("blue dominant\n"));
				}
			else if (fabs(themax - r1) < epsilon && fabs(themax - g1) < epsilon) // Red and Green are equal and maximum
					h[i][j] = 60; // Yellow
			else if (fabs(themax - b1) < epsilon && fabs(themax - g1) < epsilon) // Green and Blue are equal and maximum
					h[i][j] = 180; // Cyan
			else if (fabs(themax - r1) < epsilon&& fabs(themax - b1) < epsilon) // Red and Blue are equal and maximum
					h[i][j] = 300; // Magenta
			if (h[i][j]<0){h[i][j] += 360;}
			//if(!(j%50) && !(i%20)) printf("hue: %d\n", h[i][j]);
		}
	}
	int satHist[256] = {0};
	long int satAccu[256] = {0};
	for(i=0; i<2460; i++)  // Height
	{
		for(j=0; j<3360; j++)  // Width
		{
           	 satHist[(int)min((s[i][j] * 255), 255.0)]++;
		}
	}
	satAccu[0] = satHist[0];
	for (i=1;i<256; i++){
		satAccu[i] = satAccu[i-1] + satHist[i];
	}
	int satMin = 0;
	//finds the first nonzero value in the accumulation,
	// which is the minimum saturation value of the image. 
	for (i = 0; i < 256; ++i) {
    if (satAccu[i] != 0) {
        satMin = satAccu[i];
        break;
    	}
	}
	//total of all values-min value
	double satRange = satAccu[255]-satMin;
	double normSat[256];
	for (i=0;i<256;i++){
			normSat[i] = (satAccu[i] - satMin)/satRange;
	}
	/*goals: enhance greens -- not so much blues and reds*/
	for(i=0; i<2460; i++) { //Height
		for(j=0; j<3360; j++)  // Width
		{
				//1.0 / (1.0 + exp(-steepness * (v - midpoint)));
				s[i][j] = (((r[i][j]-b[i][j]) > contrast) && ((r[i][j]-g[i][j])> contrast))||((g[i][j]-b[i][j]>contrast) && (g[i][j]-r[i][j]> contrast))|| ((b[i][j]-r[i][j]>contrast)&& ((b[i][j]-g[i][j])> contrast)) ? normSat[(int)(s[i][j] * 255)] : s[i][j];
		}
	}

	//examine greens in this colorspace and increase their saturation specifically here...
	//convert back
	//https://www.rapidtables.com/convert/color/hsv-to-rgb.html

	for ( i = 0; i < 2460; ++i) {
        for ( j = 0; j < 3360; ++j) {
            double c = v[i][j] * s[i][j];
            double x = c * (1 - fabs(fmod(h[i][j] / 60.0, 2) - 1));
            double m = v[i][j] - c;
            double r1, g1, b1;

            if (0 <= h[i][j] && h[i][j] < 60) {
                r1 = c, g1 = x, b1 = 0;
            } else if (60 <= h[i][j] && h[i][j] < 120) {
                r1 = x, g1 = c, b1 = 0;
            } else if (120 <= h[i][j] && h[i][j] < 180) {
                r1 = 0, g1 = c, b1 = x;
            } else if (180 <= h[i][j] && h[i][j] < 240) {
                r1 = 0, g1 = x, b1 = c;
            } else if (240 <= h[i][j] && h[i][j] < 300) {
                r1 = x, g1 = 0, b1 = c;
            } else {
                r1 = c, g1 = 0, b1 = x;
            }

            r[i][j] = (r1 + m) * 255;
            g[i][j] = (g1 + m) * 255;
            b[i][j] = (b1 + m) * 255;
        }
	}
	printf("Saturation equalization ---OK!\n");
}

vector<vector<float> > generate2DGaussianKernel(int kernel_size, float sigma)
{
	//sigma = std of Gaussian distribution - spread of bell curve = 2 (spatial sigma)
	//2D vector stores kernel values; each element = Gaussian value before normalization

	printf("Generating Gaussian\n");
	vector<vector<float> > kernel(kernel_size, vector<float>(kernel_size));
	//peak of gaussian value
	int center = kernel_size / 2;
	//printf("%d\n", center);
	float total = 0;
	for (int i=0; i<kernel_size;i++)
	{
		for(int j=0;j<kernel_size;j++)
		{
			int x = abs(i-center); //horizantal distance
			int y = abs(j-center); //vertical distance
			//Gaussian Function calculation
			//float val = -(1-((x*x+y*y)/(2*sigma*sigma)))*exp((-((x*x) + (y*y))) / (2 * sigma * sigma)) / (M_PI * sigma*sigma*sigma*sigma);
			float val = exp((-((x*x) + (y*y))) / (2 * sigma * sigma)) / (2 * M_PI * sigma*sigma);
			kernel[i][j] = val; 
			total += val;
		}
	}

	for (int i=0;i<kernel_size;i++)
	{
		for(int j=0;j<kernel_size;j++)
		{
			kernel[i][j] /= total; //normalize weights such that sum of all elements = 1
		}
	}
	return kernel;
}



void bilateralSmoothing(int kernel_size, float spatial_sigma, float intensity_sigma)
{
	printf("Bilateral Smoothing\n");
	

	/*
Small σ_s and Small σ_r: Fine detail is preserved, but less noise reduction.
Small σ_s and Large σ_r: Fine detail is preserved, but smoother regions might blur more.
Large σ_s and Small σ_r: Broader smoothing with edge preservation.
Large σ_s and Large σ_r: Broad smoothing with less edge preservation, potentially useful for heavy noise reduction.
	*/
	memset(new_r, 0, sizeof(new_r));
	memset(new_g, 0, sizeof(new_g));
	memset(new_b, 0, sizeof(new_b));
	memset(new_e, 0, sizeof(new_e));


	//where does smoothing go in the processing pipeline
	// Get normal spatial kernel
	vector<vector<float> > kernel = generate2DGaussianKernel(kernel_size, spatial_sigma);


	cout << "Calculated 2D Kernel" << endl;
	for(int i = 0; i <2460 ; i += 1)
	{
		for(int j = 0 ; j<3330; j += 1)
		{

			float center_r = r[i][j];
			float center_g = g[i][j];
			float center_b = b[i][j];

			vector<vector<float> > intensity_kernel(kernel_size, vector<float>(kernel_size));

			// Get radiometric kernel.
			// Base distance on euclidean RGB color-space distance (too complex to change color-spaces)
			float total = 0;
			int temp = kernel_size / 2;
			for (int ii=0; ii<kernel_size;ii++)
			{
				for (int jj=0;jj<kernel_size;jj++)
				{
					float dist_r = 0;
					float dist_g = 0;
					float dist_b = 0;
					if (i+ii-temp < 2460 && j+jj-temp < 3330 && i+ii-temp > 0 && j+jj-temp > 0)
					{
						dist_r = center_r - r[i+ii-temp][j+jj-temp];
						dist_g = center_g - g[i+ii-temp][j+jj-temp];
						dist_b = center_b - b[i+ii-temp][j+jj-temp];
					}
					float dist = abs(dist_r) + abs(dist_g) + abs(dist_b);

					float val = exp(-dist/(2*intensity_sigma*intensity_sigma)) / (sqrt(2*M_PI)*intensity_sigma);
					intensity_kernel[ii][jj] = val;	
					total += val;
				}
			}
			for(int ii=0;ii<kernel_size;ii++)
			{
				for(int jj=0;jj<kernel_size;jj++)
				{
					intensity_kernel[ii][jj] /= total;
				}
			}

			vector<vector<float> > final_kernel(kernel_size, vector<float>(kernel_size));
			float total_kernel = 0;
			for (int ii=0;ii<kernel_size;ii++)
			{
				for(int jj=0;jj<kernel_size;jj++)
				{
					final_kernel[ii][jj] = 0;
					for (int kk=0;kk<kernel_size;kk++)
					{
						float val = kernel[ii][kk] * intensity_kernel[kk][jj];
						final_kernel[ii][jj] += val;
						total_kernel += val;
					}
				}
			}

			//cout << "Mutiplied kernel" << endl;
			// Normalize kernel
			for(int ii=0;ii<kernel_size;ii++)
			{
				for(int jj=0;jj<kernel_size;jj++)
				{
					final_kernel[ii][jj] /= total_kernel;
				}
			}

			//cout << "Normalized Kernel" << endl;

			for(int ii=0;ii<kernel_size;ii++)
			{
				for(int jj=0;jj<kernel_size;jj++)
				{
					if (i+ii-temp < 2460 && j+jj-temp < 3330 && i+ii-temp>0 && j+jj-temp>0)
					{
						new_r[i][j] += r[i+ii-temp][j+jj-temp] * final_kernel[ii][jj];
						new_b[i][j] += b[i+ii-temp][j+jj-temp] * final_kernel[ii][jj];			
						new_g[i][j] += g[i+ii-temp][j+jj-temp] * final_kernel[ii][jj];
						new_e[i][j] += e[i+ii-temp][j+jj-temp] * final_kernel[ii][jj];
					}
				}
			}


			//cout << "End Loop" << endl;
		}
	}

	for (int i=0;i<2460;i++)
	{
		for (int j=0;j<3330;j++)
		{
			r[i][j] = new_r[i][j]; 
			b[i][j] = new_b[i][j];
			g[i][j] = new_g[i][j];
			e[i][j] = new_e[i][j];
		}
	}
}
