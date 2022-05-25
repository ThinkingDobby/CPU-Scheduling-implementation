#include <stdio.h>
#include <stdlib.h>
#pragma warning(disable:4996)

#define TRUE 1
#define FALSE 0
#define ERROR -1
#define MAX_SIZE 100	// 큐 최대 크기

#define queue_cnt 4

// 오류 출력
void error_handling(char* message);

// 프로세스
typedef struct process {
	int pid;	// process id
	int priority;
	int computing_time;
	int arrival_time;	// arrival_time
} Process;

typedef Process element;
typedef int boolean;

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

	printf("\n");

	return;
}

int main(int argc, char* argv[]) {
	//큐 초기화
	Queue ready_q, q1, q2, q3, q4;
	init_queue(&ready_q), init_queue(&q1), init_queue(&q2), init_queue(&q3), init_queue(&q4);

	int cur_time = 0;

	// 프로세스 입력
	printf("Input(type process_id priority computing_time):\n");
	while (TRUE) {
		int type, pid, priority, computing_time;
		Process p;

		scanf("%d %d %d %d", &type, &pid, &priority, &computing_time);
		
		if (type == -1) {
			break;
		}
		else if (type == 0) {
			// time quantum 진행
			// pop - time quantum과 computing time 비교 -> 함수화 가능한가
		}
		else if (type == 1) {
			p.pid = pid, p.priority = priority, p.computing_time = computing_time;
			p.arrival_time = cur_time;

			push(&ready_q, p);
		}
		else {
			printf("Wrong Input\n");
		}
	}

	// -1 입력 이후

	Process result = pop(&ready_q);
	printf("%d", result.pid);

	/*
	for (int i = 0; i < 5; i++) {
		push(&q, i);
		queue_print(q);
	}

	for (int i = 0; i < 5; i++) {
		element temp = pop(&q);
		printf("%d\n", temp);
	}
	free(q.data);
	*/

	free(ready_q.data), free(q1.data), free(q2.data), free(q3.data), free(q4.data);

	return 0;
}

void error_handling(char* message) {
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}