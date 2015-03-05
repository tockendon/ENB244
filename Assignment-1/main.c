#include "hadamard.h"
#include <stdio.h>
#include <stdlib.h>

void print_usage();

int main(int argc, char** argv) {
	int i,ii;
	int N;
	char* fin;
	char* fout;	
	int format,u,v,depth;
	int arg1,arg2,arg3,arg4;	
	char** h;
	unsigned long long* lookup;
	char** img;
	unsigned long long* encin;
	if(argc < 2) {
		print_usage();		
	} else if(argv[1][0] == '-') {
		for(i = 4; i < argc; i++) {
			switch(i) {
				case 4: 
					arg1 = atoi(argv[4]);
					break;
				case 5:
					arg2 = atoi(argv[5]);
					break;
				case 6:
					arg3 = atoi(argv[6]);
					break;
				case 7:
					arg4 = atoi(argv[7]);
					break;
			}
		}		
		switch(argv[1][1]) {	
			case 'e':								// ENCRYPTION
				// check for right amount of variables			
				if(argc !=6) {
					print_usage();
					return;
				}				
				// initialise variables
				fin = argv[2];
				fout = argv[3];				
				u = arg1;
				v = arg2;				
					// find the N value
					FILE *infile;
					infile = fopen(fin, "rb");				
					fscanf(infile, "%s %d %d %d ",&format,&u,&v,&depth);
					rewind(infile);
					fclose(infile);
					N = ((depth+1)/2);					
						// allocate h
						h = malloc(N*sizeof(char*));
						for (ii=0;ii<N;ii++) {
							h[ii]= malloc(N*sizeof(char));
						}
						// allocate lookup 
						lookup = malloc(2*N*sizeof(unsigned long long));
						// allocate img
						img = malloc(v*sizeof(char*));
						for (ii=0;ii<v;ii++) {
							img[ii]= malloc(u*sizeof(char));
						}				
					// run the encryption routine
					create_hadamard_matrix(h,N);				
					convert_to_lookup_table(lookup,h,N);	
					pgm_read(fin,img,u,v);				
					enc_write(fout,img,u,v,lookup);				
				// free h
				for (ii=0; ii<N; ii++){
					free(h[ii]);
				}				
				free(h);
				// free lookup
				free(lookup);
				// free img
				for (ii=0; ii<v; ii++){
					free(img[ii]);
				}
				free(img);				
				break;			
			case 'd':								// DECRYPTION
				// check for right amount of variables
				if(argc != 8) {
					print_usage();
					return;
				}				
				// initialise variables
				fin = argv[2];
				fout = argv[3];
				format = arg1;
				u = arg2;
				v = arg3;
				depth = arg4;
				N = ((depth+1)/2);				
				if(depth > 127) {
					printf("\nError: Cannot handle characters with value greater than 127.\n", fout);
				return;
				}						
					// allocate h
					h = malloc(N*sizeof(char*));
					for (ii=0;ii<N;ii++) {
						h[ii]= malloc(N*sizeof(char));
					}
					// allocate lookup 
					lookup = malloc(2*N*sizeof(unsigned long long));
					// allocate img
					img = malloc(v*sizeof(char*));
					for (ii=0;ii<v;ii++) {
						img[ii]= malloc(u*sizeof(char));
					}
					// allocate ecin
					encin = malloc(u*v*sizeof(unsigned long long)); 					
						// run the decryption routine
						create_hadamard_matrix(h,N);				
						convert_to_lookup_table(lookup,h,N);				
						enc_read(encin,fin,u,v);				
						dec_img(img,encin,lookup,u,v,N);				
						pgm_write(fout,img,format,u,v,depth);					
				// free h
				for (ii=0; ii<N; ii++){
					free(h[ii]);
				}				
				free(h);
				// free lookup
				free(lookup);
				// free img
				for (ii=0; ii<v; ii++){
					free(img[ii]);
				}
				free(img);
				// free encin
				free(encin);				
				break;			
			case 'p':								// PACK
				if(argc != 4) {
					print_usage();
					return;
				}
				fin = argv[2];
				fout = argv[3];	
				enc_pack(fin,fout);				
				break;					
			case 'u':								// UNPACK
				if(argc != 4) {
					print_usage();
					return;
				}
				fin = argv[2];
				fout = argv[3];					
				enc_unpack(fin,fout);				
				break;					
			case 'm':								// MESSAGE DECRYPT
				// check for right amount of variables
				if(argc != 6) {
					print_usage();
					return;
				}					
				// initialise variables
				fin = argv[2];
				fout = argv[3];
				u = arg1;
				v = arg2;				
				N = 64;				
					// allocate h
					h = malloc(N*sizeof(char*));
					for (ii=0;ii<N;ii++) {
						h[ii]= malloc(N*sizeof(char));
					}
					// allocate lookup 
					lookup = malloc(2*N*sizeof(unsigned long long));
					// allocate img
					img = malloc(v*sizeof(char*));
					for (ii=0;ii<v;ii++) {
						img[ii]= malloc(u*sizeof(char));
					}
					// allocate ecin
					encin = malloc(u*v*sizeof(unsigned long long)); 					
						// run alien decryption routine	
						create_hadamard_matrix(h,N);
						convert_to_lookup_table(lookup,h,N);
						enc_read(encin,fin,u,v);
						dec_msg(fout,encin,lookup,u,v,N);				
				// free h
				for (ii=0; ii<N; ii++){
					free(h[ii]);
				}				
				free(h);
				// free lookup
				free(lookup);
				// free img
				for (ii=0; ii<v; ii++){
					free(img[ii]);
				}
				free(img);
				// free encin
				free(encin);				
				break;				
			default:
				print_usage();
				return;			
	}			
	} else { 
	print_usage();
	return;
	}	
return;
}
void print_usage() {
	printf("\nUsage: hadamard [option] <file 1> <file 2> {format u v depth}\n\n");
	printf("Option:\n\n");
	printf("\t-e encrypt file 1 and place in file 2 using u v\n");
	printf("\t-d decrypt file 1 and place in file 2 using format u v depth\n");
	printf("\t-p pack encoded data from file 1 into file 2\n");
	printf("\t-u unpack encoded data from file 1 into file 2\n");
	printf("\t-m extract alien message from file 1 and place in file 2 using u v\n\n");
	printf("Examples:\n\n");
	printf("\thadamard -e input.pgm out.enc 832 700\n");
	printf("\thadamard -d out.enc out.pgm 5 832 700 127\n");
	printf("\thadamard -p input.pgm input.pak\n");
	printf("\thadamard -u input.pak input.pgm\n");
	printf("\thadamard -m out.enc msg.txt 832 700\n");
}
