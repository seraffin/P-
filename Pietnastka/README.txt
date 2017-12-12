Implementacja algorytmow BFS oraz A* na potrzeby rozwiazywania ukladanki "Pietnastka".
W pliku main.cpp zawarte sa
metody realizujace oba algorytmy oraz pozwalajace na operowanie na całym
układzie układanki. Opis funkcji:

— isSolvable – sprawdza za pomoca standardowej formuły, czy dana układanka
jest rozwiazywalna.
— currentState – printuje aktualny stan układanki.
— switchPlaces – przy pomocy operacji bitowych zamienia miejscami dwa
podane elementy układanki.
— findBlank – zwraca połozenie pustego elementu układanki.
— findPrevMove – znajduje w wektorze odwiedzonych wezłów grafu (stanów
układanki) stan poprzedniego ruchu.
— findPath – tworzy tekstowa reprezentacje sciezki przebytej przy rozwiazywaniu
układanki.
— BFS – implementacja algorytmu Breadth-first search.
— calculateCorrectPlace – wyliczenie odległosci do poprawnego ułozenia
układanki dla algortmu A*.
— findTheBestIndex – zwraca index elementu z kolejki mozliwych ruchów z
najmniejsza odlegloscia od poprawnego ułozenia układanki.
— A1 – implementacja algorymu Astar.
— drawPuzzle – tasowanie układanki o wskazana ilosc ruchów.