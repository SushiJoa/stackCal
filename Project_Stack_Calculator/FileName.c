#if 0
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

//문자열 공백 제거 함수
void remove_space(char* str)
{
	int str_len = strlen(str);
	char temp[100] = "";
	char* tok = strtok(str, " ");
	while (tok != NULL) {
		strcat(temp, tok);
		tok = strtok(NULL, " ");
	}
	strncpy(str, temp, str_len);
}

char* get_token(char* in)
{
	static int firstIdx = 0, lastIdx = 0;
	int i;
	char* p;

	firstIdx = lastIdx;
	if (in[lastIdx] == '\0') {
		firstIdx = 0;
		lastIdx = 0;
		return NULL;
	}
	if (isdigit(in[firstIdx])) {
		lastIdx += strspn(&in[lastIdx], "1234567890.");
		p = (char*)malloc(sizeof(char) * (lastIdx - firstIdx + 1)); // NULL문자를 위한 +1
		for (i = 0; firstIdx < lastIdx; i++, firstIdx++) {
			p[i] = in[firstIdx];
		}
		p[i] = '\0';
	}
	else {
		p = (char*)malloc(2);
		p[0] = in[firstIdx];
		p[1] = '\0';
		lastIdx++;
	}

	return p;
}
// strtok 함수
char* get_op(char* s1, const char* delimit)
{
	static char* lastToken = NULL;
	char* temp;
	if (s1 == NULL) {
		s1 = lastToken;
		if (s1 == NULL)
			return NULL;
	}
	else
		s1 += strspn(s1, delimit);

	temp = strpbrk(s1, delimit);
	if (temp) {
		*temp = '\0';
		lastToken = temp + 1;
	}
	else
		lastToken = NULL;

	return s1;
}
int main(void)
{
	char str[] = "(1+2)-3";
	char* p;


	char strd[100];
	char* expr;
	int n;
	gets(strd);
	printf("%d\n\n", strlen(strd));


	remove_space(str);
	printf("%s\n", str);

	p = get_token(str);
	while (p != NULL) {
		printf("%s ", p);
		p = get_token(str);
	}
}

#endif