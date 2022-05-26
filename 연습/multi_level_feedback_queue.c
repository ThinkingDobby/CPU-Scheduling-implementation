﻿#include <stdio.h>
#include <stdlib.h>
#pragma warning(disable:4996)

#define TRUE 1
#define FALSE 0
#define ERROR -1
#define MAX_SIZE 100	// 큐 최대 크기
#define TIME 20

// 오류 출력
void error_handling(char* message);

// 프로세스
typedef struct process {
	int pid;	// process id
	int priority;
	int computing_time;
	int remaining_time;	// 남은 시간
	int arrival_time;	// arrival_time
	int end_queue;	// 종료 시점 큐
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

	printf("pid priority computing_time remaining_time arrival_time end_queue\n");
	do {
		i = (i + 1) % MAX_SIZE;
		Process data = q.data[i];
		printf("%3d %8d %14d %14d %12d %9d\n", data.pid, data.priority, data.computing_time, data.remaining_time, data.arrival_time, data.end_queue);

		if (i == q.rear) {
			break;
		}
	} while (i != q.front);

	printf("\n");

	return;
}

/*
2018158028 이치호
Multilevel Feedback Queue Algorithm
입력 및 진행: 예제와 같음
큐 개수: 4
time quantum: 20
*/
void time_passes(Process p, Queue* qs, Queue* result_q, int* cur_time);

int main(int argc, char* argv[]) {
	
	Queue ready_q;
	Queue qs[4];

	//큐 초기화
	init_queue(&ready_q);
	for (int i = 0; i < 4; i++)
		init_queue(&qs[i]);

	// 결과 큐 초기화
	Queue result_q;
	init_queue(&result_q);

	// 현재 시간
	int cur_time = 0;

	// 프로세스 입력
	printf("Input(type process_id priority computing_time):\n");
	while (TRUE) {
		int type, pid, priority, computing_time;
		Process p;

		int chk = scanf("%d %d %d %d", &type, &pid, &priority, &computing_time);
		if (chk != 4)	// 입력 값 검증
			error_handling("Wrong Input");
		
		if (type == -1) {
			break;
		}
		else if (type == 0) {
			// 가장 먼저 삽입된 프로세스 pop
			p = pop(&ready_q);

			// time quantum 진행
			time_passes(p, qs, &result_q, &cur_time);
		}
		else if (type == 1) {
			p.pid = pid, p.priority = priority, p.computing_time = computing_time;
			p.arrival_time = cur_time;
			p.remaining_time = computing_time;
			p.end_queue = -1;

			push(&ready_q, p);
		}
		else {	// 잘못된 타입 입력
			printf("Wrong Type\n");
		}
	}

	// -1 입력 이후
	queue_print(ready_q);
	printf("-----\n");
	queue_print(result_q);
	printf("-----\n");
	queue_print(qs[0]);
	printf("-----\n");
	printf("%d", cur_time);

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

	free(ready_q.data);
	for (int i = 0; i < 4; i++)
		free(qs[i].data);

	return 0;
}

void error_handling(char* message) {
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}

void time_passes(Process p, Queue* qs, Queue *result_q, int* cur_time) {
	int next_q_lev = p.end_queue + 1;

	if (p.remaining_time > TIME) {
		*cur_time += 20;
		p.remaining_time -= 20;
		push(&qs[next_q_lev], p);
	}
	else if (p.remaining_time == TIME) {
		*cur_time += 20;
		p.remaining_time -= 20;
		push(result_q, p);
	}
	else {
		*cur_time += p.remaining_time;
		p.remaining_time = 0;
		push(result_q, p);
	}
}