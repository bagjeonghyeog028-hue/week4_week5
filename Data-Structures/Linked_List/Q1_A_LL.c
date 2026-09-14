//////////////////////////////////////////////////////////////////////////////////

/* CE1007/CZ1007 Data Structures
Lab Test: Section A - Linked List Questions
Purpose: Implementing the required functions for Question 1 */

//////////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>

//////////////////////////////////////////////////////////////////////////////////

typedef struct _listnode{
	int item;
	struct _listnode *next;
} ListNode;			// You should not change the definition of ListNode

typedef struct _linkedlist{
	int size;
	ListNode *head;
} LinkedList;			// You should not change the definition of LinkedList


///////////////////////// function prototypes ////////////////////////////////////

//You should not change the prototype of this function
int insertSortedLL(LinkedList *ll, int item);

void printList(LinkedList *ll);
void removeAllItems(LinkedList *ll);
ListNode *findNode(LinkedList *ll, int index);
int insertNode(LinkedList *ll, int index, int value);
int removeNode(LinkedList *ll, int index);


//////////////////////////// main() //////////////////////////////////////////////

int main()
{
	LinkedList ll;
	int c, i, j;
	c = 1;

	//Initialize the linked list 1 as an empty linked list
	ll.head = NULL;
	ll.size = 0;

	printf("1: Insert an integer to the sorted linked list:\n");
	printf("2: Print the index of the most recent input value:\n");
	printf("3: Print sorted linked list:\n");
	printf("0: Quit:");

	while (c != 0)
	{
		printf("\nPlease input your choice(1/2/3/0): ");
		scanf("%d", &c);

		switch (c)
		{
		case 1:
			printf("Input an integer that you want to add to the linked list: ");
			scanf("%d", &i);
			j = insertSortedLL(&ll, i);
			printf("The resulting linked list is: ");
			printList(&ll);
			break;
		case 2:
			printf("The value %d was added at index %d\n", i, j);
			break;
		case 3:
			printf("The resulting sorted linked list is: ");
			printList(&ll);
			removeAllItems(&ll);
			break;
		case 0:
			removeAllItems(&ll);
			break;
		default:
			printf("Choice unknown;\n");
			break;
		}


	}
	return 0;
}

//////////////////////////////////////////////////////////////////////////////////

int insertSortedLL(LinkedList *ll, int item)
{
    // ll → 연결 리스트의 주소
    // item → 새롭게 추가하려는 숫자

    ListNode *cur = ll->head;
    // cur → 현재 확인하고 있는 노드
    // 처음에는 리스트의 첫 번째 노드(head)를 가리킴

    ListNode *prev = NULL;
    // prev → cur의 바로 이전 노드
    // 처음에는 이전 노드가 없으므로 NULL

    int i = 0;
    // i → 현재 몇 번째 위치인지 세는 변수
    // 0부터 시작

    while (cur != NULL) {// cur이 NULL이 아닐 동안 반복, 리스트의 끝까지 확인
        
        

        if (item < cur->item) {  // 새로 넣을 값(item)이 현재 노드의 값보다 작으면
            
            
            break;  // 현재 cur 앞에 넣어야 하므로 반복 종료

        } else if (cur->item == item) {  // 현재 노드의 값과 새로 넣을 값이 같으면, 중복되는 값이므로 넣지 않음

            i = -1;  // -1을 넣어서 "중복이라 삽입 실패"라는 것을 표시
            

            break;  // 반복 종료
            

        } else {  // item이 현재 값보다 크다면, 아직 넣을 위치를 못 찾았으므로 다음 노드로 이동
            
            

            i++;  // 위치를 하나 증가
            

            prev = cur;  // 현재 노드를 이전 노드로 저장
            

            cur = cur->next;  // cur을 다음 노드로 이동
            
        }
    }

    if (i == -1) {  // 중복 값이라면
       

        return -1;  // 삽입하지 않고 -1 반환
        
    }

    ListNode *new_node = malloc(sizeof(ListNode));  // 새로운 노드를 만들기 위해 메모리 공간을 할당,new_node가 새로 만든 노드를 가리킴

    new_node->item = item;  // 새 노드에 넣을 값을 저장

    new_node->next = cur;  // 새 노드의 다음 노드를 cur로 연결,새 노드 뒤에 cur이 오도록 연결
    

    if (prev != NULL) {  // 이전 노드가 존재한다면, 맨 앞에 넣는 상황이 아니라면
        

        prev->next = new_node;  // prev → new_node → cur,이전 노드가 새 노드를 가리키게 함
        
        
    } else {  // 새 노드를 맨 앞에 넣는 경우에 prev가 NULL이라면
        
        

        ll->head = new_node;  // 리스트의 시작점(head)을 새 노드로 변경
        
    }

    ll->size++;  // 노드가 하나 추가됐으므로 리스트 크기를 1 증가
    

    return i;  // 새 값이 몇 번째 위치에 들어갔는지 반환
    
}
///////////////////////////////////////////////////////////////////////////////////

void printList(LinkedList *ll){

	ListNode *cur;
	if (ll == NULL)
		return;
	cur = ll->head;

	if (cur == NULL)
		printf("Empty");
	while (cur != NULL)
	{
		printf("%d ", cur->item);
		cur = cur->next;
	}
	printf("\n");
}


void removeAllItems(LinkedList *ll)
{
	ListNode *cur = ll->head;
	ListNode *tmp;

	while (cur != NULL){
		tmp = cur->next;
		free(cur);
		cur = tmp;
	}
	ll->head = NULL;
	ll->size = 0;
}


ListNode *findNode(LinkedList *ll, int index){

	ListNode *temp;

	if (ll == NULL || index < 0 || index >= ll->size)
		return NULL;

	temp = ll->head;

	if (temp == NULL || index < 0)
		return NULL;

	while (index > 0){
		temp = temp->next;
		if (temp == NULL)
			return NULL;
		index--;
	}

	return temp;
}

int insertNode(LinkedList *ll, int index, int value){

	ListNode *pre, *cur;

	if (ll == NULL || index < 0 || index > ll->size + 1)
		return -1;

	// If empty list or inserting first node, need to update head pointer
	if (ll->head == NULL || index == 0){
		cur = ll->head;
		ll->head = malloc(sizeof(ListNode));
		ll->head->item = value;
		ll->head->next = cur;
		ll->size++;
		return 0;
	}


	// Find the nodes before and at the target position
	// Create a new node and reconnect the links
	if ((pre = findNode(ll, index - 1)) != NULL){
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
