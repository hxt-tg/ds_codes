/* The center of whole project */

#include "expression.h"
#include <stdio.h>
#include <string.h>

void printNotice();
void printTitle();

int main() {
	/* GUI */
	//printTitle();
	printf("This is a program which can process and compute compound complex expressions.\n");
	printf("For example, you can input an expression like:\n");
	printf("    (1-i)*((2+5i)/2)\n");
	printf("I will give you an answer: 3.5+1.5i\n");
	printf("If there is an error in your expression and it is cleared,\n");
	printf("You can press UP arrow to re-type and modify your last expression.\n");
	printNotice();
	char input_src[EXP_MAX], s_ans[2*NUM_MAX+4];
	complex ans;
	extern int DIV_ERR_MSG; /* Check dividend is 0 */
	extern complex previous_ans; /* get previous_ans and initialize it */
	previous_ans.imag = 0.0;
	previous_ans.real = 0.0;
	while (1){
		printf("_______________________________________________________________________________\n");
		printf("Expression:");
		gets(input_src);
		if (*input_src == 0){
			printf("           ^ Try to input something :D\n");
			continue;
		}
		if (strlen(input_src)==1&&(*input_src == '='||*input_src == '+'||*input_src == '-'||*input_src == '*'||*input_src == '/')){
			printf("           ^ Where are your operands? :P\n");
			continue;
		}
		if (*input_src == 'q' || *input_src == 'Q' || *input_src == 'e' || *input_src == 'E')
			break;
		if (*input_src == 'c' || *input_src == 'C'){
			system("cls");
			printNotice();
			continue;
		}
		if (checkExp(input_src) == ERROR)
			continue;
		preProcess(input_src);
		ans = EvaluateExpression(input_src);
		if (DIV_ERR_MSG == 1){
			DIV_ERR_MSG = 0;
			continue;
		}
			
		sprintCom(s_ans, &ans);
		printf("    Answer:%s\n", s_ans);
	}
	system("cls");
	printTitle();
	/*
	printf("┌──────────────────────────────────────┐\n"); 
	printf("│                              广告位招租  ￥12/h                            │\n"); 
	printf("│                            联系方式: 187****4676                           │\n"); 
	printf("└──────────────────────────────────────┘\n\n\n"); 
	*/
	printf("                              Thanks for your using.\n");
	printf("                       There must be some bugs in my program.\n");
	printf("                           If you find, contact me please.\n");
	printf("                               Email:782920705@qq.com\n\n");
	printf("                                    Goodbye~\n\n");
	printf("                            Press any key to quit...");
	system("pause > nul");
	return 0;
}

void printNotice(){	
	printf("\n=================================== Notice ====================================\n");
	printf("To quit you can input q(uit) or e(xit).\n");
	printf("To clear the screen you can input c(lear).\n");
	printf("To reuse previous result you can input letter \"p\"\n");
}

void printTitle(){
	printf("┌──────────────────────────────────────┐\n"); 
	printf("│                            Complex Calculator v1.3                         │\n"); 
	printf("└──────────────────────────────────────┘"); 
	
}
