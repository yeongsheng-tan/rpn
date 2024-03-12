/////////////////////////////////////////////////////
#include <stdio.h>
#include <stdlib.h>
#include <limits.h> // Include limits.h for INT_MIN
#include <ctype.h> // isdigit function header def

#define SIZE 1000 // The limit of expression length

////////////////////////////////////////////////////

typedef struct _listnode {
    double item;
    struct _listnode *next;
} ListNode;

typedef struct _linkedlist {
    int size;
    ListNode *head;
} LinkedList;

////////////////////////////////// stack    /////////

typedef struct stack {
    LinkedList ll;
} Stack;

//////////////////////// function prototypes //////////////

// You should not change the prototypes of these functions

double postfixEvaluation(char* postfix);

////////////////////////////////////////////////////////////

void push(Stack *s, double item);
double pop(Stack *s); // Change return type to double
double peek(Stack *s); // Change return type to double
int isEmptyStack(Stack *s);

//////////////////////////////////////////////////////////////
void printList(LinkedList *ll);
ListNode *findNode(LinkedList *ll, int index);
int insertNode(LinkedList *ll, int index, double item);
int removeNode(LinkedList *ll, int index);
void removeAllItems(LinkedList *ll);

//////////////////////////////////////////////////////////////

int main() {
    char postfix[SIZE];

    printf("Enter a postfix expression:\n");
    scanf("%[^\n]%*c", postfix);

    printf("The answer is %.2lf\n", postfixEvaluation(postfix)); // Correct function name

    return 0;
}

double postfixEvaluation(char* postfix) {
    // pseudo-code
    // 1) when encounter current position char is a digit, push into stack linked list
    // 2) loop next position char. repeat step (1) if is a digit, else goto step (3)
    // 3) if current position char is an operand of either (+, -, * or /), pop last 2 stack items and operate them using the current operand
    // 4) repeat step (2)
    // 5) when reach end of char list, exit function
    Stack s; // Create local temp var stack
    s.ll.head = NULL; // Initialize local temp var linked list in above stack with NULL
    s.ll.size = 0; // Set the initial linked list in the above stack with an initial size of 0

    int i = 0;
    char ch;
    double operand1, operand2, result;

    while (postfix[i] != '\0') {
        ch = postfix[i];
        // If operand, push it onto the stack
        if (isdigit(ch)) {
            sscanf(&ch, "%lf", &operand1); // Read the operand
            // printf("postfix[%d]: %c\n", i, postfix[i]);
            push(&s, operand1);
            // printf("IN IF block stack: %lf\n", peek(&s));
        } else {
            // Operator of either (+, -, * or /) encountered
            // If operator, pop two operands, perform operation, and push result
            
            // Pop right most operand (operand #2)
            operand2 = pop(&s);

            // Pop next right most operand (operand #1)
            operand1 = pop(&s);

            // printf("operand2: %lf, operand1: %lf\n", operand2, operand1);
            switch (ch) {
                case '+':
                    result = operand1 + operand2;
                    break;
                case '-':
                    result = operand1 - operand2;
                    break;
                case '*':
                    result = operand1 * operand2;
                    break;
                case '/':
                    // handle case when division operator is encountered with 2nd operand being 0 (avoid division by zero error)
                    if (operand2 == 0) {
                        printf("Error: Division by zero\n");
                        return INT_MIN; // Or handle error differently
                    }
                    result = operand1 / operand2;
                    break;
                default:
                    // handle case when operator is neither of (+, -, * or /)
                    printf("Error: Invalid operator\n");
                    return INT_MIN; // Or handle error differently
            }
            // push intermediate "operand1 operator operand2" back into stack 
            // printf("intermediate result: %.2lf\n", result);
            push(&s, result);
        }

        // loop to pick next character from "postfix" input parameter and evaluate as above
        i++;
        // printf("WHILE LOOP BEGIN stack: %lf\n", peek(&s));
    }

    // return the last element in the stack as the result
    return pop(&s);
}

//////////////////////////////////////////////////////////////////////

void push(Stack *s, double item) {
    insertNode(&(s->ll), 0, item);
}

double pop(Stack *s) { // Change return type to double
    double item;
    if (!isEmptyStack(s)) {
        item = ((s->ll).head)->item;
        removeNode(&(s->ll), 0);
        return item;
    }
    return INT_MIN;
}

double peek(Stack *s) { // Change return type to double
    return ((s->ll).head)->item;
}

int isEmptyStack(Stack *s) {
    if ((s->ll).size == 0)
        return 1;
    return 0;
}

/////////////////////////////////////////////////////////////

void printList(LinkedList *ll) {

    ListNode *cur;
    if (ll == NULL)
        return;
    cur = ll->head;

    if (cur == NULL)
        printf("Empty");
    while (cur != NULL) {
        printf("%.2lf ", cur->item);
        cur = cur->next;
    }
    printf("\n");
}

ListNode *findNode(LinkedList *ll, int index) {

    ListNode *temp;

    if (ll == NULL || index < 0 || index >= ll->size)
        return NULL;

    temp = ll->head;

    if (temp == NULL || index < 0)
        return NULL;

    while (index > 0) {
        temp = temp->next;
        if (temp == NULL)
            return NULL;
        index--;
    }

    return temp;
}

int insertNode(LinkedList *ll, int index, double value) {

    ListNode *pre, *cur;

    if (ll == NULL || index < 0 || index > ll->size + 1)
        return -1;

    // If empty list or inserting first node, need to update head pointer
    if (ll->head == NULL || index == 0) {
        cur = ll->head;
        ll->head = malloc(sizeof(ListNode));
        ll->head->item = value;
        ll->head->next = cur;
        ll->size++;
        return 0;
    }


    // Find the nodes before and at the target position
    // Create a new node and reconnect the links
    if ((pre = findNode(ll, index - 1)) != NULL) {
        cur = pre->next;
        pre->next = malloc(sizeof(ListNode));
        pre->next->item = value;
        pre->next->next = cur;
        ll->size++;
        return 0;
    }

    return -1;
}

int removeNode(LinkedList *ll, int index){

	ListNode *pre, *cur;

	// Highest index we can remove is size-1
	if (ll == NULL || index < 0 || index >= ll->size)
		return -1;

	// If removing first node, need to update head pointer
	if (index == 0){
		cur = ll->head->next;
		free(ll->head);
		ll->head = cur;
		ll->size--;

		return 0;
	}

	// Find the nodes before and after the target position
	// Free the target node and reconnect the links
	if ((pre = findNode(ll, index - 1)) != NULL){

		if (pre->next == NULL)
			return -1;

		cur = pre->next;
		pre->next = cur->next;
		free(cur);
		ll->size--;
		return 0;
	}

	return -1;
}
