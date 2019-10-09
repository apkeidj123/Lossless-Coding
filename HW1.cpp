#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <memory.h>
#include <iomanip>
#include <math.h>
#include <cstring>

#include "bitio.c"
#include "errhand.c"
#include "main.h"

#include "Adaptive Huffman.c"
#include "Arithmetic.c"
#include "Huffman.c"
#include "LZSS.c"
#include "LZW.c"

#define height 256
#define weight 256

#ifdef __STDC__

void usage_exit( char *prog_name );
void print_ratios( char *input, char *output );
long file_size( char *name );

#else
void usage_exit();
void print_ratios();
long file_size();

#endif

using namespace std;

int main()
{
    int choice1,choice2,choice3;
    char inputFilename[100],outputFilename[100];
    int argc=3;
    char *argv[3]={"HW1",inputFilename,outputFilename};

    unsigned char image[height][weight];
    int img[height][weight];
    int residual[height][weight];//after DPCM
    double freq[256]={0.0};
    double prob[256]={0.0};
    double sum = 0.0,entropy=0.0;

    memset( (void*)img , 0 , sizeof(int) * height * weight );//initialize 0
    memset( (void*)residual , 0 , sizeof(int) * height * weight );//initialize 0

    bool Exit = false;

    printf("Name: Liang-Kun Liu\nStudnet ID:103502043\n*************\n");

    printf("*Input filename: ");
    scanf("%s",inputFilename);
    //gets(buf); /* 讀入檔名 */
    //sprintf(in, """%s""", buf); // 為避免有空白,加入雙引號
    printf("*Output filename: ");
    scanf("%s",outputFilename);
    //gets(buf2);
    //sprintf(out, """%s""", buf2); // 為避免有空白,加入雙引號

    printf("*Choose one compression method:\n");
    printf("1)Huffman coding\n");
    printf("2)Adaptive Huffman coding\n");
    printf("3)Arithmetic coding\n");
    printf("4)LZSS\n");
    printf("5)LZW\n");
    printf("Your choice(1~5, other to exit):");

    scanf("%d",&choice1);

    printf("*Coding operation\n");
    printf("1)Encoding\n");
    printf("2)Decoding\n");
    printf("Your choice(1 or 2, other to exit):");

    scanf("%d",&choice2);

    printf("*Data\n");
    printf("1)Original data\n");
    printf("2)DPCM\n");
    printf("Your choice(1 or 2, other to exit):");

    scanf("%d",&choice3);

/******************choice3:Data******(1)start*********************/

    if(choice3==1){//original data
        FILE *f0 =fopen(argv [ 1 ],"rb");
            fread(image,sizeof(unsigned char),height*weight,f0);
        fclose(f0);

        for(int i = 0 ; i < height ; i++){ //char to 0~255
            for(int j = 0 ; j < weight ; j++){
                    img[i][j]=0+image[i][j];
            }
        }
        //frequency
        for(int i = 0 ; i < height ; i++){
            for(int j = 0 ; j < weight ; j++){
                for(int k = 0 ; k < 256 ; k++){
                    if(img[i][j]==k){
                        freq[k]+=1;
                    }
                }
            }
        }
        //for(int i = 0 ; i < 256 ; i++){cout<<"frequency "<<i<<"= "<<freq[i]<<endl;}
        for(int i = 0 ; i < 256 ; i++){sum+=freq[i];}//total pixel
        for(int i = 0 ; i < 256 ; i++){prob[i]=freq[i]/sum;} //probability
        //calculate entropy
        for(int i = 0 ; i < 256 ; i++){
            if(prob[i]!=0)
                entropy-= prob[i]*log2(prob[i]);
        }
        //cout<<"Original_data_Entropy = "<<setprecision(6)<<entropy<<endl;
        if(choice2!=2)
            printf("\nOriginal_data_Entropy = %.6lf\n",entropy);

/******************choice3:Data******(1)end***********************/

/******************choice2:Coding operation*****(1)start*********************/

        if(choice2==1&&!Exit){
            BIT_FILE *output;
            FILE *input;

            setbuf( stdout, NULL );

            input = fopen(argv [ 1 ], "rb" );
            if ( input == NULL )
                fatal_error( "Error opening %s for input\n", argv[ 1 ] );
            output = OpenOutputBitFile( argv[ 2 ] );
            if ( output == NULL )
                fatal_error( "Error opening %s for output\n", argv[ 2 ] );
            printf( "\nCompressing %s to %s\n", argv[ 1 ], argv[ 2 ] );

/******************choice2:Coding operation*****(1)end*********************/

/******************choice1:Compression method*****start*********************/

            if(choice1==1&&!Exit){
                printf( "Using %s\n", HCompressionName );
                HCompressFile( input, output, argc - 3, argv + 3 );
            }
            else if(choice1==2&&!Exit){
                printf( "Using %s\n", AHCompressionName );
                AHCompressFile( input, output, argc - 3, argv + 3 );
            }
            else if(choice1==3&&!Exit){
                printf( "Using %s\n", ACompressionName );
                ACompressFile( input, output, argc - 3, argv + 3 );
            }
            else if(choice1==4&&!Exit){
                printf( "Using %s\n", LSCompressionName );
                LSCompressFile( input, output, argc - 3, argv + 3 );
            }
            else if(choice1==5&&!Exit){
                printf( "Using %s\n", LWCompressionName );
                LWCompressFile( input, output, argc - 3, argv + 3 );
            }
            else{
                Exit=true;
            }

            if(!Exit){
                CloseOutputBitFile( output );
                fclose( input );
                print_ratios( argv[ 1 ], argv[ 2 ] );
            }
            else{
                fclose( input );
            }

        }
/******************choice1:Compression method*****end*********************/

/******************choice2:Coding operation*****(2)start*********************/

        else if(choice2==2&&!Exit){
            FILE *output;
            BIT_FILE *input;

            setbuf( stdout, NULL );

            input = OpenInputBitFile( argv[ 1 ] );
            if ( input == NULL )
                fatal_error( "Error opening %s for input\n", argv[ 1 ]);
            output = fopen( argv[ 2 ], "wb" );
            if ( output == NULL )
                fatal_error( "Error opening %s for output\n", argv[ 2 ]);
            printf( "\nExpanding %s to %s\n", argv[ 1 ], argv[ 2 ] );

/******************choice2:Coding operation*****(2)end*********************/

            if(choice1==1&&!Exit){
                printf( "Using %s\n", HCompressionName );
                HExpandFile( input, output, argc - 3, argv + 3 );
            }
            else if(choice1==2&&!Exit){
                printf( "Using %s\n", AHCompressionName );
                AHExpandFile( input, output, argc - 3, argv + 3 );
            }
            else if(choice1==3&&!Exit){
                printf( "Using %s\n", ACompressionName );
                AExpandFile( input, output, argc - 3, argv + 3 );
            }
            else if(choice1==4&&!Exit){
                printf( "Using %s\n", LSCompressionName );
                LSExpandFile( input, output, argc - 3, argv + 3 );
            }
            else if(choice1==5&&!Exit){
                printf( "Using %s\n", LWCompressionName );
                LWExpandFile( input, output, argc - 3, argv + 3 );
            }
            else{
                Exit=true;
            }

            if(!Exit){
                CloseInputBitFile( input );
                fclose( output );
                putc( '\n', stdout );
            }
            else{
                fclose( output );
            }

        }
        else{
            Exit=true;
        }

    }
/******************choice1:Compression method*****end*********************/

/******************choice2:Coding operation*****(2)end*********************/

/******************choice3:Data******(2)start*********************/

    else if(choice3==2){//DPCM
        FILE *f0 =fopen(argv [ 1 ],"rb");
            fread(image,sizeof(unsigned char),height*weight,f0);
        fclose(f0);

        for(int i = 0 ; i < height ; i++){ //char to 0~255
            for(int j = 0 ; j < weight ; j++){
                img[i][j]=0+image[i][j];
            }
        }
        //predictor
        float z=1.0/3.0;
        for(int i = 1 ; i < height ; i++){
            for(int j = 1 ; j < weight ; j++){
                residual[i][j]=ceil(z*img[i-1][j]+z*img[i-1][j-1]+z*img[i][j-1]-0.5);
            }
        }
        //compute residual
        for(int i = 1 ; i < height ; i++){
            for(int j = 1 ; j < weight ; j++){
                residual[i][j]-=img[i][j];
                if(residual[i][j]<0){residual[i][j] *= -1;}
            }
        }
        //0~255 to char
        for(int i = 0 ; i < height ; i++){
            for(int j = 0 ; j < weight ; j++){
                    image[i][j]=residual[i][j];
            }
        }
        FILE *f1 =fopen(argv [ 1 ],"wb");
            fwrite(image,1,height*weight,f1);
        fclose(f1);
        //frequency
        for(int i = 0 ; i < height ; i++){
            for(int j = 0 ; j < weight ; j++){
                for(int k = 0 ; k < 256 ; k++){
                    if((residual[i][j])==k){
                        freq[k]+=1;
                    }
                }
            }
        }
        for(int i = 0 ; i < 256 ; i++){sum+=freq[i];}//total pixel
        for(int i = 0 ; i < 256 ; i++){prob[i]=freq[i]/sum;} //probability
        //calculate entropy
        for(int i = 0 ; i < 256 ; i++){
            if(prob[i]!=0)
                entropy-= prob[i]*log2(prob[i]);
        }
        //cout<<"DPCM_Entropy = "<<setprecision(6)<<entropy<<endl;
        if(choice2!=2)
            printf("\nDPCM_Entropy = %.6lf\n",entropy);

/******************choice3:Data******(2)end*********************/

/******************choice2:Coding operation*****(1)start*********************/

        if(choice2==1&&!Exit){
            BIT_FILE *output;
            FILE *input;

            setbuf( stdout, NULL );

            input = fopen(argv [ 1 ], "rb" );
            if ( input == NULL )
                fatal_error( "Error opening %s for input\n", argv[ 1 ] );
            output = OpenOutputBitFile( argv[ 2 ] );
            if ( output == NULL )
                fatal_error( "Error opening %s for output\n", argv[ 2 ] );
            printf( "\nCompressing %s to %s\n", argv[ 1 ], argv[ 2 ] );

/******************choice2:Coding operation*****(1)end*********************/

/******************choice1:Compression method*****start*********************/

            if(choice1==1&&!Exit){
                printf( "Using %s\n", HCompressionName );
                HCompressFile( input, output, argc - 3, argv + 3 );
            }
            else if(choice1==2&&!Exit){
                printf( "Using %s\n", AHCompressionName );
                AHCompressFile( input, output, argc - 3, argv + 3 );
            }
            else if(choice1==3&&!Exit){
                printf( "Using %s\n", ACompressionName );
                ACompressFile( input, output, argc - 3, argv + 3 );
            }
            else if(choice1==4&&!Exit){
                printf( "Using %s\n", LSCompressionName );
                LSCompressFile( input, output, argc - 3, argv + 3 );
            }
            else if(choice1==5&&!Exit){
                printf( "Using %s\n", LWCompressionName );
                LWCompressFile( input, output, argc - 3, argv + 3 );
            }
            else{
                Exit=true;
            }

            if(!Exit){
                CloseOutputBitFile( output );
                fclose( input );
                print_ratios( argv[ 1 ], argv[ 2 ] );
            }
            else{
                fclose( input );
            }

        }
/******************choice1:Compression method*****end*********************/

/******************choice2:Coding operation*****(2)start*********************/

        else if(choice2==2&&!Exit){
            FILE *output;
            BIT_FILE *input;

            setbuf( stdout, NULL );

            input = OpenInputBitFile( argv[ 1 ] );
            if ( input == NULL )
                fatal_error( "Error opening %s for input\n", argv[ 1 ]);
            output = fopen( argv[ 2 ], "wb" );
            if ( output == NULL )
                fatal_error( "Error opening %s for output\n", argv[ 2 ]);
            printf( "\nExpanding %s to %s\n", argv[ 1 ], argv[ 2 ] );

/******************choice2:Coding operation*****(2)end*********************/

/******************choice1:Compression method*****start*********************/

            if(choice1==1&&!Exit){
                printf( "Using %s\n", HCompressionName );
                HExpandFile( input, output, argc - 3, argv + 3 );
            }
            else if(choice1==2&&!Exit){
                printf( "Using %s\n", AHCompressionName );
                AExpandFile( input, output, argc - 3, argv + 3 );
            }
            else if(choice1==3&&!Exit){
                printf( "Using %s\n", ACompressionName );
                AExpandFile( input, output, argc - 3, argv + 3 );
            }
            else if(choice1==4&&!Exit){
                printf( "Using %s\n", LSCompressionName );
                LSExpandFile( input, output, argc - 3, argv + 3 );
            }
            else if(choice1==5&&!Exit){
                printf( "Using %s\n", LWCompressionName );
                LWExpandFile( input, output, argc - 3, argv + 3 );
            }
            else{
                Exit=true;
            }

            if(!Exit){
                CloseInputBitFile( input );
                fclose( output );
                putc( '\n', stdout );
            }
            else{
                fclose( output );
            }

        }
        else{
            Exit=true;
        }

    }
/******************choice1:Compression method*****end*********************/

/******************choice2:Coding operation*****(2)end*********************/


    if(!Exit){
        printf("...Program is running and generating useful information\n");
        printf("...Writing file...\n");
        printf("...Done.\n");
    }

    system("pause");
    return 0;
}
#ifndef SEEK_END
#define SEEK_END 2
#endif


long file_size( char *name )
{
	long eof_ftell;
	FILE *file;

	file = fopen( name, "r");
	if ( file == NULL )
		return( 0L );
	fseek( file, 0L, SEEK_END );
	eof_ftell = ftell( file );
	fclose( file );
	return( eof_ftell );
}

void print_ratios( char *input, char *output )
{
	long input_size;
	long output_size;
	int ratio;
	input_size = file_size( input );
	if ( input_size == 0 )
		input_size = 1;
	output_size = file_size ( output );
	ratio = 100 - (int) ( output_size * 100L / input_size );
	printf( "\nInput bytes:	%ld\n", input_size );
	printf( "Output bytes:	%ld\n", output_size );
	if ( output_size == 0 )
		output_size = 1;
	printf( "Compression ratio: %d%%\n\n", ratio );
}
