#include <stdio.h>
#include <string.h>
//#include <cstdio>
//using namespace std;
 
static const unsigned int sm3_padding[64] = {
 0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};
 
int ctxread(unsigned char* s){
    char c;
    int len = 0;
    while(true){
        c  = getchar();
        if (c == '\n') //防止回车符加入到s[]中
            break;
        s[len++] = c;
    }
    return len;
}

//void SM3_process(unsigned char *s, unsigned int len, sm3_context* ctx){
//	int left = 0;
//	unsigned long long total = 0;
//		
//	for(int i = 0; i < len/64; i++){
//		memcpy(ctx->buffer, s + i * 64, 64);
//		Compress(ctx);
//	}
//	
//	total = len * 8;
//	left = len%64;
//	memset(&ctx->buffer[left], 0, 64 - left);	
//	memcpy(ctx->buffer, msg + i * 64, left);
//	ctx->buffer[left] = 0x80;
//	if(left <= 55){
//		for (int i = 0; i < 8; i++)
//			ctx->buffer[56 + i] = (total >> ((8 - 1 - i) * 8)) & 0xFF;
//		Compress(ctx);
//	}
//	else{
//		Compress(ctx);
//		memset(ctx->buffer, 0, 64);
//		for (int i = 0; i < 8; i++)
//			ctx->buffer[56 + i] = (total >> ((8 - 1 - i) * 8)) & 0xFF;
//		Compress(ctx);
//	}
//}

int main(){
	unsigned char ctx[1000];
	int msglen = ctxread(ctx);
//	if(msglen % 64 < 56
	
	
    for(int j=0; j<msglen; j++){
    	printf("%X",ctx[j]);
	}
    return 0;
//    char* str="sdfsf";
//	int len;
//	len=strlen(str);
//	printf("%d",len);
} 

