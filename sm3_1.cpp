#include<stdio.h>
#include <string.h>
typedef struct
{
    unsigned long long total; 
    unsigned int state[8]; 
    unsigned char buffer[64]; 
}
sm3_context;

unsigned int FF0(unsigned int x, unsigned int y, unsigned int z){
	unsigned int result = x ^ y ^ z;
	return result;
}

unsigned int FF1(unsigned int x, unsigned int y, unsigned int z){
	unsigned int result = (x & y) | (x & z) | (y & z);
	return result;
}

unsigned int GG0(unsigned int x, unsigned int y, unsigned int z){
	unsigned int result = x ^ y ^ z;
	return result;
}

unsigned int GG1(unsigned int x, unsigned int y, unsigned int z){
	unsigned int result = (x & y) | ( (~x) & z);
	return result;
}

unsigned int ROTL(unsigned int x, unsigned int n){
	unsigned int result = (((x & 0xFFFFFFFF) << n % 32) | (x >> (32 - n % 32)));
	return result;
}

unsigned int P0(unsigned int x){
	unsigned int result = x ^ ROTL(x,9) ^ ROTL(x,17);
	return result;
}

unsigned int P1(unsigned int x){
	unsigned int result = x ^ ROTL(x,15) ^ ROTL(x,23);
	return result;
}

void BiToW(unsigned int Bi[], unsigned int W[]){
    unsigned int tmp;
    for(int i = 0; i <= 15; i++)
        W[i]=Bi[i];
    for(int i = 16; i <= 67; i++){
        tmp = W[i-16] ^ W[i-9] ^ ROTL(W[i-3], 15);
        W[i] = P1(tmp) ^ ROTL(W[i-13],7) ^ W[i-6];
    }
}

void WToW1(unsigned int* W, unsigned int* W1){
    for (int i = 0; i <= 63; i++)
        W1[i] = W[i] ^ W[i+4];
}

void CF(unsigned int* W, unsigned int* W1, unsigned int* V){
	unsigned int SS1,SS2,TT1,TT2;
	unsigned int A,B,C,D,E,F,G,H;
	unsigned int FF,GG;
	unsigned int T[64];
	for(int j = 0; j < 16; j++) T[j] = 0x79CC4519;
	for(int j = 16; j < 64; j++) T[j] = 0x7A879D8A;
	A=V[0]; B=V[1]; C=V[2]; D=V[3]; E=V[4]; F=V[5]; G=V[6]; H=V[7];
	
	for (int j = 0; j <= 63; j++){
		SS1 = ROTL((ROTL(A,12) + E + ROTL(T[j],j)), 7); 
		SS2 = SS1 ^ ROTL(A,12);
		if(j <= 15){
			FF = FF0(A,B,C);
			GG = GG0(E,F,G);
		}
		else{
			FF = FF1(A,B,C);
			GG = GG1(E,F,G);
		}
		TT1 = FF + D + SS2 + W1[j];
		TT2 = GG + H + SS1 + W[j];
		D = C;
		C = ROTL(B,9);
		B = A;
		A = TT1;
		H = G;
		G = ROTL(F,19);
		F = E;
		E = P0(TT2);
	}
	V[0]=A^V[0]; V[1]=B^V[1]; V[2]=C^V[2]; V[3]=D^V[3];
	V[4]=E^V[4]; V[5]=F^V[5]; V[6]=G^V[6]; V[7]=H^V[7];
} 

void BigEndian(unsigned char* src, unsigned int len, unsigned char* des){
	unsigned char tmp = 0;
	for(int i = 0; i < len/4; i++){
		tmp = des[4*i];
		des[4*i] = src[4*i+3];
		src[4*i+3] = tmp;
		tmp = des[4*i+1];
		des[4*i+1] = src[4*i+2];
		src[4*i+2] = tmp;
	}
}

void Compress(sm3_context* ctx){
	unsigned int W[68];
	unsigned int W1[64];
	BigEndian(ctx->buffer, 64, ctx->buffer);
	BiToW((unsigned int*)ctx->buffer, W);
	WToW1(W, W1);
	CF(W, W1, ctx->state);
}
		
int ctxread(unsigned char* s){
    char c;
    int len = 0;
    while(true){
        c  = getchar();
        if (c == '\n')
            break;
        s[len++] = c;
    }
    return len;
}

void SM3_process(unsigned char* s, unsigned int len, sm3_context* ctx){
	unsigned int IV[8] = {0x7380166F, 0x4914B2B9, 0x172442D7, 
	0xDA8A0600, 0xA96F30BC,	0x163138AA, 0xE38DEE4D,	0xB0FB0E4E};
	int left = 0; int i = 0;
	for(i = 0; i < 8; i++)
		ctx->state[i] = IV[i];
	for(i = 0; i < len/64; i++){
		memcpy(ctx->buffer, s + i * 64, 64);
		printf("%X\n",ctx->buffer[i]);
		Compress(ctx);
	}
	ctx->total = len * 8;
	left = len%64;
	memset(&ctx->buffer[left], 0, 64-left);	
	memcpy(ctx->buffer, s + i * 64, left);
	ctx->buffer[left] = 0x80;
	if(left <= 55){
		for (i = 0; i < 8; i++)
			ctx->buffer[56 + i] = (ctx->total >> ((8 - 1 - i) * 8)) & 0xFF;
		Compress(ctx);
	}
	else{
		Compress(ctx);
		memset(ctx->buffer, 0, 64);
		for (i = 0; i < 8; i++)
			ctx->buffer[56 + i] = (ctx->total >> ((8 - 1 - i) * 8)) & 0xFF;
		Compress(ctx);
	}
}

void sm3self(unsigned char* message, unsigned int len, unsigned char* output){
	sm3_context* ctx = new sm3_context;
	SM3_process(message, len, ctx);
	for (int i = 0; i < 8; i++){
		output[i * 4] = (unsigned char)((ctx->state[i] >> 24) & 0xFF);
		output[i * 4 + 1] = (unsigned char)((ctx->state[i] >> 16) & 0xFF);
		output[i * 4 + 2] = (unsigned char)((ctx->state[i] >> 8) & 0xFF);
		output[i * 4 + 3] = (unsigned char)((ctx->state[i]) & 0xFF);
	}
}

int main(){
	unsigned char message[1000];
	memset(message, 0, 1000);
	unsigned char output[32];
	int messagelen = ctxread(message);
	sm3self(message, messagelen, output);
	for(int j=0; j<32; j++){
    	printf("%02x",output[j]);
	}
}
