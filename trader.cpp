#include "receiver.h"
#include <vector>
#include <string>
#include <iostream>
#include <algorithm>
#include "sort.h"
#include "Binaries.h"

using namespace std;

bool cancellation = false;
bool cancelledd1 = false;
pair<int,int> quantities = {0,0};
pair<int,int> indices = {0,0};

template <typename T>
void popRandomElement(std::vector<T>& vec, int randomIndex) {
        // Shift all elements after the removed element one unit back
        for (int i = randomIndex; i < vec.size()-1; i++) {
            vec[i] = vec[i + 1];
        }
        // Resize the vector to remove the last element (which is now a duplicate)
        vec.resize(vec.size() - 1);
}

bool cancellation_predictor(vector<int>order_numbers, vector<vector<pair<string,int>>> All_quantities, vector<char> type){
    int i = order_numbers.back();
    if(type[i] == 's'){
        for(int j = 0; j < All_quantities[i].size() ; j++){
            All_quantities[i][j].second *= -1;
        }
    }
    order_numbers.pop_back();
    for(auto it : order_numbers){
        if(type[it]=='b'){
            if(All_quantities[i]==All_quantities[it]){ return true; }
        }
        else{
            for(int j = 0; j < All_quantities[it].size() ; j++){
                All_quantities[it][j].second *= -1;
            }
            if(All_quantities[i]==All_quantities[it]){ return true; }
        }
    }
    return false; 
    
}

bool arbitrage_predictor(vector<int> order_numbers, vector<vector<pair<string,int>>> All_quantities){

    vector<pair<string,bool>> Predictor ;
    for(auto it : All_quantities[order_numbers.back()]){
        if(it.second > 0){
            Predictor.push_back({'1'+it.first,false});
        }else{
            Predictor.push_back({'0'+it.first,false});
        }
    }
    
    for(auto it : order_numbers){
        for(auto pt : All_quantities[it]){
            for(int i = 0 ; i < Predictor.size() ; i++){
                if(Predictor[i].first == '1' + pt.first ){
                    if(pt.second < 0){ Predictor[i].second = true; }
                }
                if(Predictor[i].first == '0' + pt.first){
                    if(pt.second > 0){ Predictor[i].second = true; }
                }
            }
        }
    }

    bool ans = true;
    for(auto it : Predictor){
        ans = ans&it.second;
    }
    return ans;
}

bool cancellation_predictor2(vector<int> order_numbers,vector<vector<pair<string,int>>> All_quantities, vector<int> Priceperstock, vector<char> type){
    int i = order_numbers.back();
    if(type[i] == 's'){
        for(int j = 0; j < All_quantities[i].size() ; j++){
            All_quantities[i][j].second *= -1;
        }
        Priceperstock[i] *= -1;
    }
    order_numbers.pop_back();
    for(auto it : order_numbers){
        if(type[it]!=type[i]){
            if(type[it]=='b'){
                if(All_quantities[i]==All_quantities[it] && Priceperstock[it]==Priceperstock[i]){ return true; }
            }
            else{
                for(int j = 0; j < All_quantities[it].size() ; j++){
                    All_quantities[it][j].second *= -1;
                }
                Priceperstock[it] *= -1;
                if(All_quantities[i]==All_quantities[it] && Priceperstock[it]==Priceperstock[i]){ return true; }
            }
        }
    }
    return false;
}
int converter(string num){
    int i = 0;
    bool sign = true;
    int result = 0;
    if(num[i]=='-'){
        sign = false;
        i++;
    }
    for(int j = i; j < num.size(); j++){
        int counter = num[j] - '0';
        if(10>counter&&counter>=0)result = result*10 + counter;
    }
    if(!sign){ result *= -1; return result;}
    return result;
}
// Will see later

bool check_quantities(vector<pair<string,int>> &order_code_Quantitycounter){
    for(auto it : order_code_Quantitycounter){
        if(it.second!=0){
            return false;
        }
    }
    return true;
}

void append_order_code(vector<pair<string,int>> &order_code_Quantitycounter, vector<pair<string,int>> Quantitycounter, int quantity){
    
    for(auto it : Quantitycounter){
        string stock_name = it.first;
        int Quantity = it.second*quantity;
        bool isPresent = false;
        for(int i = 0; i<order_code_Quantitycounter.size(); i++){
            if(order_code_Quantitycounter[i].first==stock_name){
                isPresent = true;
                order_code_Quantitycounter[i].second += Quantity;
                break;
            }
        }
        if(!isPresent){
            order_code_Quantitycounter.push_back({stock_name,Quantity}); 
        }
    }
}

void process_raworder(string order, vector<pair<string,int>> &QuantityCounter, int &Quantity, vector<int> &Priceperstock){  
    int X = 0;
    while(order[X]!='\0'){
        if(order[X]==' ' && order[X+1]==' '){
            order.erase(order.begin()+X);
            X--;
        }
        X++;
    }
    int i = 0;
    int j = 0;
    bool gap = false;
    string stock_name;
    string quantity;
    while(true){
        char x = order[i];
        if(x==' '){
            if(gap){
                quantity = order.substr(j,i-j);
                int integer_quantity = converter(quantity);
                QuantityCounter.push_back({stock_name,integer_quantity});
                j = i+1;
                gap = false;
                if(order[i+1]>='1' && order[i+1]<='9' || order[i+1]=='-'){
                    int k = i+1;
                    while(order[k]!=' '){
                        k++;
                    }
                    k++;
                    int p = k;
                    while(order[k]!=' '){
                        k++;
                    }
                    Quantity = converter(order.substr(p,k-p));
                    p--;
                    k = p;
                    if(order[order.size()-1]=='b'){ 
                        Priceperstock.push_back(converter(order.substr(i+1,k-i-1))); 
                        return;
                    }
                    else{
                        for(int pt = 0; pt < QuantityCounter.size(); pt++){
                            int quantity = QuantityCounter[pt].second;
                            quantity *= -1;
                            QuantityCounter[pt].second = quantity; 
                        }
                        Priceperstock.push_back(-1*converter(order.substr(i+1,k-i-1))); 
                        return;
                    }
                }
            }else{
                stock_name = order.substr(j,i-j);
                j = i+1;
                gap = true;
            }
        }
        i++;
    }
    
}

void process_order(string order,vector<pair<string,int>> &Quantitycounter,vector<int> &Totalpricecounter){

    int X = 0;
    while(order[X]!='\0'){
        if(order[X]==' ' && order[X+1]==' '){
            order.erase(order.begin()+X);
            X--;
        }
        X++;
    }
    string counter = order;     // Store a counter of the order
    int i = 0;                  // Index
    int j = 0;                 
    bool gap = false;
    string stock_name;
    string quantity ;
    while(true){
        char x = order[i];
        if(x==' '){
            if(gap){
                quantity = order.substr(j,i-j);
                int integer_quantity = converter(quantity);
                Quantitycounter.push_back({stock_name,integer_quantity});
                j = i+1;
                gap = false;
                if(order[i+1]>='1' && order[i+1]<='9' || order[i+1]=='-'){ 
                    int Price;
                    if(order[0]>='0' && order[0]<='9'){
                        int k = i+1;
                        while(order[k]!=' '){
                            k++;
                        }
                        Price = converter(order.substr(i+1,k-i-1));
                    }
                    else{ 
                        Price = converter(order.substr(i+1,order.size()-i-3));  }
                    if(order[order.size()-1]!='b'){ 
                        for(int pt = 0; pt < Quantitycounter.size(); pt++){
                            int Quantity = Quantitycounter[pt].second;
                            Quantity *= -1;
                            Quantitycounter[pt].second = Quantity; 
                        }
                        Totalpricecounter.push_back(-1*Price);
                    }
                    else{ Totalpricecounter.push_back(Price); }
                    break; 
                }
            }else{
                stock_name = order.substr(j,i-j);
                j = i+1;
                gap = true;
            }
        }
        i++;
    }
    return;
}

bool is_there_cancellation2(int i1, char type1,  int i2, char type2, vector<vector<pair<string,int>>> All_quantites, vector<int> Priceperstock, int q1, int q2){
    if(type1==type2){ return false; }
    if(type1!=type2 && Priceperstock[i1]+Priceperstock[i2]!=0){ return false; }
    if(All_quantites[i1].size()!=All_quantites[i2].size()){
        return false;
    }
    for(int i = 0; i <All_quantites[i1].size(); i++){
        if(All_quantites[i1][i].first!=All_quantites[i2][i].first || All_quantites[i1][i].second+All_quantites[i2][i].second!=0){ return false; }
    }
    if(q1>=q2){
        cancellation = true;
    }
    indices.first = i1;
    indices.second = i2;
    return true;  
}

bool is_there_cancellation(int i1, char type1,  int i2, char type2, vector<vector<pair<string,int>>> All_quantites, vector<int> Priceperstock, vector<int> &order_numbers){
    if(type1 != type2){
        if(All_quantites[i1].size()!=All_quantites[i2].size() || Priceperstock[i1]+Priceperstock[i2]!=0){ return false; }
        for(int i = 0 ; i < All_quantites[i1].size() ; i++){
            if(All_quantites[i1][i].first!=All_quantites[i2][i].first || All_quantites[i1][i].second+All_quantites[i2][i].second!=0){ return false; }
        }
        order_numbers.erase(remove(order_numbers.begin(),order_numbers.end(),i1),order_numbers.end());
        order_numbers.erase(remove(order_numbers.begin(),order_numbers.end(),i2),order_numbers.end());
        return true; 
    }
    if(All_quantites[i1]!=All_quantites[i2]){ return false; }
    if(type1=='b' && type2=='b'){
        if(Priceperstock[i1] >= Priceperstock[i2]){  
        order_numbers.erase(remove(order_numbers.begin(),order_numbers.end(),i2),order_numbers.end());
        return true; 
        }else{
        order_numbers.erase(remove(order_numbers.begin(),order_numbers.end(),i1),order_numbers.end());        
        }
    }
    if(type1=='s' && type2=='s'){
        if(Priceperstock[i1] > Priceperstock[i2]){   
        order_numbers.erase(remove(order_numbers.begin(),order_numbers.end(),i1),order_numbers.end());  
        }else{  
        order_numbers.erase(remove(order_numbers.begin(),order_numbers.end(),i2),order_numbers.end());
        return true; 
        }
    }
    return false; 

}

vector<int> check_arbitrage(vector<int> &order_numbers, vector<vector<pair<string,int>>> All_quantites, vector<int> Priceperstock,vector<char> type, vector<string> orders){
    bool ans1 = cancellation_predictor(order_numbers,All_quantites, type);
    bool ans2 = arbitrage_predictor(order_numbers,All_quantites);
    if(!ans1&&!ans2){ return {}; }
    string current_string = "";
    vector<int> arbitrage;
    int k = 0;
    for(auto it : order_numbers){
        for(int j = k; j < it ; j++){
            current_string += "0";
        }
        current_string += "1";
        k = it+1;
    }
    current_string.pop_back();
    vector<string> Possible_outcomes;
    generateAllStrings(Possible_outcomes,current_string,0);
    for(int i = 0 ; i < Possible_outcomes.size() ; i++){
        Possible_outcomes[i] += "1";
    }
    vector<pair<string,int>> arbitrage_counter;
    Possible_outcomes.pop_back();
    int maxTotalPrice = -1000;
    for(auto it : Possible_outcomes){
        int TotalProfit = 0;
        bool cancelled = false;
        pair<int,int> cancellation_counter = {0,0};
        int counter = 0; 
        for(int i = 0 ; i < it.size() ; i++){
            if(it[i]=='1'){
                counter++;
                append_order_code(arbitrage_counter,All_quantites[i],1);
                TotalProfit += Priceperstock[i]; 
            }
        }
        if(counter==2){
            int i = 0;
            for(i =0 ; i < it.size(); i++){
                if(it[i]=='1'){ break; }
            }
            if(i!=it.size()-1){
                cancelled = is_there_cancellation(i,type[i],it.size()-1,type[it.size()-1],All_quantites, Priceperstock, order_numbers);
            }
            if(cancelled){ return {}; }
        }
        bool isZero = check_quantities(arbitrage_counter);
        if(isZero){
            if(TotalProfit>maxTotalPrice && TotalProfit>0){
                maxTotalPrice = TotalProfit;
                arbitrage.clear();
                for(int i = 0 ; i < it.size() ; i++){
                    if(it[i]=='1'){ arbitrage.push_back(i); }
                }
            }
        }
        arbitrage_counter.clear();
    }
    return arbitrage;
}

bool withinBounds(const std::vector<int>& q, const std::vector<int>& Quantity) {
    for (size_t i = 0; i < q.size(); ++i) {
        if (q[i] >= Quantity[i]) {
            return false;
        }
    }
    return true;
}

int maxTp = -21474;
void explorePossibilities(std::vector<int>& q, const std::vector<int>& recursive_quantity, size_t currentIndex, vector<int> Indexes,vector<vector<pair<string,int>>> All_quantities, vector<int> Priceperstock, int& maxTp, vector<char> type, vector<pair<int,int>> &arbitrage) {
    if (withinBounds(q, recursive_quantity)) {
        int j = 0 ;
        int counter = 0;
        vector<pair<string,int>> arbitrage_counter;
        int TotalProfit = 0;
        vector<vector<pair<string,int>>> local = All_quantities; 
        for(auto it : Indexes){
            if(q[j]>0){ counter++; }
            for(int i = 0 ; i < All_quantities[it].size() ; i++){
                All_quantities[it][i].second *= q[j];
            }
            append_order_code(arbitrage_counter,All_quantities[it],1);
            TotalProfit += Priceperstock[it]*q[j];
            j++;
        }
        int i;
        if(counter==2){ 
            for(i = 0 ; i < q.size() ; i++){
                if(q[i]>0){ break; }
            }
            bool cancelledd = is_there_cancellation2(Indexes[i],type[Indexes[i]],Indexes.back(),type[Indexes.back()],local,Priceperstock, recursive_quantity[i], recursive_quantity.back());
            if(cancelledd){
                cancelledd1 = true;
                if(q[i]>quantities.first || q.back()>quantities.second){
                    quantities.first = max(quantities.first, min(q[i],q.back()));
                    quantities.second = max(quantities.second, min(q[i],q.back()));
                }
            }
        }
        if(!cancellation){
            bool isZero = check_quantities(arbitrage_counter);
            if(isZero){
                if(TotalProfit>maxTp && TotalProfit>0){
                    maxTp = TotalProfit;
                    arbitrage.clear();
                    for(int i = 0 ; i < Indexes.size() ; i++){
                        if(q[i]>0){
                            arbitrage.push_back({Indexes[i],q[i]});
                        }
                    }
                }
            }
        }
        arbitrage_counter.clear();
        All_quantities = local;
        local.clear();
        for (size_t i = currentIndex; i < q.size(); ++i) {
            std::vector<int> nextQ = q; 
            nextQ[i] += 1; 
            explorePossibilities(nextQ, recursive_quantity, i, Indexes, All_quantities, Priceperstock, maxTp, type, arbitrage); 
        }
    }
}

vector<pair<int,int>> part3_check_arbitrage(vector<vector<pair<string,int>>> All_quantites,vector<int> Priceperstock, vector<char> type, vector<int> &Quantity, vector<int> &order_numbers){
    bool ans1 = cancellation_predictor2(order_numbers,All_quantites, Priceperstock, type);
    bool ans2 = arbitrage_predictor(order_numbers,All_quantites);
    if(!ans1&&!ans2){ return {}; }    
    string current_string = "";
    vector<pair<int,int>> arbitrage;
    int k = 0;
    for(auto it : order_numbers){
        for(int j = k; j < it ; j++){
            current_string += "0";
        }
        current_string += "1";
        k = it+1;
    }
    current_string.pop_back();
    vector<string> Possible_outcomes;
    generateAllStrings(Possible_outcomes,current_string,0);
    for(int i = 0 ; i < Possible_outcomes.size() ; i++){
        Possible_outcomes[i] += "1";
    }
    vector<pair<string,int>> arbitrage_counter;
    Possible_outcomes.pop_back();
    vector<int> Indexes;
    vector<int> recursive_quantities;
    vector<int> InitialQ;
    for(auto it : Possible_outcomes){
        for(int i = 0 ; i < it.size() ; i++){
            if(it[i]=='1'){ 
                Indexes.push_back(i);
                recursive_quantities.push_back(Quantity[i]+1);
                InitialQ.push_back(0);
            }
        }
        InitialQ.pop_back();
        InitialQ.push_back(1);
        explorePossibilities(InitialQ,recursive_quantities,0,Indexes,All_quantites,Priceperstock, maxTp, type,arbitrage);
        if(cancellation){
            if(Quantity[indices.second]==quantities.second){
                Quantity[indices.first] -= quantities.first;
                Quantity[indices.second] -= quantities.second;
                order_numbers.erase(remove(order_numbers.begin(),order_numbers.end(),indices.second),order_numbers.end());               
                if(Quantity[indices.first]==0){
                order_numbers.erase(remove(order_numbers.begin(),order_numbers.end(),indices.first),order_numbers.end());
                }
                cancellation = false;  
                return {};
            }  
        }   
        InitialQ.clear();
        recursive_quantities.clear();
        Indexes.clear();
    }
    if(arbitrage.size()==0 && cancelledd1==true){
        Quantity[indices.first] -= quantities.first;
        Quantity[indices.second] -= quantities.second;  
        if(Quantity[indices.first]==0){ 
            order_numbers.erase(remove(order_numbers.begin(),order_numbers.end(),indices.first),order_numbers.end());                        
        } 
    }
    indices = {0,0};
    quantities = {0,0};
    return arbitrage;
}

int main(int argc, char* argv[]) {

    Receiver rcv;
    std::string message = rcv.readIML();
    while(message[message.size()-1]!='$'){
        message.append(rcv.readIML());
    }
    if(argv[1][0]=='2'){
        vector<string> orders;
            int i=0;
            string s="";
            while(message[i]!='\0'){
            if(message[i]=='#'){
                i++;
                orders.push_back(s);
                s="";
                continue;
            }
            s.push_back(message[i]);
            i++;
            }  
        vector<vector<pair<string,int>>> All_quantites;
        vector<pair<string,int>> QuantityCounter;
        vector<int> Priceperstock;
        vector<char> type;
        for(int i = 0 ; i < orders.size() ; i++){
            string it = orders[i];
            type.push_back(it[it.size()-1]);
            process_order(it,QuantityCounter,Priceperstock);
            string updated = "";
            for(auto pt : QuantityCounter){
                if(type.back()=='b'){
                    updated += pt.first; updated += " "; updated += to_string(pt.second); updated += " ";
                }else{
                    updated += pt.first; updated += " "; updated += to_string(-1*pt.second); updated += " ";
                }            }
            if(type.back()=='b'){ 
                updated += to_string(Priceperstock.back());
                updated += " ";
                updated += " b"; 
                orders[i] = updated; }
            else{ 
                updated += to_string(-1*Priceperstock.back());
                updated += " ";
                updated += " s"; 
                orders[i] = updated; }
            mergeSortpairs(QuantityCounter,0,QuantityCounter.size()-1);
            All_quantites.push_back(QuantityCounter);
            QuantityCounter.clear();
        }
        vector<int> order_numbers;
        int Overall_profit = 0;
        order_numbers.push_back(0);
        cout<<"No Trade"<<endl;
        for(int i = 1 ; i < orders.size() ; i++){
            order_numbers.push_back(i);
            vector<int> arbitrage_order_numbers = check_arbitrage(order_numbers,All_quantites,Priceperstock,type,orders);
            if(arbitrage_order_numbers.size()==0){ cout<<"No Trade"<<endl; }
            else{

                for(int adi=0;adi<arbitrage_order_numbers.size();adi++){
                    for(int aks=0;aks<order_numbers.size();aks++){
                        if(order_numbers[aks]==arbitrage_order_numbers[adi]){popRandomElement(order_numbers,aks);}
                    }
                }

                for(int i = arbitrage_order_numbers.size()-1 ; i >=0 ; i--){
                    if(type[arbitrage_order_numbers[i]]=='b'){
                        string back = to_string(Priceperstock[arbitrage_order_numbers[i]]) + " b";
                        cout<<orders[arbitrage_order_numbers[i]].substr(0,orders[arbitrage_order_numbers[i]].size()-back.size()); 
                        cout<<Priceperstock[arbitrage_order_numbers[i]]<<" "<<"s"<<endl;
                        Overall_profit += Priceperstock[arbitrage_order_numbers[i]];
                    }else{
                        string back = to_string(-1*Priceperstock[arbitrage_order_numbers[i]]) + " s";
                        cout<<orders[arbitrage_order_numbers[i]].substr(0,orders[arbitrage_order_numbers[i]].size()-back.size()); 
                        cout<<-1*Priceperstock[arbitrage_order_numbers[i]]<<" "<<"b"<<endl;
                        Overall_profit += Priceperstock[arbitrage_order_numbers[i]];
                    }
                }
            }
            arbitrage_order_numbers.clear();
            
        }
        cout<<Overall_profit;
    }
    
    else if(argv[1][0]=='3'){
        vector<string> orders;
            int i=0;
            string s="";
            while(message[i]!='\0'){
            if(message[i]=='#'){i+=1;
                orders.push_back(s);
                s="";
                continue;}
            s.push_back(message[i]);
            i++;
            }  
        vector<pair<string,int>> QuantityCounter;
        vector<int> arbitrage_quantities;
        vector<vector<pair<string,int>>> All_quantites;
        vector<int> Priceperstock;
        vector<char> type;
        vector<int> Quantity;
        vector<int> PrintQuantity;
        vector<int> order_numbers;
        int quantity = 0;
        for(int i = 0 ; i<orders.size() ;i++){
            string it = orders[i];
            type.push_back(it[it.size()-1]);
            process_raworder(it,QuantityCounter,quantity, Priceperstock);
            Quantity.push_back(quantity);
            PrintQuantity.push_back(quantity);
            string updated = "";
            for(auto pt : QuantityCounter){
                if(type.back()=='b'){
                    updated += pt.first; updated += " "; updated += to_string(pt.second); updated += " ";
                }else{
                    updated += pt.first; updated += " "; updated += to_string(-1*pt.second); updated += " ";
                }
            }
            if(type.back()=='b'){ 
                updated += to_string(Priceperstock.back());
                updated += " ";
                updated += to_string(Quantity.back());
                updated += " b"; 
                orders[i] = updated; }
            else{ 
                updated += to_string(-1*Priceperstock.back());
                updated += " ";
                updated += to_string(Quantity.back());
                updated += " s"; 
                orders[i] = updated; }
            mergeSortpairs(QuantityCounter,0,QuantityCounter.size()-1);
            All_quantites.push_back(QuantityCounter);
            QuantityCounter.clear();
        }
        int Overall_profit = 0;
        order_numbers.push_back(0);
        cout<<"No Trade"<<endl;
        for(int j = 1 ; j < orders.size() ; j++){
            maxTp = -21474;
            order_numbers.push_back(j);
            vector<pair<int,int>> arbitrage_order_numbers = part3_check_arbitrage(All_quantites,Priceperstock,type,Quantity,order_numbers);
            if(arbitrage_order_numbers.size()==0){ cout<<"No Trade"<<endl; }
            else{
                for(int i = arbitrage_order_numbers.size()-1 ; i >=0 ; i--){
                    if(type[arbitrage_order_numbers[i].first]=='b'){
                        string back = to_string(Priceperstock[arbitrage_order_numbers[i].first]) + " " + to_string(PrintQuantity[arbitrage_order_numbers[i].first]) + " b";
                        cout<<orders[arbitrage_order_numbers[i].first].substr(0,orders[arbitrage_order_numbers[i].first].size()-back.size()); 
                        cout<<Priceperstock[arbitrage_order_numbers[i].first]<<" "<< arbitrage_order_numbers[i].second<<" s"<<endl;
                        Overall_profit += Priceperstock[arbitrage_order_numbers[i].first]*arbitrage_order_numbers[i].second;
                    }else{
                        string back = to_string(-1*Priceperstock[arbitrage_order_numbers[i].first]) + " " + to_string(PrintQuantity[arbitrage_order_numbers[i].first]) + " s";
                        cout<<orders[arbitrage_order_numbers[i].first].substr(0,orders[arbitrage_order_numbers[i].first].size()-back.size()); 
                        cout<<-1*Priceperstock[arbitrage_order_numbers[i].first]<<" "<<arbitrage_order_numbers[i].second<<" b"<<endl;
                        Overall_profit += Priceperstock[arbitrage_order_numbers[i].first]*arbitrage_order_numbers[i].second;
                    }
                    if(Quantity[arbitrage_order_numbers[i].first]==arbitrage_order_numbers[i].second){ 
                        order_numbers.erase(remove(order_numbers.begin(),order_numbers.end(),arbitrage_order_numbers[i].first),order_numbers.end());
                    }
                    Quantity[arbitrage_order_numbers[i].first] -= arbitrage_order_numbers[i].second;
                }
            }
            arbitrage_order_numbers.clear();  
        }
        cout<<Overall_profit<<endl;
    }
    
    else{
        // error handling //
        std::cout<<"wrong argument";
        return 0;
    }

}