#include <iostream>
#include <chrono>
#include <thread>
#include <Windows.h>
#include <boost\lockfree\queue.hpp>

//큐 특징: 생산자 쓰레드 == 소비자 쓰레드. 아마 분리가 필요할것같다.
//큐에 단 하나라도 데이터가 있으면 엄청 빨리 수행된다.

#define testThreadCount 4
#define testSize 8
#define testThreadLoopCount 2500000 //

struct st_recode {
	long long min = LLONG_MAX;
	long long max = 0;
	long long total = 0;
	long long count = 0;
};

void recode_update(st_recode* container, long long data) {
	container->total = container->total + data;
	container->count = container->count + 1;
	if (container->max < data) {
		container->max = data;
	}
	if (container->min > data) {
		container->min = data;
	}
}

void recode_update(st_recode* container, st_recode data) {
	container->total = container->total + data.total;
	container->count = container->count + data.count;
	if (container->max < data.max) {
		container->max = data.max;
	}
	if (container->min > data.min) {
		container->min = data.min;
	}
}

void testThread1(boost::lockfree::queue<std::atomic<char>* >* q, st_recode* pushRec, st_recode* popRec, std::atomic<char>** testArray) {
	long long popStart = 0;
	long long popEnd = 0;
	long long pushStart = 0;
	long long pushEnd = 0;

	int count = 0;
	int r = 0;
	std::atomic<char>* data = NULL;

	pushStart = _Query_perf_counter();
	q->push(testArray[0]);
	pushEnd = _Query_perf_counter();
	recode_update(pushRec, pushEnd - pushStart);

	for (count = 1; count < testThreadLoopCount; count++) {
		if (testArray[count] == NULL) {
			break;
		}

		pushStart = _Query_perf_counter();
		q->push(testArray[count]);
		pushEnd = _Query_perf_counter();
		recode_update(pushRec, pushEnd - pushStart);

		popStart = _Query_perf_counter();
		while (!q->pop(data)) {
			Yield();
			popStart = _Query_perf_counter();
		}
		popEnd = _Query_perf_counter();
		recode_update(popRec, popEnd - popStart);

		data->fetch_add(1);
	}

	popStart = _Query_perf_counter();
	while (!q->pop(data)) {
		Yield();
		popStart = _Query_perf_counter();
	}
	popEnd = _Query_perf_counter();
	recode_update(popRec, popEnd - popStart);

	data->fetch_add(1);


	return;
}

void testThread2(boost::lockfree::queue<std::atomic<char>* >* q, st_recode* pushRec, st_recode* popRec, std::atomic<char>** testArray) {
	long long popStart = 0;
	long long popEnd = 0;
	long long pushStart = 0;
	long long pushEnd = 0;

	int count = 0;
	int r = 0;
	std::atomic<char>* data = NULL;


	pushStart = _Query_perf_counter();
	q->push(testArray[count]);
	pushEnd = _Query_perf_counter();
	recode_update(pushRec, pushEnd - pushStart);

	for (count = 1; count < testThreadLoopCount; count++) {

		pushStart = _Query_perf_counter();
		q->push(testArray[count]);
		pushEnd = _Query_perf_counter();
		recode_update(pushRec, pushEnd - pushStart);

		popStart = _Query_perf_counter();
		while (!q->pop(data)) {
			Yield();
			popStart = _Query_perf_counter();
		}
		popEnd = _Query_perf_counter();
		recode_update(popRec, popEnd - popStart);

		data->fetch_add(1);
	}

	popStart = _Query_perf_counter();
	while (!q->pop(data)) {
		Yield();
		popStart = _Query_perf_counter();
	}
	popEnd = _Query_perf_counter();
	recode_update(popRec, popEnd - popStart);

	data->fetch_add(1);

	return;
}

int main() {
	boost::lockfree::queue<std::atomic<char>* > q(0);

	std::atomic<char>** allData = new std::atomic<char>*[testThreadCount * 2 * testThreadLoopCount];

	st_recode* pushRecs = new st_recode[testThreadCount * 2];
	st_recode* popRecs = new st_recode[testThreadCount * 2];

	int count = 0;
	int count2 = testThreadCount * 2 * testThreadLoopCount;

	for (count = 0; count < count2; count++) {
		allData[count] = new std::atomic<char>;
		*allData[count] = 0;
	}

	std::thread** T1Array = new std::thread*[testThreadCount];
	std::thread** T2Array = new std::thread*[testThreadCount];

	for (count = 0; count < testThreadCount; count++) {
		T1Array[count] = new std::thread(testThread1, &q, &pushRecs[count], &popRecs[count], &allData[count * testThreadLoopCount]);
		T2Array[count] = new std::thread(testThread2, &q, &pushRecs[count + testThreadCount], &popRecs[count + testThreadCount], &allData[(count + testThreadCount) * testThreadLoopCount]);
	}

	for (count = 0; count < testThreadCount; count++) {
		T1Array[count]->join();
		T2Array[count]->join();
	}

	for (count = 0; count < count2; count++) {
		if (*allData[count] != 1) {
			std::cout << "wtf?" << std::endl;
		}
	}

	for (count = 1; count < testThreadCount; count++) {
		recode_update(&pushRecs[0], pushRecs[count]);
		recode_update(&popRecs[0], popRecs[count]);
	}


	return 0;
}
