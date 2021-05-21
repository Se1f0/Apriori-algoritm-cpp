#include <iostream>
#include <math.h>
#include <string>
#include <map>
#include <iterator>
#include <bits/stdc++.h>
using namespace std;


string numberToAlpha(int number){
    int s=0;                        //n=input number,s=for comparison
    int i=0;                          //i=finding no of chars to print
    while(s<number)
    {   
        i++;                          //this loop finds no.of chars to print
        s=s+(int)pow(26,i);
    }  
    int del=0;                        
    for(int j=0;j<i;j++)
    {
        del=del+(int)pow(26,j);       //del=finds the value to be removed from n
    }
    number=number-del;                          //n is reassigned to print particular alphabet in identified no.of digits
    string p;                         //p=stores the alphabets
    for(int j=0;j<i;j++)
    {
        char c=(char)((number%(26))+65);   //finds the char for resultant number
        p=c+p;                        //concatenates the alphabets in reverse order
        number=number/26;                       //each time n is divided by 26
    }
    return p;
}

void printVector(vector<string> vect){
    for (auto& v : vect)
    {
        cout << v << ' ';
    }
    cout << endl;
}

void printCtx(map<string,vector<string>>& ctx){
    map<string, vector<string>>::iterator it;
    for(it=ctx.begin(); it!=ctx.end(); ++it){
        cout << it->first << " : ";
        printVector(it->second);
    }
    cout << endl;
}

void printItemset(map<string,double>& itemset,double minsup){
    map<string, double>::iterator it3;
    string stat = "frequent";
    for(it3=itemset.begin(); it3!=itemset.end(); ++it3){
        if (it3->second < minsup)
        {
            stat = "Not frequent";
        }
        else
        {
            stat = "frequent";
        }
        cout << it3->first << " : " << it3->second*100 << "% -> " << stat << endl;
    }
    cout << endl;
}

void printBinaryMat(vector<vector<int>> bin){
    for (int i = 0; i < bin.size(); i++)
    {
        for (int j = 0; j < bin[i].size(); j++)
        {
            cout << bin[i][j] <<  ' ';
        }
        cout << endl;
    }
    cout << endl;
}


int getIndex(vector<string> v, char K)
{
    string s(1,K);
    int index = -1;
    auto it = find(v.begin(), v.end(), s);
    if (it != v.end())
    {
        index = it - v.begin();
    }
    return index;
}

class AssociationRule {
    private:
        string x;
        string y;
        double conf;
    public:
        AssociationRule(string x0,string y0,double supX,double supXY){
            x = x0;
            y = y0;
            conf = supXY / supX;
        }
        string getX() {
            return x;
        }
        string getY() {
            return y;
        }
        void setX(string x) {
            x = x;
        }
        void setY(string y) {
            y = y;
        }
        void printAssociationRule() {
            cout << x << "->" << y << endl << "Confidence : " << conf << endl;
        }
};

int main(){
    cout <<"How many transactions you have ?\n==>";
    int num_tra;
    cin >> num_tra;
    vector<string> tr;
    for (int i = 1; i <= num_tra; i++)
    {
        tr.push_back("T"+to_string(i));    
    }

    cout << "You transactions are :";
    printVector(tr);

    cout <<"How many products you have ?\n==>";
    int num_pr;
    cin >> num_pr;
    vector<string> pr;

    for (int i = 1; i <= num_pr; i++)
    {
        pr.push_back(numberToAlpha(i));    
    }

    cout << "Your products are :";
    printVector(pr);

    cout << "Enter for each transaction the correspending product(if there is many use comma to separate them):\n";
    map<string,vector<string>> ctx;
    for(auto& t : tr)
    {
        string in;
        vector<string> temp;
        cout << t <<":";
        cin >> in;
        for (auto& c : in)
        {
            if (isalpha(c))
            {
                string s(1,c);
                temp.push_back(s);
            }   
        }
        ctx.insert(make_pair(t,temp));   
    }

    cout << "\nHorizental representation of the context:" << endl;
    printCtx(ctx);
    
    map<string, vector<string>>::iterator it = ctx.begin();
    vector<vector<int>> bin;
    
    for (int i = 0; i < tr.size(); i++)
    {
        vector<int> v;
        for (int j = 0; j < pr.size(); j++)
        {
            vector<string>::iterator it2 = find(it->second.begin(),it->second.end(),pr[j]);
            if (it2 != it->second.end())
            {
                v.push_back(1);
            }
            else
            {
                v.push_back(0);
            }
        }
        bin.push_back(v);
        ++it;
    }

    cout << "Binary representation of the context:" << endl;
    printBinaryMat(bin);

    cout << "Enter your minsup in % :";
    double minsup;
    cin >> minsup;
    minsup = minsup / 100;
    cout << "Minsup = " << minsup << endl << endl;

    vector<map<string,double>> itemsets;
    map<string,double> itemset;
    vector<string> inf;
    map<string,double> fre;
    int level = 1;
    double sup = 0;

    for (int j = 0; j < pr.size(); j++)
    {
        sup = 0;
        for (int i = 0; i < tr.size(); i++)
        {
            sup+=bin[i][j];
        }
        itemset.insert(make_pair(pr[j],sup/tr.size()));
        if (sup/tr.size() < minsup)
        {
            inf.push_back(pr[j]);
        }
        else
        {
            fre.insert(make_pair(pr[j],sup/tr.size()));
        }
    }
    int index = fre.size();

    cout<<"itemset-"<<level<<endl;
    printItemset(itemset,minsup);
    itemsets.push_back(itemset);
    level++;
    itemset.clear();

    map<string, double>::iterator curr = fre.begin();
    map<string, double>::iterator it3;
    string item;
    vector<int> idx;

    for (curr;curr!=fre.end();++curr)
    {
        item = curr->first;
        it3 = curr;
        advance(it3,1);
        for (it3;it3 != fre.end();++it3)
        {
            sup = 0;
            item = item + it3->first;
            for (auto& c : item)
            {
                idx.push_back(getIndex(pr,c));
            }
            for (int i = 0; i < bin.size(); i++)
            {
                int ii = 0;
                if (bin[i][idx[ii]] == 1 && bin[i][idx[ii+1]] == 1)
                {
                    sup++;
                    // if (sup/tr.size() < minsup)
                    // {
                    //     inf.push_back(item);
                    // }
                    // else
                    // {
                    //     fre.insert(make_pair(item,sup/tr.size()));
                    // }
                }
            }
            itemset.insert(make_pair(item,sup/tr.size()));
            idx.clear();
            item = curr->first;
        }
    }

    cout<<"itemset-"<<level<<endl;
    printItemset(itemset,minsup);

    return 0;
}