## QSortLab

Реализовал последовательную и параллельную версии быстрой сортировки (https://ru.wikipedia.org/wiki/Быстрая_сортировка).

Провел сравнение с std::sort.

### Benchmark: qsort_par
Time of try 0: 2.11237s\
Time of try 1: 2.05235s\
Time of try 2: 2.03797s\
Time of try 3: 2.03129s\
Time of try 4: 2.07323s
#### Avg exec time: 2.06144s

### Benchmark: qsort_seq_std
Time of try 0: 7.65517s\
Time of try 1: 7.57618s\
Time of try 2: 7.52165s\
Time of try 3: 7.41677s\
Time of try 4: 7.30265s
#### Avg exec time: 7.49448s

### Speed up with std: 3.63556

### Benchmark: qsort_seq_my
Time of try 0: 9.58651s\
Time of try 1: 9.68708s\
Time of try 2: 9.80055s\
Time of try 3: 9.53711s\
Time of try 4: 9.56746s
#### Avg exec time: 9.63574s
### Speed up with my: 4.67427


