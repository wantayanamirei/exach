#include "PlanetData.h"
#include "csv.h"
#include <algorithm>
#include <boost/math/special_functions/expint.hpp> // Boost library for expn
#include <cctype>
#include <chrono>
#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>
#include <termios.h>
#include <thread>
#include <unistd.h>
#include <vector>

using namespace std;

///////////////////////////////////////
//                                   //
//              ЧАСТЬ 0              //
//          И ВСЁ ЗАВЕРТЕ...         //
//                                   //
///////////////////////////////////////

#define _USE_MATH_DEFINES // M_PI, M_E, etc.
#define KEY_UP 65         // Клавиша Вверх
#define KEY_DOWN 66       // Клавиша Вниз
#define KEY_ENTER 10      // Клавиша ENTER

RowData pdata; // Импорт данных из PlanetData.h

float bond_albedo;

// Для вывода сообщений об успехе, ошибке, акцентировании внимания на чём-либо
std::stringstream colTxt(const std::string &text, int textColor, int backgroundColor = -1) {
    std::stringstream ss;
    ss << "\x1b[";
    ss << textColor;
    if (backgroundColor != -1) {
        ss << ";" << backgroundColor;
    }
    ss << "m" << text << "\x1b[0m";
    return ss;

    // Цвета текста:
    // 30 - черный, 31 - красный, 32 - зеленый, 33 - желтый, 34 - синий, 35 - фиолетовый, 36 - голубой, 37 - белый (для UNIX-систем)
}

///////////////////////////////////////
//                                   //
//              ЧАСТЬ I              //
//             CSV-ПАРСЕР            //
//                                   //
///////////////////////////////////////

int g_row_number = -1;

void getName() {
    std::cout << colTxt("Введите название планеты: ", 33).str();
    std::getline(std::cin >> std::ws, pdata.name); // ws - на случай лишних пробелов перед вводом, pdata.name - название планеты
    for (char &c : pdata.name) {
        c = std::tolower(c);
    }

    rapidcsv::Document doc("catalogue.csv");
    auto names = doc.GetColumn<std::string>(1); // получение второго столбца (с названиями) в виде вектора строк
    for (size_t i = 0; i < names.size(); ++i) {
        for (char &c : names[i]) {
            c = std::tolower(c);
        }
        if (names[i] == pdata.name) {
            g_row_number = i; // при совпадении названия с pdata.name - сохранение номера строки
            return;
        }
    }
    std::cerr << "Планета не найдена." << std::endl;
    exit(1);
}

void getName2() {
    std::cout << colTxt("Моделирование: ", 33).str() << pdata.name << std::endl;
    rapidcsv::Document doc("catalogue.csv");
    auto names = doc.GetColumn<std::string>(1); // получение второго столбца (с названиями) в виде вектора строк
    for (size_t i = 0; i < names.size(); ++i) {
        if (names[i] == pdata.name) {
            g_row_number = i; // при совпадении названия с pdata.name - сохранение номера строки
            return;
        }
    }
}

void csvParser() {
    rapidcsv::Document doc("catalogue.csv");
    auto row = doc.GetRow<std::string>(g_row_number); // получение строки g_row_number (i) в виде вектора строк

    /*
     * Все переменные из header-строки указаны в файле Planetpdata.h (в виде struct) и в catalogue_def.txt (для более легкого определения номеров столбцов)
     * Если строка variable, полученная из ячейки, не пустая - строка с помощью std::stof преобразовывается в float variable
     * Если строка variable, полученная из ячейки, пустая - float variable приравнивается к -274 (маркер для функций из ЧII, что значение неизвестно; отладка std::stof)
     * Число -274 выбрано как ближайшее, округляя вниз, целое число после абсолютного нуля, чтобы не создавать конфликтов с float temp_calculated
     *
     */

    pdata.mass = (row[3].empty()) ? -274 : std::stof(row[3]);
    pdata.mass_sini = (row[6].empty()) ? -274 : std::stof(row[6]);
    pdata.radius = (row[9].empty()) ? -274 : std::stof(row[9]);
    pdata.orbital_period = (row[12].empty()) ? -274 : std::stof(row[12]);
    pdata.semi_major_axis = (row[15].empty()) ? -274 : std::stof(row[15]);
    pdata.eccentricity = (row[18].empty()) ? -274 : std::stof(row[18]);
    pdata.inclination = (row[21].empty()) ? -274 : std::stof(row[21]);
    pdata.angular_distance = (row[24].empty()) ? -274 : std::stof(row[24]);
    pdata.discovered = (row[25].empty()) ? -274 : std::stoi(row[25]); // Обработка int
    pdata.updated = row[26];
    pdata.impact_parameter = (row[45].empty()) ? -274 : std::stof(row[45]);
    pdata.temp_calculated = (row[54].empty()) ? -274 : std::stof(row[54]);
    pdata.temp_measured = (row[57].empty()) ? -274 : std::stof(row[57]);
    pdata.hot_point_lon = (row[58].empty()) ? -274 : std::stof(row[58]);
    pdata.geometric_albedo = (row[59].empty()) ? -274 : std::stof(row[59]);
    pdata.log_g = (row[62].empty()) ? -274 : std::stof(row[62]);
    pdata.detection_type = row[64];
    pdata.mass_measurement_type = row[65];
    pdata.radius_measurement_type = row[66];
    pdata.alternate_names = row[67];
    pdata.molecules = row[68];
    pdata.star_mass = (row[83].empty()) ? -274 : std::stof(row[83]);
    pdata.star_radius = (row[86].empty()) ? -274 : std::stof(row[86]);
    pdata.star_teff = (row[93].empty()) ? -274 : std::stof(row[93]);
    // pdata.star_sp_type = row[89];
}

///////////////////////////////////////
//                                   //
//              ЧАСТЬ II             //
//           ПРЕОБРАЗОВАНИЯ          //
//                                   //
///////////////////////////////////////

const double sec_per_day = 86400.0;       // секунд в дне
const double au_to_meters = 1.496e11;     // метров в 1 астрономической единице
const double jupiter_mass = 1.898e27;     // масса Юпитера в КГ
const double sun_mass = 1.989e30;         // масса Солнца в КГ
const double jupiter_radius_m = 69911000; // радиус Юпитера в КГ

float temp_any = -274;

// Перевод из астрономических значений в СИ
void fromSourceToSI() {
    pdata.orbital_period = pdata.orbital_period * sec_per_day;
    pdata.semi_major_axis = pdata.semi_major_axis * au_to_meters;
    pdata.mass = pdata.mass * jupiter_mass;
    pdata.radius = pdata.radius * jupiter_radius_m;
    pdata.star_mass = pdata.star_mass * sun_mass;
}

// Перевод из СИ в астрономические значений
void fromSIToSource() {
    pdata.orbital_period = pdata.orbital_period / sec_per_day;
    pdata.semi_major_axis = pdata.semi_major_axis / au_to_meters;
    pdata.mass = pdata.mass / jupiter_mass;
    pdata.radius = pdata.radius / jupiter_radius_m;
    pdata.star_mass = pdata.star_mass / sun_mass;
}

// Получение T, a, Mj, Ms
void getKeplerThird() {
    int unknown_count = 0; // количество неизвестных
    if (pdata.orbital_period <= 0) {
        unknown_count++;
        std::cout << "KT: T - ? | ";
    }
    if (pdata.semi_major_axis <= 0) {
        unknown_count++;
        std::cout << "KT: a - ? | ";
    }
    if (pdata.mass <= 0) {
        unknown_count++;
        std::cout << "KT: pM - ? | ";
    }
    if (pdata.star_mass <= 0) {
        unknown_count++;
        std::cout << "KT: sM - ? | ";
    }
    if (unknown_count == 0) {
        std::cout << colTxt("KT - ✓", 32).str() << std::endl;
    } else if (unknown_count == 1) {
        const double G = 6.67430e-11; // гравитационная постоянная
        // Преобразуем входные данные в СИ
        fromSourceToSI();
        if (pdata.orbital_period <= 0) {
            // Вычисляем период T
            double squared_period = (4 * M_PI * M_PI * pow(pdata.semi_major_axis, 3)) / (G * (pdata.star_mass + pdata.mass));
            pdata.orbital_period = sqrt(squared_period); // Извлекаем квадратный корень
            fromSIToSource();                            // Преобразуем результат в нужные единицы
            std::cout << "KT: T = " << pdata.orbital_period << " дней" << std::endl;
        } else if (pdata.semi_major_axis <= 0) {
            // Вычисляем большую полуось a
            pdata.semi_major_axis = cbrt((G * (pdata.star_mass + pdata.mass) * pdata.orbital_period * pdata.orbital_period) / (4 * M_PI * M_PI));
            fromSIToSource();
            std::cout << "KT: a = " << pdata.semi_major_axis << " а.е." << std::endl;
        } else if (pdata.mass <= 0) {
            // Вычисляем массу планеты
            pdata.mass = (4 * M_PI * M_PI * pow(pdata.semi_major_axis, 3)) / (G * pdata.orbital_period * pdata.orbital_period) - pdata.star_mass;
            fromSIToSource();
            std::cout << "KT: M = " << pdata.mass << " Mj" << std::endl;
        } else if (pdata.star_mass <= 0) {
            // Вычисляем массу звезды
            pdata.star_mass = (4 * M_PI * M_PI * pow(pdata.semi_major_axis, 3)) / (G * pdata.orbital_period * pdata.orbital_period) - pdata.mass;
            fromSIToSource();
            std::cout << "KT: SM = " << pdata.star_mass << " Msun" << std::endl;
        }
    } else {
        std::cout << colTxt("KT - ×", 31).str() << std::endl;
    }
}

// Получение Rj, surface_gravity, Mj
void getRGM() {
    int unknown_count = 0; // количество неизвестных

    // Проверяем, какие параметры неизвестны
    if (pdata.radius < 0) {
        unknown_count++;
        std::cout << "RGM: R - ? | ";
    }
    if (pdata.log_g < 0) {
        unknown_count++;
        std::cout << "RGM: g - ? | ";
    }
    if (pdata.mass < 0) {
        unknown_count++;
        std::cout << "RGM: M - ? | ";
    }

    if (unknown_count == 0) {
        std::cout << colTxt("RGM - ✓", 32).str() << std::endl;
    } else if (unknown_count == 1) {
        const double G = 6.67430e-11; // гравитационная постоянная

        // Преобразуем входные данные в СИ
        fromSourceToSI();

        if (pdata.log_g < 0) {
            // Вычисляем ускорение свободного падения g
            pdata.surface_gravity = (G * pdata.mass) / pow(pdata.radius, 2);
            std::cout << "RGM: g = " << pdata.surface_gravity << " м/с^2" << std::endl;
        } else if (pdata.radius < 0) {
            // Вычисляем радиус R
            pdata.surface_gravity = pow(10, pdata.log_g); // log_g = log10(g)
            pdata.radius = std::sqrt(G * pdata.mass / pdata.surface_gravity);
            std::cout << "RGM: R = " << pdata.radius / jupiter_radius_m << " Rj" << std::endl;
        } else if (pdata.mass < 0) {
            // Вычисляем массу M
            pdata.surface_gravity = pow(10, pdata.log_g); // log_g = log10(g)
            pdata.mass = (pdata.surface_gravity * pow(pdata.radius, 2)) / G;
            std::cout << "RGM: M = " << pdata.mass / jupiter_mass << " Mj" << std::endl;
        }

        // Преобразуем результаты в нужные единицы
        fromSIToSource();
    } else {
        std::cout << colTxt("RGM - ×", 31).str() << std::endl;
    }
}

// Избавление от pdata.temp_any
void getTempAny() {
    if (pdata.temp_calculated > -274) {
        temp_any = pdata.temp_calculated;
    }
    if (pdata.temp_measured > -274) {
        temp_any = pdata.temp_measured; // бОльший приоритет измеренной температуре, нежели посчитанной по T_eff
    } else {
        std::cout << "Нет температуры\n";
    }
}

void getBondAlbedo() {
    if (pdata.geometric_albedo != -274) {
        bond_albedo = pdata.geometric_albedo * 0.816019098804724;
        std::cout << colTxt("BA - ✓", 32).str() << std::endl;
    } else {
        std::cout << colTxt("BA - × | βs = 1.0", 31).str() << std::endl;
        bond_albedo = 0.0;
    }
    // q = 0.816019098804724;
    // A = pq; q -- фазовый интеграл (средний на основе данных о телах СС)
}

void catchErrors() {
    // тут должен быть код, который возвращает пользователя обратно в меню в случае, если для планеты отсутствуют данные (если getKeplerThird и getRGM не удалось выполнить, например)
}

///////////////////////////////////////
//                                   //
//             ЧАСТЬ III             //
//          ГАДАНИЕ НА ГУЩЕ          //
//                                   //
///////////////////////////////////////

int is_Dwarf = 0;
int is_EGP = 0;
int is_EIP = 0;
int is_ETP = 0;
int is_EEP = 0;
const int max_is_value = 8;
int planet_type = 0;

// 33-31-34-36-32

// Brown Dwarf
void isThisDwarf() {
    if (pdata.mass > 13.6) is_Dwarf = max_is_value; // для минимального порога поставлена масса, при которой начинается синтез дейтерия
    if (pdata.radius >= 0.7 && pdata.radius <= 1.4) is_Dwarf++;
    if (pdata.log_g >= 4.5) is_Dwarf++;
    if (temp_any > 800) is_Dwarf++;

    if (is_Dwarf > max_is_value) is_Dwarf = max_is_value;

    std::string bars(is_Dwarf, '|');
    std::string remainingBars((max_is_value - is_Dwarf), '-');
    std::cout << "[" << colTxt(bars, 33).str() << colTxt(remainingBars, 30).str() << "] D" << std::endl;
}

// Extrasolar Giant Planet
void isThisEGP() {
    // для минимального порога поставлена масса WASP-193 Ab, "самого легкого газового гиганта"
    // для максимального порога поставлена масса, при которой начинается синтез дейтерия
    if (pdata.mass > 0.138 && pdata.mass <= 13.2) is_EGP += 3;
    if (pdata.radius > 0.91) is_EGP += 3; // >10R⊕
    if (temp_any > 80) is_EGP += 2;
    if (pdata.log_g > 2.5) is_EGP += 2;

    // Maximum theoretical size limit assumed for a ~ 5 MJ mass object right after formation, however, for 'arbitrary initial conditions'.
    if (pdata.mass >= 4 && pdata.radius >= 7) is_EGP = max_is_value;

    std::string bars(is_EGP, '|');
    std::string remainingBars((max_is_value - is_EGP), '-');
    std::cout << "[" << colTxt(bars, 31).str() << colTxt(remainingBars, 30).str() << "] G" << std::endl;
}

// Extrasolar Ice Planet (Ice Giant)
void isThisEIP() {
    if (pdata.radius <= 0.55 && pdata.radius >= 0.31) is_EIP += 4; // 1.9 < R < 3.9 R⊕
    if (pdata.mass > 0.014 && pdata.mass <= 0.10) is_EIP += 4;

    std::string bars(is_EIP, '|');
    std::string remainingBars((max_is_value - is_EIP), '-');
    std::cout << "[" << colTxt(bars, 34).str() << colTxt(remainingBars, 30).str() << "] I" << std::endl;
}

// Extrasolar Transitional Planet
void isThisETP() {
    if (pdata.radius <= 0.36 && pdata.radius >= 0.173) is_ETP += 4; // 1.9 < R < 3.9 R⊕
    if (pdata.mass > 0.006 && pdata.mass <= 0.03) is_ETP += 4;

    std::string bars(is_ETP, '|');
    std::string remainingBars((max_is_value - is_ETP), '-');
    std::cout << "[" << colTxt(bars, 36).str() << colTxt(remainingBars, 30).str() << "] T" << std::endl;
}

// Extrasolar Earth Planet
void isThisEEP() {
    if (pdata.radius <= 0.173 && pdata.radius >= 0) is_EEP += 4; // < 1.9R⊕
    if (pdata.mass >= 0 && pdata.mass <= 0.01) is_EEP += 4;

    std::string bars(is_EEP, '|');
    std::string remainingBars((max_is_value - is_EEP), '-');
    std::cout << "[" << colTxt(bars, 32).str() << colTxt(remainingBars, 30).str() << "] E" << std::endl;
}

// Обработка значений EEP - определение мини- и супер-земель
void isThisAnyOfEarth() {
    int is_mEarth = 0;
    int is_sEarth = 0;

    if (pdata.mass <= 0.005) is_mEarth++;
    if (pdata.mass > 0.007) is_sEarth++;

    std::vector<int> values = {is_mEarth, is_sEarth};
    std::vector<std::string> names = {"мини-земля", "супер-земля"};
    int max_index = 0;
    for (int i = 1; i < values.size(); ++i) {
        if (values[i] > values[max_index]) {
            max_index = i;
        }
    }
    std::cout << names[max_index];
}

// Обработка значений EGP - определение класса газового гиганта по классификации Сударского
void isThisTheSudarskyPlanet() {
    float is_sudarsky = 0;
    int is_classI = 0;
    int is_classII = 0;
    int is_classIII = 0;
    int is_classIV = 0;
    int is_classV = 0;
    int max_is_class_value = 10;

    if (pdata.mass_sini >= 0.17 || pdata.mass >= 0.17 && pdata.mass < 50.0) is_sudarsky++;
    if (bond_albedo >= 0.54 && bond_albedo <= 0.61) is_classI++;
    if (temp_any < 195) is_classI += 2;

    if (bond_albedo >= 0.81) is_classII++;
    if (temp_any >= 195 && temp_any < 250) is_classII += 2;

    if (bond_albedo >= 0.09 && bond_albedo <= 0.20) is_classIII++;
    if (temp_any >= 350 && temp_any < 800) is_classIII += 2;

    if (bond_albedo < 0.09 && bond_albedo > 0) is_classIV++;
    if (temp_any >= 800 && temp_any < 1050) is_classIV += 2;

    if (bond_albedo >= 0.50 && bond_albedo <= 0.60) is_classV++;
    if (temp_any >= 1050) is_classV += 2;

    if (is_classI > 0) is_sudarsky = is_sudarsky + is_classI;
    if (is_classII > 0) is_sudarsky = is_sudarsky + is_classII;
    if (is_classIII > 0) is_sudarsky = is_sudarsky + is_classIII;
    if (is_classIV > 0) is_sudarsky = is_sudarsky + is_classIV;
    if (is_classV > 0) is_sudarsky = is_sudarsky + is_classV;

    std::vector<int> is_class = {is_classI, is_classII, is_classIII, is_classIV, is_classV};
    std::vector<int> ints = {2, 4, 6, 8, 10};

    int max_index = 0;
    for (int i = 1; i < is_class.size(); ++i) {
        if (is_class[i] > is_class[max_index]) {
            max_index = i;
        }
    }

    // Проверяем, есть ли другие элементы с таким же максимальным значением
    bool is_ambiguous = false;
    for (int i = 0; i < is_class.size(); ++i) {
        if (i != max_index && is_class[i] == is_class[max_index]) {
            is_ambiguous = true;
            break;
        }
    }

    if (is_ambiguous) {
        std::cout << ": неопределенный класс" << std::endl;
    } else {
        std::string bars(ints[max_index], '|');
        std::string remainingBars((max_is_class_value - ints[max_index]), '-');
        std::cout << "[" << colTxt(bars, 37).str() << colTxt(remainingBars, 37).str() << "] ";
        switch (ints[max_index]) {
        case 2:
            std::cout << "I";
            break;
        case 4:
            std::cout << "II";
            break;
        case 6:
            std::cout << "III";
            break;
        case 8:
            std::cout << "IV";
            break;
        case 10:
            std::cout << "V";
            break;
        default:
            break;
        }
    }
}

// Вызов всех функций для main()
void gadanieCall() {
    isThisDwarf();
    isThisEGP();
    isThisEIP();
    isThisETP();
    isThisEEP();
}

// Подведение итогов функций is_any
void gadanieResults() {
    std::vector<int> values = {is_Dwarf, is_EGP, is_EIP, is_ETP, is_EEP};
    std::vector<std::string> names = {"Это КК", "Это EGP: ", "Это EIP", "Это ETP", "Это EEP: "};

    // Находим максимальное значение
    int max_index = 0;
    for (int i = 1; i < values.size(); ++i) {
        if (values[i] > values[max_index]) {
            max_index = i;
        }
    }

    // Проверяем, есть ли другие элементы с таким же максимальным значением
    bool is_ambiguous = false;
    for (int i = 0; i < values.size(); ++i) {
        if (i != max_index && values[i] == values[max_index]) {
            is_ambiguous = true;
            break;
        }
    }

    if (is_ambiguous) {
        std::cout << "Неопределенный тип" << std::endl;
    } else {
        std::cout << names[max_index]; // Выводим максимальное значение
        if (values[max_index] == is_Dwarf) planet_type = 1;
        if (values[max_index] == is_EGP) {
            isThisTheSudarskyPlanet();
            planet_type = 2;
        }
        if (values[max_index] == is_EIP) planet_type = 3;
        if (values[max_index] == is_ETP) planet_type = 4;
        if (values[max_index] == is_EEP) {
            isThisAnyOfEarth();
            planet_type = 5;
        }
        std::cout << std::endl;
    }
}

///////////////////////////////////////
//                                   //
//             ЧАСТЬ IV              //
//               ВПТД                //
//                                   //
///////////////////////////////////////

// LASCIATE OGNE SPERANZA, VOI CH'ENTRATE   //
// - Данте Алигьери. Надпись на вратах Ада. //

double tpprofile(float bond_albedo, float m, float m0, float t_int, float t_irr, float kappa_S, float kappa_0, float kappa_cia, float beta_S0, float beta_L0, float el1, float el3, float pressure, int pl_type) {
    float albedo = bond_albedo;

    float n_S = -0.08;
    float l_S = 0.01;
    float ssa_S0 = 1;
    float asymmetry_S0 = 1;

    switch (pl_type) {
    case 1: // КК
        break;
    case 2: // ГГ
        ssa_S0 = 0.7 * (m / m0);
        break;
    case 3: // ХГ
        break;
    case 4: // МН
        break;
    case 5: // ЗГ
        ssa_S0 = 0.87;
        break;
    default:
        ssa_S0 = 0.5;
        break;
    }

    kappa_S *= pow((m / m0), n_S);
    beta_S0 *= pow((m / m0), l_S); // sqrt((1 - ssa_S0) / (1 - ssa_S0 * asymmetry_S0));

    float kappa_L = kappa_0 + kappa_cia * (m / m0); // при S > L - возникновение температурной инверсии
    float beta_S = kappa_S * m / beta_S0;           // параметр рассеяния
    float coeff1 = 0.25 * std::pow(t_int, 4);       // коэфф. перед первыми квадратными скобками
    float coeff2 = 0.125 * std::pow(t_irr, 4);      // коэфф. перед вторыми квадратными скобками

    // Добавляем фотохимический нагрев
    float thermal_conductivity = 0.0;
    float photoheating = 0.0;
    float shortwave_heating = 0.0;

    // условие для верхних слоев
    if ((m / m0) < 1e-6) {
        thermal_conductivity = 800.0 * std::exp(-m / (m0 * pow(10, -9.3)));
        photoheating = 200.0 * std::exp(-m / (m0 * pow(10, -8.5)));
        shortwave_heating = 7200.0 * std::exp(-m / (m0 * 1e-10));
    }

    float term1 =
        1.0 / el1 +
        m / (el3 * pow(beta_L0, 2)) *
            (kappa_0 + (kappa_cia * m) / (2 * m0));
    float term2 =
        1.0 / (2 * el1) +
        boost::math::expint(2, beta_S) *
            (kappa_S / (kappa_L * beta_S0) -
             (kappa_cia * m * beta_S0) / (el3 * kappa_S * m0 * pow(beta_L0, 2)));
    float term3 =
        kappa_0 * beta_S0 / (el3 * kappa_S * pow(beta_L0, 2)) *
        (1.0 / 3 - boost::math::expint(4, beta_S));
    float term4 =
        kappa_cia * pow(beta_S0, 2) / (el3 * m0 * pow(kappa_S, 2) * pow(beta_L0, 2)) *
        (0.5 - boost::math::expint(3, beta_S));

    // %beta_{S_0} ~ = ~ sqrt{ alignc {1-%omega_{S_0}} over {1 - %omega_{S_0} cdot g_{S_0}(P)
    // g_{S_0} = g_{S_0} cdot (P /  P_0^%gamma

    float result = pow((coeff1 * term1 + coeff2 * (term2 + term3 + term4) + 1e6 * temp_any * (photoheating + thermal_conductivity + shortwave_heating)), 0.25);
    return result;
}

void tpoutput() {
    const double rsun_to_cm = 6.9566e10;
    const double au_to_cantimeters = 1.49598e13;
    pdata.semi_major_axis = pdata.semi_major_axis * au_to_cantimeters;
    pdata.star_radius = pdata.star_radius * rsun_to_cm;

    float t_irr = pow(2, 0.5) * pdata.star_teff *
                  pow((pdata.star_radius / (2 * pdata.semi_major_axis)), 0.5) *
                  pow((1 - bond_albedo), 0.25);

    pdata.semi_major_axis /= au_to_cantimeters;
    pdata.star_radius /= rsun_to_cm;

    float g_cgs = pdata.surface_gravity * 1e2;                 // ускорение свободного падения в СГС
    float kappa_S0 = 0.06;                                     // коротковолновая прозрачность; ↗↗ - возникновение антипарникового эффекта
    float kappa_0 = 0.023;                                     // ИК и near-ИК прозрачность - влияет на kappa_L
    float kappa_cia = 0.12;                                    // CIA opacity normalization
    float beta_S0 = (1.0 - bond_albedo) / (1.0 + bond_albedo); // коротковолновый параметр рассеяния; ↗↗ - Ab ↘↘; весь TPP смещается на более низкие температуры из-за зависимости от (1 - Ab)
    float beta_L0 = 0.983;                                     // длинноволновый параметр рассеяния; ↘↘ - TPP смещается в более теплую область
    float el1 = 3.0 / 8.0;                                     // первый длинноволновый коэффициент Эддингтона
    float el3 = 1.0 / 3.0;                                     // второй длинноволновый коэффициент Эддингтона

    std::vector<double> logp;
    std::vector<double> pressure;
    std::vector<double> m;
    double bar2cgs = 1e6;

    float pmin = -9.0;
    float pmax = 2.01;
    switch (planet_type) {
    case 1: // КК
        pmin = -7.0;
        pmax = 3.01;
        break;
    case 2: // ГГ
        pmax = 2.01;
        break;
    case 3: // ХГ
        break;
    case 4: // МН
        break;
    case 5: // ЗГ
        pmin = -9.0;
        pmax = 0.01;
        break;
    default:
        break;
    }

    // Увеличение разрешения
    for (float p = pmin; p <= pmax; p += 0.01) {
        logp.push_back(p);
    }

    for (double logp_val : logp) {
        pressure.push_back(pow(10.0, logp_val));
    }

    float p0 = *max_element(pressure.begin(), pressure.end());
    float m0 = p0 * bar2cgs / g_cgs;

    for (float p : pressure) {
        m.push_back(p * bar2cgs / g_cgs);
    }

    std::vector<double> tp_prof(m.size());
    for (size_t i = 0; i < m.size(); ++i) {
        tp_prof[i] = tpprofile(bond_albedo, m[i], m0, 108, t_irr, kappa_S0, kappa_0, kappa_cia, beta_S0, beta_L0, el1, el3, pressure[i], planet_type);
    }

    if (pdata.name == "Sun e" ||
        pdata.name == "TRAPPIST-1 e" ||
        pdata.name == "HAT-P-49 b" ||
        pdata.name == "HD 209458 b") {
        std::ofstream latest("profiles/tpp_" + pdata.name + ".dat");
        for (size_t i = 0; i < m.size(); ++i) {
            latest << tp_prof[i] << std::setw(15) << pressure[i] << std::endl; // запись температуры/давления
        }
        latest.close();
    } else {
        std::ofstream latest("profiles/tpp_latest.dat");
        for (size_t i = 0; i < m.size(); ++i) {
            latest << tp_prof[i] << std::setw(15) << pressure[i] << std::endl; // запись температуры/давления
        }
        latest.close();
    }
}

///////////////////////////////////////
//                                   //
//              ЧАСТЬ V              //
//           ВЫВОД ДАННЫХ            //
//                                   //
///////////////////////////////////////

// Вывод общей информации
void dataOutput() {
    std::cout << "Название: " << pdata.name << std::endl;
    if (pdata.mass != -274) std::cout << "Масса (Mj): " << pdata.mass << std::endl;
    if (pdata.radius != -274) std::cout << "Радиус (Rj): " << pdata.radius << std::endl;
    if (pdata.orbital_period != -274) std::cout << "Период (дней): " << pdata.orbital_period << std::endl;
    if (pdata.semi_major_axis != -274) std::cout << "Большая полуось (А.Е.): " << pdata.semi_major_axis << std::endl;
    if (pdata.eccentricity != -274) std::cout << "e орбиты: " << pdata.eccentricity << std::endl;
    if (pdata.inclination != -274) std::cout << "Угол наклона орбитальной плоскости: " << pdata.inclination << std::endl;
    if (pdata.angular_distance != -274) std::cout << "Угловое расстояние (сек.): " << pdata.angular_distance << std::endl;
    if (pdata.discovered != -274) std::cout << "Дата обнаружения: " << pdata.discovered << std::endl;
    std::cout << "Дата обновления: " << pdata.updated << std::endl;
    if (pdata.impact_parameter != -274) std::cout << "amin планеты от центра звезды (для транзитных планет) (Rзвезды, %): " << pdata.impact_parameter << std::endl;
    if (pdata.temp_calculated != -274) std::cout << "Tcalc (K): " << pdata.temp_calculated << std::endl;
    if (pdata.temp_measured != -274) std::cout << "Tmeas (K): " << pdata.temp_measured << std::endl;
    if (pdata.hot_point_lon != -274) std::cout << "Hot Point Longtitude:" << pdata.hot_point_lon << std::endl;
    if (pdata.geometric_albedo != -274) std::cout << "Альбедо: " << pdata.geometric_albedo << std::endl;
    if (pdata.surface_gravity != -274) std::cout << "g (м/с^2): " << pdata.surface_gravity << std::endl;
    std::cout << "Тип обнаружения: " << pdata.detection_type << std::endl;
    std::cout << "Тип измерения массы: " << pdata.mass_measurement_type << std::endl;
    std::cout << "Тип измерения радиуса: " << pdata.radius_measurement_type << std::endl;
    std::cout << "Альтернативные названия: " << pdata.alternate_names << std::endl;
    std::cout << "Молекулы: " << colTxt(pdata.molecules, 34).str() << std::endl;
    if (pdata.star_mass != -274) std::cout << "Масса звезды: " << pdata.star_mass << std::endl;
    if (pdata.star_radius != -274) std::cout << "Радиус звезды: " << pdata.star_radius << std::endl;
    if (pdata.star_teff != -274) std::cout << "Teff звезды (K): " << pdata.star_teff << std::endl;
}

int selected = 0; // Отслеживает выбранный пункт

// Просто getch()
int getch() {
    struct termios oldt, newt;
    int ch;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    return ch;
}

// Очистка экрана после обновления выбора
void clearScreen() { std::cout << "\033[2J\033[1;1H"; }

// Отображение указателя
void printOptions(const std::vector<std::string> &options, int selected) {
    clearScreen();
    for (size_t i = 0; i < options.size(); ++i) {
        if (i == selected) {
            std::cout << "> " << options[i] << std::endl;
        } else {
            std::cout << "  " << options[i] << std::endl;
        }
    }
}

// Вызов меню
void menuSelect() {
    selected = 0;
    int option = 6;        // кол-во опций меню
    bool selecting = true; // true, если мы что-то выбираем

    std::vector<std::string> options = {
        "[1] Просмотр данных о планете",
        "[2] Построение T-P профиля",
        "[3] Определитель типа",
        "[4] Восстановление .dat по умолчанию",
        "[T] UN",
        "[5] Выход"};

    printOptions(options, selected);

    char c;
    while (selecting) { // Так долго, как мы выбираем
        c = getch();
        switch (c) { // Проверка на последний введенный символ
        case KEY_UP:
            if (selected > 0) { // Без декремента, если первая опция
                --selected;
                printOptions(options, selected);
            }
            break;
        case KEY_DOWN:
            if (selected < options.size() - 1) { // Без инкремента, если последняя опция
                ++selected;
                printOptions(options, selected);
            }
            break;
        case KEY_ENTER:
            selecting = false;
            break;
        default:
            break;
        }
    }
    selected++;
}

// Вызов расчетных функций из Ч. II
void getPrepared(int i) {
    if (i == 1) getName();
    if (i == 2) getName2();
    csvParser();
    getKeplerThird();
    getRGM();
    getTempAny();
    getBondAlbedo();
    std::cout << colTxt("========================================", 30).str() << std::endl;
}

// Функции меню
void menuAction() {
    switch (selected) {
    case 1:
        getPrepared(1);
        dataOutput();
        break;
    case 2:
        getPrepared(1);
        gadanieCall();
        gadanieResults();
        tpoutput();
        break;
    case 3:
        getPrepared(1);
        gadanieCall();
        gadanieResults();
        break;
    case 4:
        pdata.name = "Sun e";
        getPrepared(2);
        planet_type = 2;
        tpoutput();
        pdata.name = "TRAPPIST-1 e";
        getPrepared(2);
        planet_type = 5;
        tpoutput();
        pdata.name = "HAT-P-49 b";
        getPrepared(2);
        planet_type = 2;
        tpoutput();
        pdata.name = "HD 209458 b";
        getPrepared(2);
        planet_type = 2;
        tpoutput();
        break;
    default:
        exit(0);
    }
}

///////////////////////////////////////
//                                   //
//             ЧАСТЬ VI              //
//            ПОСЛЕСЛОВИЕ            //
//                                   //
///////////////////////////////////////

int main() {
    menuSelect();
    menuAction();
    return 0;
}