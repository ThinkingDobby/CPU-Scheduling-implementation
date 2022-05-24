#include <stdio.h>
#include <stdlib.h>

#define TRUE 1
#define FALSE 0
#define ERROR -1
#define MAX_SIZE 10

typedef int element;
typedef int boolean;

// #define SWAP(a,b) {int t; t = a; a = b; b = t;}

// 오류 출력
void error_handling(char* message);

// 프로세스
typedef struct process {
	int pid;	// process id
	int priority;
	int computing_time;
	int arrival_time;	// arrival_time
} Process;

// 원형 큐
typedef struct circular_queue {
	int rear;
	int front;
	element* data;
} Queue;

// 큐 초기화
void init_queue(Queue* q) {
	q->front = 0;
	q->rear = 0;
	q->data = (element*)malloc(sizeof(element) * MAX_SIZE);
}

boolean is_empty(Queue* q) {
	if (q->front == q->rear) return TRUE;
	else return FALSE;
}

boolean is_full(Queue* q) {
	if (((q->rear + 1) % MAX_SIZE) == q->front) return TRUE;
	else return FALSE;
}

// push
void push(Queue* q, element data) {
	if (is_full(q)) {
		error_handling("Queue Full\n");

		return;
	}
	else {
		q->rear = (q->rear + 1) % (MAX_SIZE);
		q->data[q->rear] = data;
	}

	return;
}

// pop
element pop(Queue* q) {
	if (is_empty(q)) {
		error_handling("Queue Empty\n");
	}
	q->front = (q->front + 1) % (MAX_SIZE);

	return q->data[q->front];
}

// 큐 출력
void queue_print(Queue q) {
	int i = q.front;

	if (is_empty(&q)) {
		error_handling("Queue Empty\n");

		return;
	}

	do {
		i = (i + 1) % MAX_SIZE;
		printf("%d | ", q.data[i]);

		if (i == q.rear) {
			break;
		}
	} while (i != q.front);

	printf("\n\n");

	return;
}

int main(int argc, char* argv[]) {
	Queue q;

	init_queue(&q);

	for (int i = 0; i < 5; i++) {
		push(&q, i);
		queue_print(q);
	}

	for (int i = 0; i < 5; i++) {
		pop(&q);
		queue_print(q);
	}
	free(q.data);

	return 0;
}

void error_handling(char* message) {
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}