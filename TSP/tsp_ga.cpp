#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <cmath>
#include <limits>
#include <iomanip>

using namespace std;

// Cấu trúc thành phố
struct City {
    int id;
    double x, y;
    
    City(int id = 0, double x = 0, double y = 0) : id(id), x(x), y(y) {}
};

// Cấu trúc cá thể (một lộ trình)
struct Individual {
    vector<int> route;  // Thứ tự các thành phố
    double fitness;     // Tổng khoảng cách (càng nhỏ càng tốt)
    
    Individual() : fitness(numeric_limits<double>::max()) {}
};

// Random engine toàn cục
random_device rd;
mt19937 gen(rd());

// Tính khoảng cách Euclid giữa 2 thành phố
double distance(const City& a, const City& b) {
    double dx = a.x - b.x;
    double dy = a.y - b.y;
    return sqrt(dx * dx + dy * dy);
}

// Tính tổng khoảng cách của một lộ trình
double calculateTotalDistance(const vector<int>& route, const vector<City>& cities) {
    double total = 0.0;
    for (size_t i = 0; i < route.size() - 1; i++) {
        total += distance(cities[route[i]], cities[route[i + 1]]);
    }
    // Quay về thành phố đầu tiên
    total += distance(cities[route.back()], cities[route[0]]);
    return total;
}

// Khởi tạo quần thể ban đầu
vector<Individual> initializePopulation(int popSize, int numCities) {
    vector<Individual> population(popSize);
    
    for (int i = 0; i < popSize; i++) {
        population[i].route.resize(numCities);
        for (int j = 0; j < numCities; j++) {
            population[i].route[j] = j;
        }
        // Shuffle ngẫu nhiên
        shuffle(population[i].route.begin(), population[i].route.end(), gen);
    }
    
    return population;
}

// Đánh giá fitness cho toàn bộ quần thể
void evaluateFitness(vector<Individual>& population, const vector<City>& cities) {
    for (auto& ind : population) {
        ind.fitness = calculateTotalDistance(ind.route, cities);
    }
}

// Tournament selection
Individual tournamentSelection(const vector<Individual>& population, int tournamentSize) {
    uniform_int_distribution<> dis(0, population.size() - 1);
    
    Individual best = population[dis(gen)];
    for (int i = 1; i < tournamentSize; i++) {
        Individual candidate = population[dis(gen)];
        if (candidate.fitness < best.fitness) {
            best = candidate;
        }
    }
    return best;
}

// Order Crossover (OX)
Individual orderCrossover(const Individual& parent1, const Individual& parent2) {
    int size = parent1.route.size();
    Individual child;
    child.route.resize(size, -1);
    
    uniform_int_distribution<> dis(0, size - 1);
    int start = dis(gen);
    int end = dis(gen);
    
    if (start > end) swap(start, end);
    
    // Copy đoạn từ parent1
    for (int i = start; i <= end; i++) {
        child.route[i] = parent1.route[i];
    }
    
    // Điền phần còn lại từ parent2
    int currentPos = (end + 1) % size;
    for (int i = 0; i < size; i++) {
        int city = parent2.route[(end + 1 + i) % size];
        
        // Kiểm tra xem city đã có trong child chưa
        bool found = false;
        for (int j = start; j <= end; j++) {
            if (child.route[j] == city) {
                found = true;
                break;
            }
        }
        
        if (!found) {
            child.route[currentPos] = city;
            currentPos = (currentPos + 1) % size;
        }
    }
    
    return child;
}

// Swap Mutation
void swapMutation(Individual& individual, double mutationRate) {
    uniform_real_distribution<> prob(0.0, 1.0);
    
    if (prob(gen) < mutationRate) {
        int size = individual.route.size();
        uniform_int_distribution<> dis(0, size - 1);
        
        int pos1 = dis(gen);
        int pos2 = dis(gen);
        
        swap(individual.route[pos1], individual.route[pos2]);
    }
}

// Thuật toán di truyền chính
Individual geneticAlgorithm(const vector<City>& cities, int popSize, int generations, 
                           double mutationRate, int tournamentSize) {
    int numCities = cities.size();
    
    // Khởi tạo quần thể
    vector<Individual> population = initializePopulation(popSize, numCities);
    evaluateFitness(population, cities);
    
    // Tìm cá thể tốt nhất ban đầu
    Individual bestOverall = *min_element(population.begin(), population.end(),
        [](const Individual& a, const Individual& b) { return a.fitness < b.fitness; });
    
    // Vòng lặp chính
    for (int gen = 0; gen < generations; gen++) {
        vector<Individual> newPopulation;
        
        // Elitism: giữ lại cá thể tốt nhất
        newPopulation.push_back(bestOverall);
        
        // Tạo quần thể mới
        while (newPopulation.size() < (size_t)popSize) {
            // Selection
            Individual parent1 = tournamentSelection(population, tournamentSize);
            Individual parent2 = tournamentSelection(population, tournamentSize);
            
            // Crossover
            Individual child = orderCrossover(parent1, parent2);
            
            // Mutation
            swapMutation(child, mutationRate);
            
            newPopulation.push_back(child);
        }
        
        population = newPopulation;
        evaluateFitness(population, cities);
        
        // Cập nhật cá thể tốt nhất
        Individual currentBest = *min_element(population.begin(), population.end(),
            [](const Individual& a, const Individual& b) { return a.fitness < b.fitness; });
        
        if (currentBest.fitness < bestOverall.fitness) {
            bestOverall = currentBest;
        }
        
        // In tiến trình với nhiều thông tin hơn
        if (gen % 100 == 0) {
            // Tính toán thống kê
            double totalFitness = 0.0;
            double worstFitness = 0.0;
            for (const auto& ind : population) {
                totalFitness += ind.fitness;
                if (ind.fitness > worstFitness) {
                    worstFitness = ind.fitness;
                }
            }
            double avgFitness = totalFitness / population.size();
            
            cout << "=== Generation " << gen << " ===" << endl;
            cout << "  Best distance    : " << fixed << setprecision(2) << bestOverall.fitness << endl;
            cout << "  Average distance : " << fixed << setprecision(2) << avgFitness << endl;
            cout << "  Worst distance   : " << fixed << setprecision(2) << worstFitness << endl;
            cout << "  Improvement      : " << fixed << setprecision(2) 
                 << (worstFitness - bestOverall.fitness) << endl << endl;
        }
    }
    
    return bestOverall;
}

int main() {
    // Dữ liệu mẫu: 30 thành phố
    vector<City> cities = {
        City(0, 60, 200), City(1, 180, 200), City(2, 80, 180),
        City(3, 140, 180), City(4, 20, 160), City(5, 100, 160),
        City(6, 200, 160), City(7, 140, 140), City(8, 40, 120),
        City(9, 100, 120), City(10, 150, 100), City(11, 50, 100),
        City(12, 200, 80), City(13, 30, 80), City(14, 120, 60),
        City(15, 180, 60), City(16, 70, 40), City(17, 160, 40),
        City(18, 20, 20), City(19, 100, 20), City(20, 190, 20),
        City(21, 40, 190), City(22, 130, 170), City(23, 170, 150),
        City(24, 90, 130), City(25, 10, 110), City(26, 150, 90),
        City(27, 60, 70), City(28, 140, 50), City(29, 110, 30)
    };
    
    // Tham số GA
    int populationSize = 100;
    int generations = 1000;
    double mutationRate = 0.02;
    int tournamentSize = 5;
    
    cout << "=== TRAVELLING SALESMAN PROBLEM - GENETIC ALGORITHM ===" << endl;
    cout << "So thanh pho: " << cities.size() << endl;
    cout << "Kich thuoc quan the: " << populationSize << endl;
    cout << "So the he: " << generations << endl;
    cout << "Ti le dot bien: " << mutationRate << endl;
    cout << "\nBat dau giai..." << endl << endl;
    
    // Chạy thuật toán
    Individual best = geneticAlgorithm(cities, populationSize, generations, mutationRate, tournamentSize);
    
    // In kết quả
    cout << "\n=== KET QUA CUOI CUNG ===" << endl;
    cout << "Lo trinh tot nhat: ";
    for (int city : best.route) {
        cout << city << " -> ";
    }
    cout << best.route[0] << endl;
    cout << "Tong khoang cach: " << fixed << setprecision(2) << best.fitness << endl;
    
    return 0;
}
