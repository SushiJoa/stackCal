#if 1
/*

num: number
opr: operator
opd: operand
elem: elementary
arith: arithmetic
std: standard
N: and
brkt: bracket
token: 
*/

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// char*형을 element로 형선언 
typedef char* element;

// 함수원형 선언
char* get_token(char* in);
element create_expr();
int prec(char* op);
char* set_std_space(char* str);
int check_matching(char* in);
int check_opr_N_num(char* in);
int is_op_num_correct(char* in);
double token_to_double(char* token);
element double_to_token(double num);
int return_expr_type(char* in);
char* infix_to_postfix(char* exp);
char* combine_3str(char* s1, char* s2, char* s3);
char* combine_3str_prec(char* s1, char* s2, char* s3);
char* infix_to_prefix(char* exp);
char* reverse_str(char* str);
char* prefix_to_infix(char* exp);
char* postfix_to_infix(char* exp);
double eval(char* exp);
char* prefix_to_postfix(char* exp);

// element형 데이터와 구조체__node를 가리키는 포인터를 멤버로 담은 구조체 __node를 선언 및  StackNode로 형변환
typedef struct __node {
	element data;	// 구조체 __node의 elemnet형 멤버
	struct __node* link;	// 구조체 __node를 가리키는 포인터 멤버
}StackNode;	//__node를 StackNode로 형변환
// 구조체 __stacktype을 가리키는 포인터를 멤버로 담은 구조체 __stack를 선언 및 StackType로 형변환
typedef struct __stacktype {
	StackNode* top;	// 스택을 구현한 연결리스트의 맨 앞을 가리키는 포인터
}StackType;		// 구조체 __stack를 StackType로 형변환
// StackType의 주소값을 전달받아 멤버인 포인터를 NULL로 초기화
void init_stack(StackType* s)
{
	s->top = NULL;	// StackType의 포인터 멤버를 NULL로 초기화
}
// 스택의 노드가 비어있는지 확인하는 함수
int is_empty(StackType* s)
{
	return s->top == NULL;	// top포인터가 NULL을 가리키면 1을 반환, 아니면 0을 반환
}
// 스택에 데이터를 담은 노드를 만들어 삽입하는 함수
void push(StackType* s, element item)
{
	StackNode* p = (StackNode*)malloc(sizeof(StackNode));	// 노드를 동적할당한 후 p에 주소값을 대입
	p->data = item;	// 동적할당받은 StackNode에 매개변수로 받은 데이터대입
	p->link = s->top;	// s->top이 가리키던 주소값을 노드의 링크가 가리키도록 함
	s->top = p;		// s->top이 동적할당 받은 노드 p를 가리키게 함.
}
// 매개변수로 주소값을 전달받은 스택에서 데이터를 꺼내는 함수
element pop(StackType* s)
{
	// 스택이 비어있을 경우 에러메세지를 띄움
	if (is_empty(s)) {
		fprintf(stderr, "Stack 빔 에러(pop)");	// Stack의 pop연산을 하던중 에러가 발생하였음을 표시
		exit(1);	// 프로그램 종료함수
	}
	// rnode포인터에 지울 노드의 주소값을 저장
	StackNode* rnode = s->top;
	// rdata변수에 지울 노드의 데이터를 저장
	element rdata = s->top->data;
	// s->top이 다음 노드를 가리키도록 함
	s->top = s->top->link;
	// rnode가 가리키던 동적할당된 노드의 메모리를 반환
	free(rnode);
	// rdata를 반환
	return rdata;
}
element peek(StackType* s)
{
	// 스택이 비어있을 경우 에러메세지를 띄움
	if (is_empty(s)) {
		fprintf(stderr, "Stack 빔 에러(peek)");	// Stack의 peek연산을 하던중 에러가 발생하였음을 표시
		exit(1);	// 프로그램 종료함수
	}
	// data변수에 현재 스택 맨 위에 있는 데이터를 저장
	element data = s->top->data;
	// data를 반환
	return data;
}



// 수식을 입력받아 반환하는 함수
element create_expr()
{
	// 100칸짜리 배열을 선언 및 초기화
	char str[200] = "";
	// 수식을 전달 받기위한 문자열 포인터 선언
	char* expr;
	//문자열의 길이를 저장할 변수 선언
	int len;
	// 수식을 입력하라는 메시지 출력 및 사용자로부터 수식 입력
	printf("Input Any type of expression : ");
	fgets(str, 200, stdin);
	// 입력받은 문자열의 길이를 계산
	len = strlen(str);
	// fgets를 통해 들어온 개행문자를 제거
	str[len - 1] = '\0';
	// 문자열의 길이만큼 동적할당. +1하지않는 이유는 개행문자삽입으로 인해 이미 +1되어있기때문.
	expr = (char*)malloc(strlen(str));
	// str문자열을 expr로 복사
	strcpy(expr, str);
	// expr의 주소값을 반환
	return expr;
}
// 연산자의 우선순위를 반환하는 함수
int prec(char* op) 
{
	// 매개변수로 전달받은 문자열의 첫번째 문자에 따라 다른 값을 반환
	switch (op[0]) {
	case '(': case ')': return 1;	// 첫 문자가 (,) 일시 1 반환
	case '+': case '-': return 2;	// 첫 문자가 +,- 일시 1 반환
	case '*': case '/': return 3;	// 첫 문자가 *,/ 일시 1 반환
	}
	return 0;	// 어떠한 연산자에도 해당하지않을시 0을 반환
}
// 전달받은 문자열의 연산자와 피연산자를 각각 반환하는 함수
element get_token(char* in)
{
	// 여러번 호출되었을시 현재 참조중인 위치를 저장하기위한 static변수 초기화
	static int firstIdx = 0, lastIdx = 0;
	int i;	// 반복문에서 사용하기위한 변수
	char* p;	// 동적할당되어 문자열 토큰을 구성해 반환할 포인터 변수

	if (in[lastIdx] == ' ') {		// 첫문자가 공백문자면 공백문자만큼 인덱스 이동
		lastIdx += strspn(&in[lastIdx], " ");
	}
	if (in[lastIdx] == '\0') {		// 첫문자가 NULL문자면 firstIdx, lastIdx변수 초기화 및 NULL반환
		firstIdx = 0;
		lastIdx = 0;
		return NULL;
	}
	// firstIdx에 lastIdx값 대입
	firstIdx = lastIdx;

	if (isdigit(in[firstIdx])) {	// 전달받은 문자열의 첫 문자가 숫자이면 문자가 숫자와 '.'이 아닐때까지 p에 복사 
		lastIdx += strspn(&in[lastIdx], "1234567890.");	// 구분자인 동안의 문자열길이를 lastIdx에 더함. lastIdx는 구분자가 아닌 문자를 가리키게 됨
		p = (char*)malloc(sizeof(char) * (lastIdx - firstIdx + 2)); // 문자열 맨뒤 공백한칸과 NULL문자를 위한 +2
		for (i = 0; firstIdx < lastIdx; i++, firstIdx++) {	// firtIdx부터 lastIdx 한칸전까지 문자열 복사
			p[i] = in[firstIdx];
		}
		// 복사된 문자열 뒤에 공백문자와 NULL문자 추가
		p[i] = ' ';	
		p[i + 1] = '\0';
	}
	else {	// 전달받은 문자열의 첫 문자가 숫자가 아닌경우 첫 문자 하나와 공백문자와 NULL문자로 구성된 문자열 생성
		p = (char*)malloc(3);	// p에 문자3개만큼의 메모리 동적할당
		p[0] = in[firstIdx];	// 첫 문자를 동적할당된 배열에 대입
		// 복사된 문자열 뒤에 공백문자와 NULL문자 추가
		p[1] = ' ';
		p[2] = '\0';
		// 전달받은 문자열의 첫 문자가 아닌 곳을 가리키도록 lastIdx에 1추가
		lastIdx++;
	}
	// 생성된 토큰 p의 주소값을 반환
	return p;
}
// 문자열의 공백문자 간격을 일정하게 만드는 함수
char* set_std_space(char* str)	//p1->array, str->p1
{
	int len;	// 문자열의 길이를 저장하는 변수
	char* temp = (char*)malloc(strlen(str) * 2 + 1);	// 전달받은 문자열에 공백문자가 없을 경우 추가되는 공백문자에 의해 늘어날 크기를 감안해 동적할당
	temp[0] = '\0';	// 동적할당받은 배열의 첫 문자를 NULL문자로 초기화

	// 문자열 토큰을 반환받아 변수에 대입
	char* token = get_token(str);
	while (token != NULL) {	// 토큰이 NULL일때까지 반복
		strcat(temp, token);	// temp의 NULL문자부터 token 문자열을 이어붙인다
		free(token);			// 사용된 token은 메모리 반환
		token = get_token(str);	// 문자열로부터 문자열 토큰을 가져옴
	}
	len = strlen(temp);	// 완성된 temp의 길이를 구함
	str = (char*)realloc(str, len + 1);	// temp의 길이 +1만큼 매개변수가 저장된 주소값이 가리키는 동적할당 메모리의 크기를 재할당. NULL문자를 위해 +1
	strncpy(str, temp, len + 1);	// str에 temp의 문자열을 len+1만큼 복사
	free(temp);	// temp가 가리키는 할당된 메모리 반환
	return str;	// 동적할당된 문자열 배열의 주소값를 반환
}
// 괄호검사하는 함수
int check_matching(char* in)
{
	StackType s;	// 스택 선언
	char ch, open_ch;	// 문자가 담길 변수 두개를 선언
	// 문자열 배열을 가리킬 변수 i와 문자열의 길이를 담는 변수 n을 선언 및 초기화
	int i, n = strlen(in); // n= 문자열의 길이
	init_stack(&s); // 스택의 초기화
	for (i = 0; i < n; i++) {	// i를 0부터 n-1까지 반복
		ch = in[i]; // ch = 다음 문자
		switch (ch) {
		case '(':	// 문자가 '('일 경우 스택에 삽입
			push(&s, &ch);
			break;
		case ')':	// 문자가 ')'일 경우 배열이 비어있으면 0을 반환, 아니면 스택에서 꺼냄(삭제 및 반환)
			if (is_empty(&s)) return 0;
			else {
				open_ch = *(pop(&s));
				if (open_ch == '(' && ch != ')') {	// 스택에서 꺼내진 문자가 '('이고 이전에 꺼낸 문자가 ')'가 아니면 0을 반환
					return 0;
				}
				break;
			}
		}
	}
	if (!is_empty(&s)) return 0; // 스택에 남아있으면 오류
	return 1;	// 스택에 남아있지않으면 1 반환
}
// 불필요한 기호가 없는지 검사
int check_opr_N_num(char* in)
{
	int n = strlen(in); // n= 문자열의 길이
	return strspn(in, "+-*/().1234567890 ") == n;	// 구분자에 포함되지 않은 문자가 없으면 1을 반환
}
// 피연산자 개수 = 연산자 개수 +1인지 검사
int is_op_num_correct(char* in)
{
	char* token;	// 문자열 토큰을 담을 포인터 변수 선언 
	int count_num = 0, count_op = 0;	// 피연산자의 개수와 연산자의 개수를 셀 변수 선언 및 초기화
	token = get_token(in);	// token에 문자열 토큰의 주소값 대입
	while (token != NULL) {	// token이 NULL이 아닐때까지 반복
		if (isdigit(token[0]))	// token의 첫 문자가 숫자이면 피연산자 개수 1증가
			count_num++;
		else if (prec(token) == 2 || prec(token) == 3)	// token의 첫 문자가 사칙연산 문자(+,-<*,/)이면 연산자 개수 1증가
			count_op++;
		free(token);	// 사용된 토큰 메모리 반환
		token = get_token(in);	// token에 문자열 토큰의 주소값 대입
	}
	return count_num == count_op + 1;	// 피연산자 개수 = 연산자 개수 +1이면 1 반환 아니면 0 반환
}
// 토큰을 숫자로 변환
double token_to_double(char* token)
{
	double num = 0;	// double형 변수 선언 및 초기화
	if (token != NULL) {	// 매개변수로 전달받은 문자열이 NULL이 아니면 변수 num에 token의 값을 변환 후 대입
		sscanf(token, "%lf", &num);
	}
	return num;	// 변환된 double형 변수값을 반환
}
// 숫자를 토큰으로 변환
element double_to_token(double num)
{
	char* token;	// 문자열 배열을 가리키는 포인터 변수 선언
	char* temp = (char*)malloc(100);	// temp에 크기 100만큼 동적할당
	int len;	// 길이를 저장하는 변수 선언

	if (num != 0)	// 매개변수로 전달받은 값이 0이 아니면 문자열로 변환후 temp에 대입
		sprintf(temp, "%lf", num);
	len = strlen(temp);	// temp 문자열의 길이를 저장
	token = (char*)malloc(len + 1);	// token에 len+1만큼 동적할당
	strncpy(token, temp, len + 1);	// temp를 token에 len+1만큼 문자열 복사
	free(temp);	// 사용된 temp의 메모리 반환
	return token;	// token을 반환
}
// 전위식 중위식 후위식 검사
int return_expr_type(char* in)	// 전위, 중위, 후위식일 경우 각각 1, 2, 3을 반환. 아닐시 0 반환
{
	char* token;	// 문자열 배열을 가리키는 포인터 변수 선언
	int is_prefix = 0, is_infix = 0, is_postfix = 0;	// 전위식, 중위식, 후위식이 되는 조건을 나타내는 변수

	token = get_token(in);		// token에 문자열 토큰의 주소값 대입
	if (prec(token) == 2 || prec(token) == 3) 		// +, -, *, /, 일 경우 is_prefix는 1
		is_prefix = 1;	
	while (token != NULL) {
		if (prec(token) == 1)	// (, )일 경우
			is_infix = 1;
		if (prec(token) == 2 || prec(token) == 3)	// +, -, *, / 일 경우 is_postfix는 1
			is_postfix = 1;
		else										// 마지막 토큰이 +, -, *, / 이 아닐 경우 is_postfix는 0
			is_postfix = 0;
		free(token);	// 사용한 token의 메모리 반환
		token = get_token(in);		// token에 문자열 토큰의 주소값 대입
	}
	free(token);		// 사용한 token의 메모리 반환

	if (!is_prefix && !is_postfix)	// is_prefix와 is_postfix가 둘다 0일경우 2반환
		return 2;
	if (is_prefix && !is_infix && !is_postfix)	// is_prefix가 1이고 is_infix, is_postfix가 0일경우 1반환
		return 1;
	if (!is_prefix && !is_infix && is_postfix)	// is_postfix가 1이고 is_infix, is_pretfix가 0일경우 3반환
		return 3;
	return 0;	// 위 경우에 해당하지 않을 경우 0 반환(오류 발생)
}
// 문자열 세개 합치는 함수
char* combine_3str(char* s1, char* s2, char* s3)
{
	char* temp = (char*)malloc(strlen(s1) + strlen(s2) + strlen(s3) + 1);	// 세 문자열의 길이의 합 + 1(NULL문자)만큼 동적할당
	strcpy(temp, s1);	// s1 문자열을 temp에 복사
	strcat(temp, s2);	// s2 문자열을 temp에 이어 붙임
	strcat(temp, s3);	// s3 문자열을 temp에 이어 붙임
	free(s1);	// s1에 할당된 메모리 반환
	free(s2);	// s2에 할당된 메모리 반환
	free(s3);	// s3에 할당된 메모리 반환
	return temp;	// temp의 주소값 반환
}
// 문자열 세개 괄호안에 합치는 함수
char* combine_3str_prec(char* s1, char* s2, char* s3)
{
	char* temp = (char*)malloc(strlen(s1) + strlen(s2) + strlen(s3) + 5);	// 세 문자열의 길이의 합 + 괄호 문자열의 길이4 + 1(NULL문자)만큼 동적할당
	strcpy(temp, "( ");	// 오른쪽괄호 문자열을 temp에 복사
	strcat(temp, s1);	// s1 문자열을 temp에 이어 붙임
	strcat(temp, s2);	// s2 문자열을 temp에 이어 붙임
	strcat(temp, s3);	// s3 문자열을 temp에 이어 붙임
	strcat(temp, ") ");	// 오른쪽괄호 문자열을 temp에 이어 붙임
	free(s1);	// s1에 할당된 메모리 반환
	free(s2);	// s2에 할당된 메모리 반환
	free(s3);	// s3에 할당된 메모리 반환
	return temp;	// temp의 주소값 반환
}

//계산기 파트
// 중위 표기 수식 -> 후위 표기 수식 변환
char* infix_to_postfix(char* exp)		
{
	int len = strlen(exp);	// 매개변수로 전달받은 문자열의 길이 저장
	char* token, * token_tmp, * p_postfix;	// 문자열토큰을 받을 포인터변수, 추가로 문자열토큰을 받을 포인터변수, 변환된 문자열을 담을 포인터변수 선언
	char* temp = (char*)malloc(strlen(exp) * 2);	// 임시로 문자열을 담을 배열을 동적할당
	StackType s;	// 스택 선언
	init_stack(&s); // 스택 초기화

	temp[0] = '\0';	// temp의 첫 문자를 NULL문자로 초기화

	token = get_token(exp);	// 매개변수로 전달받은 문자열로부터 토큰을 받음
	while (token != NULL) {
		switch (token[0]) {
		case '+': case '-': case '*': case '/': // 토큰의 첫문자가 사칙연산자일 경우
			// 스택에 있는 연산자의 우선순위가 더 크거나 같은 동안 스택에 있는 데이터를 temp에 이어붙임
			while (!is_empty(&s) && (prec(token) <= prec(peek(&s)))) {
				token_tmp = pop(&s);
				strcat(temp, token_tmp);
				free(token_tmp);	// 사용한 token_tmp의 메모리 반환
			}
			push(&s, token);	// token을 스택에 삽입
			break;
		case '(': // 왼쪽 괄호일 경우
			push(&s, token);	// token을 스택에 삽입
			break;
		case ')': // 오른쪽 괄호일 경우
			token = pop(&s);	// 스택에서 데이터를 꺼냄
			// 왼쪽 괄호를 만날때까지 스택에서 데이터를 꺼내 temp에 이어붙임
			while (token[0] != '(') {
				strcat(temp, token);
				free(token);		// 사용한 token의 메모리 반환
				token = pop(&s);	// 스택에서 데이터를 꺼냄
			}
			break;
		default: // 피연산자일 경우 temp에 이어붙임
			strcat(temp, token);	
			free(token);	// 사용한 token의 메모리 반환
			break;
		}
		token = get_token(exp);	// 매개변수로 전달받은 문자열로부터 토큰을 받음
	}
	while (!is_empty(&s)) {// 스택에 저장된 연산자들을 모두 꺼내 temp에 이어붙임
		token = pop(&s);		// 스택에서 데이터를 꺼냄
		strcat(temp, token);	// 스택에서 데이터를 꺼냄
		free(token);			// 사용한 token의 메모리 반환
	}
	len = strlen(temp);	// temp의 길이 저장
	p_postfix = (char*)malloc(len + 1);	// len+1만큼 동적할당
	strncpy(p_postfix, temp, len + 1);	// p_postfix에 temp의 문자열을 len+1만큼 복사
	free(temp);	// temp의 메모리 반환
	return p_postfix;	// 후위식으로 변환된 문자열배열을 가리키는 주소값 반환
}
// 중위 표기 수식 -> 전위 표기 수식 변환
char* infix_to_prefix(char* exp)
{
	int len;	// 문자열의 길이를 담는 변수 선언
	char* token, * op1, * op2, * p_prefix;// 문자열토큰을 받을 포인터변수, 연산자문자열을 받을 포인터변수, 변환된 문자열을 담을 포인터변수 선언
	StackType s_operand;	// 스택 선언
	StackType s_operator;	// 스택 선언
	init_stack(&s_operand); // 스택 초기화
	init_stack(&s_operator); // 스택 초기화

	char* temp = (char*)malloc(strlen(exp) * 2); // 최종 변환된 문자열을 임시로 담을 문자열 배열을 동적할당
	temp[0] = '\0';		// 문자열배열의 첫문자를 NULL문자로 초기화
	char* temp_op = (char*)malloc(strlen(exp) * 2); // 결합된 문자열을 임시로 담을 문자열 배열을 동적할당
	temp_op[0] = '\0';	// 문자열배열의 첫문자를 NULL문자로 초기화


	token = get_token(exp);	// 매개변수로 전달받은 문자열로부터 토큰을 받음
	while (token != NULL) {	// 토큰이 NULL이 아닐때까지 반복
		switch (token[0]) {
		case '+': case '-': case '*': case '/': // 토큰의 첫문자가 사칙연산자일 경우
			// 스택에 있는 연산자의 우선순위가 더 크거나 같은 동안 스택에 있는 데이터를 temp에 이어붙임			
			while (!is_empty(&s_operator) && (prec(token) <= prec(peek(&s_operator)))) {
				op2 = pop(&s_operand);
				op1 = pop(&s_operand);

				temp_op = combine_3str(pop(&s_operator), op1, op2);
				push(&s_operand, temp_op);
			}
			push(&s_operator, token);
			break;
		case '(': // 왼쪽 괄호일 경우
			push(&s_operator, token);
			break;
		case ')': // 오른쪽 괄호일 경우
			token = pop(&s_operator);
			// 왼쪽 괄호를 만날때까지 스택에서 데이터를 꺼내 temp에 이어붙임
			while (token[0] != '(') {
				op2 = pop(&s_operand);
				op1 = pop(&s_operand);

				temp_op = combine_3str(token, op1, op2);
				push(&s_operand, temp_op);
				token = pop(&s_operator);
			}
			break;
		default: // 피연산자
			push(&s_operand, token);
			break;
		}
		token = get_token(exp);
	}
	while (!is_empty(&s_operator)) {// 스택에 저장된 연산자들 출력
		op2 = pop(&s_operand);
		op1 = pop(&s_operand);

		temp_op = combine_3str(pop(&s_operator), op1, op2);
		push(&s_operand, temp_op);
	}
	token = pop(&s_operand);
	strcpy(temp, token);
	free(token);

	len = strlen(temp);
	p_prefix = (char*)malloc(len + 1);
	strncpy(p_prefix, temp, len + 1);
	return p_prefix;
}
// 문자열 역순함수
char* reverse_str(char* str)
{
	StackType s;
	char* token, * exp_reverse;
	int len;

	init_stack(&s);

	len = strlen(str);
	exp_reverse = (char*)malloc(len + 1);
	exp_reverse[0] = '\0';

	token = get_token(str);
	while (token != NULL) {
		push(&s, token);
		token = get_token(str);
	}
	while (!is_empty(&s)) {// 스택에 저장된 연산자들 출력
		token = pop(&s);
		strcat(exp_reverse, token);
	}
	return exp_reverse;
}
// 전위 표기 수식 -> 중위 표기 수식 변환
char* prefix_to_infix(char* exp)
{
	StackType s;
	char* token, * exp_reverse, * op1, * op2, * temp;
	int len;

	init_stack(&s);

	exp_reverse = reverse_str(exp);

	token = get_token(exp_reverse);
	while (token != NULL) {
		switch (token[0]) {
		case '+': case '-': case '*': case '/': // 연산자
			op1 = pop(&s);
			op2 = pop(&s);

			temp = combine_3str_prec(op1, token, op2);
			push(&s, temp);
			break;
		default: // 피연산자
			push(&s, token);
			break;
		}
		token = get_token(exp_reverse);
	}
	return pop(&s);
}
// 전위 표기 수식 -> 후위 표기 수식 변환
char* prefix_to_postfix(char* exp)
{
	char* p;
	p = prefix_to_infix(exp);
	p = infix_to_postfix(p);
	return p;
}
// 후위 표기 수식 -> 중위 표기 수식 변환
char* postfix_to_infix(char* exp)
{
	StackType s;
	char* token, * exp_reverse, * op1, * op2, * temp;
	int len;

	init_stack(&s);

	token = get_token(exp);
	while (token != NULL) {
		switch (token[0]) {
		case '+': case '-': case '*': case '/': // 연산자
			op2 = pop(&s);
			op1 = pop(&s);

			temp = combine_3str_prec(op1, token, op2);
			push(&s, temp);
			break;
		default: // 피연산자
			push(&s, token);
			break;
		}
		token = get_token(exp);
	}
	return pop(&s);
}
// 후위 표기 수식 -> 전위 표기 수식 변환
char* postfix_to_prefix(char* exp)
{
	char* p;
	p = postfix_to_infix(exp);
	p = infix_to_prefix(p);
	return p;
}
// 후위 표기 수식 계산 함수
double eval(char* exp)
{
	int len = strlen(exp);
	double result, op1, op2;
	char ch;
	char* token, * tmp_1, * tmp_2;
	StackType s;
	init_stack(&s);
	//for (i = 0; i < len; i++)
	token = get_token(exp);
	while (token != NULL) {
		ch = token[0];
		if (ch != '+' && ch != '-' && ch != '*' && ch != '/') {
			push(&s, token);
		}
		else { //연산자이면 피연산자를 스택에서 제거
			tmp_1 = pop(&s);
			tmp_2 = pop(&s);
			op2 = token_to_double(tmp_1);
			op1 = token_to_double(tmp_2);
			switch (ch) { //연산을 수행하고 스택에 저장
			case '+':
				token = double_to_token(op1 + op2);
				push(&s, token);
				break;
			case '-':
				token = double_to_token(op1 - op2);
				push(&s, token);
				break;
			case '*':
				token = double_to_token(op1 * op2);
				push(&s, token);
				break;
			case '/':
				if (op2 == 0) {
					printf("0으로 나눌 수 없습니다.\n");
					return 0;
				}
				token = double_to_token(op1 / op2);
				push(&s, token);
				break;
			}
		}
		token = get_token(exp);
	}
	token = pop(&s);
	result = token_to_double(token);
	free(token);
	return result;
}





int main(void)
{
	char* p_input = create_expr();	//  (235 + 5.2768)* 34 *    (2 +100)     3 + 2
	char* p_post, * p_pre, * p_in;
	int expr_type;
	StackType s;
	init_stack(&s);

	p_input = set_std_space(p_input);
	//printf("%s \n", p_input);
	// 식 검사
	// 식 올바른지 검사 (+_* /.()숫자이외의 문자가 있는지)
	if (check_opr_N_num(p_input) == 0) {
		printf("올바르지않은 문자가 포함되어 있습니다.\n");
		return 1;
	}
	// 피연산자 개수 = 연산자 개수+1인지 검사					 123.34+12-3/4
	if (is_op_num_correct(p_input) == 0) {
		printf("연산자 피연산자 개수가 올바르지 않습니다.\n");
		return 1;
	}
	// 1-1.괄호검사
	if (check_matching(p_input) == 0) {
		printf("괄호 짝이 맞지 않습니다.\n");
		return 1;
	}
	// 전위식 중위식 후위식 검사
	expr_type = return_expr_type(p_input);
	switch (expr_type) {
	case 1: {	// 전위식
		p_in = prefix_to_infix(p_input);
		p_post = prefix_to_postfix(p_input);
		printf("Prefix:  %s \n", p_input);
		printf("Infix: \t %s \n", p_in);
		printf("Postfix: %s \n", p_post);
		printf("result = %lf\n", eval(p_post));
		break;
	}
	case 2: {	// 중위식
		p_pre = infix_to_prefix(p_input);
		p_post = infix_to_postfix(p_input);
		printf("Prefix:  %s \n", p_pre);
		printf("Infix: \t %s \n", p_input);
		printf("Postfix: %s \n", p_post);	//Infix : (2 + 5) * 3 * (2 + 1) ,Prefix: 2 5 + 3 * 2 1 + * , Postfix: 2 5 + 3 * 2 1 + *
		printf("result = %lf\n", eval(p_post));
		break;
	}
	case 3: {	// 후위식
		p_in = postfix_to_infix(p_input);
		p_pre = postfix_to_prefix(p_input);
		printf("Prefix:  %s \n", p_pre);
		printf("Infix: \t %s \n", p_in);
		printf("Postfix: %s \n", p_input);
		printf("result = %lf\n", eval(p_input));
		break;
	}
	default: {
		printf("올바르지 않은 식입니다.\n");
		break;
	}
	}


	return 0;
}
#endif