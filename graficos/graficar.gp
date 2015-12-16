set logscale x 2
set format x '2^{%L}'
set yrange [-0.1:6]
set xrange [65536:4194304]
set xlabel "Cantidad de Elementos"
set ylabel "Tiempo [segundos]"
set key left top
set title 'Inserción'

set terminal pngcairo dashed enhanced font "Montaga-Bold,12"
set output 'insercion.png'
plot 'abb_real_random_insercion.dat' every ::1 u 1:2 w lp lw 2 dt 1 lc rgb "red" title 'ABB Real', \
     'abb_sintetico_random_insercion.dat' every ::1 u 1:2 w lp lw 2 dt 2 lc rgb "red" title 'ABB Sintético', \
     'avl_real_random_insercion.dat' every ::1 u 1:2 w lp lw 2 dt 1 lc rgb "#2E8B57" title 'AVL Real', \
     'avl_sintetico_random_insercion.dat' every ::1 u 1:2 w lp lw 2 dt 2 lc rgb "#2E8B57" title 'AVL Sintético', \
     'splaytree_real_random_insercion.dat' every ::1 u 1:2 w lp lw 2 dt 1 lc rgb "blue" title 'Splay Tree Real', \
     'splaytree_sintetico_random_insercion.dat' every ::1 u 1:2 w lp lw 2 dt 2 lc rgb "blue" title 'Splay Tree Sintético'
unset output

unset xrange
unset yrange
set xrange [65536:4194304]
set yrange [-0.1:5.3]
set title 'Búsqueda de todos los elementos'
set output 'busquedanormal.png'
plot 'abb_real_random_busquedanormal.dat' every ::1 u 1:2 w lp lw 2 dt 1 lc rgb "red" title 'ABB Real', \
     'abb_sintetico_random_busquedanormal.dat' every ::1 u 1:2 w lp lw 2 dt 2 lc rgb "red" title 'ABB Sintético', \
     'avl_real_random_busquedanormal.dat' every ::1 u 1:2 w lp lw 2 dt 1 lc rgb "#2E8B57" title 'AVL Real', \
     'avl_sintetico_random_busquedanormal.dat' every ::1 u 1:2 w lp lw 2 dt 2 lc rgb "#2E8B57" title 'AVL Sintético', \
     'splaytree_real_random_busquedanormal.dat' every ::1 u 1:2 w lp lw 2 dt 1 lc rgb "blue" title 'Splay Tree Real', \
     'splaytree_sintetico_random_busquedanormal.dat' every ::1 u 1:2 w lp lw 2 dt 2 lc rgb "blue" title 'Splay Tree Sintético'

unset output
unset yrange
unset xrange
set title 'Búsqueda con probabilidades cargadas'

set yrange [-0.01:0.1]
set output 'busquedacargada.png'
plot 'abb_real_random_busquedacargada.dat' every ::1 u 1:2 w lp lw 2 dt 1 lc rgb "red" title 'ABB Real', \
     'abb_sintetico_random_busquedacargada.dat' every ::1 u 1:2 w lp lw 2 dt 2 lc rgb "red" title 'ABB Sintético', \
     'avl_real_random_busquedacargada.dat' every ::1 u 1:2 w lp lw 2 dt 1 lc rgb "#2E8B57" title 'AVL Real', \
     'avl_sintetico_random_busquedacargada.dat' every ::1 u 1:2 w lp lw 2 dt 2 lc rgb "#2E8B57" title 'AVL Sintético', \
     'splaytree_real_random_busquedacargada.dat' every ::1 u 1:2 w lp lw 2 dt 1 lc rgb "blue" title 'Splay Tree Real', \
     'splaytree_sintetico_random_busquedacargada.dat' every ::1 u 1:2 w lp lw 2 dt 2 lc rgb "blue" title 'Splay Tree Sintético'

unset output
unset xrange
unset yrange
set title 'Eliminación'
set xrange [65536:4194304]
set output 'eliminacion.png'
plot 'abb_real_random_eliminacion.dat' every ::1 u 1:2 w lp lw 2 dt 1 lc rgb "red" title 'ABB Real', \
     'abb_sintetico_random_eliminacion.dat' every ::1 u 1:2 w lp lw 2 dt 2 lc rgb "red" title 'ABB Sintético', \
     'avl_real_random_eliminacion.dat' every ::1 u 1:2 w lp lw 2 dt 1 lc rgb "#2E8B57" title 'AVL Real', \
     'avl_sintetico_random_eliminacion.dat' every ::1 u 1:2 w lp lw 2 dt 2 lc rgb "#2E8B57" title 'AVL Sintético', \
     'splaytree_real_random_eliminacion.dat' every ::1 u 1:2 w lp lw 2 dt 1 lc rgb "blue" title 'Splay Tree Real', \
     'splaytree_sintetico_random_eliminacion.dat' every ::1 u 1:2 w lp lw 2 dt 2 lc rgb "blue" title 'Splay Tree Sintético'

unset output
unset terminal