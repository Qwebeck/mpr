1. T
2. Speedup = T (one thread) / (T n threads) - na ile czas wykonania na jednym wątku jest mniejszy od czasu wykonania na N wątkach

3. Efficiency = Speedup / N - pokazuje jak dobrze nam poszło w stosunku do idealnego przyspieszenia

Ile od idealnego speedup-u udało nam się osiągnąć (powinno być 1)

4. Sf (serial fraction. Carp - Flatt) = (1 / Speedup - 1 / N) / (1 - 1 / N) - jak duża część naszego problemu nie mogła być zrównoległona (nie rysujemy dla jednego procesora)

Powinno być 0, dlatego, że idealny speedup jest równy N
