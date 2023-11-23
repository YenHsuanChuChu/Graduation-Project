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

//����a.update_macrolist_fake();
//a.pin_number_in_macro_for_control();
//a.Area_Arrangement();
//a.area_Optimization();
//a.dmp_file(argc, argv);
//a.update_macrolist_fake();
//a.pl_updating(argc, argv);
//�}a.shifting();

int main(int argc, char* argv[])
{  
    
    place_macro a;
    a.start_t = clock();
    a.v_file( argc, argv );
    a.mlist_file( argc, argv );
    a.lef_file( argc, argv );
    a.def_file( argc, argv );
    a.txt_file( argc, argv );
    /*
    a.find_coordinate_construct_map();
    a.HPWL_result.push_back(a.calculate_HPWL());
    

    cout<<"HPWL of prototype is = "<<a.calculate_HPWL()<<endl;
    */
    
    //cout<<endl<<"wiremap size: "<<a.wiremap.size()<<endl;
    //cout<<"HPWLmap size: "<<a.HPWLmap.size()<<endl;
	  
    //cout<<endl<<"test"<<endl<<endl;
    
    
    /////////////////algorithm///////////////////
    //�]Algorithm1 + Algorithm 2(FBH_old�A���nfixed) + OPT(�Ҽ{area)
    
    //a.pin_number_in_macro();
    //a.pin_connection_in_macro();
    //a.update_macrolist_pinlist();
    
    //a.flip_by_HPWL();
    //a.Distance_Arrangement();
    //a.Area_Arrangement();
    //a.Optimization();
    //a.flip_by_HPWL();
    //a.print_result();
    //a.FBH_new();
    
    /////////////////algorithm///////////////////
    
    //a.wiremap.clear();
    //a.HPWLmap.clear();
    //a.find_coordinate_construct_map();
    //a.update_macrolist_fake();
    
    
    //�ˬdmacrolist_fake���e
    /*
    vector<Macro>::iterator miter=a.macrolist.begin();
    for(miter; miter!=a.macrolist.begin()+5; ++miter){
        cout<<"macro name : "<<miter->name<<endl;
        cout<<"center x : "<<miter->center_x<<"  center y : "<<miter->center_y<<endl;
        vector<Pin>::iterator piter=miter->pinlist.begin();
        for(piter; piter!=miter->pinlist.begin()+3; ++piter){
            cout<<"pin name : "<<piter->name<<"  pin x : "<<piter->x<<"  pin y : "<<piter->y<<endl;
        }
        cout<<"pinlist size: "<<miter->pinlist.size()<<endl;
    }
    
    //vector<Macro>::iterator miter=macrolist_fake.begin();
    for(miter=a.macrolist_fake.begin(); miter!=a.macrolist_fake.begin()+5; ++miter){
        cout<<"macro name : "<<miter->name<<endl;
        cout<<"center x : "<<miter->center_x<<"  center y : "<<miter->center_y<<endl;
        vector<Pin>::iterator piter=miter->pinlist.begin();
        for(piter=miter->pinlist.begin(); piter!=miter->pinlist.begin()+3; ++piter){
            cout<<"pin name : "<<piter->name<<"  pin x : "<<piter->x<<"  pin y : "<<piter->y<<endl;
        }
        cout<<"pinlist size: "<<miter->pinlist.size()<<endl;
    }
    */
    
    //a.pin_number_in_macro_for_control();
    //a.Area_Arrangement();
    //a.area_Optimization();
    //a.pin_connection_in_macro(); //�]stdcell
    
    /*
    //****************************************************************0324 shifting + q2
    double HPWL0,HPWL1,HPWL2;
    double timer1,timer2;
    
    HPWL0 = a.calculate_HPWL();
    
    //�]shifting
    a.shifting();
    
    a.wiremap.clear();
    a.HPWLmap.clear();
    a.find_coordinate_construct_map();
    a.HPWL_result.push_back(a.calculate_HPWL());

    //cout<<"HPWL after shifting is = "<<a.calculate_HPWL()<<endl;
    HPWL1 = a.calculate_HPWL();
    a.unfixed_macro(1);
    a.end_t = clock();
    timer1 = (a.end_t-a.start_t)/(CLOCKS_PER_SEC);
    
    //�]q2½��
    a.pin_number_in_macro_for_control();
    a.pin_connection_in_macro();
    
    a.wiremap.clear();
    a.HPWLmap.clear();
    a.find_coordinate_construct_map();
    a.HPWL_result.push_back(a.calculate_HPWL());
    
    //cout<<"HPWL after shifting + q2 flipping is = "<<a.calculate_HPWL()<<endl;
    a.end_t = clock();
    HPWL2 = a.calculate_HPWL();
    timer2 = (a.end_t-a.start_t)/(CLOCKS_PER_SEC);
    
    //a.draw_rectangle( argc, argv );
    */

    //******************** output *********************/
    
    /*
    //0326�Ȯ������@�U
    a.update_macrolist_fake();
    a.pin_number_in_macro_for_control();
    a.Area_Arrangement();
    a.area_Optimization();
    a.pin_connection_in_macro();
    a.dmp_file(argc, argv);
    
    a.pl_updating(argc, argv);
    */
    
    /*
    //a.bounding_box_percentage( argc, argv );
    a.find_coordinate_construct_map();
    a.find_cluster();
    a.draw_rectangle_connection( argc, argv );
    */
    
    ////////��discriminant�]//////////////////////////////////////////// 
    double HPWL0;
    int times=0;
    vector<double> dis_result;
    
    HPWL0 = a.calculate_HPWL();
    a.HPWL_result.push_back(HPWL0);
    
    //�]shifting
    a.shifting();
    double D = a.total_shifting_distance;
        //�M�z�ú�u��
        a.wiremap.clear();
        a.HPWLmap.clear();
        a.find_coordinate_construct_map();
        a.HPWL_result.push_back(a.calculate_HPWL());
    
    //�p��discriminant T��
    double T = 0;
    
    //��Xmacro�ƶq(N)
    int N = a.macrolist.size();
    //��X�`macro pin��(P)
    int P = 0;
    vector<Macro>::iterator miter = a.macrolist.begin();
    for (miter; miter != a.macrolist.end(); ++miter){
        P = P + miter->pinlist.size();
    }
    //����첾�Z��(S)
    int S = a.displacement_max; 
    //�`�첾(D)
    //�W����L�F
    
    T = 0.2*(N/84) + 0.1*(P/38028) + 0.3*(S/10000) + 0.4*(D/580000) ;
    dis_result.push_back(T);
    
    cout<<"N = "<<N<<", P = "<<P<<", S = "<<S<<", D = "<<D<<endl;
    cout<<"T = "<<T<<endl;
    
    //while(T<=5 && times<=4 && D!=0) {
    while(times<1) {
        //½��Q2
        a.pin_number_in_macro_for_control();
        a.pin_connection_in_macro();
            //�M�z�ú�u��
            a.wiremap.clear();
            a.HPWLmap.clear();
            a.find_coordinate_construct_map();
            a.HPWL_result.push_back(a.calculate_HPWL());
            
        a.shifting();
        D = a.total_shifting_distance;
            //�M�z�ú�u��
            a.wiremap.clear();
            a.HPWLmap.clear();
            a.find_coordinate_construct_map();
            a.HPWL_result.push_back(a.calculate_HPWL());
            
        T = 0.2*(N/84) + 0.1*(P/38028) + 0.3*(S/10000) + 0.4*(D/580000) ;
        cout<<"N = "<<N<<", P = "<<P<<", S = "<<S<<", D = "<<D<<endl;
        cout<<"T = "<<T;
        
        times++;
    }
    cout<<"repeated times = "<<times<<endl;
    //////////////calculate time/////////////////
        a.end_t = clock();
        a.total_t = (a.end_t-a.start_t)/(CLOCKS_PER_SEC);
        cout<<"Total time: "<<a.total_t<<endl;
        
    //�L�Xdiscriminant��
    vector<double>::iterator Diter = dis_result.begin();
    cout<<"The discriminant values are: ";
    for(Diter; Diter != dis_result.end(); ++Diter){
        cout<<*Diter<<" ";
    }
    cout<<endl;
    
    //�L�X�u���ܤ�
    vector<double>::iterator Hiter = a.HPWL_result.begin();
    cout<<"The HPWL results are: ";
    for(Hiter; Hiter != a.HPWL_result.end(); ++Hiter){
        cout<<*Hiter<<" ";
    }
    cout<<endl;
        
    a.dmp_file(argc, argv);
    a.pl_updating(argc, argv); 
    
    //////////////calculate time/////////////////
    
    
    //�٨S���v���禡
    /*
    a.pin_number_in_macro();
    a.Weight_Arrangement( 0.33 , 0.33 , 0.33);
    a.Optimization(); //�令seanonly��������
    a.pl_updating(argc, argv , 1);
    */
    
    
    return 0;
}
