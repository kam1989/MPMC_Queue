#pragma once
#include <Windows.h>
#include <iostream>
#include <queue>
#include <thread>
#include <vector>
#include <array>
#include <atomic>

#include <string>

namespace MPMC_LFQ {

	struct Node {
		Node* next = NULL;
		void* data = NULL;
		Node* nextGarbage = NULL;
	};


	template <int size>
	class GarbageCare {
	public:
		GarbageCare(int n) {
			int count = 0;
			int count2 = 0;
			for (count = 0; count < arraySize; count++) {
				pushNode[count] = new Node;
				popNode[count] = pushNode[count];
				//counter[count] = 0;
				for (count2 = 0; count2 < n; count2++) {
					push(new Node, count);
				}
			}
		}

		bool peek(Node** r, int n) {
			if (popNode[n] != NULL) {
				*r = popNode[n];
				return true;
			}
			return false;
		}

		void push(Node* garbage, int n) {
			garbage->nextGarbage = NULL;
			pushNode[n]->nextGarbage = garbage;
			pushNode[n] = garbage;
			//counter[n] = counter[n] + 1;
		}

		bool pop(Node** r, int n) {
			if (popNode[n]->nextGarbage != NULL) {
				*r = popNode[n];
				popNode[n] = popNode[n]->nextGarbage;
				//counter[n] = counter[n] - 1;
				return true;
			}
			return false;
		}


	private:
		int arraySize = size;
		std::array<Node*, size> pushNode;
		std::array<Node*, size> popNode;
		//std::array<int, size> counter;

	};

	template <int size>
	class NumberPool {
	public:
		NumberPool() {
			int count = 0;
			for (count = 0; count < arraySize; count++) {
				numbers[count] = 0;
			}
		}

		bool popNum(int* r) {
			int n = 1;
			int count = 0;
			for (count = 0; count < arraySize; count++) {
				if (numbers[count] == 0) {
					n = numbers[count].exchange(1);
				}
				if (n == 0) {
					*r = count;
					return true;
				}
			}
			return false;
		}

		void pushNum(int n) {
			//no check array range;
			numbers[n] = 0;
		}


	private:
		int arraySize = size;
		std::array<std::atomic<int>, size> numbers;
	};

	template <int size>
	class HazardCare {
	public:
		HazardCare() {
			int count = 0;
			for (count = 0; count < arraySize; count++) {
				hazardArray[count] = NULL;
			}
		}

		void resetHazard(int n) {
			hazardArray[n] = NULL;
		}

		void updateHazard(Node** r, std::atomic<Node*>* haz, int n) {
			while (true) {
				hazardArray[n] = *haz;
				if (*haz == hazardArray[n]) {
					*r = hazardArray[n];
					return;
				}
			}
		}

		bool checkHazardSafety(Node* n) {
			int count = 0;
			for (count = 0; count < arraySize; count++) {
				if (hazardArray[count] == n) {
					return false;
				}
			}
			return true;
		}

	private:
		int arraySize = size;
		std::array<Node*, size> hazardArray;
	};

	template <typename T, int size>
	class LF_queue {
	public:
		LF_queue() {
			Node* temp = new Node;

			pos_push = temp;
			pos_pop = temp;
		}

		bool acqHazard(int* r) {
			return numberPool.popNum(r);
		}

		void releaseHazard(int n) {
			numberPool.pushNum(n);
		}

		void releaseHazardWithReset(int n) {
			Node* temp = NULL;

			hazardCare.resetHazard(n);

			while (garbageCare.peek(&temp, n)) {
				while (hazardCare.checkHazardSafety(temp)) {
					garbageCare.pop(&temp, n);
					free(temp);
				}
			}

			numberPool.pushNum(n);
		}

		void push(T* pushData) {
			Node* temp = new Node;
			Node* temp2 = pos_push.exchange(temp);
			temp2->data = pushData;
			temp2->next = temp;

		}

		void pushWithRecycle(T* pushData, int n) {
			Node* temp = NULL;

			if (garbageCare.peek(&temp, n)) {
				if (hazardCare.checkHazardSafety(temp)) {
					if (!garbageCare.pop(&temp, n)) {
						temp = new Node;
					};
				}
				else {
					temp = new Node;
				}
			}
			else {
				temp = new Node;
			}

			Node* temp2 = pos_push.exchange(temp);
			temp2->data = pushData;
			temp2->next = temp;
		}

		bool freeGarbage(int n) {
			Node* temp = NULL;

			if (garbageCare.peek(&temp, n)) {
				if (hazardCare.checkHazardSafety(temp)) {
					garbageCare.pop(&temp, n);
					free(temp);
					return true;
				}
			}
			return false;
		}

		bool pop(T** r, int hazardArrayNum) {
			Node* temp = NULL;
			Node* temp2 = NULL;
			while (true) {
				hazardCare.updateHazard(&temp, &pos_pop, hazardArrayNum);
				if (temp->next == NULL) {
						return false;
				}

				temp2 = temp->next;
				if (pos_pop.compare_exchange_weak(temp, temp2)) {
					temp->next = NULL;
					*r = (T*)temp->data;
					garbageCare.push(temp, hazardArrayNum);
					hazardCare.resetHazard(hazardArrayNum);
					return true;
				}
			}
		}


	private:

		std::atomic<Node*> pos_push = NULL;
		std::atomic<Node*> pos_pop = NULL;

		HazardCare<size> hazardCare;
		GarbageCare<size> garbageCare = GarbageCare<size>(3000);
		NumberPool<size> numberPool;
	};
}


