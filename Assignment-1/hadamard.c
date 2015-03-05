#include "hadamard.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

// create the hadamard matrix
void create_hadamard_matrix(char** h, int N) {	
	int n,u,v;
	h[0][0] = 1;
	for (n = 2; n <= N; n *= 2) {	
		for (u = 0;  u < n/2; u++) {		
			for (v = 0; v < n/2; v++) {			
				h[u][v+n/2] = h[u][v];				
				h[u+n/2][v] = h[u][v];					
				h[u+n/2][v+n/2] = h[u][v]^1;				
			}
		}
	}	
return;
}

// convert to lookup table
void convert_to_lookup_table(unsigned long long* lookup, char **h, int N) {	
	int m,n;
	unsigned long long ones;	
	if(N < 64) {	
		ones = ~(~0llu << (N));		
	} else {	
		ones = 0xffffffffffffffffllu;		
	}	
	for(m = 0; m <N ; m++) {
		lookup[m]=0;		
		for(n = (N-1); n >= 0 ; n--) {
			if ((h[m][n]) && ((unsigned long long) 1 << n)) {	
				lookup[m]|= (unsigned long long) 1 << n;				
			} else {			
				lookup[m+N]|= (unsigned long long) 0 << abs(n-N);
			}
		}
	}	
	for(m = 0; m <(N);m++) {	
		lookup[m+N]=0;		
		lookup[m+N] = lookup[m]^ones;		
	}
return;
}

// read in a pgm image
void pgm_read(char* fin, char** img, int u, int v) {	
	int i, j;	
	FILE *infile;
	infile = fopen(fin, "rb");	
	if(infile == NULL) {	
		printf("\nError: Could not open file %s in mode rb\n", fin);		
		return;
	}	
	char type[3], rand;
	int x, y, max;	
	fscanf(infile, "%s %d %d %d %d %c",&type,&x,&y,&max,&rand);
	for(i = 0; i < v ; i++) {	
		for(j = 0 ; j < u ; j++) {		
			img[i][j] = (unsigned char)fgetc(infile);			
		}
	}	
	fclose(infile);		
return;
}

// write out a pgm image
void pgm_write(char* fout, char** img,  int format, int u, int v, int depth) {	
	char type[3];	
	switch(format) {
		case 1:
			sprintf(type, "%s", "P1");		
		break;
		case 2:
			sprintf(type, "%s", "P2");
		break;
		case 3:
			sprintf(type, "%s", "P3");
		break;
		case 4:
			sprintf(type, "%s", "P4");
		break;
		case 5:
			sprintf(type, "%s", "P5");
		break;
		default:
			sprintf(type, "%s", "P5");;
		break;
	}	
	FILE *outfile;
	outfile = fopen(fout, "wb");	
	fprintf(outfile, "%s %d %d %d\n",&type,u,v,depth);		
	if(depth > 127) {	
		printf("\nError: Cannot handle characters with value greater than 127.\n", fout);
	return;	
	}
	if(outfile == NULL) {	
		printf("\nError: Could not open file %s in mode wb\n", fout);		
		return;		
	}	
	int i, j;	
	for(i = 0; i < v ; i++) {	
		for(j = 0 ; j < u ; j++) {		
			fputc(img[i][j], outfile);			
		}
	}		
	fclose(outfile);	
return;
}

// write the encoded image out 
void enc_write(char* fout, char** img, int u, int v, unsigned long long* lookup) {	
	int i, j;		
	FILE *outfile;
	outfile = fopen(fout, "wb");	
	for(i = 0; i < v ; i++) {	
		for(j = 0 ; j < u ; j++) {		
			fwrite(&lookup[img[i][j]], 8, 1,outfile);			
		}
	}	
	fclose(outfile);	
return;
}

// read the encoding in
void enc_read(unsigned long long* encin, char* fin, int u, int v) {	
	FILE *infile;
	infile = fopen(fin, "rb");	
	if(infile == NULL) {	
		printf("\nError: Could not open file %s in mode rb\n", fin);		
		return;
	}	
	fread(encin, 8, u*v,infile);
	fclose(infile);	
return;
}

// decode the image
void dec_img(char** img, unsigned long long* encin, unsigned long long* lookup, int u, int v, int N) {
	int i,j,k,bit;
	unsigned int row;
	int count_flips;
	unsigned long long flip_check = 0x00llu;
	int flip_minimum = N;	
	k=0;	
	for(i = 0; i < v; i++) {	
		for(j = 0; j < u; j++) {		
			flip_minimum = N;			
			for(row = 0; row < N*2; row++) {				
				flip_check = encin[k]^lookup[row];				
				count_flips = BITCOUNT(flip_check);				
				if(count_flips < flip_minimum) {				
					flip_minimum = count_flips;					
					img[i][j] = row;					
				}
			}			
		k++;		
		}		
	}	
return;
}
 
// pack encoded image and write to file 
void enc_pack(char* fin, char* fout) {
	char type[3];
	int u, v, depth;	
	FILE *infile;
	infile = fopen(fin, "rb");
	fscanf(infile, "%s %d %d %d\n",&type,&u,&v,&depth);	
	if(depth > 127) {	
		printf("\nError: Cannot handle characters with value greater than 127.\n", fout);
	return;
	}	
	if(infile == NULL) {	
		printf("\nError: Could not open file %s in mode rb\n", fin);		
		return;
	}	
	FILE *outfile;
	outfile = fopen(fout, "wb");	
	fprintf(outfile, "%s %d %d %d\n",&type,u,v,depth);		
	if(depth > 127) {	
		printf("\nError: Cannot handle characters with value greater than 127.\n", fout);
	return;
	}	
	if(outfile == NULL) {		
		printf("\nError: Could not open file %s in mode wb\n", fout);	
	return;
	}	
	unsigned char a,b;
	int shift = 1;
	int num_char =0;
	int long long i;
	int counter;
	int c;	
	if((u*v) % 8 == 0) {	
		c = 1;		
	} else {	
		c = 0;		
	}	
	while ( fgetc(infile) != EOF) {	
		num_char++;		
	}	
	unsigned char copy_array[(num_char)];	
	rewind(infile);	
	fscanf(infile, "%s %d %d %d\n",&type,&u,&v,&depth);	
	for( i = 0 ; i < (num_char) ; i++) {	
		copy_array[i] = (unsigned char) fgetc(infile);		
	}		
	if(num_char == 1) {	
		a = copy_array[0];		
		a <<= 1;		
		fputc(a, outfile);		
	} else {	
		for(i = 0; i < (num_char-c); i++) {			
			if(shift > 7) {			
				shift = 1;				
				++i;
			}					
			a=copy_array[i];				
			a<<= shift;					
			if( i < num_char-c) {			
				b = copy_array[(i+1)];				
			} else {			
				b = 0b00000000;
			}			
			b >>= (7-shift);				
			fputc(a|b,outfile);				
			shift++;
			counter++;			
		} 
	}	
	fclose(outfile);
	fclose(infile);
	
return;
} 
	
// unpack encoded image and write to file
void enc_unpack(char* fin, char* fout) {
	char type[3];
	int u, v, depth;	
	FILE *infile;
	infile = fopen(fin, "rb");
	fscanf(infile, "%s %d %d %d\n",&type,&u,&v,&depth);	
	if(depth > 127) {
		printf("\nError: Cannot handle characters with value greater than 127.\n", fout);
	return;
	}	
	if(infile == NULL) {
		printf("\nError: Could not open file %s in mode rb\n", fin);
		return;
	}	
	FILE *outfile;
	outfile = fopen(fout, "wb");	
	fprintf(outfile, "%s %d %d %d\n",&type,u,v,depth);		
	if(depth > 127) {
		printf("\nError: Cannot handle characters with value greater than 127.\n", fout);
	return;
	}	
	if(infile == NULL) {
		printf("\nError: Could not open file %s in mode wb\n", fout);
		return;
	}	
	unsigned char a,b;
	int shift;
	int num_char =0;
	int i = 0;
	int k = 0;
	int counter;	
	while ( fgetc(infile) != EOF) {
		num_char++;
	}
	unsigned char copy_array[(num_char)];
	rewind(infile);
	fscanf(infile, "%s %d %d %d\n",&type,&u,&v,&depth);
	for( i = 0 ; i < (num_char) ; i++) {
		copy_array[i] = (unsigned char) fgetc(infile);
	}		
	shift = 0;
	for(i = 0; i < (u*v); i++) {	
		if(k < (u*v)) {
			if(shift > 7) { 
				shift = 0; 
			}				
			a = copy_array[i];			
			if ( i == 0 )  {
				b = 0b00000000;			
			} else if(shift ==0 && i > 0) { 
				b = copy_array[(i-1)];
				b >>= 1;
				a = 0b00000000;
				i--;
			} else {
				b = copy_array[(i-1)];
			}
			a >>= (shift +1);
			if(shift !=0 & i > 0) {
				b <<= (8- shift);
				b >>= 1;
				fputc((a|b), outfile);
				shift++;		
			} else {
				fputc((a|b), outfile);
				shift++;
			}
			k++;
		}		
	} 
	fclose(outfile);
	fclose(infile);
return;
} 
//extract message in hadamard encoding
void dec_msg(char* fout, unsigned long long* encin, unsigned long long* lookup, int u, int v, int N) {
	char alphabet[] = {'@','#','M','B','H','A','&','G','h','9','3','X','2','5','S','i','s','r',';',':',',','.',' ','\'','"','%','=','+','!','_','\r','\n'};
	int i,j,k,row;
	int flip_minimum, count_flips;
	unsigned long long flip_check = 0x00llu;
	unsigned long long difference = 0;
	unsigned long long one = 0x8000000000000000llu;	
	FILE *outfile;
	outfile = fopen(fout, "wb");		
	for(i = 0; i < u*v; i++) {		
		flip_minimum = N;		
		for(j = 0; j < (N*2); j++) {		
			flip_check = lookup[j] ^ encin[i];
			count_flips = BITCOUNT(flip_check);
			if(count_flips < flip_minimum) {				
				flip_minimum = count_flips;				
				row = j;				
			}			
		}		
		difference = lookup[row] ^ encin[i];		
		for(k = 0; k < N; k++ ) {
			if ( difference & (unsigned long long) 1 << (63-k)) {
				fputc(alphabet[(k%32)] , outfile);
			}
		}
	}
	fclose(outfile);
return;
}

int BITCOUNT(unsigned long long n) {
int count=0;    
    while (n) {
        count += n & 0x1llu ;    
        n >>= 1 ;
	}
return count;
}
