# A Detailed Macro Placer for Wirelength Minimization–Algorithm and Implementation
In this research, we target the detailed macro placement problem and propose a two-step detailed macro placer for reducing the total wirelength of the circuit. 
<br>In this problem, we assume a prototype macro placement result is given, and the placer will output the orientation and location of each macro, which can shorten the total wirelength. 
<br>In the first step, the shifting step, the placer will adjust the positions of the macros to minimize the wirelength between them. As for the second step, the flipping step, the orientations of the macros will be changed to gather the pins on them and shrink the bounding box of each wire. 
<br>The key concept behind the two-step placer is that if the improvement of the first-step algorithm reaches a user-specified goal, the second step can be skipped and the total runtime can be saved with acceptable results. 
<br>p.s. The code has been packed up in the 
