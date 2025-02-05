# Устанавливаем заголовок графика
set terminal jpeg size 1920, 1080
set title "T-P Profile"

# Устанавливаем метки осей
set xlabel "T (K)"
set ylabel "P (бар)"

# Устанавливаем логарифмическую шкалу для оси Y
set logscale y

# Устанавливаем диапазон значений для осей
set xrange [0:4200]
set yrange [1e3:1e-5]
set grid

# Устанавливаем ключ (легенду)
set key top right

# Устанавливаем терминал для сохранения в файл
set terminal jpeg
set output 'tpp.jpg'

# Загружаем данные из файла
plot 'profiles/tpp_HAT-P-49 b.dat' every 3 using 1:2 with points pt 7 ps 0.3 lc rgb 'grey0' title 'HAT-P-49 b', \
	'profiles/tpp_HD 209458 b.dat' every 3 using 1:2 with points pt 7 ps 0.3 lc rgb 'grey20' title 'HD 209458 b', \
	'profiles/tpp_15 Sge b.dat' every 3 using 1:2 with points pt 7 ps 0.3 lc rgb 'grey40' title '15 Sge b', \
	'profiles/tpp_latest.dat' every 6 using 1:2 with lines lw 4 lc rgb 'gold' title 'latest', \
	'profiles/condensations/H2O.dat' using 1:2 with lines lw 3 lc rgb 'slategray' title 'H2O clouds', \
	'profiles/condensations/NH3.dat' every 10 using 1:2 with lines lw 3 lc rgb 'skyblue' dt 2 title 'NH3 clouds'# , \
#	'profiles/condensations/H2O.dat' using 1:2 with lines lw 3 lc rgb 'skyblue' title 'NH4HS clouds', \
#	'profiles/condensations/MgSiO4.dat' using 1:2 with lines lw 3 lc rgb 'skyblue' title 'MgSiO4 clouds'
