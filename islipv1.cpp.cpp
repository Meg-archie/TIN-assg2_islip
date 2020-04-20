#include<bits/stdc++.h>
using namespace std;


//terminate_cond to keep count of rounds
int terminate_cond=0;
int flag=1,cnt=0;


map<int,int> markout; //to keep a list of requested outputs
vector<int> input_granted; //within rounds
vector<int> output_granted;
// requested input
//run for defualt, to be assigned later with "packets" vector data
vector<vector<int>> values{{0,1,2},{0,1,2},{0,2,3}};
vector<vector<int>> v1,v2;
//vector<vector<int>> values;
//run for default vector
vector<int> ioqueue(4,-1);
    
//initial output vector
vector<int> initial_output;
vector<int> initial_input;


//iteration i+1 phases
vector<int> leftoutput;
vector<int> leftinput;
 

//remove duplicate from vector
void remove(std::vector<int> &v)
{
    std::unordered_set<int> s;
    auto end = std::copy_if(v.begin(), v.end(), v.begin(), [&s](int const &i) 
    {
        return s.insert(i).second;
    });

    v.erase(end, v.end());
}


//initial accept and grant phase for iteration 1 
void first_iteration()
{
    for(int i=0;i<(int)v1.size();i++)
    {    
      while(v1[i].size()!=0)
        {
            int keyval= v1[i].front();
            v1[i].erase(v1[i].begin());
            if(markout.count(keyval)==0 && ioqueue[i]==-1)
            {   
                
                ioqueue[i]=keyval;
                //cout<<"7"<<endl;
                output_granted.push_back(keyval);
                //cout<<"8"<<endl;
            }
            markout[keyval]=1;
           
        }
    }

}



// iteration phase for more than one iteration in any round

void subsequent_iterations1()
{
    if( !leftoutput.empty())
    {   //cout<<"5b"<<endl;
        while(flag && !leftinput.empty())
        {  //cout<<"5c"<<endl;
            int elem=leftinput.front();
            //cout<<(int)v2[elem].size()<<endl;
            while(!v2[elem].empty())
            {     //cout<<"6a"<<endl;
                int key=v2[elem].front();
                v2[elem].erase(v2[elem].begin());
                if(leftoutput.front()==key || std::find(leftoutput.begin(), leftoutput.end(),key)!=leftoutput.end())
                {   //cout<<"6b"<<endl;
                    if(markout[key]==0 && ioqueue[elem]==-1)
                         {    //cout<<"7"<<endl;
                   

                            ioqueue[elem]=key;
                            input_granted.push_back(elem);
                            output_granted.push_back(key);

                            auto it = std::find(leftoutput.begin(), leftoutput.end(), key);
                            if(it != leftoutput.end())
                                 leftoutput.erase(it);

                             auto it1 = std::find(leftinput.begin(), leftinput.end(), elem);
                            if(it1 != leftinput.end())
                                 leftinput.erase(it1);

                        }
                       
                } markout[key]=1;
            }

             for(auto t: markout)
                if(t.first==1)
                  cnt++;

              if(cnt==4)flag=0;
              else flag=1;
            flag=0;
        }


    }
}


// within round for more than one iterations
void reinitialise_map_outputvalues()
{
    for(int i:leftoutput)
       if(markout[i])
            markout.erase(i);

}
// print of requested packets
void print()
{
    for(int i=0;i<(int)values.size();i++)
        if(values[i].size()!=0)
        {
            for(int j=0;j<(int)values[i].size();j++)
                cout<<values[i][j]<<" ";
            initial_input.push_back(i);
            cout<<endl;
        }

}
// within rounds: from one round to another
void change_req()
{
    for(int i=0;i<(int)values.size();i++)
    {         
        if(!values[i].empty() && ioqueue[i]!=-1)
        {
            int k=ioqueue[i];            
            if(values[i].front()==k || std::find(values[i].begin(), values[i].end(),k)!=values[i].end())
            {   
                auto it = std::find(values[i].begin(), values[i].end(), k);
                            if(it != values[i].end())
                                 values[i].erase(it);
            }
        }
    }
}
//check when all rounds end
void check_termination()
{
 for(int i=0; i<(int)values.size();i++)
     {
        if(values[i].empty())
            terminate_cond*=1;
         else terminate_cond=0;
     }
}

int main()
{	
	//values will have all requests pf packets
	/*
	i/p   	o/p
	1-> 	1,3,4
	2->		1,2
	3->		3,4
	4->		1,2
	*/
	values=packets;
    // display input request    
    print();   
    

	v1=values;
    v2=v1;
    ioqueue.reserve(1000);
    initial_output.reserve(1000);
        initial_input.reserve(1000);
        input_granted.reserve(1000);
        output_granted.reserve(1000);
        leftinput.reserve(1000);
        leftoutput.reserve(1000);
        //markout.reserve(1000);    


    //count on rounds
    int round=0;
    
   //rounds strt here
    while(!terminate_cond)
    {
        round++;
        // round i iteration 1       
        first_iteration();   

        //initialise input_granted after grant phase and accept phase
       for(int i=0;i<(int)ioqueue.size();i++)
        if(ioqueue[i]!=-1)
            input_granted.push_back(i);
         //from map to initialoutput
         for(auto elem : markout){
            if(elem.second==1)
            initial_output.push_back(elem.first);
        }

         

    //iteration >1

        std::set_difference(initial_output.begin(), initial_output.end(), output_granted.begin(), output_granted.end(),std::inserter(leftoutput, leftoutput.begin()));
     
        std::set_difference(initial_input.begin(), initial_input.end(), input_granted.begin(), input_granted.end(),std::inserter(leftinput, leftinput.begin()));
   
        int iter=2;
        while(!leftoutput.empty())
        {    
     
            flag=1;

            reinitialise_map_outputvalues();          
             //further_iterations();             
           
             subsequent_iterations1();  
            
                  
             if(leftoutput.empty()){break;}
             else
             std::set_difference(initial_output.begin(), initial_output.end(), output_granted.begin(), output_granted.end(),std::inserter(leftoutput, leftoutput.begin()));


             if(leftinput.empty()) {break;}
             else
             {
             std::set_difference(initial_input.begin(), initial_input.end(), input_granted.begin(), input_granted.end(),std::inserter(leftinput, leftinput.begin()));
             }
              if(iter==3) return 0;
              
             remove(leftinput);
             remove(leftoutput);
             iter++;
             
             cout<<"moving out "<<endl;


        }
         change_req();
         print();
         check_termination();
         initial_output.erase(initial_output.begin(), initial_output.end());
         initial_output.shrink_to_fit();
         initial_input.erase(initial_input.begin(), initial_input.end());
         initial_input.shrink_to_fit();
        
        /*input_granted.clear();
        output_granted.clear();
        leftinput.clear();
        leftoutput.clear();
        markout.clear();
        */
       
        input_granted.shrink_to_fit();
        output_granted.shrink_to_fit();
        leftinput.shrink_to_fit();
        leftoutput.shrink_to_fit();
        //markout.shrink_to_fit();;

        }    
        cout<<"rounds "<<round<<endl;
	return 0;
}