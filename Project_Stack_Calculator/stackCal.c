#if 01
/*

num: number
opr: operator
opd: operand
elem: elementary
arith: arithmetic
N: and
brkt: bracket
*/

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


typedef char* element;
typedef struct __node {
	element data;
	struct __node* link;
}StackNode;
typedef struct __stacktype {
	StackNode* top;
}StackType;
void init_stack(StackType* s)
{
	s->top = NULL;
}
int is_empty(StackType* s)
{
	return s->top == NULL;
}
void push(StackType* s, element item)
{
	StackNode* p = (StackNode*)malloc(sizeof(StackNode));
	p->data = item;
	p->link = s->top;
	s->top = p;
}
element pop(StackType* s)
{
	if (is_empty(s)) {
		fprintf(stderr, "Stack �� ����(pop)");
		exit(1);
	}
	StackNode* rnode = s->top;
	element rdata = s->top->data;
	s->top = s->top->link;
	free(rnode);
	return rdata;
}
element peek(StackType* s)
{
	if (is_empty(s)) {
		fprintf(stderr, "Stack �� ����(peek)");
		exit(1);
	}
	element rdata = s->top->data;
	return rdata;
}


char* get_token(const char* in);

// �Է°��� ��Ģ�������� Ȯ���ϴ� �Լ�
int return_arith_type(const char* p) // +, -, *, / �� ��� 1 ��ȯ, // ( ,) �� ��� 2��ȯ
{
	switch (p[0]) {
	case '+': case '-':case '*':case '/': {
		return 1;
	}
	case '(': case ')': {
		return 2;
	}
	default: {
		return 0;
	}
	}
}
// ������ �Է¹޴� �Լ�
element create_expr()
{
	char str[100] = "";
	char* expr;
	printf("Input: ");
	gets(str);
	expr = (char*)malloc(strlen(str) + 1);
	strcpy(expr, str);
	return expr;
}
// �������� �켱������ ��ȯ�Ѵ�
int prec(const char* op) {
	switch (op[0]) {
	case '(': case ')': return 1;
	case '+': case '-': return 2;
	case '*': case '/': return 3;
	}
	return 0;
}
// ���ڿ� ǥ��ȭ �Լ�	ex)"2.13 ", "+ ", "132 "
char * set_std_space(char* str)	//p1->array, str->p1
{
	int len;
	char* temp = (char*)malloc(strlen(str) * 2 + 1);
	temp[0] = '\0';

	char* token = get_token(str);
	while (token != NULL) {
		strcat(temp, token);
		free(token);
		token = get_token(str);
	}
	len = strlen(temp);
	str = (char*)realloc(str, len+1);
	strncpy(str, temp, len+1);
	free(temp);
	return str;
}
// �����ڿ� �ǿ����ڸ� ���� ��ȯ�ϴ� �Լ�
element get_token(const char* in)
{
	static int firstIdx = 0, lastIdx = 0;
	int i;
	char* p;

	if (in[lastIdx] == ' ') {		// ù���ڰ� ���鹮�ڸ� ���鹮�ڸ�ŭ �ε��� �̵�
		lastIdx += strspn(&in[lastIdx], " ");
	}
	if (in[lastIdx] == '\0') {
		firstIdx = 0;
		lastIdx = 0;
		return NULL;
	}

	firstIdx = lastIdx;

	if (isdigit(in[firstIdx])) {
		lastIdx += strspn(&in[lastIdx], "1234567890.");
		p = (char*)malloc(sizeof(char) * (lastIdx - firstIdx + 2)); // ���ڿ� �ǵ� ������ĭ�� NULL���ڸ� ���� +2
		for (i = 0; firstIdx < lastIdx; i++, firstIdx++) {
			p[i] = in[firstIdx];
		}
		p[i] = ' ';
		p[i+1] = '\0';
	}
	else {
		p = (char*)malloc(3);
		p[0] = in[firstIdx];
		p[1] = ' ';
		p[2] = '\0';
		lastIdx++;
	}

	return p;
}
// ��ȣ�˻�
int check_matching(const char* in)
{
	StackType s;
	char ch, open_ch;
	int i, n = strlen(in); // n= ���ڿ��� ����
	init_stack(&s); // ������ �ʱ�ȭ
	for (i = 0; i < n; i++) {
		ch = in[i]; // ch = ���� ����
		switch (ch) {
		case '(':
			push(&s, &ch);
			break;
		case ')':
			if (is_empty(&s)) return 0;
			else {
				open_ch = *(pop(&s));
				if (open_ch == '(' && ch != ')') {
					return 0;
				}
				break;
			}
		}
	}
	if (!is_empty(&s)) return 0; // ���ÿ� ���������� ����
	return 1;
}
// ���ʿ��� ��ȣ�� ������ �˻�
int check_opr_N_num(const char* in)
{
	int n = strlen(in); // n= ���ڿ��� ����
	// �����ڿ� ���Ե��� ���� ���ڰ� ������ 1�� ��ȯ
	return strspn(in, "+-*/().1234567890 ") == n;
}
// �ǿ����� ���� = ������ ���� +1 �˻�
int is_op_num_correct(char* in)
{
	char* token;
	int count_num = 0, count_op = 0;
	token = get_token(in);
	while (token != NULL) {
		if (isdigit(token[0]))
			count_num++;
		else if(prec(token) == 2 || prec(token) == 3)
			count_op++;
		token = get_token(in);
	}
	return count_num == count_op + 1;
}
// ��ū�� ���ڷ� ��ȯ
double token_to_double(char* token)
{
	double num = 0;
	if (token != NULL) {
		sscanf(token, "%lf", &num);
	}
	return num;
}
// ���ڸ� ��ū���� ��ȯ
element double_to_token(double num)
{
	char* token;
	char* temp = (char*)malloc(100);
	int len;

	if (num != 0) 
		sprintf(temp, "%lf", num);
	len = strlen(temp);
	token = (char*)malloc(len + 1);
	strncpy(token, temp, len + 1);
	free(temp);
	return token;
}
// ������ ������ ������ �˻�
int return_expr_type(const char* in)	// ����, ����, �������� ��� ���� 1, 2, 3�� ��ȯ. �ƴҽ� 0 ��ȯ
{
	char* token;
	int is_prefix = 0, is_infix = 0, is_postfix = 0;

	token = get_token(in);
	if (prec(token) == 2 || prec(token) == 3) 		// +, -, *, /, �� ���
		is_prefix = 1;
	while (token != NULL) {
		if (prec(token) == 1)	// (, )�� ���
			is_infix = 1;
		if (prec(token) == 2 || prec(token) == 3)	// +, -, *, /, �� ���
			is_postfix = 1;
		else
			is_postfix = 0;
		free(token);
		token = get_token(in);
	}
	free(token);
	if (!is_infix && !is_postfix)
		return 1;
	if (!is_prefix && is_infix && !is_postfix)
		return 2;
	 
	if (is_prefix) {
		if (!is_infix && !is_postfix)
			return 1;
		else
			return 0;
	}
	else {
		if (is_infix && !is_postfix)
			return 2;
		else if (!is_infix && is_postfix)
			return 3;
	}
	return 0;
}


//���� ��Ʈ
// ���� ǥ�� ���� -> ���� ǥ�� ���� ��ȯ
char* infix_to_postfix(const char* exp)		// (235 + 5.2768)* 34 *    (2 +100) 
{
	int len = strlen(exp);
	char* token, * token_tmp, * p_postfix;
	char* temp = (char*)malloc(strlen(exp) * 2);
	StackType s;
	init_stack(&s); // ���� �ʱ�ȭ
	
	temp[0] = '\0';

	//for (i = 0; i < len; i++)
	token = get_token(exp);
	while (token != NULL) {
		switch (token[0]) {
		case '+': case '-': case '*': case '/': // ������
			// ���ÿ� �ִ� �������� �켱������ �� ũ�ų� ������ ���
			while (!is_empty(&s) && (prec(token) <= prec(peek(&s)))) {
				token_tmp = pop(&s);
				strcat(temp, token_tmp);
				free(token_tmp);
			}
			push(&s, token);
			break;
		case '(': // ���� ��ȣ
			push(&s, token);
			break;
		case ')': // ������ ��ȣ
			token = pop(&s);
			// ���� ��ȣ�� ���������� ���
			while (token[0] != '(') {
				strcat(temp, token);
				free(token);
				token = pop(&s);
			}
			break;
		default: // �ǿ�����
			strcat(temp, token);
			free(token);
			break;
		}
		token = get_token(exp);
	}
	while (!is_empty(&s)) {// ���ÿ� ����� �����ڵ� ���
		token = pop(&s);
		strcat(temp, token);
		free(token);
	}
	len = strlen(temp);
	p_postfix = (char*)malloc(len + 1);
	strncpy(p_postfix, temp, len + 1);
	free(temp);
	return p_postfix;
}
// ���ڿ� ���� ��ġ�� �Լ�
char* combine_3str(const char* s1, const char* s2, const char* s3)
{
	char* temp = (char*)malloc(strlen(s1) + strlen(s2) + strlen(s3) + 3);
	strcpy(temp, s1);
	strcat(temp, s2);
	strcat(temp, s3);
	free(s1);
	free(s2);
	free(s3);
	return temp;
}
// ���ڿ� ���� ��ȣ�ȿ� ��ġ�� �Լ�
char* combine_3str_prec(const char* s1, const char* s2, const char* s3)
{
	char* temp = (char*)malloc(strlen(s1) + strlen(s2) + strlen(s3) + 5);
	strcpy(temp, "( ");
	strcat(temp, s1);
	strcat(temp, s2);
	strcat(temp, s3);
	strcat(temp, ") ");
	free(s1);
	free(s2);
	free(s3);
	return temp;
}
// ���� ǥ�� ���� -> ���� ǥ�� ���� ��ȯ
char* infix_to_prefix(const char* exp)
{
	int len;
	char* token, * op1, * op2, * p_prefix;
	StackType s_operand;
	StackType s_operator;
	init_stack(&s_operand); // ���� �ʱ�ȭ
	init_stack(&s_operator); // ���� �ʱ�ȭ

	char* temp = (char*)malloc(strlen(exp) * 2);
	temp[0] = '\0';
	char* temp_op = (char*)malloc(strlen(exp) * 2);
	temp_op[0] = '\0';


	token = get_token(exp);
	while (token != NULL) {
		switch (token[0]) {
		case '+': case '-': case '*': case '/': // ������
			// ���ÿ� �ִ� �������� �켱������ �� ũ�ų� ������ ���
			while (!is_empty(&s_operator) && (prec(token) <= prec(peek(&s_operator)))) { // (2 + 5) * 3 * (2 + 1)
				op2 = pop(&s_operand);
				op1 = pop(&s_operand);

				temp_op = combine_3str(pop(&s_operator), op1, op2);
				push(&s_operand, temp_op);
			}
			push(&s_operator, token);
			break;
		case '(': // ���� ��ȣ
			push(&s_operator, token);
			break;
		case ')': // ������ ��ȣ
			token = pop(&s_operator);
			// ���� ��ȣ�� ���������� ���
			while (token[0] != '(') {
				op2 = pop(&s_operand);
				op1 = pop(&s_operand);

				temp_op = combine_3str(token, op1, op2);
				push(&s_operand, temp_op);
				//free(token);
				token = pop(&s_operator);
			}
			break;
		default: // �ǿ�����
			push(&s_operand, token);
			//free(token);
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
	strcpy(temp, pop(&s_operand));

	len = strlen(temp);
	p_prefix = (char*)malloc(len + 1);
	strncpy(p_prefix, temp, len + 1);
	return p_prefix;
}
// ���ڿ� �����Լ�
char* reverse_str(const char* str)
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
char* prefix_to_infix(const char* exp)
{
	StackType s;
	char* token, * exp_reverse, * op1, * op2, *temp;
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
char* postfix_to_infix(const char* exp)
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
// // ���� ǥ�� ���� ��� �Լ�
double eval(char* exp)
{
	int len = strlen(exp);
	double result, op1, op2;
	char ch;
	char* token, *tmp_1, *tmp_2;
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
	char * p_input = create_expr();	//  (235 + 5.2768)* 34 *    (2 +100)     3 + 2
	char* p_post, *p_pre, *p_in;
	int expr_type;
	StackType s;
	init_stack(&s);

	p_input = set_std_space(p_input);
	// �� �˻�
	// �� �ùٸ��� �˻� (+_* /.()�����̿��� ���ڰ� �ִ���)
	if (check_opr_N_num(p_input) == 0) {
		printf("�ùٸ������� ���ڰ� ���ԵǾ� �ֽ��ϴ�.\n");
		return 1;
	}
	// �ǿ����� ���� = ������ ����+1���� �˻� 
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
		p_post = infix_to_postfix(p_in);
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
		p_pre = infix_to_prefix(p_in);
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