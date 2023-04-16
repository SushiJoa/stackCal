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

// char*���� element�� ������ 
typedef char* element;

// �Լ����� ����
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

// element�� �����Ϳ� ����ü__node�� ����Ű�� �����͸� ����� ���� ����ü __node�� ���� ��  StackNode�� ����ȯ
typedef struct __node {
	element data;	// ����ü __node�� elemnet�� ���
	struct __node* link;	// ����ü __node�� ����Ű�� ������ ���
}StackNode;	//__node�� StackNode�� ����ȯ
// ����ü __stacktype�� ����Ű�� �����͸� ����� ���� ����ü __stack�� ���� �� StackType�� ����ȯ
typedef struct __stacktype {
	StackNode* top;	// ������ ������ ���Ḯ��Ʈ�� �� ���� ����Ű�� ������
}StackType;		// ����ü __stack�� StackType�� ����ȯ
// StackType�� �ּҰ��� ���޹޾� ����� �����͸� NULL�� �ʱ�ȭ
void init_stack(StackType* s)
{
	s->top = NULL;	// StackType�� ������ ����� NULL�� �ʱ�ȭ
}
// ������ ��尡 ����ִ��� Ȯ���ϴ� �Լ�
int is_empty(StackType* s)
{
	return s->top == NULL;	// top�����Ͱ� NULL�� ����Ű�� 1�� ��ȯ, �ƴϸ� 0�� ��ȯ
}
// ���ÿ� �����͸� ���� ��带 ����� �����ϴ� �Լ�
void push(StackType* s, element item)
{
	StackNode* p = (StackNode*)malloc(sizeof(StackNode));	// ��带 �����Ҵ��� �� p�� �ּҰ��� ����
	p->data = item;	// �����Ҵ���� StackNode�� �Ű������� ���� �����ʹ���
	p->link = s->top;	// s->top�� ����Ű�� �ּҰ��� ����� ��ũ�� ����Ű���� ��
	s->top = p;		// s->top�� �����Ҵ� ���� ��� p�� ����Ű�� ��.
}
// �Ű������� �ּҰ��� ���޹��� ���ÿ��� �����͸� ������ �Լ�
element pop(StackType* s)
{
	// ������ ������� ��� �����޼����� ���
	if (is_empty(s)) {
		fprintf(stderr, "Stack �� ����(pop)");	// Stack�� pop������ �ϴ��� ������ �߻��Ͽ����� ǥ��
		exit(1);	// ���α׷� �����Լ�
	}
	// rnode�����Ϳ� ���� ����� �ּҰ��� ����
	StackNode* rnode = s->top;
	// rdata������ ���� ����� �����͸� ����
	element rdata = s->top->data;
	// s->top�� ���� ��带 ����Ű���� ��
	s->top = s->top->link;
	// rnode�� ����Ű�� �����Ҵ�� ����� �޸𸮸� ��ȯ
	free(rnode);
	// rdata�� ��ȯ
	return rdata;
}
element peek(StackType* s)
{
	// ������ ������� ��� �����޼����� ���
	if (is_empty(s)) {
		fprintf(stderr, "Stack �� ����(peek)");	// Stack�� peek������ �ϴ��� ������ �߻��Ͽ����� ǥ��
		exit(1);	// ���α׷� �����Լ�
	}
	// data������ ���� ���� �� ���� �ִ� �����͸� ����
	element data = s->top->data;
	// data�� ��ȯ
	return data;
}



// ������ �Է¹޾� ��ȯ�ϴ� �Լ�
element create_expr()
{
	// 100ĭ¥�� �迭�� ���� �� �ʱ�ȭ
	char str[200] = "";
	// ������ ���� �ޱ����� ���ڿ� ������ ����
	char* expr;
	//���ڿ��� ���̸� ������ ���� ����
	int len;
	// ������ �Է��϶�� �޽��� ��� �� ����ڷκ��� ���� �Է�
	printf("Input Any type of expression : ");
	fgets(str, 200, stdin);
	// �Է¹��� ���ڿ��� ���̸� ���
	len = strlen(str);
	// fgets�� ���� ���� ���๮�ڸ� ����
	str[len - 1] = '\0';
	// ���ڿ��� ���̸�ŭ �����Ҵ�. +1�����ʴ� ������ ���๮�ڻ������� ���� �̹� +1�Ǿ��ֱ⶧��.
	expr = (char*)malloc(strlen(str));
	// str���ڿ��� expr�� ����
	strcpy(expr, str);
	// expr�� �ּҰ��� ��ȯ
	return expr;
}
// �������� �켱������ ��ȯ�ϴ� �Լ�
int prec(char* op) 
{
	// �Ű������� ���޹��� ���ڿ��� ù��° ���ڿ� ���� �ٸ� ���� ��ȯ
	switch (op[0]) {
	case '(': case ')': return 1;	// ù ���ڰ� (,) �Ͻ� 1 ��ȯ
	case '+': case '-': return 2;	// ù ���ڰ� +,- �Ͻ� 1 ��ȯ
	case '*': case '/': return 3;	// ù ���ڰ� *,/ �Ͻ� 1 ��ȯ
	}
	return 0;	// ��� �����ڿ��� �ش����������� 0�� ��ȯ
}
// ���޹��� ���ڿ��� �����ڿ� �ǿ����ڸ� ���� ��ȯ�ϴ� �Լ�
element get_token(char* in)
{
	// ������ ȣ��Ǿ����� ���� �������� ��ġ�� �����ϱ����� static���� �ʱ�ȭ
	static int firstIdx = 0, lastIdx = 0;
	int i;	// �ݺ������� ����ϱ����� ����
	char* p;	// �����Ҵ�Ǿ� ���ڿ� ��ū�� ������ ��ȯ�� ������ ����

	if (in[lastIdx] == ' ') {		// ù���ڰ� ���鹮�ڸ� ���鹮�ڸ�ŭ �ε��� �̵�
		lastIdx += strspn(&in[lastIdx], " ");
	}
	if (in[lastIdx] == '\0') {		// ù���ڰ� NULL���ڸ� firstIdx, lastIdx���� �ʱ�ȭ �� NULL��ȯ
		firstIdx = 0;
		lastIdx = 0;
		return NULL;
	}
	// firstIdx�� lastIdx�� ����
	firstIdx = lastIdx;

	if (isdigit(in[firstIdx])) {	// ���޹��� ���ڿ��� ù ���ڰ� �����̸� ���ڰ� ���ڿ� '.'�� �ƴҶ����� p�� ���� 
		lastIdx += strspn(&in[lastIdx], "1234567890.");	// �������� ������ ���ڿ����̸� lastIdx�� ����. lastIdx�� �����ڰ� �ƴ� ���ڸ� ����Ű�� ��
		p = (char*)malloc(sizeof(char) * (lastIdx - firstIdx + 2)); // ���ڿ� �ǵ� ������ĭ�� NULL���ڸ� ���� +2
		for (i = 0; firstIdx < lastIdx; i++, firstIdx++) {	// firtIdx���� lastIdx ��ĭ������ ���ڿ� ����
			p[i] = in[firstIdx];
		}
		// ����� ���ڿ� �ڿ� ���鹮�ڿ� NULL���� �߰�
		p[i] = ' ';	
		p[i + 1] = '\0';
	}
	else {	// ���޹��� ���ڿ��� ù ���ڰ� ���ڰ� �ƴѰ�� ù ���� �ϳ��� ���鹮�ڿ� NULL���ڷ� ������ ���ڿ� ����
		p = (char*)malloc(3);	// p�� ����3����ŭ�� �޸� �����Ҵ�
		p[0] = in[firstIdx];	// ù ���ڸ� �����Ҵ�� �迭�� ����
		// ����� ���ڿ� �ڿ� ���鹮�ڿ� NULL���� �߰�
		p[1] = ' ';
		p[2] = '\0';
		// ���޹��� ���ڿ��� ù ���ڰ� �ƴ� ���� ����Ű���� lastIdx�� 1�߰�
		lastIdx++;
	}
	// ������ ��ū p�� �ּҰ��� ��ȯ
	return p;
}
// ���ڿ��� ���鹮�� ������ �����ϰ� ����� �Լ�
char* set_std_space(char* str)	//p1->array, str->p1
{
	int len;	// ���ڿ��� ���̸� �����ϴ� ����
	char* temp = (char*)malloc(strlen(str) * 2 + 1);	// ���޹��� ���ڿ��� ���鹮�ڰ� ���� ��� �߰��Ǵ� ���鹮�ڿ� ���� �þ ũ�⸦ ������ �����Ҵ�
	temp[0] = '\0';	// �����Ҵ���� �迭�� ù ���ڸ� NULL���ڷ� �ʱ�ȭ

	// ���ڿ� ��ū�� ��ȯ�޾� ������ ����
	char* token = get_token(str);
	while (token != NULL) {	// ��ū�� NULL�϶����� �ݺ�
		strcat(temp, token);	// temp�� NULL���ں��� token ���ڿ��� �̾���δ�
		free(token);			// ���� token�� �޸� ��ȯ
		token = get_token(str);	// ���ڿ��κ��� ���ڿ� ��ū�� ������
	}
	len = strlen(temp);	// �ϼ��� temp�� ���̸� ����
	str = (char*)realloc(str, len + 1);	// temp�� ���� +1��ŭ �Ű������� ����� �ּҰ��� ����Ű�� �����Ҵ� �޸��� ũ�⸦ ���Ҵ�. NULL���ڸ� ���� +1
	strncpy(str, temp, len + 1);	// str�� temp�� ���ڿ��� len+1��ŭ ����
	free(temp);	// temp�� ����Ű�� �Ҵ�� �޸� ��ȯ
	return str;	// �����Ҵ�� ���ڿ� �迭�� �ּҰ��� ��ȯ
}
// ��ȣ�˻��ϴ� �Լ�
int check_matching(char* in)
{
	StackType s;	// ���� ����
	char ch, open_ch;	// ���ڰ� ��� ���� �ΰ��� ����
	// ���ڿ� �迭�� ����ų ���� i�� ���ڿ��� ���̸� ��� ���� n�� ���� �� �ʱ�ȭ
	int i, n = strlen(in); // n= ���ڿ��� ����
	init_stack(&s); // ������ �ʱ�ȭ
	for (i = 0; i < n; i++) {	// i�� 0���� n-1���� �ݺ�
		ch = in[i]; // ch = ���� ����
		switch (ch) {
		case '(':	// ���ڰ� '('�� ��� ���ÿ� ����
			push(&s, &ch);
			break;
		case ')':	// ���ڰ� ')'�� ��� �迭�� ��������� 0�� ��ȯ, �ƴϸ� ���ÿ��� ����(���� �� ��ȯ)
			if (is_empty(&s)) return 0;
			else {
				open_ch = *(pop(&s));
				if (open_ch == '(' && ch != ')') {	// ���ÿ��� ������ ���ڰ� '('�̰� ������ ���� ���ڰ� ')'�� �ƴϸ� 0�� ��ȯ
					return 0;
				}
				break;
			}
		}
	}
	if (!is_empty(&s)) return 0; // ���ÿ� ���������� ����
	return 1;	// ���ÿ� �������������� 1 ��ȯ
}
// ���ʿ��� ��ȣ�� ������ �˻�
int check_opr_N_num(char* in)
{
	int n = strlen(in); // n= ���ڿ��� ����
	return strspn(in, "+-*/().1234567890 ") == n;	// �����ڿ� ���Ե��� ���� ���ڰ� ������ 1�� ��ȯ
}
// �ǿ����� ���� = ������ ���� +1���� �˻�
int is_op_num_correct(char* in)
{
	char* token;	// ���ڿ� ��ū�� ���� ������ ���� ���� 
	int count_num = 0, count_op = 0;	// �ǿ������� ������ �������� ������ �� ���� ���� �� �ʱ�ȭ
	token = get_token(in);	// token�� ���ڿ� ��ū�� �ּҰ� ����
	while (token != NULL) {	// token�� NULL�� �ƴҶ����� �ݺ�
		if (isdigit(token[0]))	// token�� ù ���ڰ� �����̸� �ǿ����� ���� 1����
			count_num++;
		else if (prec(token) == 2 || prec(token) == 3)	// token�� ù ���ڰ� ��Ģ���� ����(+,-<*,/)�̸� ������ ���� 1����
			count_op++;
		free(token);	// ���� ��ū �޸� ��ȯ
		token = get_token(in);	// token�� ���ڿ� ��ū�� �ּҰ� ����
	}
	return count_num == count_op + 1;	// �ǿ����� ���� = ������ ���� +1�̸� 1 ��ȯ �ƴϸ� 0 ��ȯ
}
// ��ū�� ���ڷ� ��ȯ
double token_to_double(char* token)
{
	double num = 0;	// double�� ���� ���� �� �ʱ�ȭ
	if (token != NULL) {	// �Ű������� ���޹��� ���ڿ��� NULL�� �ƴϸ� ���� num�� token�� ���� ��ȯ �� ����
		sscanf(token, "%lf", &num);
	}
	return num;	// ��ȯ�� double�� �������� ��ȯ
}
// ���ڸ� ��ū���� ��ȯ
element double_to_token(double num)
{
	char* token;	// ���ڿ� �迭�� ����Ű�� ������ ���� ����
	char* temp = (char*)malloc(100);	// temp�� ũ�� 100��ŭ �����Ҵ�
	int len;	// ���̸� �����ϴ� ���� ����

	if (num != 0)	// �Ű������� ���޹��� ���� 0�� �ƴϸ� ���ڿ��� ��ȯ�� temp�� ����
		sprintf(temp, "%lf", num);
	len = strlen(temp);	// temp ���ڿ��� ���̸� ����
	token = (char*)malloc(len + 1);	// token�� len+1��ŭ �����Ҵ�
	strncpy(token, temp, len + 1);	// temp�� token�� len+1��ŭ ���ڿ� ����
	free(temp);	// ���� temp�� �޸� ��ȯ
	return token;	// token�� ��ȯ
}
// ������ ������ ������ �˻�
int return_expr_type(char* in)	// ����, ����, �������� ��� ���� 1, 2, 3�� ��ȯ. �ƴҽ� 0 ��ȯ
{
	char* token;	// ���ڿ� �迭�� ����Ű�� ������ ���� ����
	int is_prefix = 0, is_infix = 0, is_postfix = 0;	// ������, ������, �������� �Ǵ� ������ ��Ÿ���� ����

	token = get_token(in);		// token�� ���ڿ� ��ū�� �ּҰ� ����
	if (prec(token) == 2 || prec(token) == 3) 		// +, -, *, /, �� ��� is_prefix�� 1
		is_prefix = 1;	
	while (token != NULL) {
		if (prec(token) == 1)	// (, )�� ���
			is_infix = 1;
		if (prec(token) == 2 || prec(token) == 3)	// +, -, *, / �� ��� is_postfix�� 1
			is_postfix = 1;
		else										// ������ ��ū�� +, -, *, / �� �ƴ� ��� is_postfix�� 0
			is_postfix = 0;
		free(token);	// ����� token�� �޸� ��ȯ
		token = get_token(in);		// token�� ���ڿ� ��ū�� �ּҰ� ����
	}
	free(token);		// ����� token�� �޸� ��ȯ

	if (!is_prefix && !is_postfix)	// is_prefix�� is_postfix�� �Ѵ� 0�ϰ�� 2��ȯ
		return 2;
	if (is_prefix && !is_infix && !is_postfix)	// is_prefix�� 1�̰� is_infix, is_postfix�� 0�ϰ�� 1��ȯ
		return 1;
	if (!is_prefix && !is_infix && is_postfix)	// is_postfix�� 1�̰� is_infix, is_pretfix�� 0�ϰ�� 3��ȯ
		return 3;
	return 0;	// �� ��쿡 �ش����� ���� ��� 0 ��ȯ(���� �߻�)
}
// ���ڿ� ���� ��ġ�� �Լ�
char* combine_3str(char* s1, char* s2, char* s3)
{
	char* temp = (char*)malloc(strlen(s1) + strlen(s2) + strlen(s3) + 1);	// �� ���ڿ��� ������ �� + 1(NULL����)��ŭ �����Ҵ�
	strcpy(temp, s1);	// s1 ���ڿ��� temp�� ����
	strcat(temp, s2);	// s2 ���ڿ��� temp�� �̾� ����
	strcat(temp, s3);	// s3 ���ڿ��� temp�� �̾� ����
	free(s1);	// s1�� �Ҵ�� �޸� ��ȯ
	free(s2);	// s2�� �Ҵ�� �޸� ��ȯ
	free(s3);	// s3�� �Ҵ�� �޸� ��ȯ
	return temp;	// temp�� �ּҰ� ��ȯ
}
// ���ڿ� ���� ��ȣ�ȿ� ��ġ�� �Լ�
char* combine_3str_prec(char* s1, char* s2, char* s3)
{
	char* temp = (char*)malloc(strlen(s1) + strlen(s2) + strlen(s3) + 5);	// �� ���ڿ��� ������ �� + ��ȣ ���ڿ��� ����4 + 1(NULL����)��ŭ �����Ҵ�
	strcpy(temp, "( ");	// �����ʰ�ȣ ���ڿ��� temp�� ����
	strcat(temp, s1);	// s1 ���ڿ��� temp�� �̾� ����
	strcat(temp, s2);	// s2 ���ڿ��� temp�� �̾� ����
	strcat(temp, s3);	// s3 ���ڿ��� temp�� �̾� ����
	strcat(temp, ") ");	// �����ʰ�ȣ ���ڿ��� temp�� �̾� ����
	free(s1);	// s1�� �Ҵ�� �޸� ��ȯ
	free(s2);	// s2�� �Ҵ�� �޸� ��ȯ
	free(s3);	// s3�� �Ҵ�� �޸� ��ȯ
	return temp;	// temp�� �ּҰ� ��ȯ
}

//���� ��Ʈ
// ���� ǥ�� ���� -> ���� ǥ�� ���� ��ȯ
char* infix_to_postfix(char* exp)		
{
	int len = strlen(exp);	// �Ű������� ���޹��� ���ڿ��� ���� ����
	char* token, * token_tmp, * p_postfix;	// ���ڿ���ū�� ���� �����ͺ���, �߰��� ���ڿ���ū�� ���� �����ͺ���, ��ȯ�� ���ڿ��� ���� �����ͺ��� ����
	char* temp = (char*)malloc(strlen(exp) * 2);	// �ӽ÷� ���ڿ��� ���� �迭�� �����Ҵ�
	StackType s;	// ���� ����
	init_stack(&s); // ���� �ʱ�ȭ

	temp[0] = '\0';	// temp�� ù ���ڸ� NULL���ڷ� �ʱ�ȭ

	token = get_token(exp);	// �Ű������� ���޹��� ���ڿ��κ��� ��ū�� ����
	while (token != NULL) {
		switch (token[0]) {
		case '+': case '-': case '*': case '/': // ��ū�� ù���ڰ� ��Ģ�������� ���
			// ���ÿ� �ִ� �������� �켱������ �� ũ�ų� ���� ���� ���ÿ� �ִ� �����͸� temp�� �̾����
			while (!is_empty(&s) && (prec(token) <= prec(peek(&s)))) {
				token_tmp = pop(&s);
				strcat(temp, token_tmp);
				free(token_tmp);	// ����� token_tmp�� �޸� ��ȯ
			}
			push(&s, token);	// token�� ���ÿ� ����
			break;
		case '(': // ���� ��ȣ�� ���
			push(&s, token);	// token�� ���ÿ� ����
			break;
		case ')': // ������ ��ȣ�� ���
			token = pop(&s);	// ���ÿ��� �����͸� ����
			// ���� ��ȣ�� ���������� ���ÿ��� �����͸� ���� temp�� �̾����
			while (token[0] != '(') {
				strcat(temp, token);
				free(token);		// ����� token�� �޸� ��ȯ
				token = pop(&s);	// ���ÿ��� �����͸� ����
			}
			break;
		default: // �ǿ������� ��� temp�� �̾����
			strcat(temp, token);	
			free(token);	// ����� token�� �޸� ��ȯ
			break;
		}
		token = get_token(exp);	// �Ű������� ���޹��� ���ڿ��κ��� ��ū�� ����
	}
	while (!is_empty(&s)) {// ���ÿ� ����� �����ڵ��� ��� ���� temp�� �̾����
		token = pop(&s);		// ���ÿ��� �����͸� ����
		strcat(temp, token);	// ���ÿ��� �����͸� ����
		free(token);			// ����� token�� �޸� ��ȯ
	}
	len = strlen(temp);	// temp�� ���� ����
	p_postfix = (char*)malloc(len + 1);	// len+1��ŭ �����Ҵ�
	strncpy(p_postfix, temp, len + 1);	// p_postfix�� temp�� ���ڿ��� len+1��ŭ ����
	free(temp);	// temp�� �޸� ��ȯ
	return p_postfix;	// ���������� ��ȯ�� ���ڿ��迭�� ����Ű�� �ּҰ� ��ȯ
}
// ���� ǥ�� ���� -> ���� ǥ�� ���� ��ȯ
char* infix_to_prefix(char* exp)
{
	int len;	// ���ڿ��� ���̸� ��� ���� ����
	char* token, * op1, * op2, * p_prefix;// ���ڿ���ū�� ���� �����ͺ���, �����ڹ��ڿ��� ���� �����ͺ���, ��ȯ�� ���ڿ��� ���� �����ͺ��� ����
	StackType s_operand;	// ���� ����
	StackType s_operator;	// ���� ����
	init_stack(&s_operand); // ���� �ʱ�ȭ
	init_stack(&s_operator); // ���� �ʱ�ȭ

	char* temp = (char*)malloc(strlen(exp) * 2); // ���� ��ȯ�� ���ڿ��� �ӽ÷� ���� ���ڿ� �迭�� �����Ҵ�
	temp[0] = '\0';		// ���ڿ��迭�� ù���ڸ� NULL���ڷ� �ʱ�ȭ
	char* temp_op = (char*)malloc(strlen(exp) * 2); // ���յ� ���ڿ��� �ӽ÷� ���� ���ڿ� �迭�� �����Ҵ�
	temp_op[0] = '\0';	// ���ڿ��迭�� ù���ڸ� NULL���ڷ� �ʱ�ȭ


	token = get_token(exp);	// �Ű������� ���޹��� ���ڿ��κ��� ��ū�� ����
	while (token != NULL) {	// ��ū�� NULL�� �ƴҶ����� �ݺ�
		switch (token[0]) {
		case '+': case '-': case '*': case '/': // ��ū�� ù���ڰ� ��Ģ�������� ���
			// ���ÿ� �ִ� �������� �켱������ �� ũ�ų� ���� ���� ���ÿ� �ִ� �����͸� temp�� �̾����			
			while (!is_empty(&s_operator) && (prec(token) <= prec(peek(&s_operator)))) {
				op2 = pop(&s_operand);
				op1 = pop(&s_operand);

				temp_op = combine_3str(pop(&s_operator), op1, op2);
				push(&s_operand, temp_op);
			}
			push(&s_operator, token);
			break;
		case '(': // ���� ��ȣ�� ���
			push(&s_operator, token);
			break;
		case ')': // ������ ��ȣ�� ���
			token = pop(&s_operator);
			// ���� ��ȣ�� ���������� ���ÿ��� �����͸� ���� temp�� �̾����
			while (token[0] != '(') {
				op2 = pop(&s_operand);
				op1 = pop(&s_operand);

				temp_op = combine_3str(token, op1, op2);
				push(&s_operand, temp_op);
				token = pop(&s_operator);
			}
			break;
		default: // �ǿ�����
			push(&s_operand, token);
			break;
		}
		token = get_token(exp);
	}
	while (!is_empty(&s_operator)) {// ���ÿ� ����� �����ڵ� ���
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
// ���ڿ� �����Լ�
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
	while (!is_empty(&s)) {// ���ÿ� ����� �����ڵ� ���
		token = pop(&s);
		strcat(exp_reverse, token);
	}
	return exp_reverse;
}
// ���� ǥ�� ���� -> ���� ǥ�� ���� ��ȯ
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
		case '+': case '-': case '*': case '/': // ������
			op1 = pop(&s);
			op2 = pop(&s);

			temp = combine_3str_prec(op1, token, op2);
			push(&s, temp);
			break;
		default: // �ǿ�����
			push(&s, token);
			break;
		}
		token = get_token(exp_reverse);
	}
	return pop(&s);
}
// ���� ǥ�� ���� -> ���� ǥ�� ���� ��ȯ
char* prefix_to_postfix(char* exp)
{
	char* p;
	p = prefix_to_infix(exp);
	p = infix_to_postfix(p);
	return p;
}
// ���� ǥ�� ���� -> ���� ǥ�� ���� ��ȯ
char* postfix_to_infix(char* exp)
{
	StackType s;
	char* token, * exp_reverse, * op1, * op2, * temp;
	int len;

	init_stack(&s);

	token = get_token(exp);
	while (token != NULL) {
		switch (token[0]) {
		case '+': case '-': case '*': case '/': // ������
			op2 = pop(&s);
			op1 = pop(&s);

			temp = combine_3str_prec(op1, token, op2);
			push(&s, temp);
			break;
		default: // �ǿ�����
			push(&s, token);
			break;
		}
		token = get_token(exp);
	}
	return pop(&s);
}
// ���� ǥ�� ���� -> ���� ǥ�� ���� ��ȯ
char* postfix_to_prefix(char* exp)
{
	char* p;
	p = postfix_to_infix(exp);
	p = infix_to_prefix(p);
	return p;
}
// ���� ǥ�� ���� ��� �Լ�
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
		else { //�������̸� �ǿ����ڸ� ���ÿ��� ����
			tmp_1 = pop(&s);
			tmp_2 = pop(&s);
			op2 = token_to_double(tmp_1);
			op1 = token_to_double(tmp_2);
			switch (ch) { //������ �����ϰ� ���ÿ� ����
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
					printf("0���� ���� �� �����ϴ�.\n");
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
	// �� �˻�
	// �� �ùٸ��� �˻� (+_* /.()�����̿��� ���ڰ� �ִ���)
	if (check_opr_N_num(p_input) == 0) {
		printf("�ùٸ������� ���ڰ� ���ԵǾ� �ֽ��ϴ�.\n");
		return 1;
	}
	// �ǿ����� ���� = ������ ����+1���� �˻�					 123.34+12-3/4
	if (is_op_num_correct(p_input) == 0) {
		printf("������ �ǿ����� ������ �ùٸ��� �ʽ��ϴ�.\n");
		return 1;
	}
	// 1-1.��ȣ�˻�
	if (check_matching(p_input) == 0) {
		printf("��ȣ ¦�� ���� �ʽ��ϴ�.\n");
		return 1;
	}
	// ������ ������ ������ �˻�
	expr_type = return_expr_type(p_input);
	switch (expr_type) {
	case 1: {	// ������
		p_in = prefix_to_infix(p_input);
		p_post = prefix_to_postfix(p_input);
		printf("Prefix:  %s \n", p_input);
		printf("Infix: \t %s \n", p_in);
		printf("Postfix: %s \n", p_post);
		printf("result = %lf\n", eval(p_post));
		break;
	}
	case 2: {	// ������
		p_pre = infix_to_prefix(p_input);
		p_post = infix_to_postfix(p_input);
		printf("Prefix:  %s \n", p_pre);
		printf("Infix: \t %s \n", p_input);
		printf("Postfix: %s \n", p_post);	//Infix : (2 + 5) * 3 * (2 + 1) ,Prefix: 2 5 + 3 * 2 1 + * , Postfix: 2 5 + 3 * 2 1 + *
		printf("result = %lf\n", eval(p_post));
		break;
	}
	case 3: {	// ������
		p_in = postfix_to_infix(p_input);
		p_pre = postfix_to_prefix(p_input);
		printf("Prefix:  %s \n", p_pre);
		printf("Infix: \t %s \n", p_in);
		printf("Postfix: %s \n", p_input);
		printf("result = %lf\n", eval(p_input));
		break;
	}
	default: {
		printf("�ùٸ��� ���� ���Դϴ�.\n");
		break;
	}
	}


	return 0;
}
#endif