#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <cstdlib>
#include <sstream>
#include <vector>
#include <cstring>
#include <map>

#include <time.h>
#include <stdio.h>

#include "class.h"

using namespace std;

int main(int argc, char* argv[])
{  
    place_macro a;
    a.start_t = clock();
    a.v_file( argc, argv );
    a.mlist_file( argc, argv );
    a.lef_file( argc, argv );
    a.def_file( argc, argv );
    a.txt_file( argc, argv );
    
    a.draw_rectangle( argc, argv );
    
    //////////////calculate time/////////////////
    a.end_t = clock();
    a.total_t = (a.end_t-a.start_t)/(CLOCKS_PER_SEC*60);
    cout<<"Total time: "<<a.total_t<<endl;
    
    return 0;
}
