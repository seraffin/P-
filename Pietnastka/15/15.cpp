#include <iostream>
#include <string>
#include <fstream>
#include <stdio.h>
#include <cstdio>
#include <cstdlib>
#include <time.h>
#include <list>
#include <vector>
#include <queue>

using namespace std;

uint64_t const divider = 0x1000000000000000;

int cycles = 0;

struct puzzle
{
	uint64_t state;
	int prevMove; //Kierunki : 0 = dol, 1 = lewo, 2 = gora, 3 = prawo, 4 = bez zmian
	int depth;
	int distance;
};

struct DereferenceCompareNode : public std::binary_function<puzzle*, puzzle*, bool>
{
	bool operator()(const puzzle* lhs, const puzzle* rhs) const
	{
		return lhs->distance > rhs->distance;
	}
};

bool isSolvable(uint64_t);
void currentState(uint64_t);
uint64_t switchPlaces(int, int, uint64_t);
int findBlank(uint64_t);
int findPrevMove(vector<puzzle>, uint64_t);
string findPath(vector<puzzle>, puzzle);
string BFS(uint64_t);
int calculateCorrectPlace(uint64_t);
int findTheBestIndex(vector<puzzle>);
string A1(uint64_t);
uint64_t drawPuzzle(int, uint64_t);

void print_queue(priority_queue<puzzle*, vector<puzzle*>, DereferenceCompareNode>& q) {
	while (!q.empty()) {
		std::cout << q.top()->distance << " ";
		q.pop();
	}
	std::cout << '\n';
}


int main()
{
	bool running = true;
	uint64_t puzzle;
	puzzle = 0x123456789ABCDEF0;
	//puzzle = 0x124b5730968adefc;
	//puzzle = 0x1234578b9e0adf6c;
	//puzzle = 0x012467385eabdf9c;
	//puzzle = 0x0384172c6e9a5dbf;
	//puzzle = 0x1623984c5b7fa0de;
	int menu = 0;
	int movesCount = 0;
	int depth = 0;

	while (running)
	{
		while (menu != 1 && menu != 2 && menu != 3 && menu != 4 && menu != 5 && menu != 6)
		{
			cin.clear();
			cin.sync();
			cout << endl;
			cout << endl;
			cout << "//////////////////////////////////////////////////////////////" << endl;
			cout << "Aktualny uklad ukladanki: " << endl;
			currentState(puzzle);
			cout << endl;
			cout << "Menu (wybierz liczbe): " << endl;
			cout << endl << "1 - Losowanie poczatkowej ukladanki" << endl;
			cout << "2 - Rozwiaz ukladanke metoda BFS" << endl;
			cout << "3 - Rozwiaz ukladanke metoda A* (distance to proper placement)" << endl;
			cout << "4 - Wyjscie" << endl;
			cin >> menu;
		}
		switch (menu)
		{
		case 1:
			puzzle = 0x123456789ABCDEF0;
			cin.clear();
			cin.sync();
			cout << endl << "Wybierz maksymalna ilosc ruchow: " << endl;
			cin >> movesCount;

			puzzle = drawPuzzle(movesCount, puzzle);
			break;

		case 2:
			if (puzzle != 0)
			{
				cout << "(trwa rozwiazywanie BFS)" << endl;
				cout << "Ruchy pustego elementu potrzebne do rozwiazania ukladanki: " << BFS(puzzle) << endl;
				cout << "Ilosc potrzebnych obiegow do uzyskania rozwiazania: " << cycles << endl;
				cycles = 0;
			}
			break;

		case 3:
			if (puzzle != 0)
			{
				cout << "(trwa rozwiazywanie A1)" << endl;
				cout << "Ruchy pustego elementu potrzebne do rozwiazania ukladanki: " << A1(puzzle) << endl;
				cout << "Ilosc potrzebnych obiegow do uzyskania rozwiazania: " << cycles << endl;
				cycles = 0;

			}
			break;

		case 4:
			running = false;
			break;
		default:
			break;

		}
		menu = 0;

	}

	return 0;
}

bool isSolvable(uint64_t state)
{
	int inversions = 0;
	int blankRow = 0;
	uint64_t state2 = 0;

	for (int i = 0; i < 16; i++, state = state << 4)
	{
		state2 = state;
		if (state / divider == 0)
		{
			blankRow = i / 4;
		}
		else
		{
			for (int j = i + 1; j < 17; j++, state2 = state2 << 4)
			{
				if ((state / divider > state2 / divider) && state2 / divider != 0)
					inversions++;
			}
		}

	}

	if (blankRow % 2 == 0 && inversions % 2 == 0 || 
		blankRow % 2 == 1 && inversions % 2 == 1)
	{
		return false;
	}
	else
	{
		return true;
	}
	
}

void currentState(uint64_t stan)
{
	for (int i = 0; i<16; i++, stan = stan << 4)
	{
		printf("%llu ", stan / divider);
		if (i % 4 == 3)
		{
			printf("\n");
		}
	}
}

uint64_t switchPlaces(int a, int b, uint64_t stan)
{
	uint64_t temp = 0;

	temp = ((stan << 4 * a) / divider) << (4 * (15 - b));
	temp += ((stan << 4 * b) / divider) << (4 * (15 - a));

	stan &= ~((uint64_t)0xF << (4 * (15 - a)));
	stan &= ~((uint64_t)0xF << (4 * (15 - b)));

	stan |= temp;

	return stan;
}

int findBlank(uint64_t stan)
{
	bool exists = false;
	int i = 0;
	for (; i<16 && !exists; stan = stan << 4)
	{
		if (stan / divider == 0)
		{
			exists = true;
		}
		else
		{
			i++;
		}
	}
	return i;
}

int findPrevMove(vector<puzzle> visited, uint64_t state)
{
	int prevMove;
	for (int i = visited.size(); i>0; i--)
	{
		if (state == visited[i - 1].state)
		{
			prevMove = visited[i - 1].prevMove;
			break;
		}
	}
	return prevMove;
}

string findPath(vector<puzzle> visited, puzzle finalState)
{
	string result = "";
	int prevMove = finalState.prevMove;
	uint64_t state = finalState.state;
	int blank = 15;
	while (prevMove<4)
	{
		if (prevMove == 0)
		{
			result.insert(0, "D");
			state = switchPlaces(blank, blank - 4, state);
			blank -= 4;

		}

		else if (prevMove == 1)
		{
			result.insert(0, "L");
			state = switchPlaces(blank, blank + 1, state);
			blank += 1;
		}

		else if (prevMove == 2)
		{
			result.insert(0, "G");
			state = switchPlaces(blank, blank + 4, state);
			blank += 4;
		}

		else if (prevMove == 3)
		{
			result.insert(0, "P");
			state = switchPlaces(blank, blank - 1, state);
			blank -= 1;
		}
		prevMove = findPrevMove(visited, state);
	}
	return result;
}

string BFS(uint64_t stan)
{
	list<puzzle> queue;
	vector<puzzle> visited;
	bool wasVisited;
	int maxDepth = 0;
	int blank;
	puzzle temp = { 0,0,0,0 };
	puzzle current = { stan, 4, 0, 0 };
	queue.push_back(current);
	while (!queue.empty())
	{
		wasVisited = false;
		current = queue.front();

		for (int i = visited.size(); i>0; i--)
		{

			if (current.state == visited[i - 1].state)
			{

				wasVisited = true;
				if (visited[i - 1].depth>current.depth)
				{
					visited[i - 1] = current;
				}
			}
		}
		if (current.depth > maxDepth)
			maxDepth = current.depth;

		if (wasVisited == false)
		{
			if (current.state == 0x123456789ABCDEF0)
			{
				break;
			}

			cycles++;

			visited.push_back(current);
			blank = findBlank(current.state);

			// IFy gwarantujace nienaruszalnosc scian ukladanki
			if (blank % 4 != 0) // nie przy lewej krawedzi
			{
				temp.state = switchPlaces(blank, blank - 1, current.state);
				temp.prevMove = 1;
				temp.depth = current.depth + 1;
				queue.push_back(temp);
			}

			if (blank % 4 != 3) // nie przy prawej krawedzie
			{
				temp.state = switchPlaces(blank, blank + 1, current.state);
				temp.prevMove = 3;
				temp.depth = current.depth + 1;
				queue.push_back(temp);
			}

			if (blank / 4 != 0) // nie przy gornej krawedzi
			{
				temp.state = switchPlaces(blank, blank - 4, current.state);
				temp.prevMove = 2;
				temp.depth = current.depth + 1;
				queue.push_back(temp);
			}

			if (blank / 4 != 3) // nie przy dolnej krawedzi
			{
				temp.state = switchPlaces(blank, blank + 4, current.state);
				temp.prevMove = 0;
				temp.depth = current.depth + 1;
				queue.push_back(temp);
			}
		}
		queue.pop_front();
	}
	cout << endl << "Osiagnieta glebokosc: " << maxDepth << endl;
	return findPath(visited, current);
}

int calculateCorrectPlace(uint64_t state)
{
	int result = 0;
	for (unsigned int i = 0; i<16; i++)
	{
		if (state / divider == (i + 1) % 16)
		{
			result++;
		}
		state = state << 4;
	}
	return result;
}
/*
int findTheBestIndex(priority_queue<puzzle> queue)
{
	int maxValue = 0;
	int index = 0;
	for (unsigned int i = 0; i<queue.size(); i++)
	{
		if (maxValue<queue[i].distance)
		{
			maxValue = queue[i].distance;
			index = i;
		}
	}
	return index;
}
*/
string A1(uint64_t stan)
{
	std::priority_queue<puzzle*, vector<puzzle*>, DereferenceCompareNode> queue;	//priorytet
	vector<puzzle> visited; //set albo unorderedd set
	bool wasVisited;
	int maxDepth = 0;
	int blank;
	int vectorIndex = 0;

	puzzle* temp = new puzzle;
	puzzle* current = new puzzle;
	*current = { stan, 4, 0, calculateCorrectPlace(stan) };
	queue.push(current);
/*	puzzle* current2 = new puzzle;
	*current2 = { stan, 1, 0, calculateCorrectPlace(stan) };
	puzzle* current3 = new puzzle;
	*current3 = { stan, 2, 0, calculateCorrectPlace(stan) };

	
	queue.push(current2);
	queue.push(current3);
	print_queue(queue);*/
	//cout << "-----------------" << queue.top()->prevMove << endl;
	while (!queue.empty())
	{
//		if (!visited.empty())
//			visited.erase(visited.begin());

		wasVisited = false;

		current = queue.top();
		queue.pop();
		
		for (int i = visited.size(); i>0; i--)
		{
			if (current->state == visited[i - 1].state)
			{
				wasVisited = true;
/*				if (visited[i - 1].depth > current->depth)
				{
					visited[i - 1] = *current;
				}
*/
			}
		}

		if (current->depth > maxDepth)
			maxDepth = current->depth;

		if (wasVisited == false)
		{

			if (current->state == 0x123456789ABCDEF0)
			{
				break;
			}

			cycles++;

			visited.push_back(*current);

			blank = findBlank(current->state);

			if (blank % 4 != 0)
			{
				temp->state = switchPlaces(blank, blank - 1, current->state);
				temp->prevMove = 1;
				temp->depth = current->depth + 1;
				temp->distance = calculateCorrectPlace(temp->state);
				queue.push(temp);
			}
			if (blank % 4 != 3)
			{
				temp->state = switchPlaces(blank, blank + 1, current->state);
				temp->prevMove = 3;
				temp->depth = current->depth + 1;
				temp->distance = calculateCorrectPlace(temp->state);
				queue.push(temp);
			}

			if (blank / 4 != 0)
			{
				temp->state = switchPlaces(blank, blank - 4, current->state);
				temp->prevMove = 2;
				temp->depth = current->depth + 1;
				temp->distance = calculateCorrectPlace(temp->state);
				queue.push(temp);
			}

			if (blank / 4 != 3)
			{
				temp->state = switchPlaces(blank, blank + 4, current->state);
				temp->prevMove = 0;
				temp->depth = current->depth + 1;
				temp->distance = calculateCorrectPlace(temp->state);
				queue.push(temp);
			}

		}
	}
	cout << endl << "Osiagnieta glebokosc: " << maxDepth << endl;
	return findPath(visited, *current);
}

uint64_t drawPuzzle(int n, uint64_t state)
{
	srand(time(NULL));
	int blank = 15;
	int random;
	uint64_t originalState = state;
	bool solvable = false;

//	cout << "/////" << IsSolvable(0x123456789abcdfe0) << endl;
//	cout << "/////" << IsSolvable(0x023415689a7cdfbe) << endl;

	while (solvable == false)
	{
		for (int i = 0; i < n; i++)
		{
			random = rand() % 4;

			if (random == 0 && (blank / 4 != 3))
			{
				state = switchPlaces(blank, blank + 4, state);
				blank += 4;
			}

			if (random == 1 && (blank % 4 != 0))
			{
				state = switchPlaces(blank, blank - 1, state);
				blank -= 1;
			}

			if (random == 2 && (blank / 4 != 0))
			{
				state = switchPlaces(blank, blank - 4, state);
				blank -= 4;
			}

			if (random == 3 && (blank % 4 != 3))
			{
				state = switchPlaces(blank, blank + 1, state);
				blank += 1;
			}
		}
		solvable = isSolvable(state);
		cout << solvable << endl;
		if (solvable == false)
			state = originalState;
	}
	return state;
}