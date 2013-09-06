#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <string.h>
#include <cmath>
#include <stack>
#include <limits.h>
#include <set>
#include <stdlib.h>
using namespace std;
/*
                                           Viterbi Program

INPUT: requires two command line arguments  first being the training set  second testing set (data is supposed to clean and with no blank lines)
OUTPUT:tree and classification result on self and test case
Class and Method description:
 class ReadFromFile:
                A basic file handler stores records for future access into a 2D vector named traininglist
                buildRecord() does the above function
                displayList() displays the list
                getUnique() was  just for testing and future use purposes
  class Result for storing and backtracking results;
  class Viterbi
            implementation() implements viterbi algo
            backtrack() finds solution by backtracking
            findIndex() finds index of token for internal processing


*/
//Filehandler for the ID 3
class ReadFromFile
{


public:
    vector< double> initial_prob;// To hold the initial prob
    int no_of_states;// no of states
    vector<vector <double>  >transition_prob;//transition
    int no_of_output;//no of output symbols
    vector < char > op;// output symbols need to be char
    vector<vector <double> > output_distributions;

    ReadFromFile()
    {
        //initialization of unique values
        no_of_states=0;
        no_of_output=0;
    }
    void buildRecord(char * path)
    {
        ifstream fp (path);

        if (!fp.is_open()) //
            cout<< "Error in the file path";
        else
        {
            string token;
            fp>>token;
            no_of_states=atoi(token.c_str());
            for(int i =0; i<no_of_states; i++)
            {
                fp>>token;
                double temp;
                temp=atof(token.c_str());
                if (temp!=0)
                    initial_prob.push_back(log2(temp));
                else
                    initial_prob.push_back((temp));
            }
            for (int i=0; i<no_of_states; i++)
            {
                vector <double> temp_vector;
                for (int j=0; j<no_of_states; j++)
                {
                    token.clear();
                    fp>>token;
                    double temp;
                    temp=atof(token.c_str());
                    // cout<<temp;
                    if (temp!=0)
                        temp_vector.push_back(log2(temp));
                    else
                        temp_vector.push_back((temp));
                }
                // cout <<endl;
                transition_prob.push_back(temp_vector);
            }
            fp>>token;
            no_of_output=atoi(token.c_str());
            //   cout<<no_of_output;
            for(int i=0; i<no_of_output; i++)
            {
                fp>>token;
                //      cout<<token[0]<<endl;
                op.push_back(token[0]);
            }
            for (int i=0; i<no_of_states; i++)
            {
                vector <double> temp_vector;
                for (int j=0; j<no_of_output; j++)
                {
                    token.clear();
                    fp>>token;
                    double temp;
                    temp=atof(token.c_str());
                    //   cout<<temp;
                    if (temp!=0)
                        temp_vector.push_back(log2(temp));
                    else
                        temp_vector.push_back((temp));
                }
                //    cout <<endl;
                output_distributions.push_back(temp_vector);
            }



        }



    }


};
class Result
{
public:
    double val;
    int from;

};

class Viterbi
{
public:
    ReadFromFile rd;
    vector <vector <int> > result;
    vector <vector <Result> > fin_result;
    Viterbi(ReadFromFile r)
    {
        rd=r;

    }
    void implementation(char * path)
    {
        string line;
        ifstream fp (path);
        if (!fp.is_open()) //
            cout<< "Error in the file path";
        else
        {
            cout<<"-----------INPUT-----------"<<endl;
            while(getline(fp,line))
            {
                cout<<endl;
                vector <int>  temp_vector;
                istringstream iss(line);
                string token;
                while (iss>>token)
                {
                    cout<<token;
                    temp_vector.push_back(findIndex(token[0]));
                }
                result.push_back(temp_vector);
                cout<<endl;
            }
            vector<vector <int> >::iterator p= result.begin();
            while(p!=result.end())
            {
                int counter=0;

                fin_result.clear();
                Result transfer;
                transfer.from=0;
                transfer.val=0;
                vector<Result> norm;
                for (int i=0; i<(*p).size(); i++)
                    norm.push_back(transfer);
                for(int i=0; i<rd.no_of_states; i++)
                    fin_result.push_back(norm);
                vector<int>::iterator q=(*p).begin();
                while(q!=(*p).end())
                {

                    if (counter==0)
                    {
                        double sum=0;
                        //    cout<<"check 3 1  "<<rd.output_distributions[2][1]<<endl;
                        for (int i=0; i<rd.no_of_states; i++)
                        {
//            cout<<"check "<<rd.output_distributions[i][(*q)]<<endl;
//cout<<"------Initial-------"<<endl;
                            sum=rd.initial_prob[i]+rd.output_distributions[i][(*q)];
                            vector<Result> temp_result;
                            fin_result[i][0].from=i;
                            fin_result[i][0].val=sum;
                            //    cout<<"Saumil checking "<<fin_result[i][0].val<<"At"<<i<<endl;
                            //    getchar();

                            //      cout<<sum<<"for"<<(*q)<<endl;

                        }
                    }
                    else
                    {
                        // cout<<fin_result.size()<<endl;
                        for (int i=0; i<rd.no_of_states; i++)
                        {
                            double sum=0;
                            double max=INT_MIN;
                            int pos=0;
                            for (int j=0; j<rd.no_of_states; j++)
                            {
                                //       cout<<j<<" "<<counter-1<<"and"<<j<<" "<<i<<endl;
                                //                        cout<<"check in"<<endl;

                                sum=fin_result[j][counter-1].val+rd.transition_prob[j][i];
                                //              cout<<"check out"<<fin_result[j][counter-1].val<<"   "<<rd.transition_prob[j][i]<<endl;
                                //         cout<<sum<<endl;
                                if (sum>max)
                                {
                                    max=sum;
                                    pos =j;
                                }
                            }

                            fin_result[i][counter].from=pos;
                            //////////cout<<"max "<<max<<"at "<<pos<<endl;
                            fin_result[i][counter].val=max+rd.output_distributions[i][*q];
                            //           cout<<"fin result "<<fin_result[i][counter].from<<"at value"<<fin_result[i][counter].val<<i<<" "<<counter<<" "<<rd.output_distributions[i][*q]<< " "<<max<<endl;
                            //            getchar();
                        }
                    }

                    //cout<<*q;
                    q++;
                    counter++;
                    // getchar();
                }
                cout<<"-------------------Result---------------------"<<endl;
                double maximum=0;
                double last_pos=0;
                stack <int> res;
                for(int i=0; i<rd.no_of_states; i++)
                {
                    if (maximum<fin_result[i][(*p).size()-1].val)
                    {
                        last_pos=i;
                        maximum=fin_result[i][(*p).size()-1].val;
                    }

                }
                res.push(last_pos);
                backtrack(last_pos,(*p).size()-1,res);
                //  cout<<endl<<last_pos<<"listen to sauumil"<<endl;
                //getchar();
                p++;
            }



        }


    }
    int findIndex(char token)
    {
        for (int i=0; i< rd.op.size(); i++)
        {
            if(rd.op[i]==token)
                return i;
        }
    }
    void backtrack(int index,int pos,stack <int> &res)
    {
        //cout<<"Here";
        if (pos==0)
        {
            while(!res.empty())
            {
                cout<<res.top();
                res.pop();
            }
            cout<<endl;
            return;
        }
        res.push(fin_result[index][pos].from);
        backtrack(fin_result[index][pos].from,pos-1,res);



    }
};

int main(int argc, char* argv[])
{
    ReadFromFile m;
    m.buildRecord(argv[1]);
    Viterbi v(m);
    v.implementation(argv[2]);
    return 0;
}
