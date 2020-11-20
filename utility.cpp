#include "utility.h"

vector<string> split(const string& str, const string& delim){
    vector<string> tokens;
    size_t prev = 0, pos = 0;
    do
    {
        pos = str.find(delim, prev);
        if (pos == string::npos) pos = str.length();
        string token = str.substr(prev, pos-prev);
        if (!token.empty()) tokens.push_back(token);
        prev = pos + delim.length();
    }
    while (pos < str.length() && prev < str.length());
    return tokens;
}

RGBA convert_to_RGBA(string s){

    RGBA output;

    output[0] = stoul(s.substr(1,2), nullptr, 16);
    output[1] = stoul(s.substr(3,2), nullptr, 16);
    output[2] = stoul(s.substr(5,2), nullptr, 16);
    output[3] = 0;

    return output;
}

void print(vector<vector<double> > v){
    for(auto &x: v){
        for(auto &y: x){
            cout << y << " ";
        }
        cout << endl;
    }
}

void print(vector<uint8_t> input, uint width, uint height){
    uint index = 0;
    for(size_t i=0;i<width;i++){
        for(size_t j=0;j<height;j++){
            for(size_t k=0;k<4;k++){
                cout << int(input[index++]);
                cout << ",";
            }
            cout << " ";
        }
        cout << endl;
    }
}
