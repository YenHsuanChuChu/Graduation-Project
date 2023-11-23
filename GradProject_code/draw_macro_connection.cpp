//用來生成macro間連線關係的matlab檔

#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <cstdlib>
#include <sstream>
#include <vector>
#include <cstring>
#include <map>
#include "class.h"

#include <time.h>
#include <stdio.h>

//g++ -std=c++11 algorithm.cpp algorithm_FBH.cpp HPWL.cpp algorithm_move.cpp draw_macro_connection.cpp -o w.out
//./w.out case09.v case09.lef case09.def case09.mlist case09.txt case09_macro_connection.txt


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
    
    a.find_coordinate_construct_map();
   
    a.draw_rectangle_connection( argc, argv );
    
    a.end_t = clock();
    a.total_t = (a.end_t-a.start_t)/CLOCKS_PER_SEC; 
    cout<<"Total time: "<<a.total_t<<endl;
     
    
    return 0;
}

void place_macro::v_file(int argc, char* argv[] ){
    ifstream infile(argv[1],ios::in);
    //ofstream outfile(argv[9], ios::out);
    string line;
    string macro_tempt;//暫存編號
    string pin_tempt;
    string wire_tempt;
    int counter1, counter2, stop;
    int length;
    int num;
    Data data_tempt;
    
    if (infile.is_open()){
        while ( getline (infile,line) ){
            //macro
            if(line.find("block")!=string::npos){
                //outfile<<"line : "<<line<<endl;
                counter1=0;
                counter2=0;
                stop=0;
                const char* d=" ";
                char tempt[line.length()+1];
                char *p;
                strcpy(tempt, line.c_str());
                p=strtok(tempt, d);
                while(p!=NULL){
                    
                    string savep;
                    savep.clear();
                    savep=string(p);
                    //cout<<savep<<endl;
                    
                    if(p[0]!='.' && p[0]!='b' && p[0]!='(' && p[0]!=')'){
                        macro_tempt=savep;
                        counter1=1;
                    }
                    length=savep.length();
                    
                    if(p[0]=='.'){
                        //cout<<"aaaaa: "<<length<<"  "<<savep[length-1]<<endl;
                        if(savep[length-1]==','){
                            //cout<<"!!!!!!!hahahahahah"<<endl;
                            pin_tempt=savep.substr(1, savep.find("(")-1);
                            wire_tempt=savep.substr(savep.find("(")+1, savep.length()-savep.find("(")-3);
                        }
                        else{
                            pin_tempt=savep.substr(1, savep.find("(")-1);
                            wire_tempt=savep.substr(savep.find("(")+1, savep.length()-savep.find("(")-2);
                        }
                        if(macro_tempt.length()!=0 && pin_tempt.length()!=0 && wire_tempt.length()!=0){counter2=1;}
                    }
                    if(p[0]==')' && p[1]==';' ){stop=1;}
                    if(counter1==1 && counter2==1 && stop!=1){
                        data_tempt.macro_name=macro_tempt;
                        data_tempt.pin_name=pin_tempt;
                        data_tempt.wire_name=wire_tempt;
                        wirelist.push_back(data_tempt);
                        //connectmap[wire_tempt].push_back(1);  

                        //cout<<macro_tempt<<"           "<<wire_tempt<<"            "<<pin_tempt<<endl;
                        //outfile<<"macro --> macro name : "<<macro_tempt<<"  --> wire name : "<<wire_tempt<<endl;
                        
                    }
                    
                    p=strtok(NULL, d);
                }
                //outfile<<endl<<endl;
            }
            //standard cell --> w / p m i o
            else if(line[0]!='w' && line[0]!='/' && line[0]!='p' && line[0]!='m' && line[0]!='i' && line[0]!='o' && line[0]!='e'){
                //cout<<"line : "<<line<<endl;
                counter1=0;
                counter2=0;
                stop=0;
                const char* d=" ";
                char tempt[line.length()+1];
                char *p;
                strcpy(tempt, line.c_str());
                p=strtok(tempt, d);
                while(p!=NULL){
                    string savep;
                    savep.clear();
                    savep=string(p);
                    //cout<<savep<<endl;
                    //cout<<"implement : "<<savep<<endl;

                    if(p[0]!='.' && p[0]!='(' && p[0]!=')'){
                        macro_tempt=savep;
                        counter1=1;
                    }
                    length=savep.length();
                    if(p[0]=='.'){
                        //cout<<"aaaaa: "<<length<<"  "<<savep[length-1]<<endl;
                        if(savep[length-1]==','){
                            //cout<<"!!!!!!!hahahahahah"<<endl;
                            pin_tempt=savep.substr(1, savep.find("(")-1);
                            wire_tempt=savep.substr(savep.find("(")+1, savep.length()-savep.find("(")-3);
                        }
                        else{
                            pin_tempt=savep.substr(1, savep.find("(")-1);
                            wire_tempt=savep.substr(savep.find("(")+1, savep.length()-savep.find("(")-2);
                        }
                        if(macro_tempt.length()!=0 && pin_tempt.length()!=0 && wire_tempt.length()!=0){counter2=1;}
                    }
                    if(p[0]==')' && p[1]==';' ){stop=1;}
                    if(counter1==1 && counter2==1 && stop!=1){
                        //cout<<"std cell --> std cell name : "<<macro_tempt<<"  --> wire name : "<<wire_tempt<<endl;
                        //connectmap[wire_tempt].push_back(0);   
                        num=connectmap[wire_tempt];
                        num++;
                        connectmap[wire_tempt]=num;
                        //cout<<macro_tempt<<"           "<<wire_tempt<<"            "<<pin_tempt<<endl;           
                    }
                    
                    p=strtok(NULL, d);
                }
            }
        }
        infile.close();
    }
    else if(!infile)
		cout<<".v could not be opened"<<endl;
}

void place_macro::lef_file( int argc, char* argv[] )
{
	ifstream infile(argv[2],ios::in);
	if (!infile)
		cout << ".lef could not be opened" << endl;

	Pin pins;
	Macro macros;
	string read;
	int version,microns,grid;
	string siteName,Class;
    double site_w, site_h;
    double rect_x1,rect_y1,rect_x2,rect_y2;

	while(read!="LAYER"){
		if(read=="VERSION")infile>>version;
		else if(read=="MICRONS")infile>>microns;
		else if(read=="MANUFACTURINGGRID")infile>>grid;
		else if(read == "SITE"){
            infile>>siteName;
            infile>>read>>site_w>>read>>site_h>>read;
            infile>>read>>Class;
    }
    infile>>read;
	}
	while(read!="MACRO"){      //read the standard cell directedly!!
		if(read=="LAYER")
			infile>>read;
		infile>>read;
	}
	while(read=="MACRO"){
		infile>>read>>read>>read;
		if(read=="CORE")
			while(read!="MACRO")
				infile>>read;
	}
	while(read!="LIBRARY"){
		if(read=="BLOCK")
			infile>>read>>read>>macros.type;
		else if(read=="SIZE"){
			infile>>macros.width>>read>>macros.height;
			vector<Macro>::iterator miter = macrolist.begin();
			for (miter; miter != macrolist.end(); ++miter)
    			if(miter->type==macros.type){
				  	miter->width=(macros.width)*microns;
    				miter->height=(macros.height)*microns;
    				miter->center_x=miter->x+(macros.width/2)*microns;
    				miter->center_y=miter->y+(macros.height/2)*microns;
			    }
	  }
		else if(read=="PIN"){
			while(read=="PIN"){
				infile>>pins.name;
				infile>>read;
				if(read=="DIRECTION")
          infile>>read;
				infile>>read;
				while(read!="OBS" && read!="END"){
					if(read=="RECT"){
						infile>>rect_x1>>rect_y1>>rect_x2>>rect_y2;
						pins.origin_x=calculate_center_pin_X(rect_x1,rect_x2);
						pins.origin_y=calculate_center_pin_Y(rect_y1,rect_y2);
            macros.pinlist.push_back(pins);
					}
					infile>>read;
				}
			}//while(read=="PIN")
      vector<Macro>::iterator miter = macrolist.begin();
      for (miter; miter != macrolist.end(); ++miter)
        if(miter->type==macros.type){
          miter->pinlist=macros.pinlist;
          vector<Pin>::iterator piter = miter->pinlist.begin();
          for (piter; piter != miter->pinlist.end(); ++piter){
            piter->x=miter->x+(piter->origin_x)*microns;
            piter->y=miter->y+(piter->origin_y)*microns;
          }

        }
		}
		if(read=="OBS")
		    macros.pinlist.clear();
		infile>>read;
	}
}

void place_macro::def_file( int argc, char* argv[] )
{
	ifstream infile(argv[3], ios::in);
	if (!infile)
		cout << ".def could not be opened" << endl;

	IOpad IO;
	char storage[100];
	int die_x = 0;
	int die_y = 0;
	char mark= '0';
	infile.getline(storage, 100);
	while (!(storage[0] == 'P' && storage[1] == 'I' && storage[2] == 'N' && storage[3] == 'S')) {
		infile.getline(storage, 100);
		if (storage[0] == 'E' && storage[1] == 'N' && storage[2] == 'D' && storage[4] == 'P' && storage[5] == 'R') {
			infile.getline(storage, 100, ')');
			infile.getline(storage, 100, '(');
			infile >> die_x >> die_y;
			infile.getline(storage, 100);
		}
	}
	chip_x_center = die_x / 2;
	chip_y_center = die_y / 2;
	infile >> mark;
	while (mark == '-') {
		infile >> IO.name;
		infile.getline(storage, 100, 'T');
		infile >> IO.netname;
		infile.getline(storage, 100, 'N');
		infile >> IO.type;
		infile.getline(storage, 100, '(');
		infile >> IO.x >> IO.y;
		infile.getline(storage, 100);
		infile.getline(storage, 100, ';');
		IOpadlist.push_back(IO);
		infile >> mark;
	}
}

void place_macro::mlist_file( int argc, char* argv[] )
{
	ifstream infile(argv[4],ios::in);
	if(!infile)
		cout<<".mlist could not be opened"<<endl;

	Macro macro_tempo; //暫時儲存資料的struct，方便把資料塞進vector
	string sentence;
	short int number = 0;
	short int num_for_symbols = 0 ; //解決前面()影響後面判斷的問題
	short int num_die = 0;
  string store;
	int x1 , x2;
	int y1 , y2; //record the position of die (x1,y1) & (x2,y2)
  //int component = 0; //macro總個數(改放入class)
  infile>>store>>version>>store;
  infile>>store>>design>>store;
  infile>>store>>store>>store>>UDM>>store;
  /*getline(infile,store);
  getline(infile,store);
  getline(infile,store);*/
	while(infile>>sentence){
		if(sentence=="-"){
			number = 1;
		}
		else if(sentence=="+" || sentence=="(" || sentence==")"){
			if(num_die==0){ //吃到第一個(
				num_die++;
				number++;
			}
			else if(num_die==1 && sentence=="(" ){
				num_die++;
				number = 1;
			}
			else
				number++;
		}
		//取得macro個數
		else if(sentence=="COMPONENTS" && component == 0){
			component = 1;
		}
		else if(component == 1){
			component = string_to_int(sentence);
			//cout << "component = " << component << endl; -
		}
		//可以修成case
		else if(number == 1){ //存macro_name
			if(num_die==1){
				x1 = string_to_int(sentence);
				//cout << "number = 1 , num_die = 1 , x1 = " << x1 << endl;
				number++;
			}
			else if(num_die==2){
				x2 = string_to_int(sentence);
				//cout << "number = 1 , num_die = 2 , x2 = " << x2 << endl;
				number++;
			}
			else{
				if(num_die==3 || num_die==4){
					num_die++;
					continue;
				}
				else{
					//cout << "number = 1, macro_name = " << sentence << endl;
					macro_tempo.name = sentence;
					number++;
				}
			}
		}
		else if(number == 2){ //存macro_type
			if(num_die==1){
				y1 = string_to_int(sentence);
				//cout << "number = 2 , num_die = 1 , y1 = " << y1 << endl;
				number++;
			}
			else if(num_die==2){
				y2 = string_to_int(sentence);
				//cout << "number = 2 , num_die = 2 , y2 = " << y2 << endl;
				number = 0;
				num_die++;
			}
			else{
				//cout << "number = 2, macro_type = " << sentence << endl;
				macro_tempo.type = sentence;
			}
		}
		else if(number == 3){
			//cout << "number = 3, macro_fixed = " << sentence << endl;
			if(sentence=="PLACED")
				macro_tempo.fixed = 0;
			else
				macro_tempo.fixed = 1;

		}
		else if(number == 4){
			//cout << "number = 4, macro_position = " << sentence;
			macro_tempo.x = string_to_int(sentence);
			number++;
		}
		else if(number == 5){
			//cout << " " << sentence << endl;
			macro_tempo.y = string_to_int(sentence);
		}
		else if(number == 6){
			//cout << "number = 6, macro_direction = " << sentence << endl;
			//只吃input的話都是N
			if(sentence=="N")
				macro_tempo.orient = 0;
			else if(sentence=="FN")
				macro_tempo.orient = 1;
			else if(sentence=="S")
				macro_tempo.orient = 2;
			else if(sentence=="FS")
				macro_tempo.orient = 3;

			macrolist.push_back(macro_tempo);
			number++;
		}
		else{
			number = 0;
		}
	}
 
 //檢查macro位置資訊
 vector<Macro>::iterator miter = macrolist.begin();
  for (miter; miter != macrolist.end(); ++miter){
    //cout<<fixed<<miter->x<<" "<<miter->y<<endl;
    //cout<<miter->orient<<endl;
  }
 
}

void place_macro::txt_file(int argc, char* argv[] ){
  ifstream infile(argv[5],ios::in);
  string store;
  int d;
  infile>>store>>d;
  displacement_max = d*UDM;
}

float place_macro::calculate_center_pin_X(double rect_x1,double rect_x2){
	float center_pin_x;
	center_pin_x=(rect_x1+rect_x2)/2;
	return center_pin_x;
}

float place_macro::calculate_center_pin_Y(double rect_y1,double rect_y2){
	float center_pin_y;
	center_pin_y=(rect_y1+rect_y2)/2;
	return center_pin_y;
}
 
int place_macro::string_to_int(const string input_sentence){
	char p[input_sentence.length()] ;
	int i ;
	int output_int;

    if(input_sentence.length()==1){
    	p[1]=0;
	}
	for (i = 0; i < sizeof(p); i++) {
		p[i] = input_sentence[i];
		//cout << "*"<<p[i]<<"*";
    }
    output_int = atoi(p);
    return output_int;
}

void place_macro::draw_rectangle_connection(int argc, char* argv[]){
    
    ofstream outputfile(argv[6],ios::out);

    map<string , vector<Data> >:: iterator wiremap_iter ;
    for(auto wiremap_iter=wiremap.begin() ; wiremap_iter!=wiremap.end(); ++wiremap_iter){ //跑過所有的線
        
        if(wiremap_iter->second.size()!=1){
            outputfile<<"Wire name:"<<wiremap_iter->first<<endl;
            //cout<<"Wire name:"<<wiremap_iter->first<<endl;
        
            vector<Data>:: iterator veciter=wiremap_iter->second.begin(); //跑正在看的線的所有成員macro Data
            for(veciter;veciter!=wiremap_iter->second.end();++veciter){
                
                //cout<<"veciter->macro_name: "<<veciter->macro_name<<endl;
                
                vector<Macro>::iterator miter=macrolist.begin();
                for(miter; miter!=macrolist.end(); ++miter){
                    if(veciter->macro_name==miter->name){
                        outputfile<<"rectangle('Position',[ ";
                        outputfile<<miter->x<<" "<<miter->y<<" ";
                        outputfile<<miter->width<<" "<<miter->height<<" ], 'EdgeColor','r' )"<<endl;
                        
                        //cout<<"rectangle('Position',[ ";
                        //cout<<miter->x<<" "<<miter->y<<" ";
                        //cout<<miter->width<<" "<<miter->height<<" ], 'EdgeColor','r' )"<<endl;
                    }
                }
            }
            outputfile<<endl<<endl<<endl;
            //cout<<endl<<endl<<endl;
        
        }
    }
    
    outputfile<<"rectangle('Position',[ 0 0 "<<chip_x_center*2<<" "<<chip_y_center*2<<" ])"<<endl; //chip外框
    outputfile<<"axis equal tight";  //讓x y軸的比例相同
    
    
}






