#include <stdio.h>
#include <stdlib.h>
#pragma warning(disable:4996)

#define TRUE 1
#define FALSE 0
#define ERROR -1
#define MAX_SIZE 100	// 큐 최대 크기
#define QUEUE_CNT 4	// 큐 개수

#define TIME 20	// Time Quantum

// 오류 출력
void error_handling(char* message);

// 프로세스
typedef struct process {
	int pid;	// process id
	int priority;
	int computing_time;
	int remaining_time;	// 남은 시간
	int arrival_time;	// arrival_time
	int turn_around_time;
	int queue_lev;	// 종료 시점 큐
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
		printf("Queue Empty\n");

		return;
	}

	printf("pid  priority  computing_time  remaining_time  arrival_time  turn_around_time  queue_lev\n");
	do {
		i = (i + 1) % MAX_SIZE;
		Process data = q.data[i];
		printf("%4d %9d %15d %15d %13d %16d %10d\n", data.pid, data.priority, data.computing_time, data.remaining_time, data.arrival_time, data.turn_around_time, data.queue_lev);

		if (i == q.rear) {
			break;
		}
	} while (i != q.front);

	printf("\n");

	return;
}

// 큐 출력
void result_queue_print(Queue q) {
	int i = q.front;
	int sum = 0;

	if (is_empty(&q)) {
		printf("Queue Empty\n");

		return;
	}

	printf("==================== 결과 ====================\n");
	printf("-----------------------------------------------\n");
	printf("pid  priority  computing_time  turn_around_time\n");
	printf("-----------------------------------------------\n");
	do {
		i = (i + 1) % MAX_SIZE;
		Process data = q.data[i];
		printf("%4d %9d %15d %16d\n", data.pid, data.priority, data.computing_time, data.turn_around_time);
		sum += data.turn_around_time;

		if (i == q.rear) {
			break;
		}
	} while (i != q.front);

	printf("\n");

	if (sum != 0)
		printf("average_turn_around_time: %d\n", sum / (q.rear - q.front));

	return;
}

/*
2018158028 이치호
Multilevel Feedback Queue Algorithm
입력 및 진행: 예제와 같음
큐 개수: 4
Time Quantum: 20
*/

// 시간 진행
void time_passes(Process p, Queue* qs, Queue* result_q, int* cur_time);

int main(int argc, char* argv[]) {
	
	Queue ready_q;
	Queue qs[4];

	//큐 초기화
	init_queue(&ready_q);
	for (int i = 0; i < QUEUE_CNT; i++)
		init_queue(&qs[i]);

	// 결과 큐 초기화
	Queue result_q;
	init_queue(&result_q);

	// 현재 시간
	int cur_time = 0;

	// 프로세스 입력
	printf("===================== Multilevel Feedback Queue =====================\n\n");

	printf("type          :   -1(입력 종료), 0(프로세스 생성), 1(time quantum 진행)\n");
	printf("pid           :   프로세스 아이디\n");
	printf("priority      :   프로세스 우선순위\n");
	printf("computing_time:   계산 시간\n\n");

	printf("            공백 포함해 다음 내용을 순서대로 입력\n");
	printf("            ---------------------------------------\n");
	printf("            type process_id priority computing_time\n");
	printf("            ---------------------------------------\n");

	int t = 1;
	while (TRUE) {
		int type, pid, priority, computing_time;
		Process p;

		printf("%d번째 입력:", t);
		int chk = scanf("%d %d %d %d", &type, &pid, &priority, &computing_time);
		if (chk != 4)	// 입력 값 검증
			error_handling("Wrong Input");
		
		if (type == -1) {
			break;
		}
		else if (type == 1) {
			// 가장 먼저 삽입된 프로세스 pop
			Process p = pop(&ready_q);

			time_passes(p, qs, &result_q, &cur_time);
		}
		else if (type == 0) {
			p.pid = pid, p.priority = priority, p.computing_time = computing_time;
			p.arrival_time = cur_time;
			p.remaining_time = computing_time;
			p.turn_around_time = 0;
			p.queue_lev = -1;

			push(&ready_q, p);
		}
		else {	// 잘못된 타입 입력
			printf("Wrong Type\n");
		}
		t += 1;
	}

	// 준비 큐 순회
	while (!is_empty(&ready_q)) {
		Process p = pop(&ready_q);
		time_passes(p, qs, &result_q, &cur_time);
	}

	// Multilevel Feedback Queue Scheduling
	for (int i = 0; i < QUEUE_CNT; i++) {
		while (!is_empty(&qs[i])) {
			Process p = pop(&qs[i]);
			time_passes(p, qs, &result_q, &cur_time);
		}
	}

	printf("\n");
	result_queue_print(result_q);

	free(ready_q.data);
	for (int i = 0; i < QUEUE_CNT; i++)
		free(qs[i].data);

	return 0;
}

void error_handling(char* message) {
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}

void time_passes(Process p, Queue* qs, Queue *result_q, int* cur_time) {
	int next_q_lev = p.queue_lev + 1;

	if (p.remaining_time > TIME) {
		*cur_time += 20;
		p.remaining_time -= 20;
		if (next_q_lev == QUEUE_CNT)	// 마지막 큐는 RR - 해당 큐 마지막에 프로세스 다시 추가
			next_q_lev--;
		else
			p.queue_lev = next_q_lev;
		push(&qs[next_q_lev], p);
	}
	else if (p.remaining_time == TIME) {
		*cur_time += 20;
		p.remaining_time -= 20;
		p.turn_around_time = *cur_time - p.arrival_time;
		push(result_q, p);
	}
	else {
		*cur_time += p.remaining_time;
		p.remaining_time = 0;
		p.turn_around_time = *cur_time - p.arrival_time;
		push(result_q, p);
	}
}


/*
0 1 25 80
0 2 15 40
0 3 8 30
1 0 0 0
0 4 12 10
0 5 22 30
0 6 28 50
1 0 0 0
0 7 5 20
0 8 3 40
0 9 13 60
1 0 0 0
0 10 24 45
-1 0 0 0
*/