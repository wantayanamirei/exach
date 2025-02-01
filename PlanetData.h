#ifndef ROW_DATA_H
#define ROW_DATA_H

#include <string>


struct RowData {
  int row_number;
  std::string name; // название
  std::string planet_status; // статус планеты (подтверждена/спорная)
  float mass; // масса в Mj
  float mass_error_min; // погрешность_мин; // погрешность_мин// погрешность_мин
  float mass_error_max; // погрешность_макс
  float mass_sini; // минимальная масса планеты учитывая влияние наклона орбитальной плоскости экзопланеты на её наблюдаемые характеристики и взаимодействие с родительской звездой
  float mass_sini_error_min; // погрешность_мин
  float mass_sini_error_max; // погрешность_макс
  float radius; // радиус в Rj
  float radius_error_min; // погрешность_мин
  float radius_error_max; // погрешность_макс
  float orbital_period; // период (в днях)
  float orbital_period_error_min; // погрешность_мин
  float orbital_period_error_max; // погрешность_макс
  float semi_major_axis; // большая полуось (в а.е.)
  float semi_major_axis_error_min; // погрешность_мин
  float semi_major_axis_error_max; // погрешность_макс
  float eccentricity; // эксцентрисситет орбиты
  float eccentricity_error_min; // погрешность_мин
  float eccentricity_error_max; // погрешность_макс
  float inclination; // угол наклона орбитальной плоскости
  float inclination_error_min; // погрешность_мин
  float inclination_error_max; // погрешность_макс
  float angular_distance; // угловое расстояние, заданное semi_major_axis/дистанцию (в угловых секундах)
  int discovered; // обнаружение планеты
  std::string updated; // дата обновления информации 
  float omega; // угол между 
  float omega_error_min; // погрешность_мин
  float omega_error_max; // погрешность_макс
  float tperi; // время прохождения перицентра для эксцентрических орбит (юлианская дата)
  float tperi_error_min; // погрешность_мин
  float tperi_error_max; // погрешность_макс
  float tconj; // время верхнего соединения звезды и планеты (юлианская дата)
  float tconj_error_min; // погрешность_мин
  float tconj_error_max; // погрешность_макс
  float tzero_tr; // время прохождения центра пересекаемого источника транзита
  float tzero_tr_error_min; // погрешность_мин
  float tzero_tr_error_max; // погрешность_макс
  float tzero_tr_sec; // время прохождения транзита
  float tzero_tr_sec_error_min; // погрешность_мин
  float tzero_tr_sec_error_max; // погрешность_макс
  float lambda_angle; // угол небесной проекции между орбитальным и звездным вращением (аномалия Росситера-Маклафина https://clck.ru/3EhZKg)
  float lambda_angle_error_min; // погрешность_мин
  float lambda_angle_error_max; // погрешность_макс
  float impact_parameter; // минимальное расстояние планеты от центра звезды (для транзитных планет) (Rзвезды, %)
  float impact_parameter_error_min; // погрешность_мин
  float impact_parameter_error_max; // погрешность_макс
  float tzero_vr; // время от нуля, пока идёт увеличение угловой скорости (планета движется к наблюдателю) (юлианская дата)
  float tzero_vr_error_min; // погрешность_мин
  float tzero_vr_error_max; // погрешность_макс
  float k; // полуамплитуда кривой лучевой скорости (м/с)
  float k_error_min; // погрешность_мин
  float k_error_max; // погрешность_макс
  float temp_calculated; // температура планеты (рассчитаная) (K)
  float temp_calculated_error_min; // погрешность_мин
  float temp_calculated_error_max; // погрешность_макс
  float temp_measured; // температуры планеты (измеренная) (K)
  float hot_point_lon; // долгота самой горячей точки планеты (градусы)
  float geometric_albedo; // альбедо 
  float geometric_albedo_error_min; // погрешность_мин
  float geometric_albedo_error_max; // погрешность_макс
  float log_g; // сила тяжести на поверхности (g)
  std::string publication; // [[[[[[[]]]]]]]
  std::string detection_type; // метод обнаружения (радиальных скоростей RV, транзитный transit, TTV, гравитационное микролинзирование lensing, астрометрия astrometry, прямое наблюдение imaging)
  std::string mass_measurement_type; // метод измерения массы (RV, астрометрия, модель при прямом наблюдении)
  std::string radius_measurement_type; // метод измерения радиуса (transit, модель при прямом наблюдении)
  std::string alternate_names; // альтернативные названия одной и той же планеты
  std::string molecules; // молекулы, обнаруженные в спектрах планет
  
  std::string star_name; // название род. звезды
  float ra; // верхнее восхождение
  float dec; // склонение 
  float mag_v; // магнитуда V 
  float mag_i; // магнитуда I 
  float mag_j; // магнитуда J
  float mag_h; // магнитуда H
  float mag_k; // магнитуда K
  float star_distance; // дистанция до звезды (пк)
  float star_distance_error_min; // погрешность_мин
  float star_distance_error_max; // погрешность_макс
  float star_metallicity; // десятичный логарифм массивных элементов к водороду (i.e. Log [(metals/H)star/(metals/H)Sun] )
  float star_metallicity_error_min; // погрешность_мин
  float star_metallicity_error_max; // погрешность_макс
  float star_mass; // масса в Msun
  float star_mass_error_min; // погрешность_мин
  float star_mass_error_max; // погрешность_макс
  float star_radius; // радиус в Rsun
  float star_radius_error_min; // погрешность_мин
  float star_radius_error_max; // погрешность_макс
  float star_sp_type; // спектральный тип
  float star_age; // звездная эра (Gy)
  float star_age_error_min; // погрешность_мин
  float star_age_error_max; // погрешность_макс
  float star_teff; // эффективная звездная температура
  float star_teff_error_min; // погрешность_мин
  float star_teff_error_max; // погрешность_макс
  std::string star_detected_disc; // наличие диска
  std::string star_magnetic_field; // наличие магнитного поля 
  std::string star_alternate_names; // альтернативные названия

  float surface_gravity;

};

extern RowData g_rowData;

#endif
