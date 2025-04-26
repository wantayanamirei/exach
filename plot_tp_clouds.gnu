# Устанавливаем заголовок графика
set terminal jpeg size 1920, 1080
set title "T-P Profile with clouds"

# Устанавливаем метки осей
set xlabel "T (K)"
set ylabel "P (бар)"

# Устанавливаем логарифмическую шкалу для оси Y
set logscale y

# Устанавливаем диапазон значений для осей
set xrange [0:4200]
set yrange [1e6:1e-11]
set grid

# Устанавливаем ключ (легенду)
set key top right

# Устанавливаем терминал для сохранения в файл
set terminal jpeg
set output 'tpp_clouds.jpg'

# Загружаем данные из файла
plot 'profiles/tpp_latest.dat' every 6 using 1:2 with lines lw 4 lc rgb 'gold' title 'Latest', \
	'profiles/condensations/H2O.dat' using 1:2 with lines lw 3 lc rgb 'slategray' title 'H2O clouds', \
	'profiles/condensations/NH3.dat' using 1:2 with lines lw 3 lc rgb 'skyblue' title 'NH3 clouds', \
	'profiles/condensations/MgSiO3.dat' using 1:2 with lines lw 3 lc rgb 'dark-chartreuse' title 'MgSiO3 clouds', \
	'profiles/condensations/Fe.dat' using 1:2 with lines lw 3 lc rgb 'dark-plum' title 'Fe clouds', \
	'profiles/condensations/NaK.dat' using 1:2 with lines lw 3 lc rgb 'navy' title 'Na/K clouds', \
	'profiles/condensations/H2SO4.dat' using 1:2 with lines lw 3 lc rgb 'sandybrown' title 'H2SO4 clouds', \
	'profiles/condensations/CH3OH.dat' using 1:2 with lines lw 3 lc rgb 'khaki' title 'CH4 clouds', \
	'profiles/condensations/Al2O3.dat' using 1:2 with lines lw 3 lc rgb 'turquoise' title 'Al2O3 clouds'
	