# exoplanet-atmos-characterizer
Characterizer of exoplanet atmospheric parameters.

Улучшение модели. Решение двух главных проблем:
- [ ] Изотермальное состояние в верхних слоях атмосферы (нет поглощения УФ-излучения)
- [ ] Экспоненциальный рост температуры в плотных слоях атмосферы

Остальное:
- [ ] Обработка ошибок через throw, чтобы не плодить std::cout << .....;
- [x] Замена if/else с temp.m/temp.c на функцию void setTempAny() с универсальной переменной temp_any. 
- [ ] Дебаг функции getRGM (завышенные значения)
- [ ] Обработка log_g должным образом
- [ ] Улучшение определителя планет (расширение типов из "Классификация Сударского", "Мини-нептуны", "Супер-земли", "Мини-земли" в "Коричневые карлики", (уже ныне имеющееся), "Ледяные гиганты", "Супер-Венеры", "Супер-Ио"
- [ ] Вынос всех не-приватных переменных в верхнюю часть блоков   
