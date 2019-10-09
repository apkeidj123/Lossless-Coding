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
    double freq[256]={0.0};
    double prob[256]={0.0};
    double sum = 0.0,entropy=0.0;

    FILE *f0 =fopen("lena.raw","rb");
        fread(image,sizeof(unsigned char),height*weight,f0);
    fclose(f0);

    memset( (void*)img , 0 , sizeof(int) * height * weight );//initialize 0

    for(int i = 0 ; i < height ; i++){ //char to 0~255
        for(int j = 0 ; j < weight ; j++){
                img[i][j]=0+image[i][j];

        }
    }

    for(int i = 0 ; i < height ; i++){ //frequency
        for(int j = 0 ; j < weight ; j++){
            for(int k = 0 ; k < 256 ; k++){
                if((0+image[i][j])==k){
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
    printf("entropy = %.6lf\n",entropy);
    //for(int i = 0 ; i < 256 ; i++){cout<<"frequency "<<i<<"= "<<freq[i]<<endl;}
    //for(int i = 0 ; i < 256 ; i++){cout<<"probability "<<i<<"= "<<prob[i]<<endl;}

    return 0;
}

