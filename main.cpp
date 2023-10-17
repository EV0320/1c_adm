#include <iostream>
#include <vector>
#include <algorithm>

class Infector {
public:
    Infector(int n, std::vector<std::pair<int, int>>& cities_graph) {
        edges.resize(n);
        isInfected.resize(n, false);
        infectedNeib.resize(n, 0);

        citiesCount = n;

        for (auto edge : cities_graph) { // формируем удобный вид графа
            edges[edge.first].push_back(edge.second);
            edges[edge.second].push_back(edge.first);
        }
    }

    std::vector<int> GetInfectedCities() { // получаем список оптимально зараженных городов
        // Сначала заражаем города, из которых всего 1 ребро
        std::vector<int> answer;
        int n = 0;
        for (int i = 0; i < citiesCount; ++i) {
            if (edges[i].size() < 2) {
                Infect(i);
                ++n;
                answer.push_back(i + 1);
            }
        }

        while (n < citiesCount) { // пока заразили не все города
            int newInfected = -1, newCity = -1;

            for (int i = 0; i < citiesCount; ++i) { // смотрим сколько можем заразить городов, если изначально заразим город i
                if (isInfected[i]) 
                    continue;
                int possInf = GetpossibleInfections(i);

                if (possInf >= newInfected) {
                    newInfected = possInf;
                    newCity = i;
                }
            }
            n += Infect(newCity) + 1;  // Выбрали самый убойный город и заражаем его
            answer.push_back(newCity + 1);
        }

        return answer;

    }
private:
    int citiesCount; // кол-во городов
    std::vector<std::vector<int>> edges; // связь городов
    std::vector<bool> isInfected; // список индикаторов зараженных городов
    std::vector<int> infectedNeib; // список кол-ва зараженных соседей каждого города
     
    int GetpossibleInfections(int v) { // получаем сколько можем заразить, если заразим изначально город v
        std::vector<bool> used(citiesCount, false);
        std::vector<bool> tmpIsInfected = isInfected;
        std::vector<int> tmpInfectedNeib = infectedNeib;

        return Dfs(v, 0, used, tmpIsInfected, tmpInfectedNeib);
    }

    int Infect(int v) { // заражаем изначально город v
        std::vector<bool> used(citiesCount, false);

        return Dfs(v, 0, used, isInfected, infectedNeib);
    }


    int Dfs(int v, int gotInf, std::vector<bool>& isUsed, std::vector<bool>& tmpIsInfected, std::vector<int>& tmpInfectedNeib) {
        isUsed[v] = true;
        if (!tmpIsInfected[v]) {
            tmpIsInfected[v] = true; // заразили город
            ++gotInf;
            for (auto e : edges[v])
                ++tmpInfectedNeib[e]; // обновляем кол-во заражных соседей и смежным с новых зараженным городом
        }
        for (auto e : edges[v]) {
            if (!isUsed[e] && tmpInfectedNeib[e] >= 2)
                gotInf = Dfs(e, gotInf, isUsed, tmpIsInfected, tmpInfectedNeib); // заражаем города у которых уже 2 зараженных соседа
        }
        return gotInf; // вернули сколько в итоге заразили
    }
};

int main() {
    int m, n = 0;
    std::cin >> m;

    std::vector<std::pair<int, int>> cities_graph(m);
    int first, second;
    for (int i = 0; i < m; ++i) {
        std::cin >> first >> second;

        n = std::max(n, first);
        n = std::max(n, second);

        cities_graph[i].first = first - 1;
        cities_graph[i].second = second - 1;
    }

    Infector Solver(n, cities_graph);
    std::vector<int> resultCities = Solver.GetInfectedCities();

    std::cout << resultCities.size() << '\n';
    for (auto elem : resultCities) {
        std::cout << elem << ' ';
    }
}
