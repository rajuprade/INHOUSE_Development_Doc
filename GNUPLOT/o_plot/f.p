 # Gnuplot script file for plotting data in file "force.dat"
      # This file is called   force.p
      set   autoscale                        # scale axes automatically
      unset log                              # remove any log-scaling
      unset label                            # remove any previous labels
      set xtic auto                          # set xtics automatically
      set ytic auto                          # set ytics automatically
      set title "Time between 2 chunks of Response packet(1s inter) Date-11/09/2013 Dur:12 Hrs"
      set xlabel "Sample"
      set ylabel "Time (Mili second)"
      # set key 0.01,100
     # set label "Yield Point" at 0.003,260
     # set arrow from 0.0028,250 to 0.003,280
      set xr [1:2000]
      set yr [0:1200]
      plot    "Time1secRR.dat" with linespoints
