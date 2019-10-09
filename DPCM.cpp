#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <memory.h>
#include <iomanip>
#include <math.h>

#define height 256
#define weight 256

using namespace std;

int main(){
    unsigned char image[height][weight];
    int img[height][weight];
    int residual[height][weight];//after DPCM
    double freq[256]={0.0};
    double prob[256]={0.0};
    double sum = 0.0,entropy=0.0;

    FILE *f0 =fopen("lena.raw","rb");
        fread(image,sizeof(unsigned char),height*weight,f0);
    fclose(f0);

    memset( (void*)img , 0 , sizeof(int) * height * weight );//initialize 0
    memset( (void*)residual , 0 , sizeof(int) * height * weight );

    for(int i = 0 ; i < height ; i++){ //char to 0~255
        for(int j = 0 ; j < weight ; j++){
                img[i][j]=0+image[i][j];
        }
    }

    //predictor
    float a=1.0/3.0;
    for(int i = 1 ; i < height ; i++){
        for(int j = 1 ; j < weight ; j++){
            residual[i][j]=ceil(a*img[i-1][j]+a*img[i-1][j-1]+a*img[i][j-1]-0.5);
        }
    }

    //compute residual
    for(int i = 1 ; i < height ; i++){
        for(int j = 1 ; j < weight ; j++){
            residual[i][j]-=img[i][j];
            if(residual[i][j]<0){residual[i][j] *= -1;}
        }
    }

    for(int i = 0 ; i < height ; i++){ //0~255 to char
        for(int j = 0 ; j < weight ; j++){
                image[i][j]=residual[i][j];
        }
    }

    FILE *f1 =fopen("lena_dpcm.raw","wb");
        fwrite(image,1,height*weight,f1);
    fclose(f1);

    for(int i = 0 ; i < height ; i++){ //frequency
        for(int j = 0 ; j < weight ; j++){
            for(int k = 0 ; k < 256 ; k++){
                if((residual[i][j])==k){
                    freq[k]+=1;
                }
            }
        }
    }

    for(int i = 0 ; i < 256 ; i++){sum+=freq[i];}//total pixel
    //cout<<"sum = "<<sum<<endl;

    for(int i = 0 ; i < 256 ; i++){ //probability
        prob[i]=freq[i]/sum;
    }



    for(int i = 0 ; i < 256 ; i++){//calculate entropy
        if(prob[i]!=0)
            entropy-= prob[i]*log2(prob[i]);
    }

    cout<<"entropy = "<<setprecision(10)<<entropy<<endl;


    //for(int i = 0 ; i < 256 ; i++){cout<<"frequency "<<i<<"= "<<freq[i]<<endl;}
    //for(int i = 0 ; i < 256 ; i++){cout<<"probability "<<i<<"= "<<prob[i]<<endl;}

    return 0;
}

