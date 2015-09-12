#include <stdio.h>

#define CCOMMENT 1
#define STRING 2
#define CPPCOMMENT 4
#define PP 4
#define WSPACE 8

int aboc_pretty = 0;

void aboc_close(int prev, int current){
	int i;
	int num = prev - current;
	while(num > 0){
		if(aboc_pretty){
			for(i = num + current; i > 1; --i){
				putchar('\t');
			}
		}
		putchar('}');
		if(aboc_pretty){
			putchar('\n');
		}
		--num;
	}
}

void aboc_indent(int num){
	while(num-- > 0){
		putchar('\t');
	}
}

int main(int argc, char *argv){
	int state = 0;
	int pdepth = 0;
	int depth = 0;
	int newlines = 0;
	int c;
	char prevc = ' ';
	while((c = getchar()) != EOF){
		if(c == '#' && !(state & (CCOMMENT | CPPCOMMENT | STRING))){
			if(!(state & PP)){
				putchar(';');
			}
			do{
				putchar('\n');
			} while(--newlines > 0);
			state = PP;
			putchar(c);
		}
		else if(prevc == '/' && c == '/'){
			state = CPPCOMMENT;
			putchar(c);
		}
		else if(prevc == '/' && c == '*'){
			state |= CCOMMENT;
			putchar(c);
		}
		else if(prevc == '*' && c == '/'){
			state &= ~CCOMMENT;
			putchar(c);
		}
		else if(c == '\n'){
			if(state & CCOMMENT){
				putchar(c);
			}
			else{
				++newlines;
				depth = 0;
				state |= WSPACE;
			}
		}
		else if(state & WSPACE){
			if(iswspace(c)){
				++depth;
			}
			if(isgraph(c)){
				state &= ~WSPACE;
				if(state & (PP | CPPCOMMENT)){
					state &= ~(PP | CPPCOMMENT);
				}
				else{
					if(depth == pdepth){
						printf(";");
					}
					else if(depth < pdepth){
						putchar(';');
						if(aboc_pretty){
							putchar('\n');	
						}
						aboc_close(pdepth, depth);
					}
					else{
						putchar('{');
					}
				}
				do{
					putchar('\n');
				} while(--newlines > 0);
				aboc_indent(depth);
				pdepth = depth;
				depth = 0;
				putchar(c);
			}
		}
		else{
			putchar(c);
		}
		prevc = c;
	}
	printf(";\n");
	aboc_close(pdepth, depth);
	printf("\n");

	return 0;
}
