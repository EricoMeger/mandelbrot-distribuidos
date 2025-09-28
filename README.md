# mandelbrot-distribuidos

Referências

https://ssvassiliev.github.io/Summer_School_OpenMP/08-mandel/index.html

==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==

850000 iterações, linear

tempo 1: 626226\
tempo 2: 629329\
tempo 3: 626475\
tempo 4: 626596\
tempo 5: 625738

média: 626872.8 -> 10min44seg

==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==

850000 iterações, 2 threads  - dynamic, 2 chunks

tempo 1: 312550ms\
tempo 2: 312464ms\
tempo 3: 312040ms\
tempo 4: 312317ms\
tempo 5: 312069ms

média: 312288ms -> 5min20seg

==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==

850000 iterações, 4 threads  - dynamic, 2 chunks

tempo 1: 181596ms\
tempo 2: 192887ms\
tempo 3: 181660ms\
tempo 4: 181619ms\
tempo 5: 181658ms

média: 183884ms -> 3min~1seg 

==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==

850000 iterações, 8 threads  - dynamic, 2 chunks

tempo 1: 187129ms\
tempo 2: 187202ms\
tempo 3: 187164ms\
tempo 4: 187160ms\
tempo 5: 187226ms

média: 187176.2ms -> 3min11seg

==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==

850000 iterações, 2 threads  - guided, 2 chunks

tempo 1: 311889\
tempo 2: 311912\
tempo 3: 311915\
tempo 4: 311919\
tempo 5: 311912

média: 311909.4 -> 5min19seg

==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==

850000 iterações, 4 threads  - guided, 2 chunks

tempo 1: 335748\
tempo 2: 322774\
tempo 3: 338335\
tempo 4: 335707\
tempo 5: 319162

média: 330345.2 -> 5min50seg

==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==

850000 iterações, 8 threads  - guided, 2 chunks

tempo 1: 206783\
tempo 2: 203418\
tempo 3: 206389\
tempo 4: 215196\
tempo 5: 214031

média: 209163.4 -> 3min48seg

