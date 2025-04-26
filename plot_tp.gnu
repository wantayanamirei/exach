# Устанавливаем заголовок графика
set terminal jpeg size 1920, 1080
set title "T-P Profile"

# Устанавливаем метки осей
set xlabel "T (K)"
set ylabel "P (бар)"

# Устанавливаем логарифмическую шкалу для оси Y
set logscale y

# Устанавливаем диапазон значений для осей
set xrange [0:3500]
set yrange [1e6:1e-11]
set grid

# Устанавливаем ключ (легенду)
set key top right

# Устанавливаем терминал для сохранения в файл
set terminal jpeg
set output 'tpp.jpg'

# Загружаем данные из файла
plot 'profiles/tpp_HAT-P-49 b.dat' every 3 using 1:2 with points pt 7 ps 0.3 lc rgb 'grey0' title 'HAT-P-49 b', \
	'profiles/tpp_HD 209458 b.dat' every 3 using 1:2 with points pt 7 ps 0.3 lc rgb 'grey20' title 'HD 209458 b', \
	'profiles/tpp_TRAPPIST-1 e.dat' every 3 using 1:2 with points pt 7 ps 0.3 lc rgb 'grey40' title 'TRAPPIST-1 e', \
	'profiles/Jupiter Galileo.dat' using 1:2 with lines lw 3 lc rgb 'slategray' title 'Jupiter (Galileo)', \
	'profiles/tpp_Sun e.dat' using 1:2 with lines lw 3 lc rgb 'green' title 'Jupiter (model)', \
	'profiles/tpp_latest.dat' every 6 using 1:2 with lines lw 4 lc rgb 'gold' title 'Latest'