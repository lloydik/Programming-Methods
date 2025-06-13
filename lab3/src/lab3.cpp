#include <iostream>
#include <time.h>
#include <vector>
#include <fstream>
#include <string>
#include <cstring>
#include <algorithm>

// Настройки для тестов NIST и Dieharder
const unsigned long BYTES_PER_FILE = 1<<23; // 1MB данных
const int NUM_FILES = 5;

/*!
Класс для генерации чисел линейным конгруэнтным алгоритмом
*/
class LCG {
    private:
        unsigned long seed;
        unsigned long k;
        unsigned long b;
        unsigned long m;
    public:
        LCG(unsigned long seed, unsigned long k, unsigned long b, unsigned long m)
            : seed(seed), k(k), b(b), m(m) {}

        /*!
        * @brief Генерация числа
        *
        * Основываясь на параметрах, заданных алгоритму, генерируется случайное число
        *
        * @return случайное число
        */
        unsigned long next() {
            seed = (k * seed + b) % m;
            return seed;
        }
};
/*!
Класс для генерации чисел квадратичным конгруэнтным алгоритмом
*/
class QCG {
private:
    unsigned long k1, k2, b, m, r;
public:
    QCG(unsigned long k1, unsigned long k2, unsigned long b, unsigned long m, unsigned long r)
        : k1(k1), k2(k2), b(b), m(m), r(r) {}

    unsigned long next() {
        r = (k1 * r * r + k2 * r + b) % m;
        r = (r >> 13) | (r << (32 - 13));
        return r;
    }
};
/*!
Класс для генерации чисел методом XOR Shift
*/
class XORShift {
private:
    unsigned long state;
public:
    XORShift(unsigned long seed) : state(seed) {}

    unsigned long next() {
        state ^= state << 13;
        state ^= state >> 17;
        state ^= state << 5;
        return state;
    }
};

double lcg_adapter(std::vector<unsigned long>& result, unsigned long count)
{
    LCG lcg(time(NULL), 525287150, 937402173, 836104618);
    while(result.size() != count)
        result.push_back(lcg.next());
    return 0.0;
}

double qcg_adapter(std::vector<unsigned long>& result, unsigned long count)
{
    QCG qcg(1664525, 1013904223, 123456789, 4294967295, time(NULL));
    while(result.size() != count)
        result.push_back(qcg.next());
    return 0.0;
}

double xor_adapter(std::vector<unsigned long>& result, unsigned long count)
{
    XORShift gen(time(NULL));
    while(result.size() != count)
        result.push_back(gen.next());
    return 0.0;
}


double std_adapter(std::vector<unsigned long>& result, unsigned long count)
{
    while(result.size() != count)
        result.push_back(std::rand());
    return 0.0;
}


/*!
 * @brief Подсчёт времени
 *
 * Функция для вычисления времени работы алгоритма
 *
 * @param func Функция с алгоритмом сортировки
 * @param result Вектор с результатом
 * @param result Количество генераций
 *
 * @return Время работы алгоритма
 */
double calc_algo_time(double (*func)(std::vector<unsigned long>&, unsigned long), std::vector<unsigned long>& result, unsigned long count)
{
    result.clear();
    clock_t start = clock();
    double t = func(result, count);
    clock_t end = clock();
    return t ? t : ((double)(end - start) / (double)(CLOCKS_PER_SEC));
}

/*!
 * @brief Функция открытия файла
 *
 * Функция для вычисления времени работы алгоритма
 *
 * @param outFile Поток для открытия файла
 * @param filename Имя файла для открытия
 *
 * @return 500, если ошибка, иначе 0
 */
int openOutFile(std::fstream &outFile, std::string &filename)
{
    outFile.open(filename, std::ios::out);
    if (!outFile)
    {
        std::cerr << "Unable to open output file: " << filename;
        return 500;
    }
    return 0;
}

/*!
 * @brief Функция записи в файл
 *
 * Функция для записи в файл
 *
 * @param outFile Файловый поток для записи
 * @param a Массив, который надо записать
 * @param sample_size Размер массива
 *
 * @return Ничего
 */
void writeToFile(std::fstream &outFile, std::vector<unsigned long>& result)
{
    for (int i = 0; i < result.size(); ++i)
        outFile << result[i] << " ";
}

/*!
 * @brief Структура для алгоритмов
 *
 * Функция для записи в файл
 *
 * @param name Файловый поток для записи
 * @param func Функция алгоритма
 * @param fileStream Файловый поток для вывода алгоритма
 *
 */
struct algoInfo
{
    std::string name;
    double (*func)(std::vector<unsigned long>&, unsigned long);
    std::fstream &fileStream;
    algoInfo(std::string name,double (*func)(std::vector<unsigned long>&, unsigned long), std::fstream &fileStream) : name(name), func(func), fileStream(fileStream) {}
};

/*!
 * @brief Генерация тестовых файлов для NIST и dieharder
 *
 * Функция генерирует бинарные файлы для тестирования генераторов
 *
 * @param count Количество байт для генерации
 * @param filename Имя выходного файла
 * @param func Функция генератора
 */
void generate_test_file(unsigned long count, const std::string& filename, double (*func)(std::vector<unsigned long>&, unsigned long)) {
    std::vector<unsigned long> result;
    func(result, count);
    
    std::ofstream outFile(filename, std::ios::binary);
    if (!outFile) {
        std::cerr << "Unable to open file: " << filename << std::endl;
        return;
    }
    
    for (unsigned long num : result) {
        outFile.write((const char *)(&num), sizeof(char));
    }
    outFile.close();
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        std::cerr << "Usage ./lab2 <count of files in tests directory>";
        return 400;
    }
    setlocale(LC_ALL, "Russian");

    
    std::vector<std::pair<std::string, double (*)(std::vector<unsigned long>&, unsigned long)>> generators = {
        {"lcg", lcg_adapter},
        {"qcg", qcg_adapter},
        {"xor", xor_adapter},
        {"std", std_adapter}
    };

    // Оригинальный код main
    std::fstream analyticsFile;
    std::string analyticsFilename = "analytics.txt";
    analyticsFile.open(analyticsFilename, std::ios::out);

    for (int test_id = 0; test_id < std::stoi(argv[1]); ++test_id)
    {


        std::fstream inFile;
        std::string inFilename = "tests/test_" + std::to_string(test_id) + ".txt";
        inFile.open(inFilename, std::ios::in);
        if (!inFile)
        {
            std::cerr << "Unable to open file: " << inFilename;
            return 500;
        }
        unsigned long count = 0;
        inFile >> count;
        inFile.ignore(); // Чтобы пропустить символ переноса строки из буффера


        std::vector<unsigned long> result;

        std::fstream outFileA, outFileB, outFileC, outFileD;
        std::string outFilenameA = "results/result_a_" + std::to_string(test_id) + ".txt";
        std::string outFilenameB = "results/result_b_" + std::to_string(test_id) + ".txt";
        std::string outFilenameC = "results/result_c_" + std::to_string(test_id) + ".txt";
        std::string outFilenameD = "results/result_d_" + std::to_string(test_id) + ".txt";
        if (openOutFile(outFileA, outFilenameA) == 500 || openOutFile(outFileB, outFilenameB) == 500 || openOutFile(outFileC, outFilenameC) == 500 || openOutFile(outFileD, outFilenameD) == 500)
        {
            std::cerr << "UNABLE TO OPEN FILE" << std::endl;
            return 500;
        }
        std::vector<algoInfo> algos_arr = {algoInfo("LCG", lcg_adapter, outFileA), algoInfo("QCG", qcg_adapter, outFileB), algoInfo("XOR", xor_adapter, outFileC), algoInfo("std", std_adapter, outFileD)};
        analyticsFile << count;
        for (int i = 0; i < algos_arr.size(); ++i)
        {
            // Считаем время поиска
            double algo_time = calc_algo_time(algos_arr[i].func, result, count);
            std::cout << algos_arr[i].name << " генерация под номером " << test_id << " " << count << " элементов " << " заняла: " << algo_time << " секунд" << std::endl;
            writeToFile(algos_arr[i].fileStream, result);
            analyticsFile << " " << algo_time;
        }

        
        if(test_id == std::stoi(argv[1])-1) // Так сделать быстрее, чем написать нормальный код
        {
            std::cout << "Генерация файлов..." << std::endl;
            // Генерация тестовых файлов для NIST и dieharder
            for (int i = 0; i < algos_arr.size(); ++i) {
                std::cout << "Генерация файла " << algos_arr[i].name << std::endl;
                std::string filename = "NIST_dieharder_tests/" + algos_arr[i].name + "_test.bin";
                generate_test_file(BYTES_PER_FILE, filename, algos_arr[i].func);
            }
            std::cout << "Генерация Завершена!!!" << std::endl;

        }

        analyticsFile << std::endl;
    }
    return 0;
}